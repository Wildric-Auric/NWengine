#pragma once
#include "GameObject.h"
#include "DllScripting.h"

class DynamicScript: GameComponent {
    static std::string GetType() { return "DynamicScript"; };

    DllScript* dllScript = 0;

    void Load(const char*);
    void Reload();
    void Unload();

    DynamicScript(GameObject*);
    ~DynamicScript(); 
};


