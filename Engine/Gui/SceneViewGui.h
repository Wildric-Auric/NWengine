#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"
#include "Components.h"
#include "Texture.h"


class SceneViewGui {

public:
	static bool isActive;

	static void Show() {
		if (isActive) {
			ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_MenuBar);
			ImGui::Image((void*)(intptr_t)
				Camera::ActiveCamera->fbo.RenderedImage->texture,
				ImVec2(Camera::ActiveCamera->fbo.RenderedImage->size.x, Camera::ActiveCamera->fbo.RenderedImage->size.y),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
	}
};
