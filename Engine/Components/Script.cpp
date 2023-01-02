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

std::map<GameObject*, Script> Script::componentList;


