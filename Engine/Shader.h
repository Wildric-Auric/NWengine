#pragma once
#include"glm/glm.hpp"
#include <string>
#include <map>
#include <iostream>

#define SHADER_DEFAULT "Shaders\\Textured.shader"
#define SHADER_POST_PROCESSING "Shaders\\PostProcessing.shader"

class Shader {
	public:
		unsigned int shaderProgram;
		std::string name;

		Shader(std::string path = SHADER_DEFAULT);
		void Use();
		void SetMat4x4(const char* name, const float* value);
		void SetUniform1f(const char* name, const float);
		void SetUniform3f(const char* name, float x, float y, float z);
		void SetVector2(const char* name, float value0, float value1);
		void SetUniform1i(const char* name, const int);
		static std::map<std::string, Shader> resList;
};
std::pair<const char*, const char*> parseShader(const char* path);

