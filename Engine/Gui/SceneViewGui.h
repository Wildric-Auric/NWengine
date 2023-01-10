#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"
#include "Components.h"
#include "Texture.h"
#include "PostProcessing.h"

#define SCENE_VIEW_PLAY_ICON "Ressources\\Images\\Icons\\Play.png"
#define SCENE_VIEW_NEXT_ICON "Ressources\\Images\\Icons\\Next.png"
#define SCENE_VIEW_PAUSE_ICON "Ressources\\Images\\Icons\\Pause.png"
#define SCENE_VIEW_STOP_ICON "Ressources\\Images\\Icons\\Stop.png"



class SceneViewGui {

public:
	static bool isActive;

	static void Show() {
		if (!isActive) return;

		ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_MenuBar);
		if (Camera::ActiveCamera == nullptr) { ImGui::End(); return;}
		
		uint32 texture = Camera::ActiveCamera->fbo.RenderedImage.texture;
		PostProcessing* pp = Camera::ActiveCamera->attachedObj->GetComponent<PostProcessing>();




		if (pp != nullptr) texture = pp->fbo.RenderedImage.texture;

		uint32 playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_PLAY_ICON)->texture;
		if (Globals::PLAY_MODE) {
			playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_STOP_ICON)->texture;
		}

		uint32 nextBut = RessourcesLoader::LoadTexture(SCENE_VIEW_NEXT_ICON)->texture;
		uint32 pauseBut= RessourcesLoader::LoadTexture(SCENE_VIEW_PAUSE_ICON)->texture;

		if (ImGui::ImageButton((void*)(intptr_t)
			playBut,
			ImVec2(30, 30),
			ImVec2(0, 1), ImVec2(1, 0))) {
			Globals::PLAY_MODE = 1 - Globals::PLAY_MODE;
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
			ImVec2(Camera::ActiveCamera->fbo.RenderedImage.size.x, Camera::ActiveCamera->fbo.RenderedImage.size.y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
};
