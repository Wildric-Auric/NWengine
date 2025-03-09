#pragma once
#include "Globals.h"

namespace NWCoordSys {
    struct BoundingBox {
        fVec2 center;
        fVec2 size;
    };

    fVec2 ScrToWorld(const fVec2&);
    fVec2 ScrZeroOneToWorld(const fVec2&);
    fVec2 WorldToScr(const fVec2&);
    fVec2 WorldToScrZeroOne(const fVec2&);
};
