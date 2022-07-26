#pragma once
#include "GameObject.h"
#include <vector>
class Collider2 : public GameComponent {
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
	Collider2() {};
	Collider2(GameObject* attachedObj);
	bool isColliding(Collider2 other);
	static std::map<GameObject*, Collider2> componentList;
};