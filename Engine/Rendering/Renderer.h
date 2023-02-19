#pragma once
#include "GameObject.h"
#include "Script.h"
#include "Maths.h"
#include "Camera.h"

class Renderer : public GameComponent {
private:
	Camera* inputCamera; //Read only
public:
	static std::string GetType() { return "Renderer"; }
	Renderer() {};
	Renderer(GameObject* go);
	~Renderer();

	GameObject* attachedObj;

	fVec2 strechCoeff = fVec2(1.0f, 1.0f);

	static Renderer* defaultRenderer;
	static Renderer* currentRenderer;

	std::string shaderName  = SHADER_POST_PROCESSING;
	
	void DrawOnDefaultFrame();
	void CaptureOnCamFrame();
	void Use();
	void Unuse();

	void Gui() override;
	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;

};