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
extern std::map<const char*, Texture> textures;
extern std::map<const char*, Shader> shaders;

void FreeBuffer(unsigned char* buffer);
void LoadImages();
void LoadShaders();
void LoadTextures();
void LoadGameObjects();


extern Image* IMAGES_WARRIOR_IDLE_ARRAY[6];
extern Image* IMAGES_WARRIOR_RUN_ARRAY[8];
