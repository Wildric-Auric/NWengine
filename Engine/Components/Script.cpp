#include "Script.h"
#include "Utilities.h"

#include "Context.h"
#include "Camera.h"
#include "Transform.h"
#include "Sprite.h"

Script::Script(GameObject* obj) {
	this->attachedObject = obj;
}

void Script::Start() {
	if (this->script == nullptr) return;
	this->script->Start();
}

void Script::Update() {
	if (this->script == nullptr) return;
	this->script->Update();
}

//void Script::SetScript(Scriptable* scr) {
//	this->script = scr;
//}

void Scriptable::ShaderCode(void* sprite) {
	Sprite* sprite0 = (Sprite*)sprite;
	Transform* transform = goc->GetComponent<Transform>();
	fVec2 position = transform->position;
	fVec2 scale = transform->scale;

	sprite0->shader->Use();
	sprite0->shader->SetUniform1i("uTex0", 0);
	Matrix4<float> model(1.0f);
	ScaleMat(model, fVec3(scale.x * sprite0->container.width, scale.y * sprite0->container.height, 1.0f));
	RotateMat(model, transform->rotation, fVec3(0.0f, 0.0f, 1.0f));
	TranslateMat(model, fVec3(transform->position.x, transform->position.y, sprite0->zbuffer));
	sprite0->shader->SetMat4x4("uMvp", &(Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model).values[0]);
}


