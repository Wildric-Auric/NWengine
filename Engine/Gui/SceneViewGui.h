#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"
#include "Components.h"
#include "Texture.h"
#include "PostProcessing.h"


class SceneViewGui {

public:
	static bool isActive;

	static void Show() {
		if (isActive) {
			uint32 texture = Camera::ActiveCamera->fbo.RenderedImage.texture;
			PostProcessing* pp = Camera::ActiveCamera->attachedObj->GetComponent<PostProcessing>();

			if (pp != nullptr) texture = pp->fbo.RenderedImage.texture;

			ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_MenuBar);
			ImGui::Image((void*)(intptr_t)
				texture,
				ImVec2(Camera::ActiveCamera->fbo.RenderedImage.size.x, Camera::ActiveCamera->fbo.RenderedImage.size.y),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
	}
};
