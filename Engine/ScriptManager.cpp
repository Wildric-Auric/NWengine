#include "ScriptManager.h"
#include "NWscripting.h"

Scriptable* ScriptManager::GetScriptFromDLL(std::string name, GameObject* go) {
	const DllHandle h((name + ".DLL").c_str());

	if (!h.Get()) {
		MessageBox(0, "Could not load DLL", "UnitCallDll", MB_OK);
		return nullptr;
	}

	typedef Scriptable* (*scr)(GameObject*, NW*);
	const scr func = (scr)(GetProcAddress(h.Get(), "GetScript"));

	if (!func) {
		MessageBox(0, "Can't Load Script", "UnitCallDll", MB_OK);
		return nullptr;
	}
	return (*func)(go, NW::ptr );
};


Scriptable* ScriptManager::CreateScript(std::string name, GameObject* go) {
	if (name == "player") return new player(go);
	if (name == "test")   return new test(go);
	if (name == "dllTest") return ScriptManager::GetScriptFromDLL("C:\\Users\\HP\\Desktop\\NWengine\\ScriptDlls\\dllTest", go);
	return nullptr;
}