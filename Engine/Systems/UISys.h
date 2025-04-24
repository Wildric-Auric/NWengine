#pragma once
#include "Camera.h"
#include "UISys.h"
#include "keyboard.h"

class UIWindow;


enum class UISysCursorState {
   NONE = 0,
   RESIZE = 1
};

class UISys {
    public:
    static GameObject camContainer;
    static fVec2 curPos;
    static int       curStatePriority;
    static UISysCursorState curState;
    static bool      clickEvent;
    static bool      isResposive;
    static NWin::Key clickKey; 
    static UIWindow* focusedWindow;
    static UIWindow* hoveredWindow;
    static UIWindow* topMostSelected;
    static UIWindow* topMostHovered;

    static void Init();
    static void Update();
    static void Destroy();
    static void SetCursorState(UISysCursorState s =  UISysCursorState::NONE, int priority = 0);
    static void Focus(UIWindow*);
    static void ForceFocus(UIWindow*);
    static void Hover(UIWindow*);
    static void ForceHover(UIWindow*);
    static void UnFocus();
    static bool GetClickEvent(); 
    static fVec2 GetCurPos();
};
