#include "Sprite.h"
#include "Utilities.h"

Sprite::Sprite(GameObject* go) {
	this->go = go;
};

std::map<GameObject*, Sprite> Sprite::componentList;



void Sprite::SetTexture(std::string path, bool alpha, bool repeat) {
	//TODO:: what if you want to load same image with different values of alpha and repeat
	//TODO::Add error handling
	//Automatic load?
	if (path == this->texture->name) return;
	RessourcesLoader::LoadTexture(path, alpha, repeat);
	texture = &Texture::resList[path];
	container = Quad(iVec2(0, 0), texture->size.x, texture->size.y);
}

void Sprite::SetShader(std::string path) {
	RessourcesLoader::ReloadShader(path);
	shader = &Shader::resList[path];
}

void Sprite::SetSortingLayer(uint32 order) {
	sortingLayer = order;
	if (sortingLayer != lastSortingLayer) {
		zbuffer = 1.0 / ((double)(sortingLayer + 1));
		lastSortingLayer = sortingLayer;
	}
}

void Sprite::Gui() {
	if (NWGui::DragValue<uint32>("Layering Order", &sortingLayer, ImGuiDataType_U32, 1, 1.0f, 0.0f, 6000.0f))
		SetSortingLayer(sortingLayer);
	ImGui::Separator();
	if (NWGui::FileHolder("Texture", texture->name)) {
		std::string path = GetFile("Image Files\0*.png;*.jpeg;*.jpg\0*.*\0");
		if (path != "") SetTexture(path);
	}
	ImGui::Separator();
	if (NWGui::FileHolder("Shader", shader->name)) {
		std::string path = GetFile("Shader Files\0*.shader\0*.*\0");
		if (path != "") SetShader(path);
	}
	ImGui::Separator();
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