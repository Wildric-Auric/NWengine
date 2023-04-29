#include <fstream>
#include <map>

#include "Builder.h"
#include "Utilities.h"
#include "Maths.h"
#include <iostream>

#ifndef BUILDER_EXT

#include "Globals.h"
#include "ScriptManager.h"

#endif

#ifdef _WINDLL
#include "Context.h"
#endif

std::vector<std::string> Builder::IncludeDir = {};

std::vector<std::string> Builder::LibsDir = {};

std::vector<std::string> Builder::staticLibs = {};

std::vector<std::string> Builder::objs = {};                                                                                                      
                                   

std::vector<std::string> Builder::PreprocessorMacros = {
    "GLEW_STATIC",
    "WIN32",
    "WIN_32",
    "_CRT_SECURE_NO_WARNINGS",
    "_CONSOLE",
    "_MBCS"   
    //_USRDLL and _WINDLL for DLL
};

std::string Builder::runtimeLib = "/MD"; //C runtime library Dynamically linked 

void Builder::InitCompilation(std::ofstream* ofs, std::string outputDir) {
    outputDir = ToDoubleBackSlash(outputDir);
    *ofs << "@echo off\n";
    *ofs << " if not defined DevEnvDir (call vcvars32)\n";

    //Getting it's name
    *ofs << "cl /EHsc ";
    *ofs << runtimeLib << " ";  //Runtime library
    *ofs << "/c ";

    for (std::string MAC : PreprocessorMacros) 
        *ofs << " /D" << MAC << " ";
    *ofs << "^\n";
    *ofs << " /Fo\""<< outputDir << "\" ";

    bool b = 0;
    //Adding include dirs
    for (std::string dir : IncludeDir) {
        dir = ToDoubleBackSlash(dir); //Fuck msvc and its double backslash
        if (b) {
            *ofs << "^\n ";
        }
        *ofs << "/I " << "\"" << dir << "\"";
        b = 1;
    };
}

//Output dir: only directory
void Builder::GenerateCompilationData(std::ofstream* ofs, std::string file, std::string outputDir) {
    std::string filename = "";
    std::string extension = "";
    GetFileName(file, &filename, &extension);
    //Adding cpp file
    file = ToDoubleBackSlash(file); 
    *ofs << " \"" << file << "\"";
    *ofs << "^\n";
}

//Compiles the file passed as first argument
void Builder::CompileInd(std::string file, std::string outputDir) {
    //preprocessor macros
    std::ofstream ofs("Builder.bat");
    if (!ofs) return;
    InitCompilation(&ofs, outputDir);
    GenerateCompilationData(&ofs, file, outputDir);
    ofs << " > log.txt";
    ofs.close();
}

//Compiles all files in objs vector
void Builder::Compile(std::string outputDir) {
    std::ofstream ofs("Builder.bat");
    if (!ofs) return;
    InitCompilation(&ofs, outputDir);
    for (std::string file : objs)
        GenerateCompilationData(&ofs, file, outputDir);
    ofs << " > log.txt";
    ofs.close();
}

//output is directory + name
void Builder::Link(std::string output, bool isDll) {
    output = ToDoubleBackSlash(output);
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << " if not defined DevEnvDir (call vcvars32)\n";
    ofs << "LINK ";
    for (std::string obj : objs) {
        ofs << "\"" << obj << "\"" << "^\n ";
    }
    if (isDll)
        ofs <<  "/DLL ";
    for (std::string dir : LibsDir) {
        dir = ToDoubleBackSlash(dir);
        ofs << "/LIBPATH:" << "\"" <<dir<< "\""<< "^\n ";
    }
    for (std::string lib : staticLibs) {
        ofs << lib << "^\n ";
    }
    ofs << "/OUT:"  << output << " > log.txt";
    ofs.close();
}

void Builder::GenLib(std::string output) {
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << " if not defined DevEnvDir (call vcvars32)\n";
    ofs << "lib ";
    for (std::string obj : objs) {
        obj = ToDoubleBackSlash(obj);
        ofs <<"\n" << obj << "^\n";
    }
    ofs << " /OUT:" << output;
}

//Loc is directory where 
void Builder::InitScripts(std::string scriptList, std::string scriptManager) {
    //TODO::Wrapper of ifstream and ofstream with error handling
    //Reading used scripts names and writing to ScriptManager.cpp
    std::ifstream ifs(scriptList);
    if (!ifs) {
        std::cout << "Can't open scripts file"<< std::endl;
        return; 
    }
    std::ofstream ofs;

    std::string scriptMap = "";
    std::string scripts   = "";
    std::string fileName  = "";
    std::string root      = "";
    //Iterating over lines in scripts' NWlist
    for (std::string line; std::getline(ifs, line);) {
        GetFileName(line, &fileName, nullptr, &root);
        scriptMap += "\n  {\"" + fileName + "\"," + fileName + "::GetScript" + "},";
        root = ToDoubleBackSlash(root);
        objs.push_back(root + fileName + ".cpp"); //Add user's script to files that should be compiled
        scripts +=  "#include \"" + root + fileName + ".h\"\n";
    };
    if (scriptMap.size() > 0) scriptMap.pop_back();
    ofs.close();
    ifs.close();
    //Building scriptManager map
    Vector2<std::string> parts = Vector2<std::string>("", "");
    Vector2<std::string> del = Vector2<std::string>("BEG_PPP","END_PPP");
    std::ifstream ifs0(scriptManager);
    if (!ifs0) {
        std::cout << "Can't open ScriptManager.cpp file" << std::endl;
        return;
    }
    int j = 0;
    for (std::string line; std::getline(ifs0, line);) {
        if (line.find("BEG_PPP") != std::string::npos) {
            parts.x += "//BEG_PPP\n";
            j++;
            continue;
        }
        if (line.find("END_PPP") != std::string::npos) {
            parts.y += "//END_PPP\n";
            j++;
            continue;
        }
        if (j!=1) *parts[Min(j, 1)] += line + "\n";
    }
    ifs0.close();
    std::ofstream ofs0(scriptManager);
    ofs0 << parts.x;
    ofs0 << scripts;
    ofs0 << "std::map<std::string, Scriptable* (*)(GameObject*)> ScriptManager::ScriptsMap = {\n";
    ofs0 << scriptMap <<"\n};\n" << parts.y;
    ofs0.close();
}

//Returns a vector containing all paths in a NWlist file
std::vector<std::string> Builder::GetDirs(std::string path) {
    std::fstream stream(path); //TODO::ERROR checking
    std::vector<std::string> vec;
    for (std::string line = ""; std::getline(stream, line); vec.push_back(line));
    return vec;
};

void Builder::Build() {
    //InitScripts();
}


//Should build external builder with preprocessor macro: BUILDER_EXT
#ifndef BUILDER_EXT

bool Builder::BuildGameRuntime() {
    //Linking scripts + ScriptManager + Game.lib
    Builder::LibsDir.clear();
    Builder::staticLibs.clear();
    Builder::objs = { ToDoubleBackSlash(Globals::compiledScriptDir) + "ScriptManager.obj", 
                      ToDoubleBackSlash(Globals::gameLibDir)        + "Game.lib", 
                      ToDoubleBackSlash(Globals::gameLibDir)        + "Source.obj",
                    };


    Builder::LibsDir    =  GetNWlist(Globals::compilationConfigDir + "Libs Dir.NWlist");
    Builder::staticLibs =  GetNWlist(Globals::compilationConfigDir + "Libs.NWlist");

    for (std::map<std::string, std::string>::iterator iter = ScriptManager::scriptList.begin(); iter != ScriptManager::scriptList.end(); iter++) {
        Builder::objs.push_back(ToDoubleBackSlash(Globals::compiledScriptDir) + iter->first + ".obj");
        Builder::IncludeDir.push_back(ToDoubleBackSlash(iter->second));
    }
    Builder::Link(Globals::gamePath, false);
    return Exec("Builder.bat");
}

#endif 
    


#ifdef _WINDLL

void Builder::CompileEngineDllRuntime() {
    //TODO::Error check
    ScriptManager::SaveScriptList();
    Builder::IncludeDir.clear();
    Builder::IncludeDir = GetNWlist(Globals::compilationConfigDir + "Additional include.NWlist");
    Builder::objs.clear();
    Builder::LibsDir.clear();
    Builder::LibsDir = GetNWlist(Globals::compilationConfigDir + "Libs Dir.NWlist");
    Builder::staticLibs = GetNWlist(Globals::compilationConfigDir + "Libs.NWlist");
    //Link NWengine.lib + script Objs + script manager obj to a Dll
    Builder::objs = { ToDoubleBackSlash(Globals::compiledScriptDir) + "ScriptManager.obj", 
                      ToDoubleBackSlash(Globals::engineLibDir)      + "NWengine.lib", 
                      ToDoubleBackSlash(Globals::engineLibDir)      + "NWengine.obj" 
                    };
    
    ScriptManager::CompileScriptManager();

    Exec("builder.bat"); //TODO::output result on an imgui window
    Builder::Link(Globals::dllDir + "NWengine_temp.dll", 1);

    Exec("builder.bat");

    Context::dllFlag = NW_RELOAD_DLL;
}

#endif