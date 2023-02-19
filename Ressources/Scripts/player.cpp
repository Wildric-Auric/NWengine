#include "player.h"
#include "Inputs.h"
#include "Components.h"
#include "imgui/imgui.h"
#include "Scene.h"

void player::Update() {
	Transform* transform = goc->GetComponent<Transform>();

	Collider*  collider = goc->GetComponent<Collider>();
	GameObject* obj		= Scene::currentScene->GetGameObject("Ghost");
	Collider* other		= obj->GetComponent<Collider>();

	collider->Start();

	int xDir			= Inputs::right - Inputs::left;
	int yDir			= Inputs::up - Inputs::down;
	fVec2 direction     = fVec2(xDir, yDir).normalize();
	
	if (yDir)
		transform->rotation = 90.0f * yDir;

	if (xDir) {
		transform->scale.x = 0.05f * xDir;
		transform->rotation = 0.0f;
	}

	if (collider->isColliding(other)) {
		obj->GetComponent<Transform>()->position.x = 10000;
	}
	
	transform->position = transform->position + direction * speed * Globals::deltaTime;


}


void player::Gui() {
	NWGui::DragValue("Speed", &this->speed, ImGuiDataType_Float);
}

Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};