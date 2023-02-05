#include "player.h"
#include "Inputs.h"
#include "Transform.h"
#include "Components.h"


void player::Update() {
	fVec2 direction = iVec2(Inputs::right - Inputs::left,
					   Inputs::up - Inputs::down).normalize();

	Transform* trans = this->goc->GetComponent<Transform>();
	trans->position  = trans->position + (direction * 200.0 * Globals::deltaTime);
	
}


Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};