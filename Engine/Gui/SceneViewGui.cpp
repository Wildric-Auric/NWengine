#include "imgui/imgui.h"
#include "RessourcesLoader.h"
#include "RuntimeManager.h"
#include "Renderer.h"
#include "SceneViewGui.h"
#include "Console.h"

void SceneViewGui::Init() { this->isActive = 1; }
void SceneViewGui::Show() {
	if (!isActive) return;

	ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_MenuBar);
	//if (Camera::ActiveCamera == nullptr) { ImGui::End(); return;}

	Camera* renderCam = Renderer::currentRenderer->attachedObj->GetComponent<Camera>();

	uint32 texture = renderCam->fbo.RenderedImage.texture;


	uint32 playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_PLAY_ICON)->texture;
	if (RuntimeManager::__currentMode == EngineMode::PLAY_MODE) {
		playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_STOP_ICON)->texture;
	}

	uint32 nextBut = RessourcesLoader::LoadTexture(SCENE_VIEW_NEXT_ICON)->texture;
	uint32 pauseBut = RessourcesLoader::LoadTexture(SCENE_VIEW_PAUSE_ICON)->texture;

	if (ImGui::ImageButton((void*)(intptr_t)
		playBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0)))
	{

		if (RuntimeManager::__currentMode == EngineMode::PLAY_MODE)
			RuntimeManager::switchMode(EngineMode::EDIT_MODE);

		else if (RuntimeManager::__currentMode == EngineMode::EDIT_MODE)
			RuntimeManager::switchMode(EngineMode::PLAY_MODE);
	}

	ImGui::SameLine();

	if (ImGui::ImageButton((void*)(intptr_t)
		pauseBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0))) Console::Write("Test0");

	ImGui::SameLine();

	if (ImGui::ImageButton((void*)(intptr_t)
		nextBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0))) Console::Write("Test1");

	ImGui::Image((void*)(intptr_t)
		texture,
		ImVec2(renderCam->viewPortSize.x,
			renderCam->viewPortSize.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}