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
		std::string path	 = GetFile("Text Files\0*.h\0*.*\0");
		std::string filename = "";
		std::string root	 = "";
		GetFileName(path, &filename, nullptr,&root);
		if (path == "") return;
		script = ScriptManager::CreateScript(filename, attachedObj); //TODO::Get if file is valid
		ScriptManager::scriptList.insert(std::make_pair(filename, root));
	}
}

void Script::Update() {
#ifdef NW_GAME_BUILD
	if (this->script == nullptr) return;
	this->script->Update();
#else
	if (!Globals::PLAY_MODE) return;
	if (this->script == nullptr) return;
	this->script->Update();
#endif
}

int Script::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Script", 6, sizeBuffer);
	if (this->script == nullptr) {
		WRITE_ON_BIN(data, "", 0, sizeBuffer);
		return 0;
	};
	auto path = ScriptManager::scriptList.find(this->script->GetName());
	if (path == ScriptManager::scriptList.end()) {
		WRITE_ON_BIN(data, "", 0, sizeBuffer);
	}
	else {
		WRITE_ON_BIN(data, (path->second + path->first + ".h").c_str(), (path->second + path->first + ".h").size(), sizeBuffer);
	}

	return 0;
};

int Script::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	char* buffer = new char[512];

	READ_FROM_BIN(data, buffer, sizeBuffer);
	buffer[sizeBuffer] = '\0';
	std::string filename = "";
	std::string root	 = "";
	GetFileName(buffer, &filename, nullptr, &root);
	if (filename != "") {
		ScriptManager::scriptList.insert(std::make_pair(filename, root)); //TODO::Encapsulate these two lines  within a function?
		this->script = ScriptManager::CreateScript(filename, this->attachedObj);
	}

	delete[] buffer;
	return 0;
};



std::map<GameObject*, Script> Script::componentList;


