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
    v2i   mov; 
    mov.x = Inputs::GetInputKeyPressed(NWInputKey_Right) - Inputs::GetInputKeyPressed(NWInputKey_Left);
    mov.y = Inputs::GetInputKeyPressed(NWInputKey_Up) - Inputs::GetInputKeyPressed(NWInputKey_Down);
    if (isGrounded) {
        if (mov.y == 1.0f) {
            grav = 1400.0; 
        }
        else {
            grav = 0.0;
        }
    }
    grav  = Max(grav - NWTime::GetDeltaTime() * 6000.0, -6000.0);
    tr.position.x += mov.x * NWTime::GetDeltaTime() * 300;
    tr.position.y += NWTime::GetDeltaTime() * grav;
    v2f d;
    bool isc   = col.isColliding(gnd, &d);
    tr.position = isc ? tr.position + d : tr.position; 
    col.offset.y = -1.0;
    isGrounded   = col.isColliding(gnd, 0);
    col.offset.y = 0.0;
}
