#include "UISys.h"
#include "CoordSys.h"
#include "Inputs.h"
#include "Scene.h"
#include "Sprite.h"
#include "UIWindow.h"
#include "window.h"

// Range of each window layers is of 0x10000 = 65536
// r:      b ---> l: b + 0x10000
// b is base, l layer is allocated is allocated to the window itself,
// other layers are allocated to the window content.
//

GameObject	  UISys::camContainer;
fVec2		  UISys::curPos;
bool		  UISys::clickEvent			  = 0;
bool		  UISys::clickContinuousEvent = 0;
bool		  UISys::isResposive		  = 1;
NWin::Key	  UISys::clickKey			  = NWin::Key::NWIN_KEY_LBUTTON;
UIWindow*	  UISys::focusedWindow		  = 0;
UIWindow*	  UISys::topMostSelected	  = 0;
UIWindow*	  UISys::hoveredWindow		  = 0;
UIWindow*	  UISys::topMostHovered		  = 0;
UILayerConsts UISys::layerConsts;

int				 UISys::curStatePriority = 0;
UISysCursorState UISys::curState		 = UISysCursorState::NONE;

UISysCursorState curState;

void UISys::SetCursorState(UISysCursorState s, int priority) {}

void UISys::Init() {}

void UISys::UnFocus() { focusedWindow = 0; }

int64 UISys::GetAvailableLayer() {
	auto f	 = Scene::GetCurrent()->cache.find(UIWindow::CacheConditionHasUIWindow);
	int	 num = f->second.size();
	return UISys::layerConsts.baseWindowLayer + UISys::layerConsts.windowRange * (num + 1);
}

void UISys::Update() {
	if(!isResposive)
		return;
	curPos			  = Inputs::GetMousePosition();
	fVec2 scr;
	Context::GetWinDrawAreaSize(&scr);
	curPos				 = NWCoordSys::WorldToViewportNonNormalized((NWCoordSys::ScreenNonNormalizedToWorld(curPos)));
	clickEvent			 = Inputs::GetInputOnKeyPress(clickKey);
	clickContinuousEvent = Inputs::GetInputKeyPressed(clickKey);
	//-----------Set focus and hover logic-----------
	struct MapProcData {
		UIWindow* win;
		int64	  refLayer;
	};

	if(clickEvent && topMostSelected == 0) {
		UnFocus();
	}
	if(topMostSelected && focusedWindow != topMostSelected) {
		focusedWindow	= topMostSelected;
		mapProc mapProc = [](GameObject* obj, void* d) -> int {
			UIWindow* win = ((MapProcData*)d)->win;
			int64	  ref = ((MapProcData*)d)->refLayer;
			Sprite*	  spr = obj->GetComponent<Sprite>();
			if(obj == win->attachedObject) {
				spr->SetSortingLayerFull(UISys::layerConsts.baseWindowLayer);
				return 1;
			}
			if(spr->GetSortingLayer() >= ref)
				return 0;
			int64 newlyr = spr->sortingLayer + UISys::layerConsts.windowRange;
			spr->SetSortingLayerFull(newlyr);
			return 0;
		};
		MapProcData d = {focusedWindow, focusedWindow->attachedObject->GetComponent<Sprite>()->GetSortingLayer()};
		Scene::GetCurrent()->CacheMap(UIWindow::CacheConditionHasUIWindow, mapProc, &d);
	}
	hoveredWindow	= topMostHovered;
	topMostHovered	= 0;
	topMostSelected = 0;
	//---------Cursor type logic-----------
	bool iswinfr = 1;
	if(focusedWindow)
		iswinfr = focusedWindow->GetState() == 0;
#ifdef PLTFRM_WIN32 
	NWin::Window* win = ((NWin::Window*)(Context::window));
	win->setCursor(NWin::CursorIcon::ARROW);
#endif
	if(focusedWindow) {
		UIWindowStateField st = focusedWindow->GetState();
		bool			   we = st & (Window_State_ResizeXL | Window_State_ResizeXR);
		bool			   ns = st & (Window_State_ResizeYD | Window_State_ResizeYU);
		bool			   ne = (st & Window_State_ResizeXR) && (st & Window_State_ResizeYU);
		bool			   nw = (st & Window_State_ResizeXL) && (st & Window_State_ResizeYU);
		bool			   se = (st & Window_State_ResizeXR) && (st & Window_State_ResizeYD);
		bool			   sw = (st & Window_State_ResizeXL) && (st & Window_State_ResizeYD);
#ifdef PLTFRM_WIN32 
		if(ne || sw)
			win->setCursor(NWin::CursorIcon::RESIZE_DIAG_RIGHT);
		else if(nw || se)
			win->setCursor(NWin::CursorIcon::RESIZE_DIAG_LEFT);
		else if(we)
			win->setCursor(NWin::CursorIcon::RESIZE_HORIZONTAL);
		else if(ns)
			win->setCursor(NWin::CursorIcon::RESIZE_VERT);
#endif
	}
	if(hoveredWindow) {
		int cur = hoveredWindow->IsCursorOnResize();
		if(iswinfr && cur != 0) {
#ifdef PLTFRM_WIN32 
			win->setCursor((NWin::CursorIcon)cur);
#endif
		}
	}
}

bool UISys::GetClickEvent() { return clickEvent; }
bool UISys::GetIsClicking() { return clickContinuousEvent; }

fVec2 UISys::GetCurPos() { return curPos; }

void UISys::Hover(UIWindow* win) {
	if(topMostHovered == 0) {
		topMostHovered = win;
		return;
	}
	int refLayer   = topMostHovered->attachedObject->GetComponent<Sprite>()->sortingLayer;
	int layer	   = win->attachedObject->GetComponent<Sprite>()->sortingLayer;
	topMostHovered = (layer <= refLayer) ? win : topMostHovered;
}

void UISys::ForceHover(UIWindow* win) { topMostHovered = win; }

void UISys::Focus(UIWindow* win) {
	if(topMostSelected == 0) {
		topMostSelected = win;
		return;
	}
	int refLayer	= topMostSelected->attachedObject->GetComponent<Sprite>()->sortingLayer;
	int layer		= win->attachedObject->GetComponent<Sprite>()->sortingLayer;
	topMostSelected = (layer <= refLayer) ? win : topMostSelected;
}

void UISys::ForceFocus(UIWindow* win) { topMostSelected = win; }

void UISys::Destroy() {}
