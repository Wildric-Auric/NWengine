#pragma once

#include "Globals.h"
#include"Utilities.h"

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
#include "Scene.h"
#include "Audio.h"


class Game {
public:
	static int8 Run();
	static void MainLoop();
	static void Shutdown();

	static GameObject FrameObject;
	static Sprite* RenderedTexture;
};