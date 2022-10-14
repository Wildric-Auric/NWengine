#include "Builder.h"


#define PROJECTDIR "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build\\"
#define SCRIPT_MANAGER_PATH "Data\\Engine\\Native Scripting\\ScriptManager.cpp"
#include <fstream>
#include <vector>
#include <string>
#include "Data\\Engine\\Utilities.h"
#include "Data\\Engine\\Maths.h"
std::string ProjectDir = PROJECTDIR;
std::string outputDir  = ProjectDir + "Data\\Compiled\\";
std::string exeLoc     = ProjectDir + "Data\\";
std::string exeName    = "BuildTest.exe";
std::string IncludeDir[] = {
    "Data\\dependencies\\GLEW\\include",
    "Data\\dependencies\\GLFW\\include",
    "Data\\dependencies\\vendor",
    "Data\\dependencies\\freetype\\include",
    "Data\\dependencies\\OPENAL\\include",
    "Data\\Engine",
    "Data\\dependencies\\SND\\include",
    "Data\\Engine\\Components",
    "Data\\Engine\\Audio",
    "Data\\Ressources",
    "Data\\Engine\\Context",
    "Data\\Engine\\Gui",
    "Data\\Engine\\Maths",
    "Data\\Engine\\Rendering",
    "Data\\Engine\\Native Scripting",
    "Data\\Engine\\STL"

};

std::string LibsDir[] = {
    "Data\\dependencies\\GLFW\\lib-vc2019"                                   ,
    "Data\\dependencies\\GLEW\\lib\\Release\\Win32"                          ,
    "Data\\dependencies\\freetype\\release static\\vs2015-2022\\win32"       ,
    "Data\\dependencies\\SND\\lib"                                           ,
    "Data\\dependencies\\OPENAL\\win32"                                    
};
//Don't need absolute path in the future, it should be automatized
std::string staticLibs[] = {
     "opengl32.lib"                       ,
     "freetype.lib"                       ,
     "OpenAL32.lib"                       ,
     "libsndfile-1.lib"                   ,
     "glew32s.lib"                        ,
     "glfw3.lib"                          ,
     "kernel32.lib"                       ,
     "user32.lib"                         ,
     "gdi32.lib"                          ,
     "winspool.lib"                       ,
     "comdlg32.lib"                       ,
     "advapi32.lib"                       ,
     "shell32.lib"                        ,
     "ole32.lib"                          ,
     "oleaut32.lib"                       ,
     "uuid.lib"                           ,
     "odbc32.lib"                         ,
     "odbccp32.lib"                       ,
};


std::string objs[] = {
    "Data\\dependencies\\vendor\\glm\\detail\\glm.cpp"                                                  ,
    "Data\\dependencies\\vendor\\imgui\\imgui.cpp"                                                      ,
    "Data\\dependencies\\vendor\\imgui\\imgui_demo.cpp"                                                 ,
    "Data\\dependencies\\vendor\\imgui\\imgui_draw.cpp"                                                 ,
    "Data\\dependencies\\vendor\\imgui\\imgui_impl_glfw.cpp"                                            ,
    "Data\\dependencies\\vendor\\imgui\\imgui_impl_opengl3.cpp"                                         ,
    "Data\\dependencies\\vendor\\imgui\\imgui_tables.cpp"                                               ,
    "Data\\dependencies\\vendor\\imgui\\imgui_widgets.cpp"                                              ,
    "Data\\dependencies\\vendor\\imgui\\implot\\implot.cpp"                                             ,
    "Data\\dependencies\\vendor\\imgui\\implot\\implot_items.cpp"                                       ,
    "Data\\dependencies\\vendor\\imgui\\implot\\implot_demo.cpp"                                       ,
    "Data\\Engine\\Animation.cpp"                                                                                     ,                                                                                                                 
    "Data\\Engine\\Animator.cpp"                                                                                      ,
    "Data\\Engine\\Audio.cpp"                                                                                         ,
    "Data\\Engine\\Camera.cpp"                                                                                        ,
    "Data\\Engine\\Collision.cpp"                                                                                     ,
    "Data\\Engine\\Console.cpp"                                                                                       ,
    "Data\\Engine\\Context.cpp"                                                                                       ,
    "Data\\Engine\\Game.cpp"                                                                                          ,
    "Data\\Engine\\GameObject.cpp"                                                                                    ,
    "Data\\Engine\\Globals.cpp"                                                                                       ,
    "Data\\Engine\\Inputs.cpp"                                                                                        ,
    "Data\\Engine\\NWengine.cpp"                                                                                      ,
    "Data\\Engine\\NWGui.cpp"                                                                                         ,
    "Data\\Engine\\NWstd.cpp"                                                                                         ,
    "Data\\Engine\\Parser.cpp"                                                                                        ,
    "Data\\Engine\\ParticleSystem.cpp"                                                                                ,
    "Data\\Engine\\Physics.cpp"                                                                                       ,
    "Data\\Engine\\PostProcessing.cpp"                                                                                ,
    "Data\\Engine\\Primitives.cpp"                                                                                    ,
    "Data\\Engine\\RessourcesLoader.cpp"                                                                              ,
    "Data\\Engine\\Scene.cpp"                                                                                         ,
    "Data\\Engine\\Script.cpp"                                                                                        ,
    "Data\\Engine\\ScriptManager.cpp"                                                                                 ,
    "Data\\Engine\\Shader.cpp"                                                                                        ,
    "Data\\Engine\\Source.cpp"                                                                                        ,
    "Data\\Engine\\Sprite.cpp"                                                                                        ,
    "Data\\Engine\\StaticGuiVariable.cpp"                                                                             ,
    "Data\\Engine\\Text.cpp"                                                                                          ,
    "Data\\Engine\\Texture.cpp"                                                                                       ,
    "Data\\Engine\\TileMap.cpp"                                                                                       ,
    "Data\\Engine\\Transform.cpp"                                                                                     ,
};                                                                                                      
                                                                                                       

std::vector<std::string> o;


std::string PreprocessorMacros[] = {
    "GLEW_STATIC",
    "WIN32",
    "WIN_32",
    "_CRT_SECURE_NO_WARNINGS",
    "_CONSOLE",
    "_MBCS"
};





void Builder::Compile() {
    //Creating objects out of scripts
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << "call vcvars32\n";
    ofs << "set \"macros=";
    for (std::string MAC : PreprocessorMacros) {
        ofs << "/D" << MAC << " ";
    }
    ofs << "\"\n";
    for (std::string scr : objs) {

        //Getting it's name
        std::string filename = "";
        std::string extension = "";
        bool state = 0;
        for (auto chr : scr) {
            if (chr == '.') {
                filename += extension;
                extension = "";
                state = 1;
            }
            if ((int)chr == (int)'\\') {
                filename = "";
                extension = "";
                continue;
            }
            if (!state) filename += chr;
            else extension += chr;

        }
        ofs << "cl ";
        ofs << "/MDd ";  //Runtime library
        ofs << "/c ";
        bool b = 0;
        for (std::string dir : IncludeDir) {
            if (b) {
                ofs << "^\n ";
            }
            ofs << "/I " << "\"" << ProjectDir << dir << "\"";
            b = 1;
        };

        ofs << "^\n";
        ofs << " %macros% ";
        ofs << "/Fo"  <<"\""<< outputDir << filename << ".obj" << "\" ";
        //Adding cpp file
        ofs << "\""<<ProjectDir << scr << "\"";
   
        o.push_back(filename + ".obj");
        ofs << "\n";
        ofs << "echo " << filename << " object created\n";
    }
    ofs << "\n\n\n";
    ofs << "echo Objects creation has ended.\n";
    ofs.close();
}

void Builder::Link() {
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << "call vcvars32\n";
    ofs << "LINK ";
    std::string objectLoc = outputDir;
    for (std::string obj : GetDirFiles(objectLoc)) {
        ofs << "\"" << objectLoc << obj << "\"" << "^\n ";
    }
    for (std::string dir : LibsDir) {
        ofs << "/LIBPATH:" << "\""<< ProjectDir <<dir<< "\""<< "^\n ";
    }
    for (std::string lib : staticLibs) {
        ofs << lib << "^\n ";
    }
    ofs << "/OUT:" << exeLoc << exeName;
    ofs << "\necho exe generated";
    ofs.close();
 
}

void Builder::InitScripts() {
    //TODO::Wrapper of ifstream and ofstream with error handling
    //Reading used scripts names and writing to scripts.h
    std::ifstream ifs("..\\Ressources\\Scripts\\ScriptsList.NWlist");
    if (!ifs) {
        std::cout << "Can't open scripts file"<< std::endl;
        ifs.close();
        return; 
    }
    std::ofstream ofs("..\\Engine\\Native Scripting\\Scripts.h");
    if (!ofs) {
        std::cout << "Can't open Scripts.h file" << std::endl;
        ofs.close();
        return;
    }
    std::string scriptMap = "";
    ofs << "#pragma once\n#include \"Script.h\"\n";
    for (std::string line; std::getline(ifs, line); ofs << "#include \""<<"Scripts\\\\"<<line << ".h\"\n") {
        scriptMap += "\n  {\"" + line + "\"," + line + "::GetScript" + "},";
    };
    if (scriptMap.size() > 0) scriptMap.pop_back();
    ofs.close();
    ifs.close();
    //Building scriptManager map
    Vector2<std::string> parts = Vector2<std::string>("", "");
    Vector2<std::string> del = Vector2<std::string>("BEG_PPP","END_PPP");
    std::ifstream ifs0(SCRIPT_MANAGER_PATH);
    if (!ifs0) {
        std::cout << "Can't open ScriptManager.cpp file" << std::endl;
        ifs0.close();
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
        if (j!=1) *parts[min(j, 1)] += line + "\n";
    }

    ifs0.close();
    std::ofstream ofs0(SCRIPT_MANAGER_PATH);
    ofs0 << parts.x;
    ofs0 << "std::map<std::string, Scriptable* (*)(GameObject*)> ScriptManager::ScriptsMap = {\n";
    ofs0 << scriptMap <<"\n};\n" << parts.y;
    ofs0.close();
}


void Builder::Build() {
    //Compile();
    //Link();
    InitScripts();
}