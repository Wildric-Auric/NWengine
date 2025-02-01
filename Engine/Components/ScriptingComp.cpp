#include "ScriptingComp.h"

DynamicScript::DynamicScript(GameObject* obj) {
    attachedObject = obj;
}

void DynamicScript::Load(const char* path) {
    Loader<DllScript> loader;
    std::string id = path;
    dllScript = loader.LoadFromFileOrGetFromCache((void*)&id, path, nullptr);
}

void DynamicScript::Reload(const char* path) {
    Unload();  
    if (path == 0) {
        DllScriptIdentifier id = GetIDWithAsset<DllScript*, DllScriptIdentifier>(dllScript);
        Load(id.c_str());
        return;
    }
    DllScriptIdentifier id = path;
    Load(id.c_str());
}

void DynamicScript::Unload() {
    if (dllScript == 0) 
        return;
    DllScriptIdentifier id = GetIDWithAsset<DllScript*, DllScriptIdentifier>(dllScript);
    dllScript->Clean();
    EraseRes<DllScript>(id);
    dllScript = 0;
}
