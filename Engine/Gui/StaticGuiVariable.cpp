#include "SceneViewGui.h"
#include "DebugGui.h"
#include "HierarchyGui.h"
#include "InspectorGui.h"
#include "SceneEditorGui.h"
#include "ConsoleGui.h"
#include "ScriptManagerGui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/implot/implot.h"

#include "Scene.h"
#include "GameObject.h"
#include "Inputs.h"

#include "NWGui.h"
#include "ScriptManager.h"
#include "Utilities.h"

#include "Camera.h"
#include "Components.h"
#include "Texture.h"
#include "Renderer.h"
#include "RuntimeManager.h"
#include "SceneEditor.h"
#include "Console.h"
#include "ScriptManager.h"
#include "NWproj.h"

#include "Gui.h"

#include <iostream>

//------------------------------------

void GuiWindow::Init()  {};
void GuiWindow::Show()  {};
GuiWindow::~GuiWindow() {};

//------------------------------------

void HierarchyGui::Init() { this->isActive = 1; }
void HierarchyGui::Show() {
	static int32 renaming = -1;
	static char str0[128] = "new GameObject";
	if (!isActive) {
		selected = -1;
		renaming = -1;
		return;
	}

	ImGui::Begin("Hierarchy", &isActive, ImGuiWindowFlags_MenuBar);
	if (Scene::currentScene == nullptr) {
		ImGui::End();
		return;
	}

	ImGui::OpenPopupOnItemClick("createordelete", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopupContextWindow("createordelete")) {

		if (ImGui::Selectable("new GameObject")) {
			Scene::currentScene->AddObject(GameObject());
		}
		if (selected != -1 && ImGui::Selectable("Delete GameObject")) {
			Scene::currentScene->DeleteObject(selected);
			selected = -1;
		}
		ImGui::EndPopup();
	}

	int8 i = 0;
	for (auto it = Scene::currentScene->sceneObjs.begin(); it != Scene::currentScene->sceneObjs.end(); it++) {
		if (renaming == i) {

			ImGui::InputText("input name", str0, IM_ARRAYSIZE(str0));
			if (Inputs::enter) {
				it->Rename(std::string(str0));
				renaming = -1;
			}
		}
		else if (ImGui::Selectable(it->name.c_str(), selected == i)) {
			selected = i;
			renaming = -1;
		}
		i++;
	}
	//Renaming
	if (selected != -1) {
		if (Inputs::f2) {
			renaming = selected;
		}
	}
	if (!ImGui::IsWindowFocused()) renaming = -1;
	ImGui::End();
}

//-------------------------------------

void SceneEditorGui::Init() { this->isActive = 1;}
void SceneEditorGui::Show() {
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

//-------------------------------------

void SceneViewGui::Init() { this->isActive = 1;}
void SceneViewGui::Show() {
	if (!isActive) return;

	ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_MenuBar);
	//if (Camera::ActiveCamera == nullptr) { ImGui::End(); return;}

	Camera* renderCam = Renderer::currentRenderer->attachedObj->GetComponent<Camera>();

	uint32 texture = renderCam->fbo.RenderedImage.texture;


	uint32 playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_PLAY_ICON)->texture;
	if (RuntimeManager::__currentMode == EngineMode::PLAY_MODE) {
		playBut = RessourcesLoader::LoadTexture(SCENE_VIEW_STOP_ICON)->texture;
	}

	uint32 nextBut = RessourcesLoader::LoadTexture(SCENE_VIEW_NEXT_ICON)->texture;
	uint32 pauseBut = RessourcesLoader::LoadTexture(SCENE_VIEW_PAUSE_ICON)->texture;

	if (ImGui::ImageButton((void*)(intptr_t)
		playBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0)))
	{

		if (RuntimeManager::__currentMode == EngineMode::PLAY_MODE)
			RuntimeManager::switchMode(EngineMode::EDIT_MODE);

		else if (RuntimeManager::__currentMode == EngineMode::EDIT_MODE)
			RuntimeManager::switchMode(EngineMode::PLAY_MODE);
	}

	ImGui::SameLine();

	if (ImGui::ImageButton((void*)(intptr_t)
		pauseBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0))) Console::Write("Test0");

	ImGui::SameLine();

	if (ImGui::ImageButton((void*)(intptr_t)
		nextBut,
		ImVec2(30, 30),
		ImVec2(0, 1), ImVec2(1, 0))) Console::Write("Test1");

	ImGui::Image((void*)(intptr_t)
		texture,
		ImVec2(renderCam->viewPortSize.x,
			renderCam->viewPortSize.y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

//--------------------------------------

#define ADD_COMPONENT_TO_INSPECTOR(type, go) if (ImGui::Selectable(#type)) go->AddComponent<type>();

void InspectorGui::Init() { this->isActive = 1;}
void InspectorGui::Show() {

	if (!isActive) return;


	ImGui::Begin("Inspector", &isActive);

	if (Scene::currentScene == nullptr) {
		ImGui::End();
		return;
	}



	GameObject* go = nullptr;
	HierarchyGui* hw = (HierarchyGui*)(Gui::Windows.find((uint32)GUI_WINDOW::Hierarchy)->second);
	if ( hw->selected > -1 && hw->selected < Scene::currentScene->sceneObjs.size()) {
		auto it = Scene::currentScene->sceneObjs.begin();
		std::advance(it, hw->selected);
		go = &(*it);

		int n = 1;
		for (std::map<std::string, GameComponent*>::iterator it = go->components.begin(); it != go->components.end();) {
			std::string    key = it->first;
			GameComponent* gc = it->second;
			it++;
			if (!ImGui::CollapsingHeader(key.c_str())) continue;
			gc->Gui();
			if (!NWGui::Button("Delete")) continue;
			go->DeleteComponent(key);
			n++;
		}

		ImGui::NewLine();
		ImGui::NewLine();

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("popup0");
		}

		if (ImGui::BeginPopup("popup0")) {
			ADD_COMPONENT_TO_INSPECTOR(Transform, go);
			ADD_COMPONENT_TO_INSPECTOR(Sprite, go);
			ADD_COMPONENT_TO_INSPECTOR(Collider, go);
			ADD_COMPONENT_TO_INSPECTOR(Camera, go);
			ADD_COMPONENT_TO_INSPECTOR(ParticleSystem, go);
			ADD_COMPONENT_TO_INSPECTOR(AudioEmitter, go);
			ADD_COMPONENT_TO_INSPECTOR(AudioListener, go);
			ADD_COMPONENT_TO_INSPECTOR(Script, go);
			ADD_COMPONENT_TO_INSPECTOR(TextHandler, go);
			ImGui::EndPopup();
		}
	}
	ImGui::End();
};

#undef ADD_COMPONENT_TO_INSPECTOR

//--------------------------------------

void ScriptManagerGui::Init() { this->isActive = 0; }
void ScriptManagerGui::Show() {
		static int selected = -1;
		static std::string	selected0 = "";
		static bool isSelected = 0;
		if (!isActive) return;
		ImGui::Begin("Script Manager", &isActive, ImGuiWindowFlags_MenuBar);
		int i = 0;
		for (std::map<std::string, std::string>::iterator it = ScriptManager::scriptList.begin(); it != ScriptManager::scriptList.end(); it++) {
			std::string filename = "";
			GetFileName(it->first, &filename);
			isSelected = (selected == i);
			ImGui::Selectable(filename.c_str(), &isSelected);
			if (isSelected) {
				selected = i;
				selected0 = it->first;
			}
			i++;
		}

		if (ImGui::Button("Compile All") && ScriptManager::scriptList.size() > 0) {
			if (!ScriptManager::CompileScripts())
				Console::Write("Error has occured during compilation; check \"log.txt\" for details", CONSOLE_ERROR_MESSAGE);
		};

		ImGui::OpenPopupOnItemClick("ScriptManagerPopup", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopupContextWindow("ScriptManagerPopup")) {
			if (selected == -1) {
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			else {
				if (ImGui::Selectable("CompileScript")) {
					if (!ScriptManager::CompileScript(selected0))
						Console::Write("Error has occured during compilation; check \"log.txt\" for details", CONSOLE_ERROR_MESSAGE);
				}
				else if (ImGui::Selectable("Delete")) {
					if (ScriptManager::scriptList.find(selected0) != ScriptManager::scriptList.end()) {
						ScriptManager::scriptList.erase(selected0);
						ScriptManager::SaveScriptList();
						selected = -1;
					}
				}
				ImGui::EndPopup();
			}

		}


		ImGui::End();
}

//--------------------------------------

void ConsoleGui::Init() { this->isActive = 1; }
void ConsoleGui::Show() {
	if (!isActive) return;
	ImGui::Begin("Console", &isActive, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::Button("Clear")) {
			Console::Clear();
			//ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::BeginTable("testTable0", 3, ImGuiTableFlags_Resizable)) {
		for (auto it = Console::stack.begin(); it != Console::stack.end(); it++) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			switch (it->flag) {
			case CONSOLE_DEBUG_MESSAGE:
				ImGui::Text("LOG:");
				ImGui::TableNextColumn();
				ImGui::Text((it->str + "   ").c_str());
				break;
			case CONSOLE_WARNING_MESSAGE:
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0), "WARNING:");
				ImGui::TableNextColumn();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0), (it->str + "   ").c_str());
				break;
			case CONSOLE_ERROR_MESSAGE:
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.1f, 1.0), "ERROR:");
				ImGui::TableNextColumn();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.1f, 1.0), (it->str + "   ").c_str());
				break;

			}
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(it->number).c_str());
		}
		ImGui::EndTable();
	}
	ImGui::End();
};

//--------------------------------------

void DebugGui::Init() { this->isActive = 1; } 
void DebugGui::Show() {
	if (!isActive) return;


	GameObject* container0 = nullptr;
	GameObject* container1 = nullptr;
	NWproj      proj;

	ImGui::Begin("Debug", &isActive);

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
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

