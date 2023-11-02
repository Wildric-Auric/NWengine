#pragma once
#include "GuiWindow.h"

class GuiWindow {
public:
	bool isActive = 1;
	virtual void Init();
	virtual void Show();
	virtual ~GuiWindow();
};