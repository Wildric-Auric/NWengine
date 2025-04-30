#pragma once

#include "Shader.h"

struct InlineShaderText {
	std::string vert;
	std::string frag;
};

class InlineShader {
  public:
	void AppFragGlobal(const char*);
	void AppFragMain(const char*);
	void AppVertGlobal(const char*);
	void AppVertMain(const char*);
	void SetFragOut(const char*);
	void SetVertOut(const char*);

	void	Generate();
	Shader* GetShader();

	void SetName(const char*);

	InlineShaderText _data;
	InlineShaderText _end;

	std::string		 _fragOut = "vec4(1.0)";
	std::string		 _vertOut = "uMvp * vec4(attribPos, 1.0)";
	ShaderIdentifier _id;
	Shader*			 _shader = 0;

	static uint32 _autoIncID;
};
