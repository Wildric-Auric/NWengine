#include <GL/glew.h>
#include <glfw3.h>

#include "Script.h"
#include "Inputs.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Components.h"
#include "Scene.h"
#include "Shader.h"
#include "Primitives.h"

void GameObject::Draw(int8 textureSlot) {
	Sprite* sprite = GetComponent<Sprite>();

	if (sprite == nullptr) return;		//TODO::Improve this test, not testing GameObjects with no sprite
	Transform* transform = GetComponent<Transform>();
	if (transform == nullptr) transform = this->AddComponent<Transform>();

	Scriptable temp;
	Scriptable*	scriptable;
	Script* script = GetComponent<Script>();
	if ((script == nullptr) || (script->script == nullptr)) {
		temp = Scriptable();
		scriptable = &temp;
	}
	else
		scriptable = script->script;

	scriptable->goc = this;
	scriptable->ShaderCode(sprite);

	sprite->texture->Bind(textureSlot);
	sprite->container.Draw();
};

int GameObject::numberOfGameObjects = 0;

void GameObject::Rename(std::string newName) {
	uint16 n = 0;
	name = newName;
	while (1) {
		bool br = 1;
		for (auto it = Scene::currentScene->sceneObjs.begin(); it != Scene::currentScene->sceneObjs.end(); it++) {
			if (it->name == name && &(*it) != this) {
				n += 1;
				br = 0;
				name = newName + std::to_string(n);
			}
		}
		if (br) break;
	}

	if (n == 0) name = newName;
	else name = newName + std::to_string(n);

}

void GameObject::AddToRender() {
	isRendered = 1;
	Scene::currentScene->drawList.push_back(this);
}

void GameObject::StopRendering() {
	isRendered = 0;
}

GameObject::GameObject() {
	numberOfGameObjects += 1;
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
	return nullptr;
};

GameComponent* GameObject::GetComponent(std::string type) {
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
			Scene::currentScene->AddObject(GameObject());
			Scene::currentScene->sceneObjs.back().Rename(name);
			delete[] name;
			return 1;
		}
		delete[] name;
		gc->Deserialize(data, 0);
	}
	return 2;
}


