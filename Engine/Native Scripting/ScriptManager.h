#pragma once
#include<string>
#include "Utilities.h"
#include "GameObject.h"

class ScriptManager {
public:
	static Scriptable* GetScriptFromDLL(std::string name, GameObject* go);
	static Scriptable* CreateScript(std::string name, GameObject* go);
};


