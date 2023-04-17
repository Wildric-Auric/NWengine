#include "PostProcessing.h"
#include "Inputs.h"
#include "Components.h"
#include "Scene.h"

void PostProcessing::Start() {
	container = GameObject();
    renderer  = container.AddComponent<Renderer>();
	renderer->shaderName = "Shaders\\CRT.shader";
}

void PostProcessing::Update() {	
	Camera* temp = Camera::ActiveCamera;
	renderer->Use();
}

void PostProcessing::Gui() {
	//NWGui::DragValue("Speed", &this->speed, ImGuiDataType_Float);
}

PostProcessing::~PostProcessing() {
	container.DeleteComponents();
}

Scriptable* PostProcessing::GetScript(GameObject* goc) {
	return new PostProcessing(goc);
};