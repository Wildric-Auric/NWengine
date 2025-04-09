#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"
#include "ComponentTypes.h"

class LineRenderer : public GameComponent {
    public:
        NW_ST_GET_TYPE_IMPL(LineRenderer);

        fVec2 _start;
        fVec2 _end;
        float _width = 4.0;

        LineRenderer() {};
        LineRenderer(GameObject*);

        void SetExt(const fVec2& start, const fVec2& end);
        void SetWidth(float);
};
