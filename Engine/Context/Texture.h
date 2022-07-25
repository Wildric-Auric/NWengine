#pragma once
#include"Maths.h"
#include"Globals.h"
#include<map>

#define TEXTURE_DEFAULT "Ressources\\Images\\img2.png"


class Texture {
private:
public:
	uint32 texture;  //Public for the framebuffer
	bool alpha = 0;  //ReadOnly
	bool repeat = 0; //ReadOnly
	std::string name;
	Vector2<int> size;
	Texture(){};
	Texture(int width, int height, uint8* texRes, bool alpha = true, bool repeat = false);
	void Bind(unsigned int slot);
	void UpdateTexture(int width, int height, uint8* texRes,unsigned int slot, bool alpha = 1);
	//The UpdateTexture method was added so I can use glReadPixels
	//initially I recreate Texture object in each mainloop iteration but an unknown bug occurs after several seconds, and it's not optimized
	//This solution works for now
	static std::map<std::string, Texture> resList;
};
