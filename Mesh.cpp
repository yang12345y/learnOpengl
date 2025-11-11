#include "Mesh.h"

void Mesh::Draw(Shader shader)
{
	shader.Use();

	//激活纹理，给纹理赋值
	int diffuseN = 1;
	int specularN = 1;
	int ambientN = 1;
	for (size_t i = 0; i < textures_.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string type = textures_[i].type;
		std::stringstream ss;
		if (type == "texture_diffuse")
			ss << diffuseN++;
		else if (type == "texture_specular")
			ss << specularN++;
		else if (type == "texture_ambient")
			ss << ambientN++;

		glUniform1i(glGetUniformLocation(shader.Program, (type + ss.str()).c_str()), i);
		
		glBindTexture(GL_TEXTURE_2D, textures_[i].id);
	}

	glUniform1f(glGetUniformLocation(shader.Program, "shininess"), 32.f);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->textures_.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setupMesh()
{
	/// 初始化顶点属性
	glGenVertexArrays(1, &this->VAO);

	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices_.size(), &this->vertices_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_.size(), &this->indices_[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
