#pragma once
#include "Image.h"
#include "Shader.h"
#include "Texture.h"

#define NW_DEFAULT_SHADER				 ShaderTexturedDefaultStr, &ShaderTexturedDefaultID
#define NW_DEFAULT_SHADER_BATCHED		 ShaderTexturedBatchedDefaultStr, &ShaderTexturedBatchedDefaultID
#define NW_DEFAULT_SHADER_BATCHED_STATIC ShaderTexturedBatchedStaticDefaultStr, &ShaderTexturedBatchedStaticDefaultID
#define NW_DEFAULT_SHADER_COLORED		 ShaderTexturedColoredDefaultStr, &ShaderTexturedColoredDefaultID
#define NW_DEFAULT_SHADER_TEXT			 ShaderTextDefaultStr, &ShaderTextDefaultID
#define NW_DEFAULT_SHADER_TEXT_BATCHED	 ShaderTextBatchedStr, &ShaderTextBatchedDefaultID
#define NW_DEFAULT_SHADER_TRIANGLE		 ShaderTriangleDefaultStr, &ShaderTriangleDefaultID
#define NW_DEFAULT_SHADER_CIRCLE		 ShaderCircleDefaultStr, &ShaderCircleDefaultID
#define NW_DEFAULT_SHADER_NO_ALPHA		 ShaderTexturedNoAlphaStr, &ShaderTexturedNoAlphaID

#define NW_DEFAULT_TEXTURE &ImageDefault, &TextureDefaultID

extern const ShaderText ShaderCircleDefaultStr;
extern const ShaderText ShaderTriangleDefaultStr;
extern const ShaderText ShaderTexturedDefaultStr;
extern const ShaderText ShaderTexturedColoredDefaultStr;
extern const ShaderText ShaderTexturedBatchedDefaultStr;
extern const ShaderText ShaderTextDefaultStr;
extern const ShaderText ShaderTextBatchedStr;
extern const ShaderText ShaderTexturedNoAlphaStr;
extern const ShaderText ShaderTexturedBatchedStaticDefaultStr;

extern const Image ImageDefault;

extern TextureIdentifier TextureDefaultID;
extern ShaderIdentifier	 ShaderTexturedDefaultID;
extern ShaderIdentifier	 ShaderTexturedBatchedDefaultID;
extern ShaderIdentifier	 ShaderTextDefaultID;
extern ShaderIdentifier	 ShaderTextBatchedDefaultID;
extern ShaderIdentifier	 ShaderCircleDefaultID;
extern ShaderIdentifier	 ShaderTriangleDefaultID;
extern ShaderIdentifier	 ShaderTexturedColoredDefaultID;
extern ShaderIdentifier	 ShaderTexturedNoAlphaID;
extern ShaderIdentifier	 ShaderTexturedBatchedStaticDefaultID;

//-----------Extra----------------
#define NW_INTERNAL_DEFAULT_SHADER_UI_WINDOW   ShaderUIWindowStr, &ShaderUIWindowID
#define NW_INTERNAL_DEFAULT_SHADER_UI_SLIDER   ShaderUISliderStr, &ShaderUISliderID
#define NW_INTERNAL_DEFAULT_SHADER_UI_CHECKBOX ShaderUICheckboxStr, &ShaderUICheckboxID

extern const ShaderText ShaderUIWindowStr;
extern const ShaderText ShaderUISliderStr;
extern const ShaderText ShaderUICheckboxStr;

extern ShaderIdentifier ShaderUIWindowID;
extern ShaderIdentifier ShaderUISliderID;
extern ShaderIdentifier ShaderUICheckboxID;
