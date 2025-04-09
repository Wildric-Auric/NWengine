#pragma once
#include "Globals.h"

namespace NWCoordSys {
    struct BoundingBox {
        fVec2 center;
        fVec2 size;
    };

    fVec2 ViewportToWorld(const fVec2&);
    fVec2 ViewportZeroOneToWorld(const fVec2&);
    fVec2 WorldToViewport(const fVec2&);
    fVec2 WorldToViewportZeroOne(const fVec2&);
};
