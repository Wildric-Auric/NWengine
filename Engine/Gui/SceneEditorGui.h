#pragma once

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Inputs.h"

class SceneEditorGui {
public:
	static bool isActive;
	static Camera cam;
	static bool f;  
	static void Show() {
		if (!isActive) return;
		if (!f) {
			GameObject go = GameObject();
			go.id = 123; //TODO::implement fucking id
			cam = Camera(&go);
			f = 1;
		};
		cam.Update();
		cam.Capture(0.1f, 0.0f, 0.4f, 1.0f);

		ImGui::Begin("Scene Editor", &isActive, ImGuiWindowFlags_MenuBar);

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
			cam.fbo.RenderedImage->texture,
			ImVec2(cam.fbo.RenderedImage->size.x, cam.fbo.RenderedImage->size.y),
			ImVec2(0, 1), ImVec2(1, 0));


		ImGui::End();
	};
};
