#include "CoordSys.h"
#include "Camera.h"
#include "Renderer.h"

namespace NWCoordSys {

//we suppose we have only orthographic projection for now
fVec2 WorldToScreenNonNormalized(const fVec2& p) {
    Camera* cam = Camera::GetActiveCamera();
    return (p - cam->GetPosition());
}

fVec2 ScreenToWorld(const fVec2& p) {
    Camera* cam = Camera::GetActiveCamera();
    fVec2 size;
    Context::GetWinDrawAreaSize(&size);
    return cam->GetPosition() + p * size;
}

fVec2 WorldToScreen(const fVec2& p) {
    fVec2 size;
    Context::GetWinDrawAreaSize(&size);
    return WorldToScreenNonNormalized(p) / size;
}

fVec2 ViewportToWorld(const fVec2& p) {
    fVec2 stretch = Renderer::currentRenderer->stretchCoeff;
    Camera* cam = Camera::GetActiveCamera();
    return cam->GetPosition() + p * cam->size * stretch;
}

fVec2 WorldToViewportNonNormalized(const fVec2& p) { 
    fVec2 stretch = Renderer::currentRenderer->stretchCoeff;
    return WorldToScreenNonNormalized(p)/stretch;
}

fVec2 WorldToViewport(const fVec2& p) {
    fVec2 size = Camera::GetActiveCamera()->GetSize();
    return WorldToViewportNonNormalized(p)/size;
}

};
