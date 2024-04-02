#pragma once

#include <string>
#include <unordered_map>

#include "Globals.h"
#include "Asset.h"

struct ShaderText {
	const char* vertex;
	const char* fragment;
};


typedef std::string ShaderIdentifier;

class Shader : public Asset {
	public:
		uint32            _glID         = 0;
		ShaderIdentifier  _identifier;
		void Use();
		void SetMat4x4(const char* name, const float* value);
		void SetUniform1f(const char* name, const float);
		void SetUniform3f(const char* name, float x, float y, float z);
		void SetVector2(const char* name, float value0, float value1);
		void SetUniform1i(const char* name, const int);
		void SetUniformArrayf(const char* name, float* value, int size);
		void SetUniformArrayi(const char* name, int* value, int size);

		Asset* GetFromCache(void* identifier)											 override;
		Asset* LoadFromFile(const char* path, void* identifier)						     override;
		Asset* LoadFromBuffer(void* shaderTextPtr, void* identifier)                     override;


		void Delete();

		void _GlGen(ShaderText*);

		static ShaderText parseShader(const char* path);

		NW_DECL_RES_LIST(ShaderIdentifier, Shader);
};

