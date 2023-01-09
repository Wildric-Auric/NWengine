#include "Transform.h"

Transform::Transform(GameObject* go) {
	this->attachedObj = go;
}

void Transform::Gui() {
	static iVec2 guiPosition;
	guiPosition = position;
	if (NWGui::DragValue<int>("Position", &guiPosition.x, ImGuiDataType_S32, 2)) {
		position = guiPosition;
	};
	ImGui::Separator();
	NWGui::DragValue<float>("Scale", &scale.x, ImGuiDataType_Float, 2, 0.01f);
	ImGui::Separator();
}

int Transform::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Transform", 9, sizeBuffer);
	WRITE_ON_BIN(data, &this->position.x, sizeof(this->position.x), sizeBuffer);
	WRITE_ON_BIN(data, &this->position.y, sizeof(this->position.y), sizeBuffer);
	WRITE_ON_BIN(data, &this->scale.x, sizeof(this->scale.x),       sizeBuffer);
	WRITE_ON_BIN(data, &this->scale.y, sizeof(this->position.y),    sizeBuffer);
	return offset + 25; 
}

int Transform::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &this->position.x, sizeBuffer);
	READ_FROM_BIN(data, &this->position.y, sizeBuffer);
	READ_FROM_BIN(data, &this->scale.x, sizeBuffer);
	READ_FROM_BIN(data, &this->scale.y, sizeBuffer);

	return offset + 16;
}

std::map<GameObject*, Transform> Transform::componentList;