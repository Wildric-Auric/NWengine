#pragma once

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class SceneEditorGui {
public:
	static bool isActive;
	static Camera cam;
	static bool f;  //Find a better solution (reminds me GMS scripting btw)
	static void Show() {
		if (!isActive) return;
		if (!f) {
			printf("hey");
			GameObject go = GameObject();
			go.id = 123; //TODO::implement fucking id
			cam = Camera(&go);
			f = 1;
		};
		cam.Update();
		cam.Capture();

		ImGui::Begin("Scene Editor", &isActive, ImGuiWindowFlags_MenuBar);
		ImGui::Image((void*)(intptr_t)
			cam.fbo.RenderedImage->texture,
			ImVec2(cam.fbo.RenderedImage->size.x, cam.fbo.RenderedImage->size.y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		
	};
};
