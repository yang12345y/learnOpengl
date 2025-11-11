#include "Model.h"
#include "SOIL.h"

void Model::Draw(Shader shader)
{
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		meshes_[i].Draw(shader);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "loadModel Failed: " << importer.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	cout << "模型加载完成，包含" << meshes_.size() << "个网格" << endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(procesMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::procesMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertexes;

	// 顶点（位置、法向量、纹理坐标）、面片索引、材质
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 position;
		position[0] = mesh->mVertices[i].x;
		position[1] = mesh->mVertices[i].y;
		position[2] = mesh->mVertices[i].z;
		vertex.position = position;

		glm::vec3 normal;
		normal[0] = mesh->mNormals[i].x;
		normal[1] = mesh->mNormals[i].y;
		normal[2] = mesh->mNormals[i].z;
		vertex.normal = normal;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureCoord;
			textureCoord[0] = mesh->mTextureCoords[0][i].x;
			textureCoord[1] = mesh->mTextureCoords[0][i].y;
			vertex.textureCoord = textureCoord;
		}
		else
		{
			vertex.textureCoord = glm::vec2(0.f);
		}
		vertexes.emplace_back(vertex);
	}

	std::vector<GLuint> indices;
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuseTexture = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseTexture.begin(), diffuseTexture.end());
		auto specularTexture = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularTexture.begin(), specularTexture.end());
		auto ambientTexture = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientTexture.begin(), ambientTexture.end());
	}
	cout << "网格顶点数：" << vertexes.size() << "，索引数：" << indices.size() << endl; // 若为0，说明网格数据无效
	return Mesh(vertexes, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;
		for (size_t j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(loaded_textures[j].path.C_Str(), path.C_Str()) == 0) 
			{
				textures.emplace_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip) 
		{
			Texture texture;
			texture.id = textureFromFile(path.C_Str(), directory);
			texture.type = typeName;
			texture.path = path;
			textures.emplace_back(texture);
			loaded_textures.emplace_back(texture);
		}
	}
	return textures;
}

GLuint Model::textureFromFile(string name, string directory)
{
	GLuint textureId;
	glGenTextures(1, &textureId);

	int width, height;
	string filePath = directory + "/" + name;

	unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D ,0);
	SOIL_free_image_data(image);
	return textureId;
}
