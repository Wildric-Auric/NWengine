#pragma once

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Scene.h"
#include "GameObject.h"
#include "Inputs.h"

class HierarchyGui {
public:
	static bool isActive;
	static int16 selected;
	static void Show() {
		static int16 renaming = -1;
		static char str0[128] = "new GameObject";
		if (!isActive) {
			selected = -1;
			renaming = -1;
			return;
		}

		ImGui::Begin("Hierarchy", &isActive, ImGuiWindowFlags_MenuBar);
		ImGui::OpenPopupOnItemClick("createordelete", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopupContextWindow("createordelete")) {
	
			if (ImGui::Selectable("new GameObject")) {
				Scene::currentScene->AddObject(GameObject());
			}
			if (selected != -1 && ImGui::Selectable("Delete GameObject")) {
				Scene::currentScene->DeleteObject(selected);
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

};