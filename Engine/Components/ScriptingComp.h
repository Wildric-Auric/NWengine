#pragma once
#include "GameObject.h"
#include "DllScripting.h"

class DynamicScript: public GameComponent {
public:
    static std::string GetType() { return "DynamicScript"; };

    DllScript* dllScript = 0;

    void Load(const char*);
    void Reload(const char* path = 0);
    void Unload();

    DynamicScript(GameObject*);
};


