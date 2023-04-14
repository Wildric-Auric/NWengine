#pragma once
#include "Text.h"
#include "GameObject.h"
#include <list>

#define FONT_DEFAULT			"arial.ttf"
#define SHADER_TEXT_DEFAULT		"Shaders\\TextBasicBatched.shader"

class TextHandler : public GameComponent {
public:
	static std::string GetType() { return "TextHandler"; }
	TextHandler() {};
	TextHandler(GameObject* go);
	GameObject* attachedObj;

	void  Update()							override;
	void* GetGameObject()					override;
	void  SetGameObject(void* go)			override;
	void  UpdateGlyphs();
	std::list<Character> characters;

	Font font = Font(FONT_DEFAULT); //TODO::Make font a ressouce and this a pointer
	std::string text   = "";
	std::string shader = SHADER_TEXT_DEFAULT;
	bool isBatched = 1;
	Vector4<float> colors = Vector4<float>(1.0f, 1.0f, 1.0f, 1.0f); //Each color is 10 bits

	fVec2 position = fVec2(0.0f, 0.0f);
	fVec2 scale    = fVec2(1.0f, 1.0f);

	int Serialize(std::fstream* data, int offset)	override;
	int Deserialize(std::fstream* data, int offset) override;
	void Gui()										override;
};