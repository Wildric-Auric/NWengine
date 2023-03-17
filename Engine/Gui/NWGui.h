#pragma once

#include "Globals.h"

#define GUI_SEP ImGui::Separator();
#define GUI_NEWLINE ImGui::NewLine();
#define GUI_SAMELINE ImGui::SameLine();
class NWGui {
public:
	static bool FileHolder(std::string name, std::string holdedFile);
	static bool CheckBox(std::string label, bool* value);
	static bool DragValue(const char* label, void* target, int dataType, int16 vectorSize = 1, float speed = 1.0f, float minn = 0.0f, float maxx = 0.0f);
	static std::string GenLabel(const char* string, int64 id);
};