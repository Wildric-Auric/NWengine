#include "DllScripting.h"
#include "Utilities.h"

NW_IMPL_RES_LIST(DllScriptIdentifier, DllScript)

Asset* DllScript::GetFromCache(void* identifier) {
	if (identifier == nullptr)
		return nullptr;
	auto iter = DllScript::resList.find(*(DllScriptIdentifier*)identifier);
	if (iter == DllScript::resList.end()) 
		return nullptr;
	return &iter->second;
}

Asset* DllScript::LoadFromFile(const char* path, void* data) {
    DllScriptIdentifier id = path;
    DllHandle* h = new DllHandle();
   
    h->Load(path);
    return LoadFromBuffer(h, &id);
}

Asset* DllScript::LoadFromBuffer(void* buffer, void* identifier) {
    DllScript* script = 
        &resList.emplace(*(DllScriptIdentifier*)identifier, DllScript()).first->second;
    handle = buffer;
    Move(script);
    return script;
}

void DllScript::Move(Asset* other) {
    ((DllScript*)other)->handle = this->handle;
}

void DllScript::Clean() {
    ((DllHandle*)(handle))->Free();
    delete ((DllHandle*)handle);
    handle = 0;
    EraseRes<DllScript, DllScriptIdentifier>(GetIDWithAsset<DllScript*, DllScriptIdentifier>(this));
}
    
void* DllScript::GetDllFunc(const char* name) {
    return GetDllFunction((DllHandle*)handle, name);
}


std::vector<char> DllScripting::vcVarsEnv;

bool DllScripting::CompileDll(const std::vector<std::string>& srcFile, const char* dllPath) {
    //std::string cmd = "cmd /C \"if not defined VisualStudioVersion (call vcvars64) & "; using VisualStudioVersion breaks it for some reason
    //std::string cmd = "cmd /C \"vcvars64.bat & ";
    /// del / s / q / f \"%TMP%\\_CL_*\" > nul
    std::string cmd = "cmd.exe /C \"";
    cmd += "cl.exe /Fo:tmp /LD ";
    for (const std::string& f : srcFile) {
       cmd += f + " ";
    }
    cmd += "/link /OUT:" + std::string(dllPath);
    cmd += " >> NWDllOut.log\" & del /Q *.obj *.exp *.lib";
    return Exec(cmd, vcVarsEnv.data());
}

bool DllScripting::Init() {
    return GetVcVarsEnv(vcVarsEnv);
}

void DllScripting::Destroy() {

}


