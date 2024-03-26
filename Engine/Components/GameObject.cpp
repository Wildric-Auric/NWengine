#include <GL/glew.h>

#include "Script.h"
#include "Inputs.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Components.h"
#include "Scene.h"
#include "Shader.h"
#include "Primitives.h"
#include "Batch.h"
#include "Renderer.h"

uint32 GameObject::Draw(int8 textureSlot) {
	Sprite* sprite = GetComponent<Sprite>();
	if (sprite == nullptr) return 0;	//TODO::Improve this test, not testing GameObjects with no sprite
	Transform* transform = GetComponent<Transform>();
	if (transform == nullptr) transform = this->AddComponent<Transform>();

	if (sprite->isBatched) {
		Matrix4<float> model = Matrix4<float>(1.0f);
		
		ScaleMat(model, fVec3(transform->scale.x * sprite->container.width, transform->scale.y * sprite->container.height, 1.0f));
		RotateMat(model, transform->rotation, fVec3(0.0f, 0.0f, 1.0f));
		TranslateMat(model, fVec3(transform->position.x, transform->position.y, sprite->zbuffer));

		Matrix4<float> mvp = Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model;

		fVec4 vert0 = mvp * fVec4(-0.5,  -0.5, 0.0, 1.0);
		fVec4 vert1 = mvp * fVec4(0.5 ,  -0.5, 0.0, 1.0);
		fVec4 vert2 = mvp * fVec4(0.5 ,   0.5,  0.0, 1.0);
		fVec4 vert3 = mvp * fVec4(-0.5,   0.5,  0.0, 1.0);
	
		//The last element of each stride is set in Render()
		float stride[36] = {
			//x         y          z      uv.x uv.y          user.x                    user.y                       user.z               tex
			vert0.x, vert0.y, vert0.z, 0.0, 0.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert1.x, vert1.y, vert0.z, 1.0, 0.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert3.x, vert3.y, vert0.z, 0.0, 1.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0,
			vert2.x, vert2.y, vert0.z, 1.0, 1.0, sprite->vertexAttributes.x, sprite->vertexAttributes.y, sprite->vertexAttributes.z, -1.0
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
		//Room not found->creation of another batch;
		iter->second.push_back(new Batch());
		iter->second.back()->Render(this, stride); 
		//TODO::Not accessing batch data batch vector here, call a function instead

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


