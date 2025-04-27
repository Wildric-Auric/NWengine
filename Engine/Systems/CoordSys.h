#pragma once
#include "Globals.h"
/*
Coordinate system are of the following types:
- World coordinates: Coordinates in the world, relative to the camera.
- Screen coordinates: Coordinates relative to the window.
- Viewport coordinates: Coordinates relative to the area where the rendering actually occur.
It is the area covered by the Renderer, taking stretch into account.

Screen Coordinates and viewport coordinates are either non normalized or normalized
when the functions does not specify the type, it is normalized.
Normlization range is either [-0.5,0.5] or [0,1]. When ZeroOne is not specified in the signature,
it is [-0.5,0.5].
The origin in case of zero to one normalization is buttom left, otherwise it is the center of the screen,
the world, or the viewport.
*/

namespace NWCoordSys {
    struct BoundingBox {
        fVec2 center;
        fVec2 size;
    };

    fVec2 WorldToScreenNonNormalized(const fVec2&);
    fVec2 WorldToViewportNonNormalized(const fVec2&);

    fVec2 ScreenNonNormalizedToWorld(const fVec2&);
    fVec2 ScreenToWorld(const fVec2&);
    fVec2 WorldToScreen(const fVec2&);

    fVec2 ViewportToWorld(const fVec2&);
    fVec2 WorldToViewport(const fVec2&);
};
