#include "UIWindow.h"
#include "Context.h"
#include "CoordSys.h"
#include "DefaultAssets.h"
#include "Inputs.h"
#include "Scene.h"
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "UISys.h"
#include "Utilities.h"
#include "Window.h"

int64 UIItem::DefaultUIItemGetLayerProc(UIItem* item) { return item->obj.GetComponent<Sprite>()->GetSortingLayer(); };

v2f UIItem::DefaultUIItemGetSizeProc(UIItem* item) { return item->obj.GetComponent<Sprite>()->GetSize(); }

bool UIWindow::CacheConditionHasUIWindow(GameObject* obj) { return obj->GetComponent<UIWindow>(); }

int UIWindow::UIWindowDrawCallback(void* obj) {
	UIWindow* win = ((GameObject*)obj)->GetComponent<UIWindow>();
	((GameObject*)obj)->GetComponent<Sprite>()->GetShader()->Use();
	win->SetShaderParams();

	fMat4 tempview						  = Camera::GetActiveCamera()->viewMatrix; // Temporary, should use UIManager instead
	Camera::GetActiveCamera()->viewMatrix = fMat4(1.0f);
	Context::EnableStencilTest(1);
	Context::SetStencilMask(1);
	Context::ClearStencilBuff();
	Context::SetStencilFunc(NWStencilBehaviour::NW_ALWAYS);
	int ret = Sprite::DefaultSpriteDrawCallback(obj);
	Context::SetStencilFunc(NWStencilBehaviour::NW_EQUAL);
	win->DrawItems();
	Context::EnableStencilTest(0);
	Camera::GetActiveCamera()->viewMatrix = tempview;
	return ret;
}

void UIWindow::DrawItems() {
	for(auto iter = this->items.begin(); iter != this->items.end(); iter++) {
		iter->Draw();
	}
}

int64 UIWindow::GetLayer() { return attachedObject->GetComponent<Sprite>()->GetSortingLayer(); }

int64 UIWindow::GetUIItemLayer(int64 relative) { return GetLayer() - relative; }

void UIWindow::OnAdd() {
	NW_REQUIRE_COMP(attachedObject, Transform);
	Sprite* spr = NW_REQUIRE_COMP(attachedObject, Sprite);
	spr->SetSize({100, 50});
	// Set shader
	spr->SetShader(NW_DEFAULT_SHADER_UI_WINDOW);
	Scene::GetCurrent()->AddToCache(UIWindow::CacheConditionHasUIWindow, *attachedObject);
	spr->SetSortingLayerFull(UISys::GetAvailableLayer());
	AddItem(UIItemType::TITLE, -UISys::layerConsts.windowRange + 1);
	attachedObject->SetDrawCallback(UIWindowDrawCallback);
}

UIItemType UIItem::GetType() { return type; }

int64 UIItem::GetLayer() { return _GetLayerProc(this); }

UIItem* UIWindow::_PushItem(UIItemType type, int64 layer) {
	// insert at beginning
	if(items.size() == 0 || items.front().GetLayer() <= layer) {
		items.emplace_front();
		return &items.front();
	}
	// insert at the middle
	for(auto it = ++items.begin(); it != items.end(); ++it) {
		if(it->GetLayer() > layer)
			continue;
		auto newIt = items.insert(it, {});
		return &*newIt;
	}
	// insert at the end
	items.emplace_back();
	return &items.back();
}

UIItem* UIWindow::_SetUpItem(UIItem* item, UIItemType type, int64 layer) {
	item->type	 = type;
	item->_owner = this;
	Sprite* spr	 = GetGameObject()->GetComponent<Sprite>();
#define gllmbda [](UIItem * item) -> int64
#define gslmbda [](UIItem * item) -> v2f
	switch(type) {
	case UIItemType::TEST_ZONE: {
		Sprite* spr2	= item->obj.AddComponents<Sprite, Transform>();
		item->_DrawProc = [](UIItem* item) {
			Sprite* spr2 = item->obj.GetComponent<Sprite>();
			spr2->SetShader(NW_DEFAULT_SHADER_COLORED);
			spr2->GetShader()->Use();
			spr2->GetShader()->SetUniform3f("uCol", 1.0, 0.0, 0.6);
			item->obj.Draw();
		};
		spr2->GetShader()->SetUniform3f("uCol", 1.0, 0.0, 0.0);
		spr2->sortingLayer = spr->sortingLayer + layer;
		break;
	}
	case UIItemType::TITLE: {
		item->_DrawProc		= [](UIItem* item) { item->obj.GetComponent<Text>()->DirectDraw(); };
		item->_GetLayerProc = gllmbda { return item->_owner->GetLayer(); };
		item->_GetSizeProc	= gslmbda { return {0.0, 0.0}; };
		item->_UpdateProc	= [](UIItem* item) {
			  Text* te = item->obj.GetComponent<Text>();
			  if(!te->text.size())
				  return;
			  fVec2					   s  = item->_owner->GetSize();
			  NWCoordSys::BoundingBox& bb = te->GetBBRef();
			  te->SetPosition(item->_owner->GetPosition() + fVec2(-s.x * 0.5 + bb.size.x * 0.5, s.y * 0.5 - bb.size.y * 0.5));
			  te->UpdateGlyphs(1);
			  item->obj.GetComponent<Text>()->Update();
		};
		GameObject* obj = &item->obj;
		Text*		te	= obj->AddComponents<Text, Transform>();
		te->SetShader(ShaderTextDefaultStr, &ShaderTextDefaultID);
		te->isBatched = false;
		std::string fdir;
		GetSystemFontDir(&fdir);
		fdir += "Arial.ttf";
		te->SetFont({fdir.c_str(), 15}, te->_shader);
		te->layerOrder = spr->sortingLayer - (UISys::layerConsts.windowRange - 1);
		break;
	}
	case UIItemType::LABEL: {
		item->_GetSizeProc	= gslmbda { return item->obj.GetComponent<Text>()->GetBBRef().size; };
		item->_GetLayerProc = gllmbda { return item->obj.GetComponent<Text>()->layerOrder; };
		item->_UpdateProc	= [](UIItem* item) {
			  Text* te = item->obj.GetComponent<Text>();
			  te->Update();
			  te->UpdateGlyphs(1);
			  te->SetPosition(item->obj.GetComponent<Transform>()->GetPosition());
		};

		item->_DrawProc = [](UIItem* item) { item->obj.GetComponent<Text>()->DirectDraw(); };
		Text* te		= item->obj.AddComponents<Text, Transform>();
		te->SetShader(ShaderTextDefaultStr, &ShaderTextDefaultID);
		te->isBatched = false;
		std::string fdir;
		GetSystemFontDir(&fdir);
		fdir += "Arial.ttf";
		te->SetFont({fdir.c_str(), 15}, te->_shader);
		te->layerOrder = spr->sortingLayer + layer;
		te->SetContent("Bombardino Crocodilo");
		te->UpdateGlyphs(1);
		break;
	};
	default: {
		break;
	}
	}
	return item;
}
#undef gllmbda
#undef gslmbda

UIItem* UIWindow::AddItem(UIItemType type, int64 layer) {
	UIItem* item = _PushItem(type, layer);
	return this->_SetUpItem(item, type, layer);
}

void UIWindow::OnDelete() { Scene::GetCurrent()->DeleteFromCache(UIWindow::CacheConditionHasUIWindow, *attachedObject); }

UIWindow::UIWindow(GameObject* go) { attachedObject = go; }

int UIWindow::IsCursorOnTitleBar() {
	// dist from top
	if(relPos.y < 0.0)
		return 0;
	return lsize.y * 0.5 - relPos.y <= metrics.titleBarHeight;
}

int UIWindow::IsCursorOnResize() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	return IsCursorOnWindow() && abs(rpos.x) > (spr->container.width * 0.5 - metrics.resizeAreaWidth) ||
		   abs(rpos.y) > (spr->container.height * 0.5 - metrics.resizeAreaWidth);
}

int UIWindow::IsCursorOnWindow() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	return (abs(rpos.x) < spr->container.width * 0.5 && abs(rpos.y) < spr->container.height * 0.5);
}

bool UIWindow::IsFocused() { return UISys::focusedWindow == this; }

UIWindowState UIWindow::GetState() { return state; }

fVec2 UIWindow::GetSize() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	return fVec2(spr->container.width, spr->container.height);
}

void UIWindow::SetShaderParams() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	fVec2	s	= fVec2(spr->container.width, spr->container.height);
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uRes", s.x, s.y);
	spr->GetShader()->SetUniform1f("uTitleHeight", metrics.titleBarHeight);
	spr->GetShader()->SetUniform1f("uBorderWidth", metrics.borderWidth);
	spr->GetShader()->SetUniform4f("uCol", bgCol.x, bgCol.y, bgCol.z, bgCol.z);
}

fVec2 UIWindow::GetPosition() {
	Transform* tr = attachedObject->GetComponent<Transform>();
	return tr->GetPosition();
}

void UIWindow::SetTitle(const char* c) {
	Text* te = items.back().obj.GetComponent<Text>();
	te->SetContent(c);
	te->UpdateGlyphs(1);
}

void UIWindow::SetPosition(const fVec2& pos) {
	Transform* tr = attachedObject->GetComponent<Transform>();
	tr->SetPosition(pos);
}

void UIWindow::SetSize(const fVec2& pos) {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	spr->SetSize(pos);
}

void UIWindow::Update() {
	cursor.SetCursorTopLeftWin();
	cursor.Advance(fVec2(0.0, -metrics.titleBarHeight - metrics.itemSpacing.y));

	for(UIItem& item : items) {
		fVec2 s = item.GetSize();
		cursor.CalcNextPosition(s);
		item.obj.GetComponent<Transform>()->SetPosition(cursor.GetAbsolutePos() + fVec2(s.x * 0.5, -s.y * 0.5));
		item.Update();
		cursor.Advance({s.x, 0.0});
	}

	Sprite*		  spr = attachedObject->GetComponent<Sprite>();
	Transform*	  tr  = attachedObject->GetComponent<Transform>();
	NWin::Window* win = ((NWin::Window*)(Context::window));
	fVec2		  s	  = fVec2(spr->container.width, spr->container.height);
	fVec2		  hs  = 0.5 * fVec2(spr->container.width, spr->container.height);

	rpos   = -tr->GetPosition() + UISys::curPos;
	bool m = Inputs::GetInputMouse(NWin::Key::NWIN_KEY_LBUTTON, InputKeyEvent::KeyPressed);

	if(IsCursorOnWindow() && UISys::GetClickEvent()) {
		UISys::Focus(this);
	}
	if(IsCursorOnWindow()) {
		UISys::Hover(this);
	}

	bgCol.x = IsFocused();

	if(state == UIWindowState::NONE && m && IsCursorOnWindow() && UISys::focusedWindow == this) {
		relPos	= rpos;
		lsize	= fVec2(spr->container.width, spr->container.height);
		lpos	= UISys::curPos;
		lwinPos = tr->GetPosition();
		if(IsCursorOnTitleBar())
			state = UIWindowState::MOVE;
		if(IsCursorOnResize()) {
			state = UIWindowState::RESIZE;
		}
	}
	if((state != UIWindowState::NONE) && !m) {
		state = UIWindowState::NONE;
	}
	if(state == UIWindowState::MOVE) {
		tr->SetPosition(UISys::GetCurPos() - relPos);
	}
	if(state == UIWindowState::RESIZE) {
		((NWin::Window*)(Context::window))->setCursor(NWin::CursorIcon::RESIZE_WE);
		fVec2 ps = UISys::curPos - lpos;
		ps.x	 = (ps.x);
		ps.y	 = (ps.y);
		fVec2 m	 = -0.5 * ps;
		if(relPos.x < 0.0) {
			ps.x = -(ps.x);
		}
		if(relPos.y < 0.0) {
			ps.y = -(ps.y);
		}
		fVec2 newsize = lsize + ps;
		newsize.x	  = Max<int>(newsize.x, metrics.minSize.x);
		newsize.y	  = Max<int>(newsize.y, metrics.minSize.y);
		SetSize(newsize);
		SetPosition(lwinPos - m);
		spr->SetSize(newsize);
		tr->SetPosition(lwinPos - m);
		//      if (newsize.x != metrics.minSize.x && newsize.y != metrics.minSize.y)
	}
}

UICursor::UICursor(UIWindow* w) { SetWindow(w); }

void UICursor::SetWindow(UIWindow* w) { win = w; }

void UICursor::SetPos(const fVec2& p) { pos = p; }

void UICursor::SetCursorTopLeftWin() {
	fVec2 s = win->GetSize();
	pos.x	= -s.x * 0.5;
	pos.y	= s.y * 0.5;
	origin	= pos;
}

void UICursor::Advance(const fVec2& p) {
	pos.x += p.x + win->metrics.itemSpacing.x;
	pos.y += p.y;
	CalcAdvanceBr();
}

bool UICursor::CalcNextPosition(const fVec2 offset) {
	fVec2 lpos = pos;
	fVec2 temp = pos + offset;
	pos		   = temp;
	// CalcAdvanceBr();
	if(pos == temp) {
		pos = lpos;
		return 0;
	}
	return 1;
}

void UICursor::CalcAdvanceBr() {
	if(strat == CurAdvanceStrat::None)
		return;
	if(strat == CurAdvanceStrat::BreakOnHorizontalEnd) {
		if(pos.x < win->GetSize().x * 0.5)
			return;
		pos.x = origin.x + win->metrics.itemSpacing.x;
		pos.y -= lineBreakSize;
		pos.y -= win->metrics.itemSpacing.y;
	}
}

void UICursor::SetLineBreakSize(const float v) { lineBreakSize = v; }

fVec2 UICursor::GetAbsolutePos() { return win->GetPosition() + pos; }

fVec2* UICursor::GetPos() { return &pos; }
