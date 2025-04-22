#pragma once
#include "GameObject.h"

enum class NWUiWindowState {
    NONE,
    MOVE,
    RESIZE,
    RESIZE_X,
    RESIZE_Y,
};

class UIWindow : public GameComponent {
    public:
    NW_ST_GET_TYPE_IMPL(UIWindow);
    void OnAdd() override;
    UIWindow() = default;
    UIWindow(GameObject*);

    void Update() override;

    NWUiWindowState state = NWUiWindowState::NONE;
    fVec2 relPos;
    fVec2 lsize;
    fVec2 lpos;
    fVec2 lwinPos;
};
