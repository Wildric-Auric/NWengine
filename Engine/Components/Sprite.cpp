#include "Sprite.h"

Sprite::Sprite(GameObject* go) {
	this->go = go;
};

std::map<GameObject*, Sprite> Sprite::componentList;



void Sprite::SetTexture(std::string path, bool alpha, bool repeat) {
	//TODO:: what if you want to load same image with different values of alpha and repeat
	//TODO::Add error handling
	//Automatic load?
	RessourcesLoader::LoadTexture(path, alpha, repeat);
	texture = &Texture::resList[path];
	container = Quad(iVec2(0, 0), texture->size.x, texture->size.y);
}

void Sprite::SetShader(std::string path) {
	RessourcesLoader::LoadShader(path);
	shader = &Shader::resList[path];
}