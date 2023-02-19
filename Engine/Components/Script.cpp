#include "Script.h"
#include "Utilities.h"
#include "ScriptManager.h"

#include "Context.h"
#include "Camera.h"
#include "Transform.h"
#include "Sprite.h"

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

	if (this->script != nullptr)
		this->script->Gui();
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

void Scriptable::ShaderCode(void* sprite) {
	Sprite* sprite0 = (Sprite*)sprite;
	Transform* transform = goc->GetComponent<Transform>();
	fVec2 position = transform->position;
	fVec2 scale = transform->scale;

	sprite0->shader->Use();
	sprite0->shader->SetUniform1i("uTex0", 0);
	sprite0->shader->SetVector2("uResolution", (float)Context::NATIVE_WIDTH, (float)Context::NATIVE_HEIGHT);

	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((double)position.x, (double)position.y, sprite0->zbuffer));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	model = glm::rotate(model, DegToRad(transform->rotation), glm::vec3(0, 0, 1));

	sprite0->shader->SetMat4x4("uMvp", &(Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model)[0][0]);
}


