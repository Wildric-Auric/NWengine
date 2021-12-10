#pragma once
#include"glm/glm.hpp"
#include <glfw3.h>
class Shader {
	public:
		unsigned int shaderProgram;
		Shader(const char* path = "Shaders/Shader1.shader");
		void SetMat4x4(const char* name, const GLfloat* value);
		void SetUniform1f(const char* name, const GLfloat);
		void SetVector2(const char* name, GLfloat value0, GLfloat value1);
};
std::pair<const char*, const char*> parseShader(const char* path);
