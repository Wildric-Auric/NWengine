#include "NWGui.h"
#include "Utilities.h"

//TODO::ImGui id fix
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