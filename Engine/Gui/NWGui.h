#pragma once
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Globals.h"
#define GUI_SEP ImGui::Separator();
#define GUI_NEWLINE ImGui::NewLine();
#define GUI_SAMELINE ImGui::SameLine();
class NWGui {
public:

	static bool FileHolder(std::string name, std::string holdedFile);
	static bool CheckBox(std::string label, bool* value);
template<typename T>
	static bool DragValue(const char* label, void* target, ImGuiDataType dataType, int16 vectorSize = 1, float speed = 1.0f, T minn = 0.0f, T maxx = 0.0f) {
		bool ret = 0;
		if (ImGui::BeginTable("testTable0", 2)) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(label);
			ImGui::TableNextColumn();
			//TODO::integrate ImGui ids
			ret = ImGui::DragScalarN((std::string("##")+std::string(label)).c_str(), dataType, target, vectorSize, speed, &minn, &maxx);
			ImGui::EndTable();
		}
		
		return ret;
	}
};