#pragma once
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Texture.h"



typedef void* NW_FT_Lib ;
typedef void* NW_FT_Face;

class TextSystem {
private:
public:
	static NW_FT_Lib lib;
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
	NW_FT_Face face;
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


