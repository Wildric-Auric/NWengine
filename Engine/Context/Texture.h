#pragma once
#include"Globals.h"
#include<map>

#define TEXTURE_DEFAULT "Ressources\\Images\\DefaultBox100x100.png"


class Texture {
private:
public:
	uint32 texture = 0;  //Public for the framebuffer
	bool alpha	   = 0;  //ReadOnly
	bool repeat	   = 0; //ReadOnly
	std::string name;
	Vector2<int> size;
	Texture(){};
	Texture(int width, int height, uint8* texRes, int alpha = true, bool repeat = false, bool genMipMap = true, bool linear = false);
	void Delete();
	void Bind(unsigned int slot);
	void UpdateTexture(int width, int height, uint8* texRes,unsigned int slot, bool alpha = 1);
	static std::map<std::string, Texture> resList;
};
