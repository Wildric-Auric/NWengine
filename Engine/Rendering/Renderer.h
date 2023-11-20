#pragma once
#include "GameObject.h"
#include "Script.h"
#include "Maths.h"
#include "Camera.h"

class Renderer {
public:
	Renderer(const std::string& shaderPath = SHADER_POST_PROCESSING);
	~Renderer();

	std::string shaderName;
	GameObject  componentContainer = GameObject();
	Camera*     target			   = nullptr; 
	fVec2 strechCoeff			   = fVec2(1.0f, 1.0f);
	//Intended to be used as decorator; Sobel(Blur(), 1) would update renderer, called Blur, camera  and then use 
	// its framebuffer to draw it using Sobel renderer
	// captureOnDefaultFrame is a boolean indicating if we should draw on the the default framebuffer on camera framebuffer
	//TODO::Test this
	Renderer* operator()(Renderer* renderer, bool captureOnDefaultFrame = 0);
	Renderer* operator()(bool captureOnDefaultFrame = 0);

	void DrawOnDefaultFrame();
	void CaptureOnCamFrame();
	void Use();
	void Unuse();
	void SetShader(const std::string& shaderPath);
	//Add components to componentContainer
	void SetUp();

	static void Init();
	static void Destroy();

	static std::string GetType() { return "Renderer"; }
	static Renderer* defaultRenderer;
	static Renderer* currentRenderer;

};