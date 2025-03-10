#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"

class LineRenderer : public GameComponent {
    public:
        static std::string GetType() {return "LineRenderer";}

        fVec2 _start;
        fVec2 _end;
        float _width = 4.0;

        LineRenderer() {};
        LineRenderer(GameObject*);

        void SetExt(const fVec2& start, const fVec2& end);
        void SetWidth(float);
};
