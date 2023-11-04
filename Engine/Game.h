#pragma once

#ifndef NW_DEFAULT_SCENE
#define NW_DEFAULT_SCENE "scene.NWscene"
#endif

#include "Globals.h"
#include "Utilities.h"

#include <iostream>

#include "Primitives.h"
#include "Inputs.h"
#include "Context.h"
#include "RessourcesLoader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "Audio.h"
#include "Components.h"
#include "Batch.h"
#include "Text.h"
#include "ScriptManager.h"
#include "SceneEditor.h"

#include "NWGui.h" //TODO::Remove this!!! No GUI in GAME BUILD, just for testing now

class Game {
public:
	static int8 Run();
	static void MainLoop();
	static void Shutdown();

	static GameObject FrameObject;
	static Sprite* RenderedTexture;
};