#pragma once
#include "GameObject.h"
#include "GuiObject.h"

class Collider : public GameComponent {
	//Base Collider class
private:
	Vector2<int> manualSize;
	int outerCircleEdge = 0;
	std::vector<Vector2<int>> edges; //Trigonometric orientation
public:
	static std::string GetType() { return "Collider"; };
	GameObject* attachedObj;
	iVec2* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	iVec2* scale; //ReadOnly
	iVec2 offset;
	iVec2 GetPosition();
	iVec2 GetSize();
	void SetEdges(std::vector<iVec2> newEdges);
	void Resize(Vector2<int> newSize);
	Collider() {};
	Collider(GameObject* attachedObj);
	bool isColliding(Collider other);
	static std::map<GameObject*, Collider> componentList;

	void Gui() override; //Should not exist in Game Build; TODO::Make it so
};