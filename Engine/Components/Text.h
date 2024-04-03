#pragma once
#include "Font.h"
#include "GameObject.h"
#include "Shader.h"
#include <list>

#define FONT_DEFAULT			"Ressources\\Fonts\\arial.ttf"

class Text : public GameComponent {
public:
	static std::string GetType() { return "Text"; }
	Text() = default;
	~Text();
	Text(GameObject* go);
	GameObject* attachedObj = nullptr;

	void  Update()							override;
	void* GetGameObject()					override;
	void  SetGameObject(void* go)			override;
	void  UpdateGlyphs();
	void  SetFont(const std::string& path, Shader*);
	void  SetShader(Shader*);
	std::list<Character> characters;

	Shader* _shader    = nullptr;
	Font* font		   = nullptr;
	std::string text   = "";
	bool isBatched     = 1;
	Vector4<float> colors = Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f); //Each color is 10 bits

	fVec2 position = fVec2(0.0f, 0.0f);
	fVec2 scale    = fVec2(1.0f, 1.0f);

	int Serialize(std::fstream* data, int offset)	override;
	int Deserialize(std::fstream* data, int offset) override;
};