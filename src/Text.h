#pragma once

#include "Text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>
#include "Maths.h"
#include "ShaderManager.h"
#include "string"

struct Character {
	unsigned int id;
	Vector2<int> size;
	Vector2<int> bearing;
	unsigned int advance;
};

class Font {
private:
	FT_Library ft;
	FT_Face face;
	unsigned int VAO, VBO;
public:
	Font(const char* font);
	std::map<char, Character> chars;
	void DisplayText(std::string text, Vector2<int> position, Shader* shader, Vector3<float> textColor, uint8_t slot = 0);
	
};