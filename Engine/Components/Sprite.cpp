#include "Sprite.h"
#include "Utilities.h"
#include "Scene.h"
#include "Script.h"
#include "Batch.h"

Sprite::Sprite(GameObject* obj) {
	attachedObject = obj;
	obj->SetDrawCallback(Sprite::DefaultSpriteDrawCallback);
};



void Sprite::SetTexture(std::string path, bool alpha) {
	//TODO::Add error handling
	Texture loader;
	_texId				 = {path.c_str(), alpha};
    texture              = (Texture*)loader.LoadFromFileOrGetFromCache((void*)&_texId, path.c_str(), nullptr);
	container.UpdateSize(texture->_size.x, texture->_size.y);
}

void Sprite::SetTexture(Texture* tex) {
	this->texture		 = tex;
	container.UpdateSize(texture->_size.x, texture->_size.y);
}

void Sprite::SetTexture(const Image* im, TextureIdentifierPtr id) {
	Loader<Texture> l;
	_texId = *(TextureIdentifier*)id;
	texture = l.LoadFromBufferOrGetFromCache(id, (void*)im, id);
	container.UpdateSize(texture->_size.x, texture->_size.y);
}

void Sprite::SetShader(std::string path) {
	Loader<Shader> l;
	Sprite::shader = l.LoadFromFileOrGetFromCache((void*)&path, path.c_str(), nullptr);
}

void Sprite::SetShader(const ShaderText& st, ShaderIdentifier* id) {
		Loader<Shader> l;
		shader = l.LoadFromBufferOrGetFromCache(id, (void*)&st, id);
}

void Sprite::SetShader(Shader* s) {
	shader = s;
}

void Sprite::SetSortingLayer(int order) {
	sortingLayer = order;
	uint64 unsignedLayer = sortingLayer + 0xFFFFFFFF;
	if (sortingLayer != _lastSortingLayer) {
		zbuffer = 1.0 / ((double)(unsignedLayer + 1));
		_lastSortingLayer = sortingLayer;
		if (_isRendered)
			Scene::currentScene->Rearrange(this);
	}	
}

void Sprite::Batch(BatchType type) {
	if (type == BatchType::UNBATCHED) return;
	attachedObject->SetDrawCallback(Batch::DefaultBatchDrawCallback);
	this->_isBatched = type; //TODO::Callsomething in Batch class maybe???
}

void Sprite::UnBatch() {
	attachedObject->SetDrawCallback(Sprite::DefaultSpriteDrawCallback);
	this->_isBatched = BatchType::UNBATCHED;
}

void Sprite::Render() {
	_isRendered = 1;
	_shouldDraw = 1;
}

void Sprite::StopRendering() {
	_isRendered = 0;
}

void Sprite::Update() {
	if (!_shouldDraw) return;
	Scene::currentScene->Rearrange(this);
	_shouldDraw = 0;
}

Sprite::~Sprite() { 
	//this->container.Delete(); No longer vertex array buffer in quad
	
	for (auto iter = Scene::currentScene->drawList.begin(); iter != Scene::currentScene->drawList.end(); ++iter) {
		if (*iter != this)
			continue;
		Scene::currentScene->drawList.erase(iter);
		return;
	}
}


int Sprite::DefaultSpriteDrawCallback(void* data) {
	GameObject* obj = (GameObject*)data;

	Sprite* sprite = obj->GetComponent<Sprite>();

	Scriptable temp;
	Scriptable* scriptable;
	Script* script = obj->GetComponent<Script>();
	if ((script == nullptr) || (script->script == nullptr)) {
		temp = Scriptable();
		scriptable = &temp;
	}
	else
		scriptable = script->script;

	scriptable->goc = obj;
	scriptable->ShaderCode(sprite);

	int textureSlot = 0;
	sprite->texture->Bind(textureSlot);
	sprite->container.Draw();
	sprite->shader->Unuse();

	return sprite->sortingLayer;
}

void BatchExtra::AddAttribute(int num) {
    rawData.resize(rawData.size() + num);
    relativeIndex.push_back(num);
}

void BatchExtra::SetAttribute(int index, void* data) {
    int acc = 0;
    for (int i = 0; i < index; ++i) {
        acc += relativeIndex[i];
    }
    std::memcpy(&rawData[acc], data, sizeof(float) * relativeIndex[index]);
}

void* BatchExtra::GetData(int num) {
    int acc = 0;
    for (int i = 0; i < num; ++i) {
        acc += relativeIndex[i];
    }
    return &rawData[acc];
}

bool BatchExtra::IsCompatible(BatchExtra* other) {
    return IsCompatible(other->relativeIndex.data(), other->relativeIndex.size());
}

bool BatchExtra::IsCompatible(int* container, int size) {
    if (this->relativeIndex.size() != size) return 0;
    for (int i = 0; i < size; ++i) {
        if (this->relativeIndex[i] != container[i]) return 0;
    }
    return 1;
}
