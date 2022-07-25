#pragma once
#include <map>
#include "GameObject.h"
#include "RessourcesLoader.h"


class Sprite : public GameComponent {
private:
public:
	static std::string GetType() { return "Sprite"; };
	Texture* texture = &Texture::resList[TEXTURE_DEFAULT];
	Sprite() {};
	Sprite(GameObject* go);
	Shader* shader = &Shader::resList[SHADER_DEFAULT];
	Quad container = Quad(iVec2(0), Texture::resList[TEXTURE_DEFAULT].size.x, Texture::resList[TEXTURE_DEFAULT].size.y); //ReadOnly
	GameObject* go;
	int16 sortingLayer = 0;

	void SetTexture(std::string path, bool alpha = 1, bool repeat = 0);
	void SetShader(std::string path);
	static std::map<GameObject*, Sprite> componentList;
};