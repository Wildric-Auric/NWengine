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

#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot/implot.h"
#include "Gui.h"

#include "Primitives.h"
#include "Inputs.h"
#include "Maths.h"
#include "Context.h"
#include "Texture.h"
#include "Text.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Utilities.h"
#include "Scene.h"
#include "Audio.h"
#include "Components.h"

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