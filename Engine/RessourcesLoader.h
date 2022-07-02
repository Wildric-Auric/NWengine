#pragma once
#include "Shader.h"
#include "GameObject.h"
#include <map>
#include <string>

class Image {
private:
	int channels;
public:
	Image(std::string path = "Ressources/Images/test.png", bool alpha = 1);
	uint8* tex;
	int width;
	int height;
	bool alpha;
	void Clear();
	static std::map<std::string, Image> resList;
	
};


class RessourcesLoader {
public:
	//It seems like a namespace
	static void LoadDefaultRessources();
	static Shader* LoadShader(std::string path);
	static Texture* LoadTexture(std::string path, bool alpha  = 1, bool repeat = 1);
	static void ClearUnusedTextures();
	static void ClearAllTextures();
};

void FreeBuffer(uint8* buffer);
//void LoadRessources();
