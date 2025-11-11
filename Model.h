#pragma once

#include "GL/glew.h"// Contains all the necessery OpenGL includes
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <vector>
#include <string>

#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model(const char* path)
	{
		loadModel(path);
	}

	void Draw(Shader shader);
	std::vector<Mesh> meshes_;
private:
	
	string directory;///< 存储文件路径的目录，加载纹理的时候会用到
	vector<Texture> loaded_textures;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh procesMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLuint textureFromFile(string name, string directory);
};

