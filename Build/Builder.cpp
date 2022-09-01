//This files fills GNUbuild.bat and executes it
#define PROJECTDIR "C:\\Users\\HP\\Desktop\\NWengine\\"

#include <fstream>
#include <vector>
#include <string>

#define UTILDIR "..\\Engine\\STL\\Utilities.h"
#include UTILDIR

static std::string ProjectDir = PROJECTDIR;
static std::string IncludeDir[] = {
    "dependencies\\GLEW\\include",
    "dependencies\\GLFW\\include",
    "dependencies\\vendor",
    "dependencies\\freetype\\include",
    "dependencies\\OPENAL\\include",
    "Engine",
    "dependencies\\SND\\include",
    "Engine\\Components",
    "Engine\\Audio",
    "Ressources",
    "Engine\\Context",
    "Engine\\Gui",
    "Engine\\Maths",
    "Engine\\Rendering",
    "Engine\\Native Scripting",
    "Engine\\STL"

};

static std::string LibsDir[] = {
    "dependencies\\GLFW\\lib-vc2019"                                   ,
    "dependencies\\GLEW\\lib\\Release\\Win32"                          ,
    "dependencies\\freetype\\release static\\vs2015-2022\\win32"       ,
    "dependencies\\SND\\lib"                                           ,
    "dependencies\\OPENAL\\win32"                                    
};
//Don't need absolute path in the future, it should be automatized
static std::string staticLibs[] = {
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


static std::string objs[] = {
    "dependencies\\vendor\\glm\\detail\\glm.cpp"                                                  ,
    "dependencies\\vendor\\imgui\\imgui.cpp"                                                      ,
    "dependencies\\vendor\\imgui\\imgui_demo.cpp"                                                 ,
    "dependencies\\vendor\\imgui\\imgui_draw.cpp"                                                 ,
    "dependencies\\vendor\\imgui\\imgui_impl_glfw.cpp"                                            ,
    "dependencies\\vendor\\imgui\\imgui_impl_opengl3.cpp"                                         ,
    "dependencies\\vendor\\imgui\\imgui_tables.cpp"                                               ,
    "dependencies\\vendor\\imgui\\imgui_widgets.cpp"                                              ,
    "Engine\\Audio\\Audio.cpp"                                                                    ,
    "Engine\\Components\\Collision.cpp"                                                           ,
    "Engine\\Components\\PostProcessing.cpp"                                                      ,
    "Engine\\Components\\Script.cpp"                                                              ,
    "Engine\\Components\\Transform.cpp"                                                           ,
    "Engine\\Gui\\Console.cpp"                                                                    ,
    "Engine\\STL\\NWstd.cpp"                                                                      ,
    "Engine\\STL\\Parser.cpp"                                                                     ,
    "Engine\\Components\\Particles.cpp"                                                           ,
    "Engine\\Components\\Physics.cpp"                                                             ,
    "Engine\\Rendering\\Camera.cpp"                                                               ,
    "Engine\\Context\\Context.cpp"                                                                ,
    "Engine\\Components\\GameObject.cpp"                                                          ,
    "Engine\\Globals.cpp"                                                                         ,
    "Engine\\Context\\Inputs.cpp"                                                                 ,
    "Engine\\Context\\Primitives.cpp"                                                             ,
    "Engine\\NWengine.cpp"                                                                        ,
    "Engine\\Context\\RessourcesLoader.cpp"                                                       ,
    "Engine\\Scene.cpp"                                                                           ,
    "Engine\\Shader.cpp"                                                                          ,
    "Engine\\Source.cpp"                                                                          ,
    "Engine\\Text.cpp"                                                                            ,
    "Engine\\Context\\Texture.cpp"                                                                ,
    "Engine\\TileMap.cpp"                                                                         ,
    "Ressources\\Scripts\\player.cpp"                                                             ,
    "Engine\\Components\\Sprite.cpp"                                                              ,
    "Engine\\Gui\\StaticGuiVariable.cpp"                                                          ,
    "Engine\\Animation.cpp"                                                                       ,
    "Engine\\Native Scripting\\ScriptManager.cpp"                                                 ,
    "Engine\\Components\\Animator.cpp"                                                          
};


std::vector<std::string> o;


std::string PreprocessorMacros[] = {
    "GLEW_STATIC",
    "WIN32",
    "_CRT_SECURE_NO_WARNINGS",
    "_CONSOLE",
    "_MBCS"
};

std::string outputDir = "";
std::string exeName = "BuildTest.exe";


static void Compile() {
    //Creating objects out of scripts
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << "call vcvars32\n";
    ofs << "set \"macros=";
    for (std::string MAC : PreprocessorMacros) {
        ofs << "/D" << MAC << " ";
    }
    ofs << "\"\n";
        outputDir = ProjectDir + "Build\\Engine Build\\Data\\";
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

static void Link() {
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << "call vcvars32\n";
    ofs << "LINK ";
    std::string objectLoc = "Engine Build\\Data\\";
    for (std::string obj : GetDirFiles(objectLoc)) {
        ofs << "\"" << objectLoc << obj << "\"" << "^\n ";
    }
    for (std::string dir : LibsDir) {
        ofs << "/LIBPATH:" << "\""<< ProjectDir <<dir<< "\""<< "^\n ";
    }
    for (std::string lib : staticLibs) {
        ofs << lib << "^\n ";
    }
    ofs << "/OUT:NWengine-MSVC.exe";
    ofs << "\necho exe generated";
    ofs.close();
 
}


static void Execute() {

}
int main() {
    //Compile();
    Link();
}