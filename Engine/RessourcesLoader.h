#pragma once
#include "ShaderManager.h"
#include "GameObject.h"
#include <map>
#include <string>

class Image {
public:
	Image(const char* path = "Ressources/Images/test.png", bool alpha = 1);
	unsigned char* tex;
	int width;
	int height;
	int channels;
	bool alpha;
};
extern unsigned char* behindPixels;

extern std::map<int, GameObject*> allObjects;
extern std::map<std::string, GameObject> objects;
extern std::map<std::string, Texture> textures;
extern std::map<std::string, Image> images;
extern std::map<std::string, Shader> shaders;

void FreeBuffer(unsigned char* buffer);
void LoadRessources();
