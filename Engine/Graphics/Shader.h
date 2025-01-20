#pragma once

#include <string>
#include <unordered_map>

#include "Globals.h"
#include "Asset.h"

/**
 * @brief Struct representing the text of a shader.
 */
struct ShaderText {
    const char* vertex; /**< The vertex shader text. */
    const char* fragment; /**< The fragment shader text. */
};

typedef std::string ShaderIdentifier; /**< The type for shader identifiers. */

/**
 * @brief Class representing a shader asset.
 */
class Shader : public Asset {
public:
    uint32 _glID = 0; /**< The OpenGL ID of the shader. */
    ShaderIdentifier _identifier; /**< The identifier of the shader. */
    std::unordered_map<int,int> _enabledAtts{{0,0}};

    void _EnableAtt(int i);

    void _DisableAtt(int i);

    
    /**
     * @brief Use this shader for rendering.
     */
    void Use();

    void Unuse();

    
    /**
     * @brief Set a 4x4 matrix uniform in the shader.
     * @param location The location of the uniform.
     * @param value The value of the uniform.
     */
    void SetMat4x4(int location, const float* value);

    void SetUniform1f(int location, const float value);

    void SetVector2(int loc, float value0, float value1);

    void SetUniform1i(int loc, const int value);
    
    void SetUniform3f(int loc, float x, float y, float z);
    
    void SetUniform4f(int loc, float x, float y, float z, float w);
    
    void SetUniformArrayf(int loc, float* value, int size);
    
    void SetUniformArrayi(int loc, int* value, int size);

    /**
     * @brief Set a 4x4 matrix uniform in the shader.
     * @param name The name of the uniform.
     * @param value The value of the uniform.
     */
    void SetMat4x4(const char* name, const float* value);

    /**
     * @brief Set a float uniform in the shader.
     * @param name The name of the uniform.
     * @param value The value of the uniform.
     */
    void SetUniform1f(const char* name, const float value);

    /**
     * @brief Set a 3-component float vector uniform in the shader.
     * @param name The name of the uniform.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     * @param z The z component of the vector.
     */
    void SetUniform3f(const char* name, float x, float y, float z);

    /**
     * @brief Set a 4-component float vector uniform in the shader.
     * @param name The name of the uniform.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     * @param z The z component of the vector.
     * @param w The w component of the vector.
     */
    void SetUniform4f(const char* name, float x, float y, float z, float w);

    /**
     * @brief Set a 2-component float vector uniform in the shader.
     * @param name The name of the uniform.
     * @param value0 The first component of the vector.
     * @param value1 The second component of the vector.
     */
    void SetVector2(const char* name, float value0, float value1);

    /**
     * @brief Set an integer uniform in the shader.
     * @param name The name of the uniform.
     * @param value The value of the uniform.
     */
    void SetUniform1i(const char* name, const int value);

    /**
     * @brief Set an array of float uniforms in the shader.
     * @param name The name of the uniform.
     * @param value The array of float values.
     * @param size The size of the array.
     */
    void SetUniformArrayf(const char* name, float* value, int size);

    /**
     * @brief Set an array of integer uniforms in the shader.
     * @param name The name of the uniform.
     * @param value The array of integer values.
     * @param size The size of the array.
     */
    void SetUniformArrayi(const char* name, int* value, int size);

    /**
     * @brief Get a shader asset from the cache.
     * @param identifier The identifier of the shader.
     * @return The shader asset if found in the cache, nullptr otherwise.
     */
    Asset* GetFromCache(void* identifier) override;

    /**
     * @brief Load a shader asset from a file.
     * @param path The path to the shader file.
     * @param identifier The identifier of the shader.
     * @return The loaded shader asset.
     */
    Asset* LoadFromFile(const char* path, void* identifier) override;

    /**
     * @brief Load a shader asset from a buffer.
     * @param shaderTextPtr A pointer to the ShaderText struct.
     * @param identifier The identifier of the shader.
     * @return The loaded shader asset.
     */
    Asset* LoadFromBuffer(void* shaderTextPtr, void* identifier) override;

    /**
     * @brief Delete the shader.
     */
    void Delete();

    /**
     * @brief Generate and bind the OpenGL shader program.
     * @param shaderText A pointer to the ShaderText struct.
     */
    void _GlGen(ShaderText* shaderText);

    /**
     * @brief Parse a shader file and return the shader text.
     * @param path The path to the shader file.
     * @return The parsed shader text.
     */
    static ShaderText parseShader(const char* path);

    NW_DECL_RES_LIST(ShaderIdentifier, Shader);
};
