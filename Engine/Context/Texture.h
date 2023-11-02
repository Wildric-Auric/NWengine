#pragma once
#include"Globals.h"
#include<map>

#define TEXTURE_DEFAULT "Ressources\\Images\\DefaultBox100x100.png"

//Input may be given by user in constructor
struct TextureData {
	std::string name = "";
	uint8 alpha	     = 1;
	uint8 repeat	 = 0;
	uint8 genMipMap  = 1;
	uint8 linear     = 0;
};

class Texture {
private:
public:
	TextureData _texData;
	//Following fields are deprecated
	//URGENT TODO::REFACTOR CODE TO USE ONLY TEXTUREDATA!!!!
	uint32 texture = 0;  //Public for the framebuffer
	bool alpha	   = 0;  //ReadOnly
	bool repeat	   = 0;  //ReadOnly
	std::string name;
	Vector2<int> size;
	Texture(){};
	Texture(std::string name, int width, int height, uint8* texRes, uint8 alpha = 1, uint8 repeat = 0, uint8 genMipMap = 1, uint8 linear = 0);
	Texture(const uint8* texRes, const iVec2& size, const TextureData& texData);
	void Delete();
	void Bind(unsigned int slot);
	void UpdateTexture(int width, int height, uint8* texRes,unsigned int slot, bool alpha = 1);
	static std::map<std::string, Texture> resList;
};
