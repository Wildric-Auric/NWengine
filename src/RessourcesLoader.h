#pragma once
#include "ShaderManager.h"
#include "GameObject.h"
#include <map>

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


void FreeBuffer(unsigned char* buffer);
void LoadImages();
void LoadShaders();
void LoadTextures();
void LoadGameObjects();

extern Image IMAGE_APPLE;
extern Image IMAGE_WARRIOR_IDLE_1;
extern Image IMAGE_WARRIOR_IDLE_2;
extern Image IMAGE_WARRIOR_IDLE_3;
extern Image IMAGE_WARRIOR_IDLE_4;
extern Image IMAGE_WARRIOR_IDLE_5;
extern Image IMAGE_WARRIOR_IDLE_6;
extern Image* IMAGES_WARRIOR_IDLE_ARRAY[6];
extern Image IMAGE_BACKGROUND;
extern Image IMAGE_TREE1;
extern Image IMAGE_TREE2;
extern Image IMAGE_BUSH1;
extern Image IMAGE_BUSH2;
extern Image IMAGE_GROUND;
extern Image IMAGE_TILE1;
extern Image IMAGE_GROUND_TILE0;
extern Image IMAGE_GROUND_TILE1;
extern Image IMAGE_GROUND_TILE2;

extern Texture* tex;
extern Texture* grabTex;
extern Texture* warriorTex;
extern Texture* backgroundTex;
extern Texture* bush1Tex;
extern Texture* bush2Tex;
extern Texture* tree1Tex;
extern Texture* tree2Tex;
extern Texture* groundTex;
extern Texture* tile1Tex;
extern Texture* texture_groundTile0;
extern Texture* texture_groundTile1;
extern Texture* texture_groundTile2;



extern Shader* shader_default;
extern Shader* shader_lightSurface;
extern Shader* shader_grabPass;
extern Shader* shader_postProcessing;
extern Shader* shader_simple;


extern GameObject* lesbeanApple;
extern GameObject* lesbeanApple2;
extern GameObject* grabPass;
extern GameObject* lightSurface;
extern GameObject* postProcessing;
extern GameObject* warrior;
extern GameObject* background;
extern GameObject* background1;
extern GameObject* background2;
extern GameObject* background3;
extern GameObject* background4;
extern GameObject* tree1;
extern GameObject* tree2;
extern GameObject* bush1;
extern GameObject* bush2;
extern GameObject* ground;
extern GameObject* wallTile0;
extern GameObject* groundTile0;
extern GameObject* groundTile1;
extern GameObject* groundTile2;
