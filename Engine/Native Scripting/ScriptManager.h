#pragma once
#include<string>
#include<map>
#include "Utilities.h"
#include "Script.h"


class ScriptManager {
public:
	static std::map<std::string, Scriptable* (*)(GameObject*)> ScriptsMap;
	static std::map<std::string, std::string> scriptList;

	static Scriptable* GetScriptFromDLL(std::string name, GameObject* go);
	static Scriptable* GetScriptFromHeaderFile(std::string name, GameObject* go);
	static Scriptable* CreateScript(std::string name, GameObject* go);

	static void SaveScriptList();
	static void LoadScriptList();
	static bool CompileScripts(); //Should return 0 if action has failed
	static bool CompileScript(std::string element);
};


