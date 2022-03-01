#pragma once
#include"glm/glm.hpp"
//#include <glfw3.h>
#include <string>
#include <iostream>
class Shader {
	public:
		unsigned int shaderProgram;
		std::string name;

		Shader(const char* path = "Shaders/Shader1.shader", std::string name = "none");
		void SetMat4x4(const char* name, const float* value);
		void SetUniform1f(const char* name, const float);
		void SetUniform3f(const char* name, float x, float y, float z);
		void SetVector2(const char* name, float value0, float value1);
		void SetUniform1i(const char* name, const int);
};
std::pair<const char*, const char*> parseShader(const char* path);

