#pragma once
#include "Asset.h"
#include <unordered_map>
#include <string>


typedef std::string DllScriptIdentifier;

class DllScript : public Asset {
    public:
    void* handle = 0;
    Asset* GetFromCache(void* identifier) override;
    Asset* LoadFromFile(const char* path, void* data = nullptr) override;
    Asset* LoadFromBuffer(void* buffer, void* identifier) override;
    void* GetDllFunc(const char*);
    template<typename T> T GetDllFunc(const char* funcName) {
        return (T)GetDllFunc(funcName);
    }
    void Move(Asset* other) override;
    void Clean() override;

    NW_DECL_RES_LIST(DllScriptIdentifier, DllScript);
};

class DllScripting { 
    public:
    static std::vector<char> vcVarsEnv;
    static bool CompileDll(const std::vector<std::string>& srcFile, const char* dllPath);
    static bool Init();
    static void Destroy();
};

