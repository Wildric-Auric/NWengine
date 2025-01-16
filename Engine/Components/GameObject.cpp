#include <GL/glew.h>

#include "Script.h"
#include "Inputs.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "Shader.h"
#include "Primitives.h"
#include "Batch.h"
#include "Renderer.h"

int GameObject::numberOfGameObjects = 0;


int GameObject::Draw() {
	if (_drawProc == nullptr) return 0;
	return _drawProc(this);
};


void GameObject::SetDrawCallback(DrawCallback proc) {
	_drawProc = proc;
}


GameObject::GameObject() {
	numberOfGameObjects += 1;
};


GameObject::GameObject(const GameObject& other) {
	for (auto iter = other.components.begin(); iter != other.components.end(); ++iter) {
		this->components.emplace(iter->first, iter->second);
		iter->second->SetGameObject((void*)this);
	}
};


GameObject::~GameObject() {};

void GameObject::DeleteComponent(std::string typeName) {
	if (components.find(typeName) == components.end()) return;
	delete components[typeName];
	components.erase(typeName);
}

void GameObject::DeleteComponents() {
	for (std::map<std::string, GameComponent*>::iterator component = components.begin(); component != components.end(); component++) {
		delete component->second;
	}
}
//std::map< GameObject*, GameComponent > GameComponent::componentList;

GameComponent* GameObject::AddComponent(std::string type) {
	ADD_COMPONENT(Transform         , type);
	ADD_COMPONENT(Sprite            , type);
	ADD_COMPONENT(ParticleSystem    , type);
	ADD_COMPONENT(AudioEmitter      , type);
	ADD_COMPONENT(AudioListener     , type);
	ADD_COMPONENT(Camera			, type);
	ADD_COMPONENT(Script            , type);
	ADD_COMPONENT(Collider			, type);
	ADD_COMPONENT(Text              , type);
	return nullptr;
};

GameComponent* GameObject::GetComponent(const std::string& type) {
	std::map<std::string, GameComponent*>::iterator pair = components.find(type);
	if (pair == components.end()) return nullptr;
	return pair->second;
}

int GameObject::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	const char* temp  = this->name.c_str();

	WRITE_ON_BIN(data, temp, this->name.size(), sizeBuffer);

	for (std::map<std::string, GameComponent*>::iterator iter = this->components.begin(); iter != this->components.end(); iter++) {
		iter->second->Serialize(data, 0);
	}
	return 0;
}
int GameObject::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	while (!data->fail()) {

		data->read((char*)&sizeBuffer, sizeof(int));
		if (sizeBuffer < 1) return 0;
		char* name = new char[sizeBuffer + 1];
		data->read(name, sizeBuffer);
		name[sizeBuffer] = '\0';

		GameComponent* gc = AddComponent(name);

		if (gc == nullptr) {
			GameObject& obj =  Scene::currentScene->AddObject();
			Scene::currentScene->Rename(name, &obj);
			delete[] name;
			return 1;
		}
		delete[] name;
		gc->Deserialize(data, 0);
	}
	return 2;
}


