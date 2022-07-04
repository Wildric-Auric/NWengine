#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SceneEditorGui.h"
#include "Scene.h"
#include "Utilities.h"
#include "Console.h"
class DebugGui {
public:
	static bool isActive;
	static void Show() {
		if (!isActive) return;
		ImGui::Begin("Debug", &isActive, ImGuiWindowFlags_MenuBar);
		ImGui::Text("fps = %f", Globals::fps);
		ImGui::DragInt2("cam pos", &SceneEditorGui::cam.position.x);
		ImGui::DragFloat("zoom", &SceneEditorGui::cam.zoom, 0.1, -0.0, 10.0);
		static bool b = 0;
		static int a = 0;
		if (ImGui::Button("fuck")) GetFile();
		ImGui::DragInt("flag", &a, 0.2f, 0, 4);
		ImGui::Checkbox("Save Scene", &b);
		if (b) {
			b = 0;
			Console::Write((std::string("Hello, this is a test") + std::to_string(a)).c_str(), a);

		}
		ImGui::Separator();
		ImGui::ShowDemoWindow();
		static ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::End();
	}
};
