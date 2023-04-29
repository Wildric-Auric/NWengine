#include "Gui.h"
#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"
#include "ScriptManagerGui.h"
#include "NWproj.h"

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

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open Project")) {
				std::string path = GetFile("Shader Files\0*.NWproj\0*.*\0");
				if (path != "") {
					std::string dir		 = "";
					std::string filename = "";
					GetFileName(path, &filename, nullptr, &dir);
					NWproj proj = NWproj();
					proj.dir  = dir;
					proj.name = filename;
					proj.MakeCurrent();
				}
			}

			if (ImGui::MenuItem("New Project")) {
				std::string path	 = SaveAs("NWproj\0*.NWproj\0All\0*.*\0") + ".NWproj";
				std::string dir		 = "";
				std::string fileName = "";
				std::string file = GetFileName(path, &fileName, nullptr, &dir);
				std::string dir0 = ToSingleBackSlash(dir) + fileName + "\\";
				MakeDir(dir0);
				if (CopyDirectory(dir0, "ProjectTemplate\\")) {
					NWproj proj = NWproj(dir0);
					proj.MakeCurrent();
				}
				else Console::Write("Failed to create project");
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Explore")) {
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
				ScriptManager::CompileScriptManager();
				Builder::CompileEngineDllRuntime();
#else		
				Console::Write("Build failed: Cannot recompile static debugging engine", CONSOLE_WARNING_MESSAGE);
#endif
			}

			if (ImGui::MenuItem("Build game")) {
				std::string path = SaveAs("");
				std::string dir = "";
				std::string fileName = "";
				std::string file = GetFileName(path, &fileName, nullptr, &dir);
				std::string dir0 = ToSingleBackSlash(dir) + fileName + "\\";
				MakeDir(dir0);
				if (CopyDirectory(dir0, "GameBuildTemplate\\")) {
					Globals::gamePath = dir0 + fileName + ".exe";
					if (!Builder::BuildGameRuntime())
						Console::Write("Game build failed: See log.txt for more details", CONSOLE_ERROR_MESSAGE);
				}
				else Console::Write("Failed to create project", CONSOLE_ERROR_MESSAGE);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene")) {

			if (ImGui::MenuItem("New Scene")) {
				std::string path = SaveAs("NWscene\0*.NWscene\0All\0*.*\0") + ".NWscene";
				std::string dir = "";
				std::string fileName = "";
				std::string file = GetFileName(path, &fileName, nullptr, &dir);
				MakeFile(dir + file);
			}
			if (ImGui::MenuItem("Save") && Scene::currentScene != nullptr) {
				Scene::currentScene->Save();
			}

			if (ImGui::MenuItem("Load")) {
				if (NWproj::currentProj == nullptr || NWproj::currentProj->dir == "")  
					Console::Write("Cannot load scene; you should open a valid NWproj first.", CONSOLE_WARNING_MESSAGE);
				else {
					std::string path = GetFile("NWscene\0*.NWscene");
					if (path != "") {
						delete Scene::currentScene;
						(Scene::currentScene = new Scene(path))->LoadScene();
						NWproj::currentProj->defaultScenePath = Scene::currentScene->name;
						NWproj::currentProj->Save();
					}
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	NWGui::AutoInc		= 1; //Renitialize autoincremented id on each gui update;

	ConsoleGui::Show();
	if (Scene::currentScene == nullptr) return;
	SceneViewGui::Show();
	DebugGui::Show();
	HierarchyGui::Show();
	InspectorGui::Show();
	SceneEditorGui::Show();
	ScriptManagerGui::Show();
}

void Gui::Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}