#include "UIWindow.h"
#include "Context.h"
#include "CoordSys.h"
#include "DefaultAssets.h"
#include "Scene.h"
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "UISys.h"
#include "Utilities.h"

UIColorScheme uiColorSchemePreset_Test = {
	{0, 0, 0, 1},		  // bg;
	{1, 1, 1, 1},		  // fg;
	{1, 1, 1, 1},		  // win;
	{0.5, 0.5, 0.5, 0.5}, // winRest;
	{0.5, 0.5, 0.5, 1.0}, // winHover;
	{1, 1, 1, 1},		  // winSelect;
	{1, 0, 0, 1},		  // winBar;
	{0, 0, 0, 1},		  // winBrdr;
	{0, 0, 1, 1},		  // winBrdrResize;
	{0, 0, 0, 1},		  // text;
	{1, 1, 1, 1},		  // titleText;
};
UIColorScheme uiColorSchemePreset_Light = {};
UIColorScheme uiColorSchemePreset_Dark	= {};
UIColorScheme currentUIColorScheme		= uiColorSchemePreset_Test;

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
	spr->SetShader(NW_INTERNAL_DEFAULT_SHADER_UI_WINDOW);
	Scene::GetCurrent()->AddToCache(UIWindow::CacheConditionHasUIWindow, *attachedObject);
	spr->SetSortingLayerFull(UISys::GetAvailableLayer());
	AddItem(UIItemType_Title, -UISys::layerConsts.windowRange + 1);
	attachedObject->SetDrawCallback(UIWindowDrawCallback);
	itemsHeap.SetUp(UI_ITEM_HEAP_UNIT_SIZE, 128);
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
#define dplmbda [](UIItem * item) -> void
#define gllmbda [](UIItem * item) -> int64
#define gslmbda [](UIItem * item) -> v2f
	switch(type) {
	case UIItemType_TestZone: {
		Sprite* spr2	= item->obj.AddComponents<Sprite, Transform>();
		item->_DrawProc = [](UIItem* item) {
			Sprite* spr2 = item->obj.GetComponent<Sprite>();
			spr2->SetShader(NW_DEFAULT_SHADER_COLORED);
			spr2->GetShader()->Use();
			spr2->GetShader()->SetUniform3f("uCol", 1.0, 0.0, 0.6);
			item->obj.Draw();
		};
		spr2->sortingLayer = spr->sortingLayer + layer;
		item->prop		   = item->prop | ItemProp::Item_Prop_Selectable;
		break;
	}
	case UIItemType_Title: {
		item->_DrawProc = [](UIItem* item) {
			Text* te = item->obj.GetComponent<Text>();
			te->GetShader()->Use();
			te->GetShader()->SetUniform4f("uCol", UNWRP_COL(te->colors));
			te->DirectDraw();
		};
		item->_GetLayerProc = gllmbda { return item->_owner->GetLayer(); };
		item->_GetSizeProc	= gslmbda { return {0.0, 0.0}; };
		item->_UpdateProc	= [](UIItem* item) -> int {
			  Text* te = item->obj.GetComponent<Text>();
			  if(!te->text.size())
				  return 0;
			  fVec2					   s  = item->_owner->GetSize();
			  NWCoordSys::BoundingBox& bb = te->GetBBRef();
			  te->SetPosition(item->_owner->GetPosition() + fVec2(-s.x * 0.5 + bb.size.x * 0.5, s.y * 0.5 - bb.size.y * 0.5));
			  te->UpdateGlyphs(1);
			  item->obj.GetComponent<Text>()->Update();
			  return 0;
		};
		GameObject* obj = &item->obj;
		Text*		te	= obj->AddComponents<Text, Transform>();
		te->SetShader(ShaderTextDefaultStr, &ShaderTextDefaultID);
		te->colors	  = currentUIColorScheme.titleText;
		te->isBatched = false;
		std::string fdir;
		GetSystemFontDir(&fdir);
		fdir += "Arial.ttf";
		te->SetFont({fdir.c_str(), 15}, te->_shader);
		te->layerOrder = spr->sortingLayer - (UISys::layerConsts.windowRange - 1);
		item->endln	   = 0;
		break;
	}
	case UIItemType_Label: {
		item->_GetSizeProc	= gslmbda { return item->obj.GetComponent<Text>()->GetBBRef().size; };
		item->_GetLayerProc = gllmbda { return item->obj.GetComponent<Text>()->layerOrder; };
		item->_UpdateProc	= [](UIItem* item) -> int {
			  Text* te = item->obj.GetComponent<Text>();
			  te->Update();
			  te->UpdateGlyphs(1);
			  te->SetPosition(item->obj.GetComponent<Transform>()->GetPosition());
			  return 0;
		};
		item->_DrawProc = [](UIItem* item) {
			Text* te = item->obj.GetComponent<Text>();
			te->GetShader()->Use();
			te->GetShader()->SetUniform4f("uCol", UNWRP_COL(te->colors)); // TODO::Make this automatic
			te->DirectDraw();
		};
		Text* te = item->obj.AddComponents<Text, Transform>();
		te->SetShader(ShaderTextDefaultStr, &ShaderTextDefaultID);
		te->colors	  = currentUIColorScheme.text;
		te->isBatched = false;
		std::string fdir;
		GetSystemFontDir(&fdir);
		fdir += "Arial.ttf";
		te->SetFont({fdir.c_str(), 15}, te->_shader);
		te->layerOrder = spr->sortingLayer + layer;
		item->endln	   = 0;
		break;
	}
	case UIItemType_Slider: {
		Sprite& spr		  = item->obj.Add<Sprite, Transform>();
		item->_UpdateProc = [](UIItem* item) -> int {
			bool isDragged = item->_owner->draggedItem == item;
			v2f& p		   = item->obj.Get<Transform>()->position;
			v2f& cp		   = UISys::curPos;
			v2f	 s		   = item->GetSize();
			if(isDragged) {
				SetSliderPosX(item, CLAMP(p.x - cp.x, -0.5 * s.x, 0.5 * s.x));
			}
			return 0;
		};
		item->_DrawProc = dplmbda {
			Shader* sh = item->obj.Get<Sprite>()->GetShader();
			sh->Use();
			sh->SetUniform2f("uRes", item->GetSize().x, item->GetSize().y);
			sh->SetUniform4f("uCol", UNWRP_COL(currentUIColorScheme.bg));
			sh->SetUniform1f("uPosX", READ(float, item->data));
			item->obj.Draw();
		};
		spr.SetShader(NW_INTERNAL_DEFAULT_SHADER_UI_SLIDER);
		spr.SetSize({100.0, 20});
		spr.sortingLayer = spr.sortingLayer + layer;
		item->data		 = itemsHeap.Alloc();
		SetSliderPosX(item, 0.0f);
		item->prop |= ItemProp::Item_Prop_Selectable;
		break;
	}
	case UIItemType_Checkbox: {
		Sprite& spr		  = item->obj.Add<Sprite, Transform>();
		item->_UpdateProc = [](UIItem* item) -> int {
			bool isClicked = item->_owner->clickedItem == item;
			v2f	 s		   = item->GetSize();
			if(isClicked) {
				SetCheckBoxState(item, !GetCheckBoxState(item));
			}
			return 0;
		};
		item->_DrawProc = dplmbda {
			Shader* sh = item->obj.Get<Sprite>()->GetShader();
			sh->Use();
			sh->SetUniform1i("uState", GetCheckBoxState(item));
			sh->SetUniform4f("uBgCol", UNWRP_COL(currentUIColorScheme.bg));
			sh->SetUniform4f("uFgCol", UNWRP_COL(currentUIColorScheme.fg));
			item->obj.Draw();
		};
		spr.SetShader(NW_INTERNAL_DEFAULT_SHADER_UI_CHECKBOX);
		spr.sortingLayer = spr.sortingLayer + layer;
		item->data		 = itemsHeap.Alloc();
		SetCheckBoxState(item, 0);
		item->prop |= ItemProp::Item_Prop_Selectable;
		break;
	}
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
	itemsOrd.push_back(item);
	return this->_SetUpItem(item, type, layer);
}

UIItem* UIWindow::AddItem(UIItemType type, int64 layer, uint8 endln) {
	UIItem* it = AddItem(type, layer);
	it->endln  = endln;
	return it;
}

void UIWindow::OnDelete() {
	Scene::GetCurrent()->DeleteFromCache(UIWindow::CacheConditionHasUIWindow, *attachedObject);
	itemsHeap.Clean();
}

UIWindow::UIWindow(GameObject* go) { attachedObject = go; }

int UIWindow::IsCursorOnTitleBar() {
	// dist from top
	if(relPos.y < 0.0)
		return 0;
	return lsize.y * 0.5 - relPos.y <= metrics.titleBarHeight;
}

i32 UIWindow::ComputeResizeState() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	if(!IsCursorOnWindow())
		return 0;
	bool rx = (abs(rpos.x) > (spr->container.width * 0.5 - metrics.resizeAreaWidth));
	bool ry = (abs(rpos.y) > (spr->container.height * 0.5 - metrics.resizeAreaWidth));
	i32	 ret;
	if(rx)
		ret = ret | (UIWindowState::Window_State_RESIZE_X * ((prop & Window_Prop_ResizableX) != 0));
	if(ry)
		ret = ret | (UIWindowState::Window_State_RESIZE_Y * ((prop & Window_Prop_ResizableY) != 0));
	return ret;
}

i32 UIWindow::ComputeMoveState() {
	bool b	 = IsCursorOnTitleBar();
	i32	 ret = 0;
	if(b)
		ret = (Window_State_MOVE_X * ((prop & Window_Prop_MovableX) != 0)) |
			  (Window_State_MOVE_Y * ((prop & Window_Prop_MovableY) != 0));
	return ret;
}

int UIWindow::IsCursorOnResize() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	if(!IsCursorOnWindow())
		return 0;
	bool rx = (abs(rpos.x) > (spr->container.width * 0.5 - metrics.resizeAreaWidth));
	bool ry = (abs(rpos.y) > (spr->container.height * 0.5 - metrics.resizeAreaWidth));
	return (int)rx * ((prop & Window_Prop_ResizableX) != 0) + 2 * (int)ry * ((prop & Window_Prop_ResizableY) != 0);
}

int UIWindow::IsCursorOnWindow() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	return (abs(rpos.x) < spr->container.width * 0.5 && abs(rpos.y) < spr->container.height * 0.5);
}

bool UIWindow::IsFocused() { return UISys::focusedWindow == this; }

i32 UIWindow::GetState() { return state; }

fVec2 UIWindow::GetSize() {
	Sprite* spr = attachedObject->GetComponent<Sprite>();
	return fVec2(spr->container.width, spr->container.height);
}

void UIWindow::SetShaderParams() {
	Sprite* spr	  = attachedObject->GetComponent<Sprite>();
	fVec2	s	  = fVec2(spr->container.width, spr->container.height);
	v4f		bgCol = currentUIColorScheme.winRest;
	if(UISys::focusedWindow == this) {
		bgCol = currentUIColorScheme.winSelect;
	} else if(UISys::hoveredWindow == this) {
		bgCol = currentUIColorScheme.winHover;
	}
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uRes", s.x, s.y);
	spr->GetShader()->SetUniform1f("uTitleHeight", metrics.titleBarHeight);
	spr->GetShader()->SetUniform1f("uBorderWidth", metrics.borderWidth);
	spr->GetShader()->SetUniform4f("uCol", UNWRP_COL(bgCol));
	spr->GetShader()->SetUniform4f("uBarCol", UNWRP_COL(currentUIColorScheme.winBar));
	spr->GetShader()->SetUniform4f("uBorderCol", UNWRP_COL(currentUIColorScheme.winBrdr));
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
	bool m	   = UISys::GetClickEvent();
	bool m1	   = UISys::GetIsClicking();
	v2f	 mp	   = UISys::curPos;
	bool hitbs = 0; // has item been selected this frame?

	cursor.SetCursorTopLeftWin();
	cursor.Advance(fVec2(0.0, -metrics.titleBarHeight - metrics.itemSpacing.y));

	if(!m1)
		draggedItem = 0;
	for(UIItem* it : itemsOrd) {
		UIItem& item = *it;
		v2f		s	 = item.GetSize();
		v2f		cp	 = cursor.GetAbsolutePos();
		v2f		itp	 = cp + fVec2(s.x * 0.5, -s.y * 0.5);
		cursor.CalcNextPosition(s);
		item.obj.GetComponent<Transform>()->SetPosition(itp);

		clickedItem = 0;
		if(m1 && !m && selectedItem == it) { // Items may be dragged without cursor being inside
			draggedItem = selectedItem;
			// printf("Dragged!");
		}
		if(NWCoordSys::IsPointInside(mp, itp, s)) {
			if(m && (it->prop & Item_Prop_Selectable)) {
				selectedItem = it;
				clickedItem	 = it;
				hitbs		 = 1;
				printf("Selected Item, %d\n", it);
			}
			hoveredItem = it;
		}
		item.Update();
		cursor.Advance({s.x, 0.0});
		int e = item.LateUpdate();
		while(e--) {
			cursor.SetCursorOnNextLineBeg();
		}
	}
	if(m && !hitbs)
		selectedItem = 0;

	Sprite*	   spr = attachedObject->GetComponent<Sprite>();
	Transform* tr  = attachedObject->GetComponent<Transform>();
	fVec2	   s   = fVec2(spr->container.width, spr->container.height);
	fVec2	   hs  = 0.5 * fVec2(spr->container.width, spr->container.height);

	rpos = -tr->GetPosition() + UISys::curPos;

	if(IsCursorOnWindow() && m) {
		UISys::Focus(this);
	}
	if(IsCursorOnWindow()) {
		UISys::Hover(this);
	}

	if(state == 0 && m1 && IsCursorOnWindow() && UISys::focusedWindow == this) {
		relPos		 = rpos;
		lsize		 = fVec2(spr->container.width, spr->container.height);
		lpos		 = UISys::curPos;
		lwinPos		 = tr->GetPosition();
		i32 mvState	 = ComputeMoveState();
		i32 resState = ComputeResizeState();
		state		 = state | mvState;
		state		 = state | resState;
	}
	if((state != 0) && !m1) {
		state = 0;
	}
	if(state & (Window_State_MOVE_X | Window_State_MOVE_Y)) {
		tr->SetPosition(UISys::GetCurPos() - relPos);
	}
	if(state & (Window_State_RESIZE_X | Window_State_RESIZE_Y)) {
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
	lineBreakSize = Max(lineBreakSize, offset.y);
	return 1;
}

void UICursor::CalcAdvanceBr() {
	switch(strat) {
	case CurAdvanceStrat::None:
		return;
	case CurAdvanceStrat::BreakOnHorizontalEnd:
		if(pos.x < win->GetSize().x * 0.5)
			return;
		SetCursorOnNextLineBeg();
		break;
	case CurAdvanceStrat::FixedWidth:
		if(pos.x < fixedWidth)
			return;
		SetCursorOnNextLineBeg();
		break;
	}
}

void UICursor::SetCursorOnNextLineBeg() {
	pos.x = origin.x + win->metrics.itemSpacing.x;
	pos.y -= lineBreakSize;
	pos.y -= win->metrics.itemSpacing.y;
	lineBreakSize = 20;
}

void UICursor::SetLineBreakSize(const float v) { lineBreakSize = v; }

fVec2 UICursor::GetAbsolutePos() { return win->GetPosition() + pos; }

fVec2* UICursor::GetPos() { return &pos; }

//-------------------UIManager-------------------

Camera* UIManager::GetCamera() { return _camContainer.GetComponent<Camera>(); }

Camera* UIManager::GetTmpCamera() { return _lastCam; }

void UIManager::OnAdd() { _camContainer.AddComponent<Camera>(); }

void UIManager::Update() {}

void UIManager::Bind() {
	_lastCam = Camera::GetActiveCamera();
	GetCamera()->Use();
}

void UIManager::Unbind() {
	if(_lastCam)
		_lastCam->Use();
}
