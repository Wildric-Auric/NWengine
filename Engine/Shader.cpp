#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Globals.h"
#include "Console.h"
#include <string.h>

static bool parseUniformLine(std::string* name, DataTypes* type) {

};

std::map<std::string, Shader> Shader::resList;

std::pair<const char*, const char*> Shader::parseShader(const char* path)
{
	std::string frag, vert = "";    //TODO:: Make it directly in the heap
	uint8 current = 0;
	std::ifstream file(path);
	for (std::string line; std::getline(file, line);)
	{
		if (line.find("//fragment shader") != -1)
			current = 1;
		else if (line.find("//vertex shader") != -1) current = 2;
		if (current == 1) frag += line + '\n';
		else if (current == 2) vert += line + '\n';
	}
	file.close();
	return std::make_pair(_strdup(&vert[0]), _strdup(&frag[0]));
}

Shader::Shader(std::string path) {
	this->name = path;
	std::pair<const char*, const char*> shaderSrc = parseShader(path.c_str());

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(shaderSrc.first), NULL);
	glCompileShader(vertexShader);
	int successInfo;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		Console::Write((std::string("SHADER::VERTEX::COMPILATION FAILED AT: ") + path).c_str(), CONSOLE_ERROR_MESSAGE);
		Console::Write(log, CONSOLE_ERROR_MESSAGE);
	}
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(shaderSrc.second), NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		Console::Write((std::string("SHADER::FRAGMENT::COMPILATION FAILED AT: ") + path).c_str(), CONSOLE_ERROR_MESSAGE);
		Console::Write(log, CONSOLE_ERROR_MESSAGE);
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successInfo);
	if (!successInfo) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		Console::Write((std::string("SHADER::SHADER::PROGRAM::LINKING FAILED AT: ") + path).c_str(), CONSOLE_ERROR_MESSAGE);
		Console::Write(log, CONSOLE_ERROR_MESSAGE);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};

void Shader::Use() {
	glUseProgram(shaderProgram);
}

void Shader::SetMat4x4(const char* name, const float* value) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);
}

void Shader::SetUniform1f(const char* name, float value) {
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::SetVector2(const char* name, float value0, float value1) {
	glUniform2f(glGetUniformLocation(shaderProgram, name), value0, value1);
}

void Shader::SetUniform1i(const char* name, const int value) {
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::SetUniform3f(const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderProgram, name), x, y, z);
};

void Shader::SetUniformArrayf(const char* name, float* value, int size) {
	glUniform1fv(glGetUniformLocation(shaderProgram, name), size, value);
}

void Shader::SetUniformArrayi(const char* name, int* value, int size) {
	glUniform1iv(glGetUniformLocation(shaderProgram, name), size, value);
}


void Shader::Delete() {
	glDeleteProgram(this->shaderProgram);
}


