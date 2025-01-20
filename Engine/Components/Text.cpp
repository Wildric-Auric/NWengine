#include "Text.h"
#include "Transform.h"
#include "Sprite.h"
#include "Utilities.h"
#include "Scene.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) {
	this->attachedObject = go;
};

void Text::Update() {
	float x = this->position.x;
	uint32 temp  = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();


		sprite->vertexAttributes.x = *((float*)&temp );
		sprite->vertexAttributes.y = *((float*)&temp0);

		transform->scale.x = this->scale.x;
		transform->scale.y = this->scale.y;
		transform->position.x = x + (chr->glyph->bearing.x + chr->glyph->size.x / 2) * transform->scale.x;
		transform->position.y = this->position.y - (chr->glyph->size.y - chr->glyph->bearing.y - chr->glyph->size.y / 2) * transform->scale.y;
			
		sprite->Update(); 
		x += (chr->glyph->advance / 64 ) * transform->scale.x;
	}
}

void Text::SetShader(Shader* s) {
	_shader = s;
}

fVec2 Text::GetSize() {
	fVec2 ret;
	std::list<Character>::iterator iter = characters.begin();
	for (auto c = characters.begin(); c != characters.end(); ++c) {
		ret.x += (c->glyph->advance / 64) * c->go.GetComponent<Transform>()->scale.x * scale.x;
		ret.y = Max(c->glyph->size.y * c->go.GetComponent<Transform>()->scale.y * scale.y, ret.y);
	}
	return ret;
}

void Text::UpdateGlyphs() {
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
		sprite->SetShader(_shader); //TODO::AddFunction to set shader by its pointer
		if (this->isBatched)
			sprite->Batch();
		sprite->SetTexture(&iter->glyph->texture);
		sprite->SetSortingLayer(layerOrder);
		sprite->container.UpdateSize(iter->glyph->size.x, iter->glyph->size.y);
		++iter;
	}
	while (iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}
}

void Text::SetFont(const std::string& path, Shader* shader) {
	if (path == "")
		return;
	if (this->font != nullptr)
		this->font->Clean();
	Font loader;
	FontIdentifier fid = path;
	font = (Font*)loader.LoadFromFileOrGetFromCache(&fid, path.c_str(), &fid);
	_shader = shader;
	UpdateGlyphs();
}

Text::~Text() {
	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Sprite* sprite = chr->go.GetComponent<Sprite>();
		sprite->_isRendered = 0;
	}
	Scene::currentScene->ForceRenderStop();
	if (font)
		font->Clean();
}


