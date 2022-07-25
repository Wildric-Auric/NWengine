#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"
#include <map>
class Transform : public GameComponent {
public:
	iVec2 position = iVec2(0,0);
	fVec2 scale = fVec2(1.0f, 1.0f);
	GameObject* attachedObj;
	std::string GetType() { return "Transform"; }
	Transform() {};
	Transform(GameObject* go);
	static std::map<GameObject*, Transform> componentList;
};
