#include "imgui/imgui.h"
#include "imgui/imguizmo/ImGuizmo.h"
#include "imgui/implot/implot.h"

#include "DebugGui.h"
#include "Utilities.h"
#include "NWproj.h"
#include "Renderer.h"
#include "NWGui.h"
#include "Scene.h"
#include <iostream>


void DebugGui::Init() { this->isActive = 1; }
void DebugGui::Show() {
	if (!isActive) return;


	GameObject* container0 = nullptr;
	GameObject* container1 = nullptr;
	NWproj      proj;

	ImGui::Begin("Debug", &isActive);

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
	static bool a = 1;


	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
	static float point = 0.0f;
	static float point0 = 1.0f;
	static fVec2 point1 = fVec2(0.0, 0.0);
	static fVec2 point2 = fVec2(1.0, 0.0);
	NWGui::CubicBezierInterpolationPlot("Test", &point, &point0, &point1, &point2);

	if (ImGui::Button("Test Proj")) {
		MakeDir("C:\\Users\\HP\\source\\repos\\Wildric-Auric\\Hello");
		CopyDirectory("C:\\Users\\HP\\source\\repos\\Wildric-Auric\\Hello", "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\ProjectTemplate\\");
	}

	if (ImGui::Button("Save")) {
		proj.Save();
	}
	if (ImGui::Button("Load")) {
		proj.Load();
	}

	if (ImGui::Button("Test Renderer")) {
		std::vector<std::string> aa = GetDirFiles(".");
		for (auto str : aa) {
			std::cout << str << std::endl;
		}
		//container0 = new GameObject();
		//container1 = new GameObject();

		//container0->AddComponent<Renderer>()->shaderName = "Shaders\\Inversed.shader";
		//container1->AddComponent<Renderer>()->shaderName = "Shaders\\PostProcessing1.shader";
	}
	if (container0 != nullptr) {
		container0->GetComponent<Renderer>()->CaptureOnCamFrame();

		container1->GetComponent<Renderer>()->target = container0->GetComponent<Camera>();
		container1->GetComponent<Renderer>()->CaptureOnCamFrame();

		//Passing cam1 framebuffer to current renderer
		Renderer::currentRenderer->target = container1->GetComponent<Camera>();
	}
	ImGui::End();
}