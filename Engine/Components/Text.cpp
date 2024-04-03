#include "Text.h"
#include "Transform.h"
#include "Sprite.h"
#include "Utilities.h"
#include "Scene.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) {
	this->attachedObj = go;
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
		sprite->container.UpdateSize(iter->glyph->size.x, iter->glyph->size.y);
		++iter;
	}
	while (iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}
}




void* Text::GetGameObject(){
	return (void*)this->attachedObj;
}
void  Text::SetGameObject(void* go) {
	this->attachedObj = (GameObject*)go;
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

int Text::Serialize(std::fstream* data, int offset) {
	//FontIdentifier fid = GetIDWithAsset<Font*, FontIdentifier>(this->font);

	//int sizeBuffer = 0;
	//WRITE_ON_BIN(data, "Text", 11, sizeBuffer);
	//WRITE_ON_BIN(data, fid.c_str(),			fid.size(), sizeBuffer);
	//WRITE_ON_BIN(data, &this->shader[0] ,	this->shader.size(), sizeBuffer);
	//WRITE_ON_BIN(data, &this->isBatched,	sizeof(isBatched), sizeBuffer);
	//WRITE_ON_BIN(data, &position.x,			sizeof(position.x), sizeBuffer);
	//WRITE_ON_BIN(data, &position.y,			sizeof(position.x), sizeBuffer);
	//WRITE_ON_BIN(data, &scale.x,			sizeof(scale.x), sizeBuffer);
	//WRITE_ON_BIN(data, &scale.y,			sizeof(scale.y), sizeBuffer);
	//WRITE_ON_BIN(data, &colors.r,			sizeof(colors.r), sizeBuffer);
	//WRITE_ON_BIN(data, &colors.g,			sizeof(colors.g), sizeBuffer);
	//WRITE_ON_BIN(data, &colors.b,			sizeof(colors.b), sizeBuffer);
	//WRITE_ON_BIN(data, &colors.a,			sizeof(colors.a), sizeBuffer);
	//WRITE_ON_BIN(data, &this->text[0],		this->text.size(), sizeBuffer);
	return 0;
}
//TODO::Make serialization functions for different basic datatypes?
int Text::Deserialize(std::fstream* data, int offset) {
	//int sizeBuffer = 0; 
	//char* fontTemp    = new char[512];
	//char* shaderTemp  = new char[512];
	//char* textTemp    = new char[512];

	//READ_FROM_BIN(data, fontTemp, sizeBuffer);				fontTemp[sizeBuffer]    = '\0';
	//READ_FROM_BIN(data, shaderTemp, sizeBuffer);			shaderTemp[sizeBuffer] =  '\0';
	//READ_FROM_BIN(data, &this->isBatched	, sizeBuffer);
	//READ_FROM_BIN(data, &position.x			, sizeBuffer);
	//READ_FROM_BIN(data, &position.y			, sizeBuffer);
	//READ_FROM_BIN(data, &scale.x     		, sizeBuffer);
	//READ_FROM_BIN(data, &scale.y			, sizeBuffer);
	//READ_FROM_BIN(data, &colors.r			, sizeBuffer);
	//READ_FROM_BIN(data, &colors.g			, sizeBuffer);
	//READ_FROM_BIN(data, &colors.b			, sizeBuffer);
	//READ_FROM_BIN(data, &colors.a			, sizeBuffer);
	//READ_FROM_BIN(data, textTemp			, sizeBuffer);  textTemp[sizeBuffer]  = '\0';

	//std::string fontPath = std::string(fontTemp);
	//this->shader		 = std::string(shaderTemp);
	//this->text			 = std::string(textTemp);
	//this->SetFont(fontPath);

	//delete[] fontTemp;
	//delete[] shaderTemp;
	//delete[] textTemp;
	return 0;
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


