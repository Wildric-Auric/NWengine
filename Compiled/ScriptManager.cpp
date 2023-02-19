#include "ScriptManager.h"
#include "Builder.h"

Scriptable* ScriptManager::GetScriptFromDLL(std::string name, GameObject* go) {
	//Deprecated, scripts are no longer dlls
	//const DllHandle h((name + ".dll").c_str());
	//if (!h.Get()) {
	//	MessageBox(0, "Could not load DLL", "UnitCallDll", MB_OK);
	//	return nullptr;
	//}

	//typedef Scriptable* (*scr)(GameObject*);
	//const scr func = (scr)(GetProcAddress(h.Get(), "GetScript"));

	//if (!func) {
	//	MessageBox(0, "Can't Load Script", "UnitCallDll", MB_OK);
	//	return nullptr;
	//}
	//return (*func)(go);
	return nullptr;
};

Scriptable* ScriptManager::GetScriptFromHeaderFile(std::string name, GameObject* go) {
	if (name == "") return nullptr;
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

void ScriptManager::SaveScriptList() {
	std::fstream file(Globals::scriptListPath, std::ios::out, std::ios::trunc);
	if (!file) return;
	for (std::map<std::string, std::string>::iterator iter = ScriptManager::scriptList.begin(); iter != ScriptManager::scriptList.end(); iter++)
		file << iter->second << iter->first << ".h" << "\n";
}

void ScriptManager::LoadScriptList() {
	std::fstream file(Globals::scriptListPath, std::ios::in);
	if (!file) return;
	scriptList.clear();
	for (std::string line = ""; std::getline(file, line);) {
		std::string fileName = "";
		std::string root	 = "";
		GetFileName(line, &fileName, nullptr, &root);
		scriptList.insert(std::make_pair(fileName, root));
	}
}

std::map<std::string, std::string> ScriptManager::scriptList = {};
//BEG_PPP
#include "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Ressources\\Scripts\\player.h"
std::map<std::string, Scriptable* (*)(GameObject*)> ScriptManager::ScriptsMap = {

  {"player",player::GetScript}
};
//END_PPP

bool ScriptManager::CompileScripts() {
	if (ScriptManager::scriptList.size() < 1) return 0;
	Builder::objs.clear();
	Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
	for (std::map < std::string, std::string > ::iterator iter = ScriptManager::scriptList.begin(); iter != ScriptManager::scriptList.end(); iter++) {
		//std::string root = "";
		//std::string filename = "";
		//GetFileName(iter->second, &filename, nullptr, &root);
		Builder::IncludeDir.push_back(iter->second);
		Builder::objs.push_back(iter->second + iter->first + ".cpp");
	}
	Builder::Compile(Globals::compiledScriptDir);
	Exec("builder.bat");
	return 1;
}

bool ScriptManager::CompileScript(std::string element) {
	auto iter = ScriptManager::scriptList.find(element);
	if (iter == ScriptManager::scriptList.end()) return 0;
	Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
	Builder::IncludeDir.push_back(iter->second);
	Builder::CompileInd(iter->second + iter->first + ".cpp", Globals::compiledScriptDir);
	Exec("builder.bat");
	return 1;
}


bool ScriptManager::CompileScriptManager() {
	Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
	ScriptManager::SaveScriptList();
	for (std::map<std::string, std::string>::iterator iter = ScriptManager::scriptList.begin(); iter != ScriptManager::scriptList.end(); iter++) {
		Builder::objs.push_back(Globals::compiledScriptDir + iter->first + ".obj");
	}
	//Compile script manager
	int size = Builder::objs.size();
	Builder::InitScripts(Globals::scriptListPath, Globals::scriptManagerPath);
	while (Builder::objs.size() > size)
		Builder::objs.pop_back(); //Bad solution to refactor

	Builder::CompileInd(Globals::scriptManagerPath, Globals::compiledScriptDir);
	Exec("builder.bat");
	return 1;
}
