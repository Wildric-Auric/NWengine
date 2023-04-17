#include "Ball.h"
#include "Inputs.h"
#include "Components.h"
#include "imgui/imgui.h"
#include "Scene.h"

#include "Player.h"

void Ball::Update() {
	Transform* transform = this->goc->GetComponent<Transform>();
	Collider*  collider  = this->goc->GetComponent<Collider>();
	collider->Start();
	transform->position  = transform->position + direction * speed * Globals::deltaTime;	

	if (transform->position.x > 1080.0 * 0.5 || transform->position.x < -1080.0 * 0.5)
		transform->position = fVec2(0.0f, 0.0f);

	Player* player1		 =  (Player*)Scene::currentScene->GetGameObject("Player1")->GetComponent<Script>()->script;
	Player* player2		 =  (Player*)Scene::currentScene->GetGameObject("Player2")->GetComponent<Script>()->script;

	Collider* pCollider1 =  player1->goc->GetComponent<Collider>();
	Collider* pCollider2 =  player2->goc->GetComponent<Collider>();

	Collider* wallUp   = Scene::currentScene->GetGameObject("WallUp")->GetComponent<Collider>();
	Collider* wallDown = Scene::currentScene->GetGameObject("WallDown")->GetComponent<Collider>();

	fVec2 depth;
	//Wall collision
	if (collider->isColliding(wallUp, &depth) || collider->isColliding(wallDown, &depth)) {
		transform->position = transform->position + depth;
		direction.y = -direction.y;
	}

	//Collision with player1
	if (collider->isColliding(pCollider1, &depth)) {
		transform->position = transform->position + depth;
		direction.x = -direction.x;
		direction.y = direction.y + 0.2 * direction.y * player1->direction;
		direction   = direction.normalize();
	}
	//Collision with player2
	if (collider->isColliding(pCollider2, &depth)) {
		transform->position = transform->position + depth;
		direction.x         = -direction.x;
		direction.y = direction.y + 0.2 * player2->direction;
		direction   = direction.normalize();
	}
}

void Ball::Gui() {
	NWGui::DragValue("direction", &this->direction, ImGuiDataType_Float, 2);
	NWGui::DragValue("Speed", &this->speed, ImGuiDataType_Float);
}

Scriptable* Ball::GetScript(GameObject* goc) {
	return new Ball(goc);
};