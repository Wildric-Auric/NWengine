#include "Sprite.h"
#include "Utilities.h"
#include "Scene.h"
#include "Script.h"
#include "Batch.h"

Sprite::Sprite(GameObject* obj) {
	this->attachedObj = obj;
	obj->SetDrawCallback(Sprite::DefaultSpriteDrawCallback);
};

std::map<GameObject*, Sprite> Sprite::componentList;



void Sprite::SetTexture(std::string path, bool alpha) {
	//TODO::Add error handling
	Texture loader;
	_texId				 = {path.c_str(), alpha};
    texture              = (Texture*)loader.LoadFromFileOrGetFromCache((void*)&_texId, path.c_str(), nullptr);
	container.UpdateSize(texture->_size.x, texture->_size.y);
}

void Sprite::SetTexture(Texture* tex) {
	this->texture		 = tex;
	this->texture->_size = tex->_size;
	container.UpdateSize(texture->_size.x, texture->_size.y);
}

void Sprite::SetShader(std::string path) {
	Shader loader;
	Sprite::shader = (Shader*)loader.LoadFromFileOrGetFromCache((void*)&path, path.c_str(), nullptr);
}

void Sprite::SetSortingLayer(uint32 order) {
	sortingLayer = order;
	if (sortingLayer != _lastSortingLayer) {
		zbuffer = 1.0 / ((double)(sortingLayer + 1));
		_lastSortingLayer = sortingLayer;
		if (_isRendered)
			Scene::currentScene->Rearrange(this);
	}	
}

void Sprite::Batch(BatchType type) {
	if (type == BatchType::UNBATCHED) return;
	attachedObj->SetDrawCallback(Batch::DefaultBatchDrawCallback);
	this->_isBatched = type; //TODO::Callsomething in Batch class maybe???
}

void Sprite::UnBatch() {
	attachedObj->SetDrawCallback(Sprite::DefaultSpriteDrawCallback);
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

int Sprite::Serialize(std::fstream* data, int offset) {
	TextureIdentifier texId;

	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Sprite", 6, sizeBuffer);

	WRITE_ON_BIN(data, &sortingLayer, sizeof(sortingLayer), sizeBuffer);


	const char* temp0 = texId.name.c_str();
	WRITE_ON_BIN(data, temp0, texId.name.size(), sizeBuffer); //TODO::Save transparency too

	const char* temp = shader->_identifier.c_str();
	WRITE_ON_BIN(data, temp, shader->_identifier.size(), sizeBuffer);

	return 0;
};

int Sprite::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	//loads layer order
	READ_FROM_BIN(data, &sortingLayer, sizeBuffer);
	SetSortingLayer(sortingLayer);
	//loads texture
	char* buffer = new char[512]; //TODO::Macro
	READ_FROM_BIN(data, buffer, sizeBuffer);
	buffer[sizeBuffer] = '\0';
	SetTexture(std::string(buffer), 1); //TODO::Serialize alpha

	//loads shader
	buffer = new char[512];
	READ_FROM_BIN(data, buffer, sizeBuffer);
	buffer[sizeBuffer] = '\0';
	SetShader(std::string(buffer));
	delete[] buffer;

	return 0;
};

void  Sprite::SetGameObject(void* go) {
	this->attachedObj = (GameObject*)go;
}
void* Sprite::GetGameObject() {
	return (void*)attachedObj;
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


unsigned int Sprite::DefaultSpriteDrawCallback(void* data) {
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

	return sprite->sortingLayer;
}
