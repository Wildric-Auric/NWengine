#include "Script.h"
#include "Utilities.h"
#include "ScriptManager.h"

#include "Context.h"
#include "Camera.h"
#include "Transform.h"
#include "Sprite.h"
#include "RuntimeManager.h"


Script::Script(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

void Script::Start() {
	if (this->script == nullptr) return;
	this->script->Start();
}

void Script::Update() {
#ifdef NW_GAME_BUILD
	if (this->script == nullptr) return;
	this->script->Update();
#else
	if (!RuntimeManager::__currentMode == EngineMode::PLAY_MODE) return;
	if (this->script == nullptr) return;
	this->script->Update();
#endif
}

//void Script::SetScript(Scriptable* scr) {
//	this->script = scr;
//}


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
	Matrix4<float> model(1.0f);
	ScaleMat(model, fVec3(scale.x * sprite0->container.width, scale.y * sprite0->container.height, 1.0f));
	RotateMat(model, transform->rotation, fVec3(0.0f, 0.0f, 1.0f));
	TranslateMat(model, fVec3(transform->position.x, transform->position.y, sprite0->zbuffer));
	sprite0->shader->SetMat4x4("uMvp", &(Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model).values[0]);
}


