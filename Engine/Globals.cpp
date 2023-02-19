#include "Globals.h"
#include <windows.h>

double Globals::fps = 60.0;
double Globals::deltaTime = 0.016;        //Read only in everyfile except mainloop
//Consts      
bool const Globals::DEBUG_MODE = true;
//Uniforms
float Globals::uTime = 0;
//Paths
std::string Globals::projectDir        = "C:\\\\Users\\\\HP\\\\source\\\\repos\\\\Wildric-Auric\\\\NWengine\\\\";
std::string Globals::scriptListPath    = "Scripts.NWlist";
std::string Globals::scriptManagerPath = "Compiled\\\\ScriptManager.cpp";
std::string Globals::compilationConfigDir = "C:\\\\Users\\\\HP\\\\source\\\\repos\\\\Wildric-Auric\\\\NWengine\\\\Build\\\\Config\\\\";
std::string Globals::compiledScriptDir = "Compiled\\\\";
std::string Globals::dllDir			   = projectDir;
std::string Globals::gamePath          = projectDir + "myGame.exe";
std::string Globals::engineLibDir	   = Globals::projectDir + "Build\\\\Data\\\\x86\\\\EngineDll\\\\";
std::string Globals::gameLibDir        = Globals::projectDir + "Build\\\\Data\\\\x86\\\\Game\\\\";
bool Globals::PLAY_MODE = 0;