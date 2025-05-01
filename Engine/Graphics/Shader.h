#pragma once

#include "Asset.h"
#include "Globals.h"
#include "ShaderParser.h"
#include <string>
#include <unordered_map>

/**
 * @brief Struct representing the text of a shader.
 */
struct ShaderText {
	const char* vertex;	  /**< The vertex shader text. */
	const char* fragment; /**< The fragment shader text. */
};

typedef std::string ShaderIdentifier; /**< The type for shader identifiers. */

/**
 * @brief Class representing a shader asset.
 */
class Shader : public Asset {
  public:
	static ShaderParser parser;

	uint32													 _glID = 0;	  /**< The OpenGL ID of the shader. */
	ShaderIdentifier										 _identifier; /**< The identifier of the shader. */
	std::unordered_map<int, int>							 _enabledAtts{{0, 0}};
	std::unordered_map<std::string, ShaderParserUniformData> reflectedUniforms;

	void _EnableAtt(int i);
	void _DisableAtt(int i);

	void SetReflectedUniforms(const ShaderParser&);
	void SetEnabledAtts(const ShaderParser&);

	/**
	 * @brief Use this shader for rendering.
	 */
	void Use();

	void DirectUse();

	void Unuse();

	/**
	 * @brief Set a 4x4 matrix uniform in the shader.
	 * @param location The location of the uniform.
	 * @param value The value of the uniform.
	 */
	void SetMat4x4(int location, const float* value);

	void SetVector2(int loc, float value0, float value1);

	void SetUniform1f(int location, const float value);

	void SetUniform1i(int loc, const int value);

	void SetUniform2f(int loc, float x, float y);

	void SetUniform2i(int loc, int x, int y);

	void SetUniform3f(int loc, float x, float y, float z);

	void SetUniform3i(int loc, int x, int y, int z);

	void SetUniform4f(int loc, float x, float y, float z, float w);

	void SetUniform4i(int loc, int x, int y, int z, int w);

	void SetUniformArrayf(int loc, float* value, int size);

	void SetUniformArrayi(int loc, int* value, int size);

	void SetMat4x4(const char* name, const float* value);

	void SetUniform1f(const char* name, const float value);

	void SetUniform1i(const char* name, const int value);

	void SetUniform2f(const char* name, float x, float y);

	void SetUniform2i(const char* name, int x, int y);

	void SetUniform3f(const char* name, float x, float y, float z);

	void SetUniform3i(const char* name, int x, int y, int z);

	void SetUniform4f(const char* name, float x, float y, float z, float w);

	void SetUniform4i(const char* name, int x, int y, int z, int w);

	void SetVector2(const char* name, float value0, float value1);

	void SetUniformArrayf(const char* name, float* value, int size);

	void SetUniformArrayi(const char* name, int* value, int size);

	void SetUniformArray2f(const char* name, float* value, int size);

	void SetUniformArray2f(int loc, float* value, int size);

	Asset* GetFromCache(void* identifier) override;

	Asset* LoadFromFile(const char* path, void* identifier) override;

	Asset* LoadFromBuffer(void* shaderTextPtr, void* identifier) override;

	Asset* _LoadDirect(void* shaderTextPtr, void* identifier);

	void Clean() override;

	void Move(Asset* other) override;

	void Delete();

	void _GlGen(ShaderText* shaderText);

	static ShaderText fastParseShader(const char* path);

	NW_DECL_RES_LIST(ShaderIdentifier, Shader);
};

//------------------COMPUTE SHADER------------------

typedef const char* ComputeShaderText;
typedef std::string ComputeShaderIdentifier;

struct ComputeShaderCapabilities {
	iVec3 workGroupNum;
	iVec3 localSize;
	int32 maxInvoc;
};

class ComputeShader : public Shader {
  public:
	static ShaderParser		parser;
	iVec3					_dispatchSize;
	ComputeShaderIdentifier _identifier;
	void					_GlGen(ComputeShaderText* src2);

	Asset* GetFromCache(void* identifier) override;
	Asset* LoadFromFile(const char* path, void* identifier) override;
	Asset* LoadFromBuffer(void* shaderTextPtr, void* identifier) override;
	Asset* _LoadDirect(void* shaderTextPtr, void* identifier);
	void   SetReflectedUniforms(const ShaderParser& p);
	void   Move(Asset* other) override;
	void   Delete();
	void   Dispatch(const iVec3&);
	void   Dispatch();
	void   SetDispatchSize(const iVec3&);

	void Clean() override;

	static bool QueryCapabilities(ComputeShaderCapabilities* cap);

	NW_DECL_RES_LIST(ComputeShaderIdentifier, ComputeShader);
};
