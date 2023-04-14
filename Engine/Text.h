#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Texture.h"

class TextSystem {
private:
public:
	static FT_Library lib;
	static bool Init();
	static void Destroy();
};

class Glyph {
public:
	Glyph();
	Texture    texture;
	uint32	   advance;
	iVec2	   bearing;
	iVec2	   size;
};

class Font {
private:
	FT_Face face;
public:
	std::unordered_map<char, Glyph> charactersMap;
	std::string name = "";

	Font(){};
	Font(std::string path);
	bool LoadFont(std::string path);
	void Delete();
};

struct Character {
	Glyph*	   glyph;
	GameObject go;
};


