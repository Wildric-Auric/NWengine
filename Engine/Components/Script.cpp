#include "Script.h"

Script::Script(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
}

std::map<GameObject*, Script> Script::componentList;


