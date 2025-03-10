#include "LineRenderer.h"
#include "DefaultAssets.h"
#include "Components.h"


LineRenderer::LineRenderer(GameObject* go) {
    attachedObject = go;
    attachedObject->AddComponent<Transform>();
    Sprite* spr = attachedObject->AddComponent<Sprite>();
    spr->SetShader(NW_DEFAULT_SHADER);
    spr->SetTexture(NW_DEFAULT_TEXTURE);
    SetExt(_start, _end);
    SetWidth(_width);
}

void LineRenderer::SetExt(const fVec2& start, const fVec2& end) {
    Sprite* spr       = attachedObject->GetComponent<Sprite>();
    Transform* tr     = attachedObject->GetComponent<Transform>();
    _start = start;
    _end   = end;
    tr->SetRotation(fVec2(1.0,0.0).GetAngle((end - start).normalize()));
    float magn        = (end - start).magnitude();
    spr->container.width = magn;
    tr->position = start;
    tr->position.x += 0.5 * magn;
    tr->SetRotationAnchor(fVec2(-magn*0.5,0.0));
}

void LineRenderer::SetWidth(float w) {
    Sprite* spr           = attachedObject->GetComponent<Sprite>();
    _width = w;
    spr->container.height = w;
}
