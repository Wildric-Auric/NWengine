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
	RessourcesLoader::LoadShader(path);
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
		printf(path.c_str());
		if (path != "") SetShader(path);
	}
	ImGui::Separator();
}