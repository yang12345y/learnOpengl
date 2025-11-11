#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>; // 包含glew来获取所有的必须OpenGL头文件
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Shader
{
public:
	GLuint Program;
	Shader(const char* vertextPath, const char* fragmentPath, int i);
	Shader(const char* vertextPath, const char* geometryPath, const char* fragmentPath, int i);
	void Use();
	void setMat4(const char* text, glm::mat4 mat) const;
	void setMat3(const char* text, glm::mat3 mat) const;
	void set1i(const char* text, GLuint val) const;
	void set3f(const char* text, glm::vec3 val) const;
	void set1f(const char* text, float val) const;
	void setVec3(const char* text, glm::vec3 val) const;
};

