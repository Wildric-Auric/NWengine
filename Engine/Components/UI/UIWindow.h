#pragma once
#include "GameObject.h"

enum class UIWindowState {
    NONE,
    MOVE,
    RESIZE,
    RESIZE_X,
    RESIZE_Y,
};

struct UIWindowMetrics {
    int titleBarHeight = 20;
    int resizeAreaWidth = 5;
    int borderWidth     = 2;
};

class UIWindow : public GameComponent {
    public:
    NW_ST_GET_TYPE_IMPL(UIWindow);
    void OnAdd() override;
    void OnDelete() override;
    UIWindow() = default;
    UIWindow(GameObject*);

    void Update() override;

    int IsCursorOnTitleBar();
    int IsCursorOnResize();
    int IsCursorOnWindow();
    bool IsFocused();
    UIWindowState GetState(); 
    void SetShaderParams();

    UIWindowState state = UIWindowState::NONE;
    fVec2 relPos;
    fVec2 rpos;
    fVec2 lsize;
    fVec2 lpos;
    fVec2 lwinPos;

    fVec4 bgCol = fVec4(1.0,1.0,1.0,1.0);
    bool _tmpisFocused = 0;

    UIWindowMetrics metrics;

    static bool CacheConditionHasUIWindow(GameObject* obj);
};
