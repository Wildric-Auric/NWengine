#pragma once
#include "Text.h"
#include "GameObject.h"
#include <list>
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

	Font* font;
	std::string text;
	std::string shader;
	bool isBatched = 0;
	Vector4<uint16> colors; //Each color is 10 bits

	fVec2 position = fVec2(0.0f, 0.0f);
	fVec2 scale    = fVec2(1.0f, 1.0f);
};