#pragma once
#include "Globals.h"
#include "ScriptManager.h"

class ScriptManagerGui {

public:
	static bool isActive;
	static void Init() {}
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
};