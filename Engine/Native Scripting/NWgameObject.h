#pragma once

#include "GameObject.h"
#include "NWscripting.h"

class NWgameObject {
private:
	GameObject go;
public:
	void GetComponent(std::string type, NW* nws);
	void AddComponent(std::string type, NW* nws);
	void DeleteComponent(std::string type, NW* nws);
	void Rename(std::string newname);
};