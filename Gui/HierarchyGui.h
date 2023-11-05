#pragma once
#include "Globals.h"
#include "GuiWindow.h"
#include "GameObject.h"

class HierarchyGui : public GuiWindow {
public:
	int32		_selected			= -1;
	GameObject* _selectedGameObject = nullptr;
	//Returns the selected game object; parameter index is the buffer where will be stored the index of the object in scene's gameobjects' container
	GameObject* GetSelectedObject(int* index = nullptr);
	void Init() override;
	void Show() override;
};