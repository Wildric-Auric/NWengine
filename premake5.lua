workspace "NWengine"
    configurations {"Debug32", "Release32" }
    architecture "x86"
    language   "C++"
    cppdialect "C++11"
    targetdir "Bin/%{prj.name}/%{cfg.buildcfg}"
    objdir    "Bin/objs"
    location "./"
    defines {"GLEW_STATIC"}
    characterset("MBCS")
    buildoptions { "/EHsc" }
    --location "./"
    project "NWengineProject"
                    defines {"NW_GUI_APP"}
                    targetname ("NWengine")
                    location "./"
                    kind "ConsoleApp"
                    files {
                        "%{prj.location}/Engine/**.cpp",
                        "%{prj.location}/Gui/**.cpp",
                        "%{prj.location}/Build/Builder.cpp",
                        "%{prj.location}/dependencies/vendor/**.cpp"
                    }

                    includedirs {
                        "%{prj.location}/",
                        "%{prj.location}/dependencies/GLEW/include",
                        "%{prj.location}/dependencies/GLFW/include",
                        "%{prj.location}/dependencies/vendor",
                        "%{prj.location}/dependencies/IRRKLANG/include",
                        "%{prj.location}/dependencies/freetype/include",
                        "%{prj.location}/dependencies/OPENAL/include",
                        "%{prj.location}/Engine",
                        "%{prj.location}/dependencies/SND/include",
                        "%{prj.location}/Engine/Components",
                        "%{prj.location}/Engine/Audio",
                        "%{prj.location}/Ressources",
                        "%{prj.location}/Engine/Context",
                        "%{prj.location}/Gui",
                        "%{prj.location}/Engine/Maths",
                        "%{prj.location}/Engine/Rendering",
                        "%{prj.location}/Engine/STL",
                        "%{prj.location}/Engine/Reflection",
                        "%{prj.location}/Engine/Native Scripting",
                        "%{prj.location}/Build",
                    }
    project "NWEngineCore" 
                      kind "StaticLib"
                      targetname ("NWengineCore")
                      location "./"
                      includedirs {
                        "%{prj.location}/",
                        "%{prj.location}/dependencies/GLEW/include",
                        "%{prj.location}/dependencies/GLFW/include",
                        "%{prj.location}/dependencies/vendor",
                        "%{prj.location}/dependencies/IRRKLANG/include",
                        "%{prj.location}/dependencies/freetype/include",
                        "%{prj.location}/dependencies/OPENAL/include",
                        "%{prj.location}/Engine",
                        "%{prj.location}/dependencies/SND/include",
                        "%{prj.location}/Engine/Components",
                        "%{prj.location}/Engine/Audio",
                        "%{prj.location}/Ressources",
                        "%{prj.location}/Engine/Context",
                        "%{prj.location}/Engine/Maths",
                        "%{prj.location}/Engine/Rendering",
                        "%{prj.location}/Engine/STL",
                        "%{prj.location}/Engine/Reflection",
                        "%{prj.location}/Engine/Native Scripting",
                        "%{prj.location}/Build",
                    }
                    files {
                        "%{prj.location}/Engine/**.cpp",
                        "%{prj.location}/Build/Builder.cpp",
                        "%{prj.location}/dependencies/vendor/glm/**.cpp"
                    }
                    --removefiles { "%{prj.location}/Engine/NWengine.cpp", "%{prj.location}/Engine/Game.cpp", "%{prj.location}/Engine/Source.cpp"}
    
    workspace "*"
        libdirs { 
                        "%{wks.location}/dependencies/GLFW/lib-vc2019",
                        "%{wks.location}/dependencies/GLEW/lib/Release/Win32",
                        "%{wks.location}/dependencies/IRRKLANG/lib/Win32-visualStudio",
                        "%{wks.location}/dependencies/freetype/lib/win32",
                        "%{wks.location}/dependencies/SND/lib",
                        "%{wks.location}/dependencies/OPENAL/win32"
                }
        filter "architecture:x86" 
            links {
                "opengl32.lib",
                "glfw3.lib",
                "freetype.lib",
                "OpenAL32.lib",
                "libsndfile-1.lib",
                "glew32s.lib"
            }

        filter "configurations:*Debug*"
            defines {"NW_DEBUG", "_DEBUG"}
            symbols "On"
        filter "configurations:*Release*"
            defines {"NW_NDEBUG", "NW_RELEASE"}
            optimize "On"
        filter {}