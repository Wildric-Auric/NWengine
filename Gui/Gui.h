#pragma once
#include <unordered_map>
#include "GuiWindow.h"
#include "Globals.h"
#include "NWGui.h"

enum class GUI_WINDOW {
	None,
	SceneView,
	Hierarchy,
	Inspector,
	SceneEditor,
	Console,
	ScriptManager,
	Debug
};

class Gui {
public:
	//Windows are heap allocated
	static std::unordered_map<uint32, GuiWindow*> Windows;
	static void Init(void* window);
	//Called at each frame
	static void Begin();
	//Called at end of frame
	static void Render();
	static void Update();
	static void Destroy();
};