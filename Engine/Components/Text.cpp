#include "Text.h"
#include "Transform.h"
#include "Sprite.h"
#include "Scene.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) {
	this->attachedObject = go;
};

void Text::SetPosition(const fVec2& pos) {
    position = pos;
}

void Text::SetCenterPosition(const fVec2& pos) {
    fVec2 s = GetSize();
    position = pos + fVec2(-s.x, s.y - _firstLineYSize*2.0) * 0.5f;
}

void Text::SetTopLeftPosition(const fVec2& pos) {
    fVec2 s = GetSize();
    position = _bb.center;
    position.y += s.y / 2.0f;
}

void Text::SetScale(const fVec2& value) {
    scale = value;
}

fVec2 Text::GetPosition() {
    return position;
}
    
fVec2 Text::GetPositionCenter() {
    return position + _bb.center;
}
    
fVec2 Text::GetPostionTopLeft() {
    return position + _bb.center + fVec2(-_bb.size.x * 0.5, + _bb.size.y * 0.5);
}

void Text::SetBoxHorizontalWrap(const float value) {
    constraints.boxHorizontalWrap = value;
}

void Text::SetHorizontalAlignment(const TextHorizontalAlignment value) {
    constraints.halign = value;
}

void Text::SetFixedLineSpacing(const float value) {
    constraints.fixedLineSpacing = value;
}

struct ApplyCnData {
    float x = 0.0f;
    float y = 0.0f;
    float xacc = 0.0f;
    float yacc = 0.0f;
    float yoff = 0.0f;
    float lastx = 0.0f;
    float maxYSize = 0.0f;
    float firstLineYSize = 0.0f;
    float firstLineYOff  = 0.0f;
    fVec2 size;
    Text* text;
};

static void ApplyConstraints(Character* chr, TextIterData* data) {
    ApplyCnData& d = *(ApplyCnData*)data->other;
    fVec2 scale = d.text->scale;
    float dx = (chr->glyph->GetAdvanceX()) * scale.x;
    bool boxXCond = d.text->constraints.boxHorizontalWrap != 0.0f && d.xacc + dx >= d.text->constraints.boxHorizontalWrap;
    if (boxXCond) {
        d.x    = d.text->position.x;
        float tmp = 
        (d.text->constraints.fixedLineSpacing != 0.0) ? 
        d.text->constraints.fixedLineSpacing 
        : d.text->font->_inf.linespace * scale.y;
        d.y      -= tmp;
        d.size.y += tmp;
        d.firstLineYSize == 0.0f ? d.firstLineYOff = d.yoff : 0;
        d.firstLineYSize == 0.0f ? d.firstLineYSize = d.maxYSize : 0;
        d.xacc  = 0.0f;
        d.yoff  = 0.0f;
    }
    d.yoff     = Max(((float)chr->glyph->size.y - chr->glyph->bearing.y) * scale.y, d.yoff);
    d.maxYSize = Max(((float)chr->glyph->size.y + d.yoff) * scale.y, d.maxYSize);
    d.lastx = d.x;
    d.x    += dx;
    d.xacc += dx;     
    d.size.x = Max(d.xacc, d.size.x);
}

static void PostProcess(ApplyCnData* d) {
    if (d->firstLineYSize == 0.0f) {
        d->firstLineYSize = d->maxYSize;
        d->firstLineYOff  = d->yoff;
    }
    else { 
        d->firstLineYSize += d->yoff; 
    }
    d->size.y += d->firstLineYSize;
}

void Text::Update() {
    ApplyCnData d;
	d.x = position.x;
    d.y = position.y; 
    d.text = this;

	uint32 temp  = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

    TextIterData tdata;
    tdata.chrIndex = 0;
    tdata.chrNum = characters.size();
    
    float halignOffset = 0.0;

    if (constraints.halign == TextHorizontalAlignment::CENTER && constraints.boxHorizontalWrap) {
        fVec2 s = GetSize(); //TODO::Cache the size
        halignOffset = constraints.boxHorizontalWrap * 0.5;
    }
    
    TextIterData d1; 
    d1.other = &d;

	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();

      float dx = (chr->glyph->GetAdvanceX()) * transform->scale.x;

      sprite->vertexAttributes.x = *((float*)&temp );
	  sprite->vertexAttributes.y = *((float*)&temp0);

      ApplyConstraints(&*chr, &d1);    

      transform->scale.x = scale.x;
      transform->scale.y = scale.y;
      transform->position.x  = d.lastx + (chr->glyph->bearing.x + chr->glyph->size.x / 2.0f) * transform->scale.x;
      transform->position.y  = d.y + (chr->glyph->bearing.y - chr->glyph->size.y / 2.0) * transform->scale.y; 
      transform->position.x += halignOffset;	

      chrCbk(&*chr, &tdata);
      ++tdata.chrIndex;
      sprite->Update(); 
    }
}

void Text::SetChrCallback(CharacterUpdateCallback cbk) {
    chrCbk = (cbk == 0) ? [](Character*, TextIterData*)->void{} : cbk;
}

void Text::SetContent(const char* str) {
    text = str;
}

void Text::SetContentAndUpdateGlyphs(const char* str) {
    SetContent(str);
    UpdateGlyphs();
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
    return _bb.size;
}

NWCoordSys::BoundingBox& Text::GetBBRef() {
    return _bb;
}

void Text::GetBB(NWCoordSys::BoundingBox* bb) {
   *bb =  _bb;
}

fVec2 Text::_ForceRecGetSize(float* ybearingoffset) {
	fVec2 ret;
	std::list<Character>::iterator iter = characters.begin();
    float tmp = 0.0f;
    TextIterData d0;
    ApplyCnData  d1;
    d1.text = this;
    d0.other= &d1;
	for (auto c = characters.begin(); c != characters.end(); ++c) {
        ApplyConstraints(&*c, &d0);
	}
    PostProcess(&d1);
    ret = d1.size;
    ybearingoffset ? *ybearingoffset = d1.yoff : 0;
	return ret;
}

void Text::UpdateGlyphs() {
	if (characters.size() < 1) 
		characters.push_back(Character());

	std::list<Character>::iterator iter = characters.begin();
    ApplyCnData d0;
    TextIterData d1;
    d0.text  = this;
    d0.x = position.x;
    d0.y = position.y;
    d1.other = &d0;

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

        ApplyConstraints(&*iter, &d1);
		++iter;
	}
    
    PostProcess(&d0);
    _yoff = d0.yoff;
    _firstLineYOffset = d0.firstLineYOff;
    _firstLineYSize   = d0.firstLineYSize;
    _bb.size = d0.size;
    _bb.center = fVec2(d0.size.x * 0.5, d0.yoff + 0.5*d0.firstLineYSize - d0.size.y * 0.5);
	while (iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}
}

void Text::SetFont(const FontIdentifier& id) { 
	if (id.path == "")
		return;
	if (this->font != nullptr)
		this->font->Clean();
	Font loader;
    uint32 n = (id.nativeSize == 0) ? 64 : id.nativeSize;
	font = (Font*)loader.LoadFromFileOrGetFromCache((void*)&id, id.path.c_str(), (void*)&n);
}

void Text::SetFont(const FontIdentifier& id, Shader* shader) {
    SetFont(id);
	SetShader(shader);
}

void Text::SetFont(const FontIdentifier& sf, const std::string& shdrPath) {
    SetFont(sf);
    SetShader(shdrPath);
}

void Text::SetFont(const FontIdentifier& sf, const ShaderText& st, ShaderIdentifier* id) {
    SetFont(sf);
    SetShader(st,id);
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
