#pragma once
#include "Globals.h"
#include "GuiWindow.h"

class HierarchyGui : public GuiWindow {
public:
	int32 selected = -1;
	void Init() override;
	void Show() override;
};