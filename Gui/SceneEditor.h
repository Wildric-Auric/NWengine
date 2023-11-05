#pragma once
#include "Camera.h"

class SceneEditor {
public:
	static Camera* cam;
	static GameObject cameraObj;
	static void Init();
	static void Update();
};