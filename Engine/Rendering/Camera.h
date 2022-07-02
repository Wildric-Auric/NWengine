#pragma once
#include"Maths.h"
#include"Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "FrameBuffer.h"

class Camera {
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix = glm::mat4(viewMatrix);

	Camera() {};
	Camera(GameObject* go);

	void Capture();
	void Update();
	void ChangeOrtho(float minX, float maxX, float minY, float maxY);
	void MoveTo(Vector2<int> target, float interpolationTime);
	Vector2<int> position;
	float rotation = 0.0f;
	float zoom = 1.0;

	FrameBuffer fbo;

	static Camera* ActiveCamera;
	static std::map<GameObject*, Camera> componentList;

};