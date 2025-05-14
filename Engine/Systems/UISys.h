#pragma once
#include "Camera.h"
#include "keyboard.h"
#include "Shader.h"

class UIWindow;

enum class UISysCursorState { NONE = 0, RESIZE = 1 };

struct UILayerConsts {
	int64 baseWindowLayer = 0xFFFFFFFF00000000;
	int64 windowRange	  = 0x10000;
};

class UISys {
  public:
	static UILayerConsts	layerConsts;
	static GameObject		camContainer;
	static fVec2			curPos;
	static int				curStatePriority;
	static UISysCursorState curState;
	static bool				clickEvent;
    static bool             clickContinuousEvent;
	static bool				isResposive;
	static NWin::Key		clickKey;
	static UIWindow*		focusedWindow;
	static UIWindow*		hoveredWindow;
	static UIWindow*		topMostSelected;
	static UIWindow*		topMostHovered;

	static void	 Init();
	static void	 Update();
	static void	 Destroy();
	static void	 SetCursorState(UISysCursorState s = UISysCursorState::NONE, int priority = 0);
	static void	 Focus(UIWindow*);
	static void	 ForceFocus(UIWindow*);
	static void	 Hover(UIWindow*);
	static void	 ForceHover(UIWindow*);
	static void	 UnFocus();
	static bool	 GetClickEvent();
    static bool  GetIsClicking();
	static fVec2 GetCurPos();
	static int64 GetAvailableLayer();
};
