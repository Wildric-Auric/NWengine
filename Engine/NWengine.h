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


#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Gui.h"
#include "Globals.h"
#include "Primitives.h"
#include "Inputs.h"
#include "Maths.h"
#include "Context.h"
#include "Texture.h"
#include "Text.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Utilities.h"
#include "TileMap.h"
#include "Scene.h"
#include "Audio.h"
#include "Components.h"
#include "NWscripting.h"



class NWengine {
public:
	static int8 Run();
	static void MainLoop();
	static void Shutdown();
};