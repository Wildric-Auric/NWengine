#include "Globals.h"
#include <windows.h>
#include <fstream>

//Paths
std::string Globals::installationDir      = "";
std::string Globals::projDir              = "";

std::string Globals::engineLibDir	      = Globals::installationDir + "Build\\Data\\x86\\EngineDll\\";
std::string Globals::gameLibDir           = Globals::installationDir + "Build\\Data\\x86\\Game\\";
std::string Globals::compilationConfigDir = Globals::installationDir + "Build\\Config\\";

std::string Globals::scriptListPath       = Globals::projDir + "Scripts.NWlist";
std::string Globals::scriptManagerPath    = Globals::projDir + "Compiled\\ScriptManager.cpp";
std::string Globals::compiledScriptDir	  = Globals::projDir + "Compiled\\";
std::string Globals::gamePath             = Globals::projDir + "myGame.exe";
std::string Globals::dllDir			      = Globals::projDir;


void Globals::SetInstallationDir(std::string dir) {
	Globals::installationDir      = dir;
	Globals::compilationConfigDir = Globals::installationDir + "Build\\Config\\";
	Globals::engineLibDir		  = Globals::installationDir + "Build\\Data\\x86\\EngineDll\\";
	Globals::gameLibDir			  = Globals::installationDir + "Build\\Data\\x86\\Game\\";
}

void Globals::SetProjDir(std::string dir) {
	std::fstream file = std::fstream("CurrentProj.NWlist", std::ios_base::out | std::ios_base::trunc);
	file << dir;
	file.close();
	Globals::projDir              = dir;
    Globals::scriptListPath		  = Globals::projDir + "Scripts.NWlist";
    Globals::scriptManagerPath	  = Globals::projDir + "Compiled\\ScriptManager.cpp";
    Globals::compiledScriptDir	  = Globals::projDir + "Compiled\\";
    Globals::gamePath			  = Globals::projDir + "myGame.exe";
    Globals::dllDir			      = Globals::projDir;
}

