#pragma once
#include "Font.h"
#include "GameObject.h"
#include <list>

#define FONT_DEFAULT			"Ressources\\Fonts\\arial.ttf"
#define SHADER_TEXT_DEFAULT		"Ressources\\Shaders\\TextBasicBatched.shader"

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
	void  SetFont(const std::string& path);
	std::list<Character> characters;

	Font* font		   = nullptr;
	std::string text   = "";
	std::string shader = SHADER_TEXT_DEFAULT;
	bool isBatched = 1;
	Vector4<float> colors = Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f); //Each color is 10 bits

	fVec2 position = fVec2(0.0f, 0.0f);
	fVec2 scale    = fVec2(1.0f, 1.0f);

	int Serialize(std::fstream* data, int offset)	override;
	int Deserialize(std::fstream* data, int offset) override;
};