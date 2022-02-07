#pragma once
#include"Maths.h"
#include"Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Camera {
public:
	Camera(float minX, float maxX, float minY, float maxY);
	void Update();
	void MoveTo(Vector2<int> target, float interpolationTime);
	Vector2<int> position;
	float zoom = 1.0;

};