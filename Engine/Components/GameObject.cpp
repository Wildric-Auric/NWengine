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
#include "Batch.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

uint32 GameObject::Draw(int8 textureSlot) {
	Sprite* sprite = GetComponent<Sprite>();
	if (sprite == nullptr) return 0;	//TODO::Improve this test, not testing GameObjects with no sprite
	Transform* transform = GetComponent<Transform>();
	if (transform == nullptr) transform = this->AddComponent<Transform>();

	if (sprite->isBatched) {
		glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((double)transform->position.x, (double)transform->position.y, sprite->zbuffer));
		model = glm::rotate(model, DegToRad(transform->rotation), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(transform->scale.x * sprite->container.width, transform->scale.y * sprite->container.height, 1.0f));
		glm::mat4x4 mvp   = Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model;

		glm::vec4 vert0 = mvp * glm::vec4(-0.5,  -0.5, 0.0, 1.0);
		glm::vec4 vert1 = mvp * glm::vec4(0.5 ,  -0.5, 0.0, 1.0);
		glm::vec4 vert2 = mvp * glm::vec4(0.5 ,   0.5,  0.0, 1.0);
		glm::vec4 vert3 = mvp * glm::vec4(-0.5,   0.5,  0.0, 1.0);
	
		//The last element of each stride is set in Render()
		float stride[36] = {
			//x         y          z      uv.x uv.y          user.x                    user.y                       user.z               tex
			vert0[0], vert0[1], vert0[2], 0.0, 0.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert1[0], vert1[1], vert1[2], 1.0, 0.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert3[0], vert3[1], vert3[2], 0.0, 1.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert2[0], vert2[1], vert2[2], 1.0, 1.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0
		}; //TODO:: The free buffer accessible to the user
		//First layer batch creation
		std::unordered_map<uint32, std::vector<Batch*>>::iterator iter = Batch::batchMap.find(sprite->sortingLayer);
		if (iter == Batch::batchMap.end()) {
			Batch::batchMap.insert(Batch::batchMap.end(), std::make_pair(sprite->sortingLayer, std::vector<Batch*>{new Batch()}))
			->second.back()->Render(this, stride);
			return sprite->sortingLayer;
		}
		//Find room in batch
		for (Batch* batchGroup : iter->second) {
			if (batchGroup->Render(this, stride)) return sprite->sortingLayer; 
		}
		//Room not found->creation of another batch
		iter->second.push_back(new Batch());
		iter->second.back()->Render(this, stride);

		return sprite->sortingLayer;
	}

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

	return sprite->sortingLayer;
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
	ADD_COMPONENT(TextHandler       , type);
	ADD_COMPONENT(Renderer          , type);
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


