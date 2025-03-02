#include "Text.h"
#include "Transform.h"
#include "Sprite.h"
#include "Scene.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) {
	this->attachedObject = go;
};

void Text::Update() {
	float x = position.x;
    float y = position.y; 
    float tmp = 0.0;
    float maxYAdvance = 0.0f;
	uint32 temp  = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

    TextIterData tdata;
    tdata.chrIndex = 0;
    tdata.chrNum = characters.size();
	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();


		sprite->vertexAttributes.x = *((float*)&temp );
		sprite->vertexAttributes.y = *((float*)&temp0);

		transform->scale.x = scale.x;
		transform->scale.y = scale.y;
		transform->position.x = x + (chr->glyph->bearing.x + chr->glyph->size.x / 2) * transform->scale.x;
		transform->position.y = y - (chr->glyph->size.y - chr->glyph->bearing.y - chr->glyph->size.y / 2) * transform->scale.y;
			
		chrCbk(&*chr, &tdata);
        ++tdata.chrIndex;
		sprite->Update(); 

		x += (chr->glyph->GetAdvanceX()) * transform->scale.x;
        tmp += (chr->glyph->GetAdvanceX()) * transform->scale.x;
        maxYAdvance = Max(chr->glyph->GetAdvanceY(), maxYAdvance);
        
        if (constraints.boxHorizontalWrap == 0.0f || tmp < constraints.boxHorizontalWrap)
            continue; 
        x  = position.x;
        tmp = 0.0f;

        y -= (constraints.fixedLineSpacing != 0.0) ? constraints.fixedLineSpacing 
             : maxYAdvance * transform->scale.y;
        
    }
}

void Text::SetChrCallback(CharacterUpdateCallback cbk) {
    chrCbk = (cbk == 0) ? [](Character*, TextIterData*)->void{} : cbk;
}

void Text::SetContent(const char* str) {
    text = str;
}

void Text::SetConstraint(const TextConstraint& c) {
    constraints = c;
}


void Text::SetShader(Shader* s) {
	_shader = s;
}

void Text::SetShader(std::string path) {
	Loader<Shader> l;
	_shader = l.LoadFromFileOrGetFromCache((void*)&path, path.c_str(), nullptr);
}

void Text::SetShader(const ShaderText& st, ShaderIdentifier* id) {
		Loader<Shader> l;
		_shader = l.LoadFromBufferOrGetFromCache(id, (void*)&st, id);
}

fVec2 Text::GetSize() {
	fVec2 ret;
	std::list<Character>::iterator iter = characters.begin();
	for (auto c = characters.begin(); c != characters.end(); ++c) {
		ret.x += (c->glyph->GetAdvanceX()) * c->go.GetComponent<Transform>()->scale.x * scale.x;
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
		sprite->SetShader(_shader); 
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
