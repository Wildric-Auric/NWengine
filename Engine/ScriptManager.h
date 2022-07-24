#pragma once
#include<string>
#include "Scripts/player.h"
#include "Scripts/test.h"
#include "Utilities.h"
#include "GameObject.h"

class ScriptManager {
public:
	static Scriptable* GetScriptFromDLL(std::string name, GameObject* go);
	static Scriptable* CreateScript(std::string name, GameObject* go);
};


inline Scriptable* CreateScript(std::string name, GameObject* go) {

	if (name == "player") return new player(go);
	if (name == "test")   return new test(go);
	if (name == "dllTest") return ScriptManager::GetScriptFromDLL("C:\\Users\\HP\\Desktop\\NWengine\\ScriptDlls\\dllTest", go);
	return nullptr;

};


