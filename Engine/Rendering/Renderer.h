#pragma once
#include "GameObject.h"
#include "Script.h"
#include "Maths.h"
#include "Camera.h"

class Renderer : public GameComponent {
public:
	static std::string GetType() { return "Renderer"; }
	Renderer() {};
	Renderer(GameObject* go);
	~Renderer();

	GameObject* attachedObj;
	Camera*     target; //TODO::Update logic in nwengine.cpp and game.cpp

	fVec2 strechCoeff = fVec2(1.0f, 1.0f);

	static Renderer* defaultRenderer;
	static Renderer* currentRenderer;

	std::string shaderName  = SHADER_POST_PROCESSING;
	
	void DrawOnDefaultFrame();
	void CaptureOnCamFrame();
	void Use();
	void Unuse();
};