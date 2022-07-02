#include "Transform.h"

Transform::Transform(GameObject* go) {
	this->attachedObj = go;
}

std::map<GameObject*, Transform> Transform::componentList;