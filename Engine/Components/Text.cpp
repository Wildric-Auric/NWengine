#include "Text.h"
#include "Scene.h"
#include "Sprite.h"
#include "Transform.h"

#define MAX_BIT_10 1023.0
Text::Text(GameObject* go) { this->attachedObject = go; };

void Text::SetPosition(const fVec2& pos) { _bb.center = pos; }

void Text::SetScale(const fVec2& value) { scale = value; }

fVec2 Text::GetPosition() { return _bb.center; }

fVec2 Text::GetPostionTopLeft() { return _bb.center + fVec2(-_bb.size.x * 0.5, +_bb.size.y * 0.5); }

void Text::SetBoxHorizontalWrap(const float value) { constraints.boxHorizontalWrap = value; }

void Text::SetHorizontalAlignment(const TextHorizontalAlignment value) { constraints.halign = value; }

void Text::SetFixedLineSpacing(const float value) { constraints.fixedLineSpacing = value; }

void Text::Update() {
	uint32 temp	 = (uint32)(this->colors.g * MAX_BIT_10) | ((uint32)(this->colors.r * MAX_BIT_10) << 0xA);
	uint32 temp0 = (uint32)(this->colors.a * MAX_BIT_10) | ((uint32)(this->colors.b * MAX_BIT_10) << 0xA);

	TextIterData tdata;
	tdata.chrIndex = 0;
	tdata.chrNum   = characters.size();

	float halignOffset = 0.0;
	float offset	   = 0.0;

	for(std::list<Character>::iterator chr = characters.begin(); chr != characters.end(); ++chr) {
		Transform* transform	   = chr->go.GetComponent<Transform>(); // TODO::add transform variable to glyph
		Sprite*	   sprite		   = chr->go.GetComponent<Sprite>();
		sprite->vertexAttributes.x = *((float*)&temp);
		sprite->vertexAttributes.y = *((float*)&temp0);

		chrCbk(&*chr, &tdata);
		++tdata.chrIndex;
		sprite->Update();
	}
}

void Text::SetChrCallback(CharacterUpdateCallback cbk) {
	chrCbk = (cbk == 0) ? [](Character*, TextIterData*) -> void {} : cbk;
}

void Text::SetContent(const char* str) { text = str; }

void Text::SetContentAndUpdateGlyphs(const char* str) {
	SetContent(str);
	UpdateGlyphs();
}

void Text::SetConstraint(const TextConstraint& c) { constraints = c; }

void Text::SetShader(Shader* s) { _shader = s; }

void Text::SetShader(std::string path) {
	Loader<Shader> l;
	_shader = l.LoadFromFileOrGetFromCache((void*)&path, path.c_str(), nullptr);
}

void Text::SetShader(const ShaderText& st, ShaderIdentifier* id) {
	Loader<Shader> l;
	_shader = l.LoadFromBufferOrGetFromCache(id, (void*)&st, id);
}

fVec2 Text::GetSize() { return _bb.size; }

NWCoordSys::BoundingBox& Text::GetBBRef() { return _bb; }

void Text::GetBB(NWCoordSys::BoundingBox* bb) { *bb = _bb; }

void Text::CalcBB(TextConstraintIterData* d) {
	int n	   = d->lineNum;
	int width  = MMAX(lngstline, d->cur.x);
	_bb.size.y = n * font->_inf.height * scale.y;
	_bb.size.x = width * scale.x;
	_bearing   = d->lastBearing;
}

void Text::SetRelCharPos(Character* chr) {
	Transform* tr = chr->go.GetComponent<Transform>();
	fVec2	   apos;
	apos.x = -_bb.size.x * 0.5;
	apos.y = _bb.size.y * 0.5 + (-font->_inf.height + _bearing) * scale.y;
	tr->Translate(_bb.center + apos);
}

void Text::MapOnChar(CharacterUpdateCallback f, void* data) {
	int i = 0;
	for(Character& c : characters) {
		TextIterData d;
		d.chrNum   = characters.size();
		d.chrIndex = i++;
		d.other	   = data;
		f(&c, &d);
	}
}

void Text::DirectDraw() {
	for(Character& c : characters) {
		c.go.Draw();
	}
}

void Text::UpdateGlyphs(bool dontRender) {
	_isRendered = !dontRender;
	if(characters.size() < 1)
		characters.push_back(Character());

	std::list<Character>::iterator iter = characters.begin();

	TextConstraintIterData tid{};
	lngstline = 0.0;
	for(char c : text) {
		if(c == '\n') {
			++tid.br;
			continue;
		}

		if(iter == characters.end()) {
			characters.push_back(Character());
			iter = --characters.end();
		}
		SetChrComps(&*iter, c);
		ApplyConstraint(&*iter, &tid);
		++iter;
	}
	while(iter != characters.end()) {
		iter->go.DeleteComponents();
		iter = characters.erase(iter);
	}

	CalcBB(&tid);
	for(iter = characters.begin(); iter != characters.end(); ++iter) {
		SetRelCharPos(&*iter);
	}
}

void Text::Render() {
	_isRendered = 1;
	for(Character& c : characters) {
		Sprite* spr		  = c.go.GetComponent<Sprite>();
		spr->sortingLayer = layerOrder + 1;
		spr->SetSortingLayerFull(layerOrder);
	}
}

void Text::StopRendering() {
	_isRendered = 0;
	for(Character& chr : characters) {
		Sprite* sprite = chr.go.GetComponent<Sprite>();
		sprite->StopRendering();
		sprite->DontDraw();
	}
	Scene::currentScene->ForceRenderStop();
}

void Text::ApplyConstraint(Character* chr, TextConstraintIterData* data) {
	Transform&				tr	 = *chr->go.GetComponent<Transform>();
	iVec2&					bear = chr->glyph->bearing;
	TextConstraintIterData& d	 = *data;
	Glyph&					g	 = *chr->glyph;

	float dx	= d.cur.x + chr->glyph->GetAdvanceX();
	int	  wrapx = (dx >= constraints.boxHorizontalWrap) + data->br;
	if(wrapx) {
		lngstline = MMAX(d.cur.x, lngstline);
		d.cur.x	  = 0.0;
		dx		  = chr->glyph->GetAdvanceX();
		d.cur.y -= (wrapx * font->_inf.height);
		d.lineNum += wrapx;
		d.lastBearing = 0;
		data->br	  = 0;
	}
	int yoff	  = g.size.y - bear.y; // Under the line on which we write
	tr.position.x = (d.cur.x + bear.x) * scale.x;
	tr.position.y = (d.cur.y - yoff) * scale.y;

	tr.scale = scale;

	tr.position.x += g.size.x * tr.scale.x / 2.0;
	tr.position.y += g.size.y * tr.scale.y / 2.0;

	d.lastBearing = Max(d.lastBearing, yoff);

	d.cur.x = dx;
}

void Text::SetChrComps(Character* chr, char c) {
	chr->go.AddComponent<Transform>();
	Sprite* sprite = chr->go.AddComponent<Sprite>();
	chr->glyph	   = &this->font->charactersMap.find(c)->second;
	sprite->SetShader(_shader);
	if(this->isBatched)
		sprite->Batch();
	sprite->SetTexture(&chr->glyph->texture);
	sprite->texture->SetMinFilter(TexMinFilter::NW_NEAREST_MIPMAP_NEAREST);
	sprite->texture->SetMaxFilter(TexMaxFilter::NW_LINEAR);
	if(_isRendered)
		sprite->SetSortingLayer(layerOrder);
	else {
		sprite->sortingLayer = layerOrder;
		sprite->DontDraw();
		sprite->StopRendering();
	}
}

void Text::SetFont(const FontIdentifier& id) {
	if(id.path == "")
		return;
	if(this->font != nullptr)
		this->font->Clean();
	Font   loader;
	uint32 n = (id.nativeSize == 0) ? 64 : id.nativeSize;
	font	 = (Font*)loader.LoadFromFileOrGetFromCache((void*)&id, id.path.c_str(), (void*)&n);
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
	SetShader(st, id);
}

Text::~Text() {
	StopRendering();
	if(font)
		font->Clean();
}
