#include "Sprite.h"
#include "Utilities.h"
#include "Scene.h"

Sprite::Sprite(GameObject* go) {
	this->attachedObj = go;
};

std::map<GameObject*, Sprite> Sprite::componentList;



void Sprite::SetTexture(std::string path, bool alpha, bool repeat) {
	//TODO:: what if you want to load same image with different values of alpha and repeat
	//TODO::Add error handling
	//Automatic load?
	if (path == this->texture->name) return;
	RessourcesLoader::LoadTexture(path, alpha, repeat);
	texture = &Texture::resList[path];
	container.UpdateSize(texture->size.x, texture->size.y);
}

void Sprite::SetTexture(Texture* tex) {
	if (tex->name == this->texture->name) return;
	this->texture = tex;
}

void Sprite::SetShader(std::string path) {
	RessourcesLoader::LoadShader(path);
	shader = &Shader::resList[path];
}

void Sprite::SetSortingLayer(uint32 order) {
	sortingLayer = order;
	if (sortingLayer != lastSortingLayer) {
		zbuffer = 1.0 / ((double)(sortingLayer + 1));
		lastSortingLayer = sortingLayer;
		if (isRendered)
			Scene::currentScene->Rearrange(this);
	}
	
}

void Sprite::Render() {
	isRendered = 1;
	shouldDraw = 1;
}

void Sprite::StopRendering() {
	isRendered = 0;
}

void Sprite::Update() {
	if (!shouldDraw) return;
	Scene::currentScene->Rearrange(this);
	shouldDraw = 0;
}

int Sprite::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Sprite", 6, sizeBuffer);

	WRITE_ON_BIN(data, &sortingLayer, sizeof(sortingLayer), sizeBuffer);


	const char* temp0 = texture->name.c_str();
	WRITE_ON_BIN(data, temp0, texture->name.size(), sizeBuffer);

	const char* temp = shader->name.c_str();
	WRITE_ON_BIN(data, temp, shader->name.size(), sizeBuffer);

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
	SetTexture(std::string(buffer));

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