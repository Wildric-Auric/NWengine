#pragma once
#include "GameObject.h"
#include "DllScripting.h"
#include "ComponentTypes.h"

class DynamicScript: public GameComponent {
public:
    NW_ST_GET_TYPE_IMPL(DynamicScript);

    DllScript* dllScript = 0;

    void Load(const char*);
    void Reload(const char* path = 0);
    void Unload();

    DynamicScript(GameObject*);
};


