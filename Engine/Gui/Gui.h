#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"
#include "ScriptManagerGui.h"

class Gui {
public:
	static void Init(void* window) {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
		ImGui_ImplOpenGL3_Init();
		ImPlot::CreateContext();
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
				if (ImGui::MenuItem("Console"))			    ConsoleGui::isActive		 = 1;
				if (ImGui::MenuItem("Script Manager"))      ScriptManagerGui::isActive   = 1;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Compilation")) {
				if (ImGui::MenuItem("Recompile engine")) {
					//TODO::Error check
					//TODO::Move the processing to separate translation unit
					ScriptManager::SaveScriptList();
					Builder::IncludeDir.clear();
					Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
					Builder::objs.clear();
					Builder::LibsDir.clear();
					Builder::LibsDir = GetNWlist(Globals::compilationConfigDir + "Libs Dir.NWlist");
					Builder::staticLibs = GetNWlist(Globals::compilationConfigDir + "Libs.NWlist");
					//Link NWengine.lib + script Objs + script manager obj to a Dll
					Builder::objs = { Globals::compiledScriptDir + "ScriptManager.obj", Globals::engineLibDir + "NWengine.lib", Globals::engineLibDir + "NWengine.obj"};
					for (std::map<std::string, std::string>::iterator iter = ScriptManager::scriptList.begin(); iter != ScriptManager::scriptList.end(); iter++) {
						Builder::objs.push_back(Globals::compiledScriptDir + iter->first + ".obj");
						Builder::IncludeDir.push_back(iter->second);
					}                                                                                  
					//Compile script manager
					int size = Builder::objs.size();
					Builder::InitScripts(Globals::scriptListPath, Globals::scriptManagerPath);
					while (Builder::objs.size() > size)
						Builder::objs.pop_back(); //Bad solution to refactor
 
					Builder::CompileInd(Globals::scriptManagerPath, Globals::compiledScriptDir);
					Exec("builder.bat"); //TODO::output result on an imgui window
					Builder::Link(Globals::dllDir + "NWengine_temp.dll", 1);

					Exec("builder.bat");

					Context::dllFlag = NW_RELOAD_DLL;
				}
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
		ConsoleGui::Show();
		ScriptManagerGui::Show();
	}
};

