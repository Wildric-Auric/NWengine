#pragma once
#include "GameObject.h"
#include "DllScripting.h"
#include "ComponentTypes.h"

class DynamicScript: GameComponent {
    NW_ST_GET_TYPE_IMPL(DynamicScript);

    DllScript* dllScript = 0;

    void Load(const char*);
    void Reload();
    void Unload();

    DynamicScript(GameObject*);
    ~DynamicScript(); 
};


