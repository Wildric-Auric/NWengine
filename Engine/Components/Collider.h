#pragma once
#include "GameObject.h"
#include "GuiObject.h"

class Collider : public GameComponent {
	//Base Collider class
private:

public:
	static std::string GetType() { return "Collider"; }
	GameObject* attachedObj;

	fVec2 offset   = fVec2(0.0f, 0.0f);

	fVec2 GetPosition();
	fVec2 GetEdgePosition(int index);
	Collider() {};
	Collider(GameObject* go);
	std::vector<fVec2> edges = {fVec2(100,100), fVec2(-100,100), fVec2(-100,-100), fVec2(100,-100)}; //Trigonometric orientation

	bool Sat(Collider* other, fVec2* depthBuffer		 = nullptr);
	bool isColliding(Collider* other, fVec2* depthBuffer = nullptr);

	OVERRIDE_GUI
	void Start()  override;
	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;
};

