#pragma once
#include "Image.h"
#include "Shader.h"
#include "Texture.h"

#define NW_DEFAULT_SHADER			   ShaderTexturedDefaultStr, &ShaderTexturedDefaultID
#define NW_DEFAULT_SHADER_BATCHED	   ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID
#define NW_DEFAULT_SHADER_COLORED	   ShaderTexturedColoredDefaultStr, &ShaderTexturedColoredDefaultID
#define NW_DEFAULT_SHADER_TEXT		   ShaderTextDefaultStr, &ShaderTextDefaultID
#define NW_DEFAULT_SHADER_TEXT_BATCHED ShaderTextBatchedStr, &ShaderTextBatchedDefaultID
#define NW_DEFAULT_SHADER_TRIANGLE	   ShaderTriangleDefaultStr, &ShaderTriangleDefaultID
#define NW_DEFAULT_SHADER_CIRCLE	   ShaderCircleDefaultStr, &ShaderCircleDefaultID

#define NW_DEFAULT_TEXTURE &ImageDefault, &TextureDefaultID

extern const ShaderText ShaderCircleDefaultStr;

extern const ShaderText ShaderTriangleDefaultStr;

extern const ShaderText ShaderTexturedDefaultStr;

extern const ShaderText ShaderTexturedColoredDefaultStr;

extern const ShaderText ShaderTexturedBatchedDefaultStr;

extern const ShaderText ShaderTextDefaultStr;

extern const ShaderText ShaderTextBatchedStr;

extern const Image ImageDefault;

extern TextureIdentifier TextureDefaultID;

extern ShaderIdentifier ShaderTexturedDefaultID;

extern ShaderIdentifier ShaderTexturedBatchedDefaultID;

extern ShaderIdentifier ShaderTextDefaultID;

extern ShaderIdentifier ShaderTextBatchedDefaultID;

extern ShaderIdentifier ShaderCircleDefaultID;

extern ShaderIdentifier ShaderTriangleDefaultID;

extern ShaderIdentifier ShaderTexturedColoredDefaultID;
