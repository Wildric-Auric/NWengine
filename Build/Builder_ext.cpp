#include "Builder.h"
#include <iostream>

#define HELP_LOG "/C : Compile engine and scripts, will generate Object files; note that it compiles as THE EXECUTABLE USING NWengine DLL \n------------\n"\
				 "/CDebug: Will compile engine files to obj for NWengine debug executable, no scripting or game build sytem will be available \n------------\n"\
				 "/CGame: Will compile files for game executable build \n------------\n"\
			     "/CDll: Will compile files that will be linked as NWengine.DLL \n------------\n"\
			     "/Lib: Will merge obj files in temporary build directory into .lib file \n------------\n"\
				 "/L: link files to generate executable \n------------\n"\
				 "/LDll: link files to generate Dll"
#define UNK_LOG  "Unknown command. Existing commands are the following ones\n"

int main(int argc, const char* argv[]) {
	if (argc <= 1)  {
		std::cout << HELP_LOG << std::endl;
		return 0;
	}
	std::string cur = "";
	for (int i = 1; i < argc; i++) {
		cur = argv[i];
		if (cur == "/CDebugExe") {
			Builder::IncludeDir = Builder::GetDirs("Build\\Config\\Additional include.NWlist");
			Builder::objs				= Builder::GetDirs("Build\\Config\\Files.NWlist");
			Builder::PreprocessorMacros = Builder::GetDirs("Build\\Config\\Macros.NWlist");
			Builder::Compile("Build\\\\temp\\\\");
		}
		else if (cur == "/C") {
			Builder::IncludeDir = Builder::GetDirs("Build\\Config\\Additional include.NWlist");
			Builder::objs = {
				"Engine\\Source.cpp",
				"Engine\\NWengine.cpp"
			};
			Builder::PreprocessorMacros = Builder::GetDirs("Build\\Config\\Macros.NWlist");
			Builder::PreprocessorMacros.push_back("NW_DLL_ENGINE"); //Funny story
			Builder::Compile("Build\\\\temp\\\\");
		}

		else if (cur == "/CGame") {
			Builder::IncludeDir = Builder::GetDirs("Build\\Config\\Additional include.NWlist");
			Builder::objs = Builder::GetDirs("Build\\Config\\Files.NWlist");
			Builder::PreprocessorMacros = Builder::GetDirs("Build\\Config\\Macros.NWlist");
			Builder::PreprocessorMacros.push_back("NW_GAME_BUILD");

			Builder::Compile("Build\\\\temp\\\\");
		}

		else if (cur == "/CDll") {
			Builder::IncludeDir = Builder::GetDirs("Build\\Config\\Additional include.NWlist");
			Builder::objs = Builder::GetDirs("Build\\Config\\Files.NWlist");
			Builder::PreprocessorMacros = Builder::GetDirs("Build\\Config\\Macros.NWlist");
			Builder::PreprocessorMacros.push_back("_USRDLL");
			Builder::PreprocessorMacros.push_back("_WINDLL");

			Builder::Compile("Build\\\\temp\\\\");
		}

		else if (cur == "/L") {
			Builder::LibsDir = Builder::GetDirs("Build\\Config\\Libs Dir.NWlist");
			Builder::staticLibs = Builder::GetDirs("Build\\Config\\Libs.NWlist");
			Builder::objs.clear();
			Builder::objs.push_back("Build\\temp\\*.obj");
			Builder::Link("Build\\temp\\temp.exe");
		}

		else if (cur == "/Lib") {
			Builder::objs.clear();
			Builder::objs.push_back("Build\\temp\\*.obj");
			Builder::GenLib("Build\\temp\\temp.lib");
		}

		else if (cur == "/LDll") {
			Builder::LibsDir = Builder::GetDirs("Build\\Config\\Libs Dir.NWlist");
			Builder::staticLibs = Builder::GetDirs("Build\\Config\\Libs.NWlist");
			Builder::objs.clear();
			Builder::objs.push_back("Build\\temp\\*.obj");
			Builder::Link("Build\\temp\\temp.dll", 1);
		}
		else {
			std::cout << UNK_LOG  << HELP_LOG << std::endl;
		}
	}
	return 0;
}