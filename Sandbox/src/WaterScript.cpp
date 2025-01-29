#include "Scripts.h"
#include "Scene.h"
#include "Components.h"
#include "Inputs.h"
#include "AudioLib.h"



#include "NWTime.h"

void WaterScript::Start() {
	//test();
	Scene* s    = Scene::GetCurrent();
	Camera& cam = *(s->GetGameObject("CameraObj")->GetComponent<Camera>());
	auto spr = this->goc->AddComponent<Sprite>();
	auto tr  = this->goc->AddComponent<Transform>();
	this->goc->AddComponent<AudioListener>();
	spr->SetShader("../Sandbox/assets/Shaders/Water.shader");
	spr->SetTexture(&cam.fbo.GetAtt(0).tex);
	spr->SetSortingLayer(WATER_LAYER);
	tr->position.y        = - 0.7 * spr->container.height + 0.35 * spr->container.height;
	spr->container.UpdateSize(spr->container.width, spr->container.height * 0.3);
}

static float t = 0.0;
void WaterScript::Update() {
	auto spr = this->goc->AddComponent<Sprite>();
	t += NWTime::GetDeltaTime();
	spr->shader->Use();
	spr->shader->SetUniform1f("uTime", t);
	spr->shader->Unuse();
}

