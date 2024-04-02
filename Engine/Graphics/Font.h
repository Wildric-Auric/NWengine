#pragma once
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Texture.h"
#include "Asset.h"

typedef void* NW_FT_Lib ;
typedef void* NW_FT_Face;
typedef std::string FontIdentifier;

class Glyph {
public:
	Glyph() = default;
	void Delete();
	uint32	   advance = 0;
	iVec2	   bearing;
	iVec2	   size;
	Texture    texture;
};

class Font : public Asset {
private:
	NW_FT_Face face  = nullptr;
public:
	std::unordered_map<char, Glyph> charactersMap;
	std::string name = "";

	Font()  = default;
	bool LoadFont(std::string path);

	void Delete();
	
	Asset* GetFromCache(void* identifier)											  override;
	Asset* LoadFromFile(const char* path, void* data)		                          override;
	Asset* LoadFromBuffer(void* ftFace, void* data)									  override;                


	static NW_FT_Lib lib;
	static bool Init();
	static void Destroy();

	NW_DECL_RES_LIST(FontIdentifier, Font);
};

struct Character {
	Glyph*	   glyph;
	GameObject go;
};


