#include "Gui.h"
#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"
#include "ScriptManagerGui.h"
#include "NWproj.h"
#include "Console.h"
#include "Utilities.h"
#include "Scene.h"
#include "Builder.h"

#include "imgui/implot/implot.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imguizmo/ImGuizmo.h"

std::unordered_map<uint32, GuiWindow*> Gui::Windows;

void Gui::Init(void* window) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
	ImGui_ImplOpenGL3_Init();
	ImPlot::CreateContext();
	static ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("Ressources/fonts/arial.ttf", 16.0f);


	Gui::Windows.insert({{(uint32)GUI_WINDOW::SceneView,     new SceneViewGui() },
						 {(uint32)GUI_WINDOW::Debug,         new DebugGui()},
						 {(uint32)GUI_WINDOW::Hierarchy,     new HierarchyGui()},
						 {(uint32)GUI_WINDOW::Inspector,     new InspectorGui()},
						 {(uint32)GUI_WINDOW::SceneEditor,   new SceneEditorGui()},
						 {(uint32)GUI_WINDOW::Console,		 new ConsoleGui()},
						 {(uint32)GUI_WINDOW::ScriptManager, new ScriptManagerGui()},

	});	

	for (std::pair<const uint32, GuiWindow*>& it : Gui::Windows) {
		it.second->Init();
	};
}

void Gui::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void Gui::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
			if (ImGui::MenuItem("Scene View Window"))	(Gui::Windows.find((uint32)GUI_WINDOW::SceneView))->second->isActive		= 1;
			if (ImGui::MenuItem("Debugging Window"))    (Gui::Windows.find((uint32)GUI_WINDOW::Debug))->second->isActive			= 1;
			if (ImGui::MenuItem("Hierarchy Window"))    (Gui::Windows.find((uint32)GUI_WINDOW::Hierarchy))->second->isActive		= 1;
			if (ImGui::MenuItem("Inspector Window"))    (Gui::Windows.find((uint32)GUI_WINDOW::Inspector))->second->isActive		= 1;
			if (ImGui::MenuItem("Scene Editor"))        (Gui::Windows.find((uint32)GUI_WINDOW::SceneEditor))->second->isActive		= 1;
			if (ImGui::MenuItem("Console"))			    (Gui::Windows.find((uint32)GUI_WINDOW::Console))->second->isActive			= 1;
			if (ImGui::MenuItem("Script Manager"))      (Gui::Windows.find((uint32)GUI_WINDOW::ScriptManager))->second->isActive	= 1;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Compilation")) {
			if (ImGui::MenuItem("Recompile engine")) {
#ifdef _WINDLL
				if (!Builder::CompileEngineDllRuntime())
					Console::Write("Build failed: See log.txt for more details", CONSOLE_ERROR_MESSAGE);
#else		
				Console::Write("Build failed: Cannot recompile static debugging engine", CONSOLE_WARNING_MESSAGE);
#endif
			}

			if (ImGui::MenuItem("Build game")) {
				std::string path = SaveAs("");
				if (path != "") {
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
					else Console::Write("Failed to create game build", CONSOLE_ERROR_MESSAGE);
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene")) {

			if (ImGui::MenuItem("New Scene")) {
				std::string path = SaveAs("NWscene\0*.NWscene\0All\0*.*\0");
				if (path != "") {
					path += ".NWscene";
					std::string dir = "";
					std::string fileName = "";
					std::string file = GetFileName(path, &fileName, nullptr, &dir);
					MakeFile(dir + file);
					//Load the scene
					//TODO::Embed this within a function
					delete Scene::currentScene;
					(Scene::currentScene = new Scene(dir + file))->LoadScene();
					NWproj::currentProj->defaultScenePath = Scene::currentScene->name;
					NWproj::currentProj->Save();
				}
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

	for (auto& it : Gui::Windows) {
		it.second->Show();
	};
}

void Gui::Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}