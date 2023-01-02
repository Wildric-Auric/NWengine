#pragma once
#include"Maths.h"
#include"Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "FrameBuffer.h"

class Camera : public GameComponent {
public:
	static std::string GetType() { return "Camera"; };

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix = glm::mat4(viewMatrix);

	iVec2 size;
	iVec2 viewPortSize;

	Camera() {};
	Camera(GameObject* go);
	GameObject* attachedObj;

	void Capture(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void CaptureWithPostProcessing(void* postProcessing, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	void Update();
	void ChangeOrtho(float minX, float maxX, float minY, float maxY);
	void ChangeOrtho(float sizeX, float sizeY);
	void MoveTo(Vector2<int> target, float interpolationTime);
	Vector2<int> position;
	float rotation = 0.0f;
	float zoom = 1.0;

	FrameBuffer fbo;

	static Camera* ActiveCamera;

	void Gui() override;

};