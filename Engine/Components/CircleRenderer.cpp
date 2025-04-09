#include "CircleRenderer.h" 
#include "DefaultAssets.h"
#include "Components.h"

CircleRenderer::CircleRenderer(GameObject* go) {
    attachedObject = go;
    NW_REQUIRE_COMP(attachedObject, Transform);
    Sprite* spr = NW_REQUIRE_COMP(attachedObject, Sprite);
    spr->SetShader(NW_DEFAULT_SHADER_CIRCLE);
    spr->SetTexture(NW_DEFAULT_TEXTURE);
    SetRadius(64.0);
}

void CircleRenderer::SetRadius(float r) {
    Transform* tr = attachedObject->GetComponent<Transform>();
    Sprite* spr   = attachedObject->GetComponent<Sprite>();
    float d = 2.0f * r;
    tr->SetScale({1.0f,1.0f});
    spr->SetSize({d,d});
}

void CircleRenderer::SetPosition(const fVec2& pos) {
    Transform* tr = attachedObject->GetComponent<Transform>();
    tr->position = pos;
}

void CircleRenderer::SetRenderingAA(float value) {
    _aaValue  = (value / 2.0);
    _aaValue *= _aaValue;
    Sprite* spr = attachedObject->GetComponent<Sprite>();
    spr->shader->Use();
    spr->shader->SetUniform1f("uAA",_aaValue);
}

float CircleRenderer::GetRadius() {
    Sprite* spr   = attachedObject->GetComponent<Sprite>();
    return spr->container.width * 0.5;
}
