#pragma once
#include"GameObject.h"
#include"Maths.h"
class Rigidbody {
public:
	GameObjectClone* attachedGameObject;
	iVec2 offset; //useless
	Rigidbody(GameObjectClone* attached);
	iVec2 GetPosition();
	void Update();
};