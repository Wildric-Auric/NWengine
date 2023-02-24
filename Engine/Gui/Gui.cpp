#include "Gui.h"
#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"
#include "ScriptManagerGui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


void Gui::Init(void* window) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	ImGui_ImplOpenGL3_Init();
	ImPlot::CreateContext();
	static ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("Ressources/fonts/arial.ttf", 16.0f);


	SceneViewGui::Init();
	DebugGui::Init();
	HierarchyGui::Init();
	InspectorGui::Init();
	SceneEditorGui::Init();
	ConsoleGui::Init();
	ScriptManagerGui::Init();
}


void Gui::Update() {
	ImGui::DockSpaceOverViewport();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Explore"))
		{
			if (ImGui::MenuItem("Scene View Window"))	SceneViewGui::isActive = 1;
			if (ImGui::MenuItem("Debugging Window"))    DebugGui::isActive = 1;
			if (ImGui::MenuItem("Hierarchy Window"))    HierarchyGui::isActive = 1;
			if (ImGui::MenuItem("Inspector Window"))    InspectorGui::isActive = 1;
			if (ImGui::MenuItem("Scene Editor"))        SceneEditorGui::isActive = 1;
			if (ImGui::MenuItem("Console"))			    ConsoleGui::isActive = 1;
			if (ImGui::MenuItem("Script Manager"))      ScriptManagerGui::isActive = 1;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Compilation")) {
			if (ImGui::MenuItem("Recompile engine")) {
#ifdef _WINDLL
				Builder::CompileEngineDllRuntime();
#else		
				Console::Write("Build failed: Cannot recompile static debugging engine");
#endif
			}

			if (ImGui::MenuItem("Compile ScriptManager file")) {
				Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
				ScriptManager::CompileScriptManager();
			}

			if (ImGui::MenuItem("Build game")) {
				Builder::BuildGameRuntime();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Save"))	Scene::currentScene->Save(); //TODO::Not save during
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	SceneViewGui::Show();
	DebugGui::Show();
	HierarchyGui::Show();
	InspectorGui::Show();
	SceneEditorGui::Show();
	ConsoleGui::Show();
	ScriptManagerGui::Show();
}