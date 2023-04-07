#include "NWGui.h"
#include "Utilities.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

uint32 NWGui::AutoInc = 1;

bool NWGui::FileHolder(std::string name, std::string holdedFile) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable0", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(name.c_str());
		ImGui::TableNextColumn();
		ret = ImGui::Button(holdedFile.c_str(), ImVec2(150, 0));
		ImGui::EndTable();
	}
	return ret;
}

bool NWGui::CheckBox(std::string label, bool* value) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable1", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(label.c_str());
		ImGui::TableNextColumn();
		ret = ImGui::Checkbox((std::string("##") + label).c_str(), value);
		ImGui::EndTable();
	}
	return ret;
}

bool NWGui::Button(std::string label) {
	return ImGui::Button(NWGui::GenLabel(label.c_str(), AutoInc++).c_str());
}


bool NWGui::DragValue(const char* label, void* target, int dataType, int16 vectorSize, float speed, float minn, float maxx) {
	bool ret = 0;
	if (ImGui::BeginTable("testTable0", 2)) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text(label);
		ImGui::TableNextColumn();
		ret = ImGui::DragScalarN(NWGui::GenLabel("", AutoInc++).c_str(), dataType, target, vectorSize, speed, &minn, &maxx);
		ImGui::EndTable();
	}

	return ret;
}

std::string NWGui::GenLabel(const char* string, uint32 id) {
	return std::string(string) + std::string("##") + std::to_string(id);
}