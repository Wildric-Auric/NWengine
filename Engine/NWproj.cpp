
#include "NWproj.h"
#include "Utilities.h"
#include "vector"
#include "Globals.h"
#include "Context.h"
#include "Scene.h"
#include <fstream>

NWproj::NWproj(const std::string& directory) {
	std::vector<std::string>  li = GetDirFiles(directory, ".NWproj");
	if (li.size() < 1)
		return;
	this->dir  = directory;
	this->name = li[0];
}

std::string NWproj::GetCurrentProjFromInstallationDir() {
	std::vector<std::string> li = GetNWlist(Globals::installationDir + "CurrentProj.NWlist");	
	if (li.size() < 1)
		return "";
	return li[0];
}

NWproj* NWproj::currentProj = nullptr;

void NWproj::Load() {
	std::string path  = this->dir + name;
	std::fstream file = std::fstream(path, std::ios_base::in | std::ios_base::binary);
	if (!file) return;
	Context::SetTitle(("NWengine (" + this->dir + this->name + ")").c_str());
	//Empty file
	file.seekg(0, std::ios::end);
	if (file.tellg() < 1) {
		file.close();
		return;
	}
	file.seekg(0, std::ios::beg);
	//Start reading
	int sizeBuffer;
	file.read((char*)&sizeBuffer, sizeof(int));
	char* data = new char[sizeBuffer + 1];
	data[sizeBuffer] = '\0';
	file.read((char*)data, sizeBuffer);
	this->defaultScenePath = std::string(data);
	delete[] data;
	file.close();
}

void NWproj::Save() {
	std::string path = this->dir + this->name;
	std::fstream file = std::fstream(path, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
	if (!file) return;
	int size = Scene::currentScene->name.size();
	file.write((const char*)&size, sizeof(int));
	file.write(Scene::currentScene->name.c_str(), size);
	int i = 0;
	file.write((const char*)&i, sizeof(int));
	file.close();
}

void NWproj::MakeCurrent() {
	std::fstream file = std::fstream("CurrentProj.NWlist", std::ios_base::out | std::ios_base::trunc);
	file << this->dir;
	Context::dllFlag = NW_RELOAD_DLL;
	file.close();
}
