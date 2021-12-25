#pragma once
#include"glm/glm.hpp"
#include <glfw3.h>
#include <iostream>
class Shader {
	public:
		unsigned int shaderProgram;
		Shader(const char* path = "Shaders/Shader1.shader");
		void SetMat4x4(const char* name, const GLfloat* value);
		void SetUniform1f(const char* name, const GLfloat);
		void SetUniform3f(const char* name, float x, float y, float z);
		void SetVector2(const char* name, GLfloat value0, GLfloat value1);
		void SetUniform1i(const char* name, const int);
};
std::pair<const char*, const char*> parseShader(const char* path);

void LoadShaders();

extern Shader* shader_default;
extern Shader* shader_lightSurface;
extern Shader* shader_grabPass;
extern Shader* shader_postProcessing;