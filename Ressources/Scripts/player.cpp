#include "player.h"
#include "Inputs.h"
#include "Transform.h"
#include "Components.h"
#include "Camera.h"

void player::Update() {
	fVec2 direction = fVec2(Inputs::right - Inputs::left, Inputs::up - Inputs::down).normalize();
	Transform* trans = goc->GetComponent<Transform>();
	trans->position = trans->position + direction * speed * Globals::deltaTime;
}


void player::Gui() {
	NWGui::DragValue<float>("Speed", &speed, ImGuiDataType_Float);
	NWGui::DragValue<float>("Uniform Red", &uniformRed, ImGuiDataType_Float, 1, 0.1, 0.0f, 1.0f);
}

void player::ShaderCode(void* sprite) {
	Scriptable::ShaderCode(sprite);
	((Sprite*)sprite)->shader->SetUniform1f("uRed", this->uniformRed);
}


Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};