#include <GL/glew.h>

#include "Script.h"
#include "Inputs.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "Shader.h"
#include "Primitives.h"
#include "Batch.h"
#include "ScriptingComp.h"
#include "LineRenderer.h"
#include "CircleRenderer.h"

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
		iter->second->SetGameObject(this);
	}
}


GameObject::~GameObject() {};

void GameObject::DeleteComponent(uint32 typeName) {
	if (components.find(typeName) == components.end()) return;
	components[typeName]->OnDelete();
	delete components[typeName];
	components.erase(typeName);
}

void GameObject::DeleteComponents() {
	for (auto component = components.begin(); component != components.end(); component++) {
		component->second->OnDelete();
		delete component->second;
	}
	components.clear();
}

GameComponent* GameObject::AddComponent(const std::string& type) {
	ADD_COMPONENT(Transform         , type);
	ADD_COMPONENT(Sprite            , type);
	ADD_COMPONENT(ParticleSystem    , type);
	ADD_COMPONENT(AudioEmitter      , type);
	ADD_COMPONENT(MultiAudioEmitter , type);
	ADD_COMPONENT(AudioListener     , type);
	ADD_COMPONENT(Camera			, type);
	ADD_COMPONENT(Script            , type);
	ADD_COMPONENT(Collider			, type);
	ADD_COMPONENT(CircleCollider, type);
	ADD_COMPONENT(Text              , type);
	ADD_COMPONENT(DynamicScript     , type);
	ADD_COMPONENT(BatchExtra        , type);
	ADD_COMPONENT(LineRenderer      , type);
	ADD_COMPONENT(CircleRenderer    , type);
	return nullptr;
};

GameComponent* GameObject::GetComponent(const uint32 type) {
	auto pair = components.find(type);
	if (pair == components.end()) return nullptr;
	return pair->second;
}

