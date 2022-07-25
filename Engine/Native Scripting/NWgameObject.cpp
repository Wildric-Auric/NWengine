#pragma once
#include "NWgameObject.h"


void NWgameObject::AddComponent(std::string type, NW* nws ) {
	std::map<GameObject*, GameComponent>* list = nws->componentsLists["Transform"];
}

void NWgameObject::GetComponent(std::string type, NW* nws) {

}

void NWgameObject::DeleteComponent(std::string type, NW* nws) {

}
