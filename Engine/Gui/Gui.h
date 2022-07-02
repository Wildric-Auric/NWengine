#pragma once

#include <GL/glew.h>
#include <glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"

class Gui {
public:
	static void Init(uint64 window) {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
		ImGui_ImplOpenGL3_Init();
		static ImGuiIO& io = ImGui::GetIO(); 
		//*(void)io;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.Fonts->AddFontFromFileTTF("Ressources/fonts/arial.ttf", 16.0f);

	}

	static void Update() {
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Explore"))
			{
				if (ImGui::MenuItem("Scene View Window"))	SceneViewGui::isActive       = 1;
				if (ImGui::MenuItem("Debugging Window"))    DebugGui::isActive           = 1;
				if (ImGui::MenuItem("Hierarchy Window"))    HierarchyGui::isActive       = 1;
				if (ImGui::MenuItem("Inspector Window"))    InspectorGui::isActive       = 1;
				if (ImGui::MenuItem("Scene Editor"))        SceneEditorGui::isActive	 = 1;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//Capture mouse
		//isMouseOnGui = io.WantCaptureMouse;

		SceneViewGui::Show();
		DebugGui::Show();
		HierarchyGui::Show();
		InspectorGui::Show();
		SceneEditorGui::Show();
	}
};
