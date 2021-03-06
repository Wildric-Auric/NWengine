#pragma once

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Console.h"

class ConsoleGui {
public:
	static bool isActive;
	static void Show() {
		ImGui::Begin("Console", &isActive, ImGuiWindowFlags_MenuBar);
		for (auto it = Console::stack.begin(); it != Console::stack.end(); it++) {

			switch (it->flag) {
				case CONSOLE_DEBUG_MESSAGE:
					ImGui::Text((it->str + "   ").c_str());
					break;
				case CONSOLE_WARNING_MESSAGE:
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0), ("WARNING: " + it->str + "   ").c_str());
					break;
				case CONSOLE_ERROR_MESSAGE:
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.1f, 1.0), ("ERROR: " + it->str + "   ").c_str());
					break;
				
			}
			ImGui::SameLine();
			ImGui::Text(std::to_string(it->number).c_str());
			ImGui::Separator();
		}
		ImGui::End();
	};

	

};