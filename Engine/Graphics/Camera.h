#pragma once
#include "GameObject.h"
#include "FrameBuffer.h"
#include "Maths.h"
class Camera : public GameComponent {
public:
	static std::string GetType() { return "Camera"; }; 
	//This static method calls capture on ActiveCamera if not null
	static void UpdateActiveCamera();

	Matrix4<float> projectionMatrix;
	Matrix4<float> viewMatrix = Matrix4<float>(1.0f);

	fVec2 size;
	fVec2 viewPortSize;
	fVec3 clearColor = fVec3(0.0,0.0, 0.0);
	float alpha      = 1.0f;
	bool isActive = 0;
	GameObject* attachedObj = nullptr;

	Camera() = default;
	Camera(GameObject* go);
	~Camera();

	void Capture();
	void Update();
	void ChangeOrtho(float sizeX, float sizeY);
	void MoveTo(Vector2<int> target, float interpolationTime);
	void Use();
	fVec2 position = fVec2(0.0f,0.0f);
	float rotation = 0.0f;
	float zoom = 1.0;

	FrameBuffer fbo;

	static Camera* ActiveCamera;

	int Serialize(std::fstream* data, int offset)    override; 
	int Deserialize(std::fstream* data, int offset)  override;
};