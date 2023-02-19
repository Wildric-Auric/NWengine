#pragma once
#include <string>
#include <map>
#include "Globals.h"

#define SHADER_DEFAULT "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Shaders\\Textured.shader"
#define SHADER_POST_PROCESSING "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Shaders\\PostProcessing.shader"

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
		std::map<std::string, DataTypes> uniforms;

		static std::map<std::string, Shader> resList;
		static std::pair<const char*, const char*> parseShader(const char* path);
};

