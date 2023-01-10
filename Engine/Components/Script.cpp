#include "Script.h"
#include "Utilities.h"
#include "ScriptManager.h"

Script::Script(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

void Script::Gui() {
	std::string text = "None";
	if (this->script != nullptr) text = this->script->GetName();
	if (NWGui::FileHolder("Script", text.c_str())) {
		std::string path = GetFile("Text Files\0*.h\0*.*\0");
		std::string filename = "";
		GetFileName(path, &filename);
		if (path != "") script = ScriptManager::CreateScript(filename, attachedObj); //TODO::Get if file is valid
	}
}

void Script::Update() {
#ifdef NW_GAME_BUILD
	if (this->script == nullptr) return;
	this->script.Update()
#else
	if (!Globals::PLAY_MODE) return;
	if (this->script == nullptr) return;
	this->script->Update();
#endif
}

std::map<GameObject*, Script> Script::componentList;


