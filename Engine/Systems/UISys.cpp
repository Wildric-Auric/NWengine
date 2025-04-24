#include "UISys.h"
#include "Inputs.h"
#include "window.h"
#include "UIWindow.h"
#include "CoordSys.h"
#include "Sprite.h"
#include "Scene.h"


GameObject UISys::camContainer;
fVec2      UISys::curPos;
bool       UISys::clickEvent             = 0;
bool       UISys::isResposive            = 1;
NWin::Key  UISys::clickKey               = NWin::Key::NWIN_KEY_LBUTTON;
UIWindow*  UISys::focusedWindow          = 0;
UIWindow*  UISys::topMostSelected        = 0;
UIWindow*  UISys::hoveredWindow          = 0;
UIWindow*  UISys::topMostHovered         = 0;

int UISys::curStatePriority = 0;
UISysCursorState UISys::curState = UISysCursorState::NONE;

UISysCursorState curState;

void UISys::SetCursorState(UISysCursorState s, int priority) {
    //if (state :e)
}

void UISys::Init() {

}

void UISys::UnFocus() {
    focusedWindow = 0;
}

void UISys::Update() {
    if (!isResposive) return;
    NWin::Window* win = ((NWin::Window*)(Context::window));
    //TODO::World to viewport??
    curPos     = Inputs::GetMousePosition();
    curPos     = NWCoordSys::WorldToViewportNonNormalized(curPos);
    clickEvent = win->_getKeyboard().onKeyPress(clickKey);
    //-----------Set focus and hover logic-----------
    if (clickEvent && topMostSelected == 0) {
        UnFocus();
    }
    if (topMostSelected && focusedWindow != topMostSelected) {
        focusedWindow = topMostSelected;
        mapProc mapProc = [](GameObject* obj, void* d) -> int {
            UIWindow* win = (UIWindow*)d;
            Sprite* spr = obj->GetComponent<Sprite>();
            int newlayer = obj == win->attachedObject ? -100 : spr->sortingLayer + 1;
            spr->SetSortingLayer(newlayer);
            return 0; 
        };
        Scene::GetCurrent()->CacheMap(UIWindow::CacheConditionHasUIWindow,  mapProc, focusedWindow);
    }
    hoveredWindow = topMostHovered; 
    topMostHovered  = 0;
    topMostSelected = 0;
    //---------Cursor type logic-----------
    win->setCursor(NWin::CursorIcon::ARROW);
    if (focusedWindow && focusedWindow->GetState() == UIWindowState::RESIZE) {
        win->setCursor(NWin::CursorIcon::RESIZE_WE);
    }
    else if (hoveredWindow && hoveredWindow->IsCursorOnResize()) {
        win->setCursor(NWin::CursorIcon::RESIZE_WE);
    }
}

bool UISys::GetClickEvent() {
    return clickEvent;
}

fVec2 UISys::GetCurPos() {
    return curPos;
}


void UISys::Hover(UIWindow* win) {
    if (topMostHovered == 0) {
        topMostHovered = win;
        return;
    }
    int refLayer = topMostHovered->attachedObject->GetComponent<Sprite>()->sortingLayer;
    int layer    = win->attachedObject->GetComponent<Sprite>()->sortingLayer;
    topMostHovered = (layer <= refLayer) ?  win : topMostHovered;
}

void UISys::ForceHover(UIWindow* win) {
    topMostHovered = win;
}

void UISys::Focus(UIWindow* win) {
    if (topMostSelected == 0) {
        topMostSelected = win;
        return;
    }
    int refLayer = topMostSelected->attachedObject->GetComponent<Sprite>()->sortingLayer;
    int layer    = win->attachedObject->GetComponent<Sprite>()->sortingLayer;
    topMostSelected = (layer <= refLayer) ?  win : topMostSelected;
}

void UISys::ForceFocus(UIWindow* win) {
    topMostSelected = win;
}

void UISys::Destroy() {
}

