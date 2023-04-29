#include "player.h"
#include "Inputs.h"
#include "Components.h"
#include "Scene.h"

void player::Update() {
	Transform* transform = this->goc->GetComponent<Transform>();
	speed     = 500.0f;
	if (Inputs::space)
		speed = 2500.0f;
	fVec2 direction		 = fVec2((int)Inputs::right - (int)Inputs::left, 
							(int)Inputs::up	   - (int)Inputs::down);
	transform->position = transform->position + direction * speed * Globals::deltaTime;

}


void player::Gui() {
	//NWGui::DragValue("Speed", &this->speed, ImGuiDataType_Float);
}

Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};