#pragma once
#include "Scene.h"


class RuntimeManager {
public:
	static EngineMode __currentMode; //Read only
	static Scene* scene;

	static int switchMode(EngineMode);


};