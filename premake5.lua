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
    --location "./
    project "NWEngineCore" 
                      kind "StaticLib"
                      defines { "NW_GAME_BUILD" } --Doing it thrhough a macro is deprecated see Script.cpp update()
                      targetname ("NWengineCore")
                      location "./"
                       includedirs {
                        "%{prj.location}/",
                        "%{prj.location}/dependencies/GLEW/include",
                        "%{prj.location}/dependencies/GLFW/include",
                        "%{prj.location}/dependencies/vendor",
                        "%{prj.location}/dependencies/SND/include",
                        "%{prj.location}/dependencies/freetype/include",
                        "%{prj.location}/dependencies/OPENAL/include",
                        "%{prj.location}/Engine/**",
                        "%{prj.location}/Engine/"
                    }
                    files {
                        "%{prj.location}/Engine/**.cpp",
                        "%{prj.location}/Engine/**.h",  
                        "%{prj.location}/dependencies/vendor/nwin/*.cpp",
                        "%{prj.location}/premake5.lua"
                    }

    project "Sandbox" 
                      kind "ConsoleApp"
                      defines { "NW_GAME_BUILD" } --Doing it thrhough a macro is deprecated see Script.cpp update()
                      targetname ("Sandbox")
                      location "./"
                      includedirs {
                        "%{prj.location}/",
                        "%{prj.location}/dependencies/GLEW/include",
                        "%{prj.location}/dependencies/vendor",
                        "%{prj.location}/dependencies/SND/include",
                        "%{prj.location}/dependencies/freetype/include",
                        "%{prj.location}/dependencies/OPENAL/include",
                        "%{prj.location}/dependencies/SND/include",
                        "%{prj.location}/Engine/**",
                        "%{prj.location}/Engine/",

                        "%{prj.location}/Sandbox/src"
                    }
                    files {
                        "%{prj.location}/Engine/**.cpp",
                        "%{prj.location}/dependencies/vendor/nwin/*.cpp",
                        "%{prj.location}/Sandbox/src/**.cpp",
                        "%{prj.location}/Engine/**.h", 
                        "%{prj.location}/premake5.lua"
                    }
                    links {
                        "dwmapi.lib"
                    }
                    --removefiles { "%{prj.location}/Engine/NWengine.cpp", "%{prj.location}/Engine/Game.cpp", "%{prj.location}/Engine/Source.cpp"}
                    
    workspace "*"
        libdirs { 
                        "%{wks.location}/dependencies/GLEW/lib/Release/Win32",
                        "%{wks.location}/dependencies/freetype/lib/win32",
                        "%{wks.location}/dependencies/SND/lib",
                        "%{wks.location}/dependencies/OPENAL/win32"
                }
        filter "architecture:x86" 
            links {
                "opengl32.lib",
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