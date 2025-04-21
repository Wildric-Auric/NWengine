#pragma once
#include "GameObject.h"

class UIWindow : public GameComponent {
    public:
    NW_ST_GET_TYPE_IMPL(UIWindow);
    void OnAdd() override;
    UIWindow() = default;
    UIWindow(GameObject*);

    void Update() override;

    int state = 0;
    fVec2 relPos;
    fVec2 lsize;
    fVec2 lpos;
};
