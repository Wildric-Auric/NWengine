#include "PlayerController.h"
#include "Components.h"
#include "Collider.h"
#include "Inputs.h"
#include "NWTime.h"
#include "Scene.h"

void PlayerController::Start() {
    Scene* s = Scene::GetCurrent();
    gnd =  s->GetGameObject("gndObj")->Get<Collider>();
}

void PlayerController::Update() {
    Collider&  col = *goc->Get<Collider>();
    Transform& tr  = *goc->Get<Transform>();
    v2i mov; 
    mov.x = Inputs::GetInputKeyPressed(NWInputKey_Right) - Inputs::GetInputKeyPressed(NWInputKey_Left);
    mov.y = Inputs::GetInputKeyPressed(NWInputKey_Up) - Inputs::GetInputKeyPressed(NWInputKey_Down);
    mov   = mov.normalize();
    tr.position.x += mov.x * NWTime::GetDeltaTime() * 300;
    tr.position.y += mov.y * NWTime::GetDeltaTime() * 300;
    v2f d;
    bool isc = col.isColliding(gnd, &d);
    tr.position = isc ? tr.position + d : tr.position;
}
