#pragma once
#include "GameObject.h"

class Collider2 {
	//Base Collider class
private:
	Vector2<int> manualSize;
	int outerCircleEdge = 0;
public:
	GameObjectClone* attachedObj;
	Vector2<int> edges[3]; //Trigonometric orientation
	Vector2<int>* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	Vector2<int>* scale; //ReadOnly
	Vector2<int> offset;
	Vector2<int> GetPosition();
	Vector2<int> GetSize();
	void Resize(Vector2<int> newSize);
	Collider2() {};
	Collider2(GameObjectClone* attachedObj, Vector2<int> offset = Vector2<int>(0, 0));

	bool isColliding(Collider2 other);
};