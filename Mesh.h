#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "assimp/types.h"

#include <vector>
#include <string.h>

#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;///< 纹理坐标
};

struct Texture
{
	GLuint id;///< 产生的纹理id对象
	std::string type;///< 纹理类型：diffuse/specular
	aiString path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertexs, std::vector<GLuint> indices, std::vector<Texture> textures) :
		vertices_(vertexs), indices_(indices), textures_(textures)
	{
		setupMesh();
	}

	void Draw(Shader shader);
	GLuint VAO;
	std::vector<GLuint> indices_;
	std::vector<Texture> textures_;
private:
	std::vector<Vertex> vertices_;
	
	

	GLuint VBO, EBO;

	void setupMesh();
};