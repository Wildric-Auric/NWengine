#include "Player.h"
#include "Inputs.h"
#include "Components.h"
#include "imgui/imgui.h"
#include "Scene.h"

void Player::Update() {
	//movement
	if (this->goc->name == "Player2")
		direction = Inputs::up - Inputs::down;
	else 
		direction = Inputs::d - Inputs::space;

	Transform* transform = this->goc->GetComponent<Transform>();
	transform->position.y += direction * Globals::deltaTime * speed;
	

	//Collisions
	GameObject* wallUp   = Scene::currentScene->GetGameObject("WallUp");
	GameObject* wallDown = Scene::currentScene->GetGameObject("WallDown");

	if ((wallUp == nullptr) || (wallDown == nullptr))
		return;
	Collider* col0 = wallUp->GetComponent<Collider>();
	Collider* col1 = wallUp->GetComponent<Collider>();

	fVec2 depth;

	col0->Start();
	col1->Start();
	 
	Collider* collider = this->goc->GetComponent<Collider>();

	collider->Start();
	if (collider->isColliding(col0, &depth) || collider->isColliding(col1, &depth)) {
		transform->position = transform->position + depth;
	}

}

void Player::Gui() {
	NWGui::DragValue("Speed", &this->speed, ImGuiDataType_Float);
}

Scriptable* Player::GetScript(GameObject* goc) {
	return new Player(goc);
};