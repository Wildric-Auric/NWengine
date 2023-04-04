#pragma once

/*


					 /$$   /$$ /$$      /$$                               /$$
					| $$$ | $$| $$  /$ | $$                              |__/
					| $$$$| $$| $$ /$$$| $$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$
					| $$ $$ $$| $$/$$ $$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
					| $$  $$$$| $$$$_  $$$$| $$$$$$$$| $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$
					| $$\  $$$| $$$/ \  $$$| $$_____/| $$  | $$| $$  | $$| $$| $$  | $$| $$_____/
					| $$ \  $$| $$/   \  $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
					|__/  \__/|__/     \__/ \_______/|__/  |__/ \____  $$|__/|__/  |__/ \_______/
										    /$$  \ $$
									           |  $$$$$$/
										    \______/

																														*/

#include "Globals.h"
#include"Utilities.h"

#ifndef NW_DLL_ENGINE

#include <iostream>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot/implot.h"
#include "Gui.h"

#include "Primitives.h"
#include "Inputs.h"
#include "Context.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components.h"
#include "ScriptManager.h"
#include "SceneEditor.h"
#include "Batch.h"
#include "Text.h"
#include "SceneEditor.h"

namespace NWengine {
	int Run();
	void MainLoop();
	void Shutdown();
}

#endif

#ifdef _WINDLL
	extern "C" {
		__declspec(dllexport) int DllRun();
	}
#endif