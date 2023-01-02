#pragma once
#include "Maths.h"
#include "Globals.h"
#include "GameObject.h"

class Transform : public GameComponent {
public:
	static std::string GetType() { return "Transform"; };

	fVec2 position = fVec2(0.0f,0.0f);
	fVec2 scale = fVec2(1.0f, 1.0f);
	GameObject* attachedObj;
	Transform() {};
	Transform(GameObject* go);
	void Gui() override;
	static std::map<GameObject*, Transform> componentList;
};
