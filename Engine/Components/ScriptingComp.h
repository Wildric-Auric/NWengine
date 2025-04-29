#pragma once
#include "ComponentTypes.h"
#include "DllScripting.h"
#include "GameObject.h"

class DynamicScript : public GameComponent {
  public:
	NW_ST_GET_TYPE_IMPL(DynamicScript);

	DllScript* dllScript = 0;

	void Load(const char*);
	void Reload(const char* path = 0);
	void Unload();

	DynamicScript(GameObject*);
};
