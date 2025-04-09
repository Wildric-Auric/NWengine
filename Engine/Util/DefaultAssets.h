#pragma once
#include "Texture.h"
#include "Image.h"
#include "Shader.h"

#define NW_DEFAULT_SHADER ShaderTexturedDefaultStr,&ShaderTexturedDefaultID
#define NW_DEFAULT_SHADER_BATCHED ShaderTexturedBatchedDefaultStr,&ShaderTexturedBatchedDefaultID
#define NW_DEFAULT_SHADER_TEXT ShaderTextDefaultStr,&ShaderTextDefaultID
#define NW_DEFAULT_SHADER_TEXT_BATCHED ShaderTextBatchedStr,&ShaderTextBatchedDefaultID
#define NW_DEFAULT_SHADER_TRIANGLE ShaderTriangleDefaultStr,&ShaderTriangleDefaultID
#define NW_DEFAULT_SHADER_CIRCLE ShaderCircleDefaultStr,&ShaderCircleDefaultID

#define NW_DEFAULT_TEXTURE &ImageDefault,&TextureDefaultID

extern const ShaderText ShaderCircleDefaultStr;
extern ShaderIdentifier  ShaderCircleDefaultID;


extern ShaderIdentifier  ShaderTriangleDefaultID;

extern const ShaderText ShaderTriangleDefaultStr;
/**
 * @brief The default textured shader text.
 */
extern const ShaderText ShaderTexturedDefaultStr;

/**
 * @brief The default batched textured shader text.
 */
extern const ShaderText ShaderTexturedBatchedDefaultStr;

/**
 * @brief The default shader text.
 */
extern const ShaderText ShaderTextDefaultStr;

/**
 * @brief The default batched shader text.
 */
extern const ShaderText ShaderTextBatchedStr;

/**
 * @brief The default image.
 */
extern const Image ImageDefault;

/**
 * @brief The default texture identifier.
 */
extern TextureIdentifier TextureDefaultID;

/**
 * @brief The default textured shader identifier.
 */
extern ShaderIdentifier ShaderTexturedDefaultID;

/**
 * @brief The default batched textured shader identifier.
 */
extern ShaderIdentifier ShaderTexturedBatchedDefaultID;

/**
 * @brief The default shader identifier.
 */
extern ShaderIdentifier ShaderTextDefaultID;

/**
 * @brief The default batched shader identifier.
 */
extern ShaderIdentifier ShaderTextBatchedDefaultID;
