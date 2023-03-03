
#pragma once
#include "GameObject.h"
#include "RessourcesLoader.h"

#include "Primitives.h"

class Sprite : public GameComponent {
private:
	uint32 lastSortingLayer = 0;
	bool   shouldDraw		= 1;
public:
	static std::string GetType() { return "Sprite"; };
	Texture* texture = &Texture::resList[TEXTURE_DEFAULT];
	bool isBatched   = 0;
	bool isRendered	 = 1;
	Sprite() {};
	Sprite(GameObject* go);
	~Sprite();
	Shader* shader = &Shader::resList[SHADER_DEFAULT];
	Quad container = Quad(Texture::resList[TEXTURE_DEFAULT].size.x, Texture::resList[TEXTURE_DEFAULT].size.y); //ReadOnly
	GameObject* go;
	uint32 sortingLayer = 0; //ReadOnly
	double zbuffer = 1.0; //ReadOnly

	void SetTexture(std::string path, bool alpha = 1, bool repeat = 0);
	void SetTexture(Texture* tex);
	void SetShader(std::string path);
	void SetSortingLayer(uint32 order);

	void Render();
	void StopRendering();

	void Update() override;
	void Gui() override;
	static std::map<GameObject*, Sprite> componentList;

	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;
};