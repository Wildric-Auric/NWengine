#pragma once
#include "Globals.h"
#include "ScriptManager.h"

class ScriptManagerGui {

public:
	static bool isActive;
	static void Show() {
		static int selected				= -1;
		static std::string	selected0   = "";
		static bool isSelected =  0;
		if (!isActive) return;
		ImGui::Begin("Script Manager", &isActive, ImGuiWindowFlags_MenuBar);
		int i = 0;
		for (std::map<std::string, std::string>::iterator it = ScriptManager::scriptList.begin(); it != ScriptManager::scriptList.end(); it++) {
			std::string filename = "";
			GetFileName(it->first, &filename);
			isSelected = (selected == i);
			ImGui::Selectable(filename.c_str(), &isSelected);
			if (isSelected) {
				selected  = i;
				selected0 = it->first;
			}
			i++;
		}

		if (ImGui::Button("Compile All")) {
			ScriptManager::CompileScripts();
		};

		ImGui::OpenPopupOnItemClick("ScriptManagerPopup", ImGuiPopupFlags_MouseButtonRight);

		ImGui::End();

		if (ImGui::BeginPopupContextWindow("ScriptManagerPopup") && selected != -1 ) {
			if (ImGui::Selectable("CompileScript")) {
				ScriptManager::CompileScript(selected0);
			};
			if (ImGui::Selectable("Delete")) {
				if (ScriptManager::scriptList.find(selected0) != ScriptManager::scriptList.end())
						ScriptManager::scriptList.erase(selected0);
			};
			ImGui::EndPopup();
		}
	}
};