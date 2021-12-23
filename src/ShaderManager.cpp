#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "ShaderManager.h"
#include "glm/glm.hpp"

std::pair<const char*, const char*> parseShader(const char* path)
{
	std::string frag, vert = "";    //TODO: Make it directly in the heap
	unsigned int current = 0;
	std::ifstream file(path);
	for (std::string line; std::getline(file, line);)
	{
		if (line.find("//fragment shader") != -1)
		{
			current = 1;
		}
		else if (line.find("//vertex shader") != -1) current = 2;
		if (current == 1) frag += line + '\n';
		else if (current == 2) vert += line + '\n';
	}

	return std::make_pair(_strdup(&vert[0]), _strdup(&frag[0]));
}

Shader::Shader(const char* path) {
	std::pair<const char*, const char*> shaderSrc = parseShader(path);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(shaderSrc.first), NULL);
	glCompileShader(vertexShader);
	int successInfo;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED\n" << log << std::endl;
	}
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(shaderSrc.second), NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED\n" << log << std::endl;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //TODO: Add error message

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successInfo);
	if (!successInfo) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED\n" << log << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};

void Shader::SetMat4x4(const char* name, const GLfloat* value) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, value);
}

void Shader::SetUniform1f(const char* name, GLfloat value) {
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::SetVector2(const char* name, GLfloat value0, GLfloat value1) {
	glUniform2f(glGetUniformLocation(shaderProgram, name), value0, value1);
}

void Shader::SetUniform1i(const char* name, const int value) {
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void Shader::SetUniform3f(const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(shaderProgram, name), x, y, z);
}

