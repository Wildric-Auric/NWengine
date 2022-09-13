#pragma once
#include<string>
#include<map>
#include "Utilities.h"
#include "GameObject.h"
//S
#include "Scripts/player.h"
//E

class ScriptManager {
public:
	static std::map<std::string, Scriptable* (*)(GameObject*)> ScriptsMap;
	static Scriptable* GetScriptFromDLL(std::string name, GameObject* go);
	static Scriptable* GetScriptFromHeaderFile(std::string name, GameObject* go);
	static Scriptable* CreateScript(std::string name, GameObject* go);
};


