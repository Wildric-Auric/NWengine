#pragma once
#include "GameObject.h"
#include "Script.h"
#include "Maths.h"
#include "Camera.h"


class Renderer {
public:
	Renderer() = default;
	Renderer(const std::string& shaderPath);
	~Renderer();

	GameObject  componentContainer = GameObject();
	Camera*     target			   = nullptr; 
	fVec2		stretchCoeff		   = fVec2(1.0f, 1.0f);
	/*
		Intended to be used as decorator; Sobel(Blur(), 1) would update renderer, called Blur, camera and then use
		its framebuffer to draw it using Sobel renderer
		captureOnDefaultFrame is a boolean indicating if we should draw on the the default framebuffer on camera framebuffer
	*/
	Renderer* operator()(Renderer* renderer, bool captureOnDefaultFrame = 0);
	Renderer* operator()(bool captureOnDefaultFrame = 0);

	void DrawOnDefaultFrame();
	void CaptureOnCamFrame();
	//Make this renderer as current renderer
	void Use();
	//Make default renderer as current. Default renderer is managed by Core frontend or custom one
	void Unuse();
	void SetShader(const std::string& shaderPath);
	//Add components to componentContainer
	void SetUp();

	//Sets up default renderer
	static void Init();
	static void Destroy();

	static Renderer* defaultRenderer;
	static Renderer* currentRenderer;
	inline static std::string GetType() { return "Renderer"; }

};