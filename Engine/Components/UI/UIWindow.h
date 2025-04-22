#pragma once
#include "GameObject.h"

enum class NWUiWindowState {
    NONE,
    MOVE,
    RESIZE,
    RESIZE_X,
    RESIZE_Y,
};

struct UIWindowMetrics {
    int titleBarHeight = 20;
    int resizeAreaWidth = 5;
};

class UIWindow : public GameComponent {
    public:
    NW_ST_GET_TYPE_IMPL(UIWindow);
    void OnAdd() override;
    UIWindow() = default;
    UIWindow(GameObject*);

    void Update() override;

    int IsCursorOnTitleBar();
    int IsCursorOnResize();
    int IsCursorOnWindow();

    NWUiWindowState state = NWUiWindowState::NONE;
    fVec2 relPos;
    fVec2 rpos;
    fVec2 lsize;
    fVec2 lpos;
    fVec2 lwinPos;

    UIWindowMetrics metrics;
};
