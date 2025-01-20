#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "Shader.h"
#include "Globals.h"
#include "FrameBuffer.h"


ShaderText Shader::parseShader(const char* path)
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
	return {_strdup(&vert[0]), _strdup(&frag[0])};
}

void Shader::_GlGen(ShaderText* src) {

	uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &(src->vertex), NULL);
	glCompileShader(vertexShader);
	int successInfo;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		NW_LOG_ERROR((std::string("SHADER::VERTEX::COMPILATION FAILED AT: ") + src->vertex).c_str());
		NW_LOG_ERROR(log);
	}
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &(src->fragment), NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successInfo);
	if (!successInfo) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		NW_LOG_ERROR((std::string("SHADER::FRAGMENT::COMPILATION FAILED AT: ") + src->fragment).c_str());
		NW_LOG_ERROR(log);
	}
	_glID = glCreateProgram();
	glAttachShader(_glID, vertexShader);
	glAttachShader(_glID, fragmentShader);
	glLinkProgram(_glID);

	glGetProgramiv(_glID, GL_LINK_STATUS, &successInfo);
	if (!successInfo) {
		glGetProgramInfoLog(_glID, 512, NULL, log);
		NW_LOG_ERROR((std::string("SHADER::LINKAGE FAILED")).c_str());
		NW_LOG_ERROR(log);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
};

Asset* Shader::GetFromCache(void* identifier) {
	if (identifier == nullptr)
		return nullptr;
	auto iter = Shader::resList.find(*(ShaderIdentifier*)identifier);
	if (iter == Shader::resList.end()) 
		return nullptr;
	return &iter->second;
}

Asset* Shader::LoadFromFile(const char* path, void* identifier) {
	ShaderText res = Shader::parseShader(path);
	return LoadFromBuffer(&res, identifier);
}

Asset* Shader::LoadFromBuffer(void* shaderTextPtr, void* identifier) {
	Shader* shader = &resList.emplace(*(ShaderIdentifier*)identifier, Shader()).first->second;
	shader->_identifier = *(ShaderIdentifier*)identifier;
	shader->_GlGen((ShaderText*)shaderTextPtr);
	return shader;
}

NW_IMPL_RES_LIST(ShaderIdentifier, Shader)




void Shader::Use() {
	NW_GL_CALL(glUseProgram(_glID));
    FrameBuffer* fbo = FrameBuffer::GetCurrent();
    if (!fbo) return;
    for (int i = 0; i < fbo->GetAttNum(); ++i) {
        if (_enabledAtts.find(i) != _enabledAtts.end()) {
		    NW_GL_CALL(glColorMaski(i,1,1,1,1));
        }
        else {
            NW_GL_CALL(glColorMaski(i,0,0,0,0));
        }
    }
}

void Shader::Unuse() {
	NW_GL_CALL(glUseProgram(0));
    FrameBuffer* fbo = FrameBuffer::GetCurrent();
    if (!fbo) return;
    for (int i = 0; i < fbo->GetAttNum();++i) {
		NW_GL_CALL(glColorMaski(i,1,1,1,1));
    }
}

void Shader::_DisableAtt(int i) {
    _enabledAtts.erase(i);
}

void Shader::_EnableAtt(int i) {
    _enabledAtts[i] = i;
}


void Shader::SetMat4x4(int loc, const float* value) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void Shader::SetUniform1f(int loc, float value) {
    glUniform1f(loc, value);
}

void Shader::SetVector2(int loc, float value0, float value1) {
    glUniform2f(loc, value0, value1);
}

void Shader::SetUniform1i(int loc, const int value) {
    glUniform1i(loc, value);
}

void Shader::SetUniform3f(int loc, float x, float y, float z) {
    glUniform3f(loc, x, y, z);
}

void Shader::SetUniform4f(int loc, float x, float y, float z, float w) {
    glUniform4f(loc, x, y, z, w);
}

void Shader::SetUniformArrayf(int loc, float* value, int size) {
    glUniform1fv(loc, size, value);
}

void Shader::SetUniformArrayi(int loc, int* value, int size) {
    glUniform1iv(loc, size, value);
}

void Shader::SetMat4x4(const char* name, const float* value) {
	SetMat4x4(glGetUniformLocation(_glID, name), value);
}

void Shader::SetUniform1f(const char* name, float value) {
	SetUniform1f(glGetUniformLocation(_glID, name), value);
}

void Shader::SetVector2(const char* name, float value0, float value1) {
	SetVector2(glGetUniformLocation(_glID, name), value0, value1);
}

void Shader::SetUniform1i(const char* name, const int value) {
	SetUniform1i(glGetUniformLocation(_glID, name), value);
}

void Shader::SetUniform3f(const char* name, float x, float y, float z)
{
	SetUniform3f(glGetUniformLocation(_glID, name), x, y, z);
};

void Shader::SetUniform4f(const char* name, float x, float y, float z, float w) {
	SetUniform4f(glGetUniformLocation(_glID, name), x, y, z, w);
}

void Shader::SetUniformArrayf(const char* name, float* value, int size) {
	SetUniformArrayf(glGetUniformLocation(_glID, name), value, size);
}

void Shader::SetUniformArrayi(const char* name, int* value, int size) {
	SetUniformArrayi(glGetUniformLocation(_glID, name), value, size);
}

void Shader::Delete() {
	glDeleteProgram(this->_glID);
}
