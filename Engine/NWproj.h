#pragma once

#include <string>
class NWproj {
public:
	NWproj() {};
	NWproj(const std::string& directory);

	std::string dir				 = "";
	std::string name			 = ""; //Only filename
	std::string defaultScenePath = "";

	static NWproj* currentProj;

	void Save();
	void Load();
	void MakeCurrent();

	static std::string GetCurrentProjFromInstallationDir();
};