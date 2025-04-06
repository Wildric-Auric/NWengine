#pragma once
#include "Globals.h"
#include "GameObject.h"

class CircleRenderer : public GameComponent {
    public:
    float _aaValue = 0.0;

    CircleRenderer() {};
    CircleRenderer(GameObject*);
    
    void SetRadius(float);
    void SetRenderingAA(float);
    void SetPosition(const fVec2&);
    float GetRadius();
};
