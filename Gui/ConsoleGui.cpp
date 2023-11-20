#include "imgui/imgui.h"
#include "Console.h"
#include "ConsoleGui.h"

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