#pragma once
#include "GameObject.h"
#include "FrameBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public GameComponent {
public:
	static std::string GetType() { return "Camera"; };

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix = glm::mat4(viewMatrix);

	fVec2 size;
	fVec2 viewPortSize;
	fVec3 clearColor = fVec3(0.0,0.0, 0.0);
	float alpha      = 1.0f;
	bool isActive = 0;

	Camera() {};
	Camera(GameObject* go);
	~Camera();
	GameObject* attachedObj;

	void Capture();
	//void CaptureWithPostProcessing(void* postProcessing, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	void Update();
	void ChangeOrtho(float minX, float maxX, float minY, float maxY);
	void ChangeOrtho(float sizeX, float sizeY);
	void MoveTo(Vector2<int> target, float interpolationTime);
	void Use();
	fVec2 position = fVec2(0.0f,0.0f);
	float rotation = 0.0f;
	float zoom = 1.0;

	FrameBuffer fbo;

	static Camera* ActiveCamera;

	void Gui() override;

	int Serialize(std::fstream* data, int offset)    override; 
	int Deserialize(std::fstream* data, int offset)  override;
};