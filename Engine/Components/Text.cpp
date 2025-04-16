#include "Text.h"
#include "Transform.h"
#include "Sprite.h"
#include "Scene.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) {
	this->attachedObject = go;
};

void Text::SetPosition(const fVec2& pos) {
    _bb.center = pos;
}

void Text::SetCenterPosition(const fVec2& pos) {
    fVec2 s = GetSize();
    _bb.center = pos + fVec2(-s.x, s.y - _firstLineYSize*2.0) * 0.5f;
}

void Text::SetTopLeftPosition(const fVec2& pos) {
    fVec2 s = GetSize();
    _bb.center.y += s.y / 2.0f;
}

void Text::SetScale(const fVec2& value) {
    scale = value;
}

fVec2 Text::GetPosition() {
    return _bb.size;
}
    
fVec2 Text::GetPositionCenter() {
    return _bb.center;
}
    
fVec2 Text::GetPostionTopLeft() {
    return _bb.center + fVec2(-_bb.size.x * 0.5, + _bb.size.y * 0.5);
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

void Text::Update() {

	uint32 temp  = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

    TextIterData tdata;
    tdata.chrIndex = 0;
    tdata.chrNum = characters.size();
    
    float halignOffset = 0.0;
    float offset = 0.0;

	for (std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform       = chr->go.GetComponent<Transform>(); //TODO::add transform variable to glyph
		Sprite*    sprite          = chr->go.GetComponent<Sprite>();
        sprite->vertexAttributes.x = *((float*)&temp);
  	    sprite->vertexAttributes.y = *((float*)&temp0);

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

void Text::CalcBB(TextConstraintIterData* d) {
    int n = d->lineNum;
    int width = (d->lineNum == 1 ) ? d->cur.x : constraints.boxHorizontalWrap;
    _bb.size.y = n * font->_inf.height;
    _bb.size.x   = width; 
}

void Text::SetRelCharPos(Character* chr, TextConstraintIterData* d) {
    Transform* tr = chr->go.GetComponent<Transform>();
    fVec2 apos;
    apos.x = -_bb.size.x* 0.5;
    apos.y = _bb.size.y * 0.5 - font->_inf.height + d->lastBearing;
    tr->Translate(_bb.center + apos);
}

void Text::UpdateGlyphs() {
	if (characters.size() < 1) 
		characters.push_back(Character());

	std::list<Character>::iterator iter = characters.begin();

    TextConstraintIterData tid;
	for (char c : text) {
		if (iter == characters.end()) {
			characters.push_back(Character());
			iter = --characters.end();
		}
        SetChrComps(&*iter, c);
        ApplyContraint(&*iter, &tid);
		++iter;
	}
	while (iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}

    CalcBB(&tid);
    for (iter = characters.begin(); iter != characters.end(); ++iter) {
        SetRelCharPos(&*iter, &tid); 
    }
}

void Text::ApplyContraint(Character* chr, TextConstraintIterData* data) {
    Transform& tr  = *chr->go.GetComponent<Transform>();
    iVec2& bear    = chr->glyph->bearing;
    TextConstraintIterData& d = *data;
    Glyph& g = *chr->glyph;

    float dx = d.cur.x + chr->glyph->GetAdvanceX();
    bool wrapx = dx >= constraints.boxHorizontalWrap;
    if (wrapx) {
        d.cur.x  = 0.0; dx = chr->glyph->GetAdvanceX();
        d.cur.y -= font->_inf.height;
        d.lastBearing = 0;
        ++d.lineNum;
    }
    int yoff = g.size.y - bear.y; //Under the line on which we write
    tr.position.x  = d.cur.x + bear.x;
    tr.position.y  = d.cur.y - yoff;

    tr.position.x += g.size.x / 2.0;
    tr.position.y += g.size.y / 2.0;

    d.lastBearing = Max(d.lastBearing, yoff);

    d.cur.x = dx;
}

void Text::SetChrComps(Character* chr, char c) {
		chr->go.AddComponent<Transform>();
		Sprite* sprite = chr->go.AddComponent<Sprite>();
		chr->glyph = &this->font->charactersMap.find(c)->second;
		sprite->SetShader(_shader); 
		if (this->isBatched)
			sprite->Batch();
		sprite->SetTexture(&chr->glyph->texture);
		sprite->SetSortingLayer(layerOrder);
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
