#include "UISys.h"
#include "Inputs.h"
#include "window.h"
#include "UIWindow.h"
#include "CoordSys.h"
#include "Sprite.h"
#include "Scene.h"


GameObject UISys::camContainer;
fVec2 UISys::curPos;
bool  UISys::clickEvent                 = 0;
bool  UISys::isResposive                = 1;
UIWindow* UISys::focusedWindow          = 0;
UIWindow* UISys::topMostSelected        = 0;
NWin::Key UISys::clickKey               = NWin::Key::NWIN_KEY_LBUTTON;

int UISys::curStatePriority = 0;
UISysCursorState UISys::curState = UISysCursorState::NONE;

UISysCursorState curState;

void UISys::SetCursorState(UISysCursorState s, int priority) {
    //if (state :e)
}

void UISys::Init() {

}

void UISys::Update() {
    if (!isResposive) return;
    NWin::Window* win = ((NWin::Window*)(Context::window));
    //TODO::World to viewport??
    curPos     = Inputs::GetMousePosition();
    curPos     = NWCoordSys::WorldToViewportNonNormalized(curPos);
    clickEvent = win->_getKeyboard().onKeyPress(clickKey);

    if (topMostSelected && focusedWindow != topMostSelected) {
        focusedWindow = topMostSelected;
        mapProc mapProc = [](GameObject* obj, void* d) -> int {
            UIWindow* win = (UIWindow*)d;
            Sprite* spr = obj->GetComponent<Sprite>();
            int newlayer = 0;
            if (obj == win->attachedObject) {
                newlayer = -100;
            }
            else {
                newlayer = spr->sortingLayer + 1;
            }
            spr->SetSortingLayer(newlayer);
            return 0; 
        };
        Scene::GetCurrent()->CacheMap(UIWindow::CacheConditionHasUIWindow,  mapProc, focusedWindow);
    }
    topMostSelected = 0;
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

