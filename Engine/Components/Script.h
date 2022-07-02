#pragma once
#include <GameObject.h>

class Script {
public:
	Script() {};
	Script(GameObject* attachedObj);
	GameObject* attachedObj;
	Scriptable* script = nullptr;
	static std::map<GameObject*, Script> componentList;
};

