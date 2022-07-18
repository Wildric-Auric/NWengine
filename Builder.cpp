//This files fills GNUbuild.bat and executes it
#include <string>
#include <fstream>
#include <vector>

static std::string IncludeDir[] = {
    "C:\\Users\\HP\\Desktop\\NWengine",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\GLEW\\include",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\GLFW\\include",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\vendor",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\freetype\\include",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\OPENAL\\include",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine",
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\SND\\include",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Components",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Audio",
    "C:\\Users\\HP\\Desktop\\NWengine\\Ressources",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Context",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Gui",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Maths",
    "C:\\Users\\HP\\Desktop\\NWengine\\Engine\\Rendering"

};


static std::string LibsDir[] = {
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\GLFW\\lib-mingw"                                   ,
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\GLEW\\lib\\Release\\Win32"                          ,
    "\"C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\freetype\\release static\\vs2015-2022\\win32\""       ,
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\SND\\lib"                                           ,
    "\"C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\OPENAL\\win32\""
};
//Don't need absolute path in the future, it should be automatized
static std::string staticLibs[] = {
    "-lkernel32"                                                                                       ,   
    "-luser32"                                                                                         ,   
    "-lgdi32"                                                                                          ,   
    "-lwinspool"                                                                                       ,   
    "-lcomdlg32"                                                                                       ,   
    "-ladvapi32"                                                                                       ,   
    "-lshell32"                                                                                        ,   
    "-lole32"                                                                                          ,   
    "-loleaut32"                                                                                       ,   
    "-luuid"                                                                                           ,   
    "-lodbc32"                                                                                         ,   
    "-lodbccp32"                                                                                       ,
    "-lopengl32"                                                                                       ,
    "-lglfw3"                                                                                          ,
    "-lglew32"                                                                                         ,                                      
    //"\"C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\freetype\\release static\\vs2015-2022\\win32\\freetype.lib\""                    ,
    "-lopenal32"                                                                                       ,
    "C:\\Users\\HP\\Desktop\\NWengine\\dependencies\\SND\\lib\\libsndfile-1.lib"
};


static std::string objs[] = {
    "dependencies\\vendor\\glm\\detail\\glm.cpp"                                                    ,
    "dependencies\\vendor\\imgui\\imgui.cpp"                                                        ,
    "dependencies\\vendor\\imgui\\imgui_demo.cpp"                                                   ,
    "dependencies\\vendor\\imgui\\imgui_draw.cpp"                                                   ,
    "dependencies\\vendor\\imgui\\imgui_impl_glfw.cpp"                                              ,
    "dependencies\\vendor\\imgui\\imgui_impl_opengl3.cpp"                                           ,
    "dependencies\\vendor\\imgui\\imgui_tables.cpp"                                                 ,
    "dependencies\\vendor\\imgui\\imgui_widgets.cpp"                                                ,
    "Engine\\Audio\\Audio.cpp"                                                                     ,
    "Engine\\Components\\Collision.cpp"                                                            ,
    "Engine\\Components\\PostProcessing.cpp"                                                       ,
    "Engine\\Components\\Script.cpp"                                                               ,
    "Engine\\Components\\Transform.cpp"                                                            ,
    "Engine\\Console.cpp"                                                                         ,
    "Engine\\NWstd.cpp"                                                                           ,
    "Engine\\Parser.cpp"                                                                          ,
    "Engine\\Components\\Particles.cpp"                                                            ,
    "Engine\\Components\\Physics.cpp"                                                              ,
    "Engine\\Rendering\\Camera.cpp"                                                                ,
    "Engine\\Context\\Context.cpp"                                                                 ,
    "Engine\\Components\\GameObject.cpp"                                                           ,
    "Engine\\Globals.cpp"                                                                         ,
    "Engine\\Inputs.cpp"                                                                          ,
    "Engine\\Context\\Primitives.cpp"                                                              ,
    "Engine\\NWengine.cpp"                                                                        ,
    "Engine\\RessourcesLoader.cpp"                                                                ,
    "Engine\\Scene.cpp"                                                                           ,
    "Engine\\Shader.cpp"                                                                          ,
    "Engine\\Source.cpp"                                                                          ,
    "Engine\\Text.cpp"                                                                            ,
    "Engine\\Texture.cpp"                                                                         ,
    "Engine\\TileMap.cpp"                                                                         ,
    "Ressources\\Scripts\\dllTest.cpp"                                                             ,
    "Ressources\\Scripts\\player.cpp"                                                              ,
    "Ressources\\Scripts\\test.cpp"                                                                ,
    "Engine\\Components\\Sprite.cpp"                                                               ,
    "Engine\\Gui\\StaticGuiVariable.cpp"                                                           ,
};


std::vector<std::string> o;


std::string PreprocessorMacros[] = {
    "GLEW_STATIC",
    "WIN32"

};

std::string outPutDir = "GNUBuild\\";





static void Build(bool compile, bool link) {
    //Creating objects out of scripts
    std::ofstream ofs("GNUbuild.bat");
    if (compile) {

        ofs << "@echo off\n";
        for (std::string scr : objs) {
            ofs << "g++ ";

            ofs << "-c " << scr;

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
            o.push_back(filename + ".o");
            for (std::string MAC : PreprocessorMacros) {
                ofs << " -D " << MAC << " ";
            }
            ofs <<" -o " << outPutDir << filename << ".o ";
            bool b = 0;
            for (std::string dir : IncludeDir) {
                if (b) {
                    ofs << "^\n ";
                }
                ofs << "-I " << dir;

                b = 1;
            };
            ofs << "\n";
            ofs << "echo " << filename << " object created\n";
        }

        ofs << "\n\n\n";
        ofs << "echo Objects creation has ended.\n";


    }

    if (link) {
        ofs << "g++ ";
        for (std::string MAC : PreprocessorMacros) {
            ofs << " -D " << MAC << " ";
        }

        bool b = 0;
        for (std::string dir : IncludeDir) {
            if (b) {
                ofs << "^\n ";
            }
            ofs << "-I " << dir;

            b = 1;
        };
        ofs << " ";
        b = 0;
        for (std::string dir : LibsDir) {
            if (b) {
                ofs << "^\n ";
            }
            ofs << "-L " << dir;
            b = 1;
        }
        ofs << " ";
        for (std::string lib : staticLibs) {
            ofs <<lib << "^\n ";
        }

        for (std::string obj : o) {
            ofs << outPutDir + obj << "^\n ";
        }
        ofs << "-o GNUBuild\\NWengine-GNU-Build-2022.exe";
        ofs << "\necho exe generated";
    }

    ofs.close();
}

static void Execute() {

}
int main() {
    Build(1, 1);
}