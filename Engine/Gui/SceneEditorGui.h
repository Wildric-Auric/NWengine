#pragma once

#include "imgui/imgui.h"
#include "Inputs.h"
#include "SceneEditor.h"

class SceneEditorGui {
public:
	static bool isActive;
	static void Init() {};
	static void Show() {
		if (!isActive) return;
		
		ImGui::Begin("Scene Editor", &isActive, ImGuiWindowFlags_MenuBar);
		if (SceneEditor::cam == nullptr) return;
		Camera& cam = *SceneEditor::cam;
		SceneEditor::Update();

		ImGuiIO& io = ImGui::GetIO();

	    iVec2 mousePosition = iVec2(io.MousePos.x - ImGui::GetCursorScreenPos().x, cam.size.y - io.MousePos.y + ImGui::GetCursorScreenPos().y);
		bool hover = ImGui::IsWindowFocused(); //* (mousePosition.x < cam.size.x&& mousePosition.x > 0 && mousePosition.y < cam.size.y&& mousePosition.y > 0);
		static bool onpress = 0;
		static iVec2 pos0 = 0;
		static iVec2 camPos;

		if (!onpress && hover && Inputs::left_click) {
			onpress = 1;
			pos0 = mousePosition;
			camPos = cam.position;
		}

		if (!hover || !Inputs::left_click) onpress = 0;
		if (onpress) cam.position = camPos - mousePosition + pos0;

		ImGui::Image((void*)(intptr_t)
			cam.fbo.RenderedImage.texture,
			ImVec2(cam.fbo.RenderedImage.size.x, cam.fbo.RenderedImage.size.y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	};
};
