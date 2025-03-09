#include "CoordSys.h"
#include "Camera.h"

namespace CoordSys {

//we suppose we have only orthographic projection for now
fVec2 ScrToWorld(const fVec2& p) {
    Camera* cam = Camera::GetActiveCamera();
    return cam->GetPosition() + p * cam->GetSize();
}

fVec2 ScrZeroOneToWorld(const fVec2& p) {
    return ScrToWorld(p * 2.0f + fVec2(1.0f,1.0f));
}

fVec2 WorldToScrZeroOne(const fVec2& p) {
    Camera* cam = Camera::GetActiveCamera();
    return (p - cam->GetPosition()) / cam->GetSize();
}

fVec2 WorldToScr(const fVec2& p) {
    return WorldToScrZeroOne(p) - fVec2(0.5,0.5);
}

};
