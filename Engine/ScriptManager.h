#pragma once
#include<string>
#include "Scripts/player.h"
#include "Scripts/test.h"
#include "Utilities.h"


inline Scriptable* GetScriptFromDLL(std::string name, GameObject* go) {
	const DllHandle h((name + ".DLL").c_str());

	if (!h.Get()) {
		MessageBox(0, "Could not load DLL", "UnitCallDll", MB_OK);
		return nullptr;
	}

	typedef Scriptable* (*scr)(GameObject*);
	const scr func = (scr)(GetProcAddress(h.Get(), "GetScript"));

	if (!func) {
		MessageBox(0, "Can't Load Script", "UnitCallDll", MB_OK);
		return nullptr;
	}
	return (*func)(go);
};

inline Scriptable* CreateScript(std::string name, GameObject* go) {

	if (name == "player") return new player(go);
	if (name == "test")   return new test(go);
	if (name == "dllTest") return GetScriptFromDLL("C:/Users/HP/Desktop/NWengine/Ressources/Scripts/dllTest", go);
	return nullptr;

};


