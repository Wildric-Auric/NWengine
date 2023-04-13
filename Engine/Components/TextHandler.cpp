#include "TextHandler.h"
#include "Transform.h"
#include "Sprite.h"

TextHandler::TextHandler(GameObject* go) {
	this->attachedObj = go;
};

void TextHandler::Update() {
	float x = this->position.x;
	uint32 temp  = (uint32)this->colors.g | ((uint32)this->colors.r << 0xA);
	uint32 temp0 = (uint32)this->colors.a | ((uint32)this->colors.b << 0xA);

	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();


		sprite->vertexAttributes.x = *((float*)&temp );
		sprite->vertexAttributes.y = *((float*)&temp0);

		transform->position.x = x + (chr->glyph->bearing.x + chr->glyph->size.x / 2) * transform->scale.x * this->scale.x;
		transform->position.y = this->position.y - (chr->glyph->size.y - chr->glyph->bearing.y - chr->glyph->size.y / 2) * transform->scale.y * this->scale.y;
		sprite->Update(); 
		sprite->isBatched = this->isBatched;
		x += (chr->glyph->advance / 64 ) * transform->scale.x;
	}
}

void TextHandler::UpdateGlyphs() {
	if (characters.size() < 1) 
		characters.push_back(Character());

	std::list<Character>::iterator iter = characters.begin();
	for (char c : text) {
		if (iter == characters.end()) {
			characters.push_back(Character());
			iter = --characters.end();
		}
		iter->go.AddComponent<Transform>();
		Sprite* sprite = iter->go.AddComponent<Sprite>();

		iter->glyph = &this->font->charactersMap.find(c)->second;
		sprite->SetShader(this->shader); //TODO::AddFunction to set shader by its pointer
		sprite->isBatched = this->isBatched;
		sprite->SetTexture(&iter->glyph->texture);
		sprite->container.UpdateSize(iter->glyph->size.x, iter->glyph->size.y);
		++iter;
	}
	while (iter != characters.end()) {
		iter = characters.erase(iter);
	}
}




void* TextHandler::GetGameObject(){
	return (void*)this->attachedObj;
}
void  TextHandler::SetGameObject(void* go) {
	this->attachedObj = (GameObject*)go;
}
