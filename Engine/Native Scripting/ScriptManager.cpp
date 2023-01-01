#include "ScriptManager.h"

Scriptable* ScriptManager::GetScriptFromDLL(std::string name, GameObject* go) {
	//Deprecated, scripts are no longer dlls
	const DllHandle h((name + ".dll").c_str());
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

Scriptable* ScriptManager::GetScriptFromHeaderFile(std::string name, GameObject* go) {
	auto result = ScriptManager::ScriptsMap.find(name);
	if (result == ScriptManager::ScriptsMap.end()) {
		Scriptable* nullReturn = new Scriptable(go);
		nullReturn->__nwname = name;
		return nullReturn;
	}
	return result->second(go);
};

Scriptable* ScriptManager::CreateScript(std::string name, GameObject* go) {
	return ScriptManager::GetScriptFromHeaderFile(name, go);
}


//BEG_PPP
std::map<std::string, Scriptable* (*)(GameObject*)> ScriptManager::ScriptsMap = {

};
//END_PPP
