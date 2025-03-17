#include "TriangleRenderer.h"
#include "Sprite.h"
#include "Transform.h"
#include "DefaultAssets.h"


TriangleRenderer::TriangleRenderer(GameObject* go) {
    attachedObject = go;
}

void TriangleRenderer::OnAdd() {
    Sprite* spr = attachedObject->AddComponent<Sprite>();
    attachedObject->AddComponent<Transform>();
    attachedObject->SetDrawCallback(TriangleDrawCallback);
    spr->SetShader(NW_DEFAULT_SHADER_TRIANGLE);
    spr->SetSize({1.0f,1.0f});
}

int TriangleRenderer::TriangleDrawCallback(void* data) {
    GameObject* obj = (GameObject*)data;
    Sprite*     spr = obj->GetComponent<Sprite>();
    TriangleRenderer* triR = obj->GetComponent<TriangleRenderer>();
    Sprite::PrepDefaultDrawCallback(data);
    spr->GetShader()->Use();
    spr->GetShader()->SetUniformArray2f("uVert", (float*)(&triR->_coord), 3);
    triR->_triangle.Draw(); 
    spr->GetShader()->Unuse();
    return spr->sortingLayer;
}

void TriangleRenderer::SetBaseLeft(const fVec2& v) {
    _coord.bl = v;
}

void TriangleRenderer::SetBaseRight(const fVec2& v) {
    _coord.br = v;
}

void TriangleRenderer::SetTop(const fVec2& v) {
    _coord.up = v;
}

void TriangleRenderer::SetCoord(const TriangleCoord& v) {
   _coord = v; 
}
