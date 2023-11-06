#include "Transform.h"

Transform::Transform(GameObject* go) {
	this->attachedObj = go;
}

int Transform::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Transform", 9, sizeBuffer);
	WRITE_ON_BIN(data, &this->position.x, sizeof(this->position.x), sizeBuffer);
	WRITE_ON_BIN(data, &this->position.y, sizeof(this->position.y), sizeBuffer);
	WRITE_ON_BIN(data, &this->scale.x, sizeof(this->scale.x),       sizeBuffer);
	WRITE_ON_BIN(data, &this->scale.y, sizeof(this->position.y),    sizeBuffer);
	WRITE_ON_BIN(data, &this->rotation, sizeof(this->rotation),     sizeBuffer);
	return offset + 25;  //Not updated
}

int Transform::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &this->position.x, sizeBuffer);
	READ_FROM_BIN(data, &this->position.y, sizeBuffer);
	READ_FROM_BIN(data, &this->scale.x,  sizeBuffer);
	READ_FROM_BIN(data, &this->scale.y,  sizeBuffer);
	READ_FROM_BIN(data, &this->rotation, sizeBuffer)
	return offset + 16; //Not updated
}

//TODO::The same for all GameComponents
void  Transform::SetGameObject(void* go) {
	attachedObj = (GameObject*)go;
}
void* Transform::GetGameObject() {
	return (void*)attachedObj;
}
//-----
std::map<GameObject*, Transform> Transform::componentList;