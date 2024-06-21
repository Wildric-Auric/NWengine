workspace "NWengine"
    configurations {"Debug32", "Release32" }
    architecture "x86"
    language   "C++"
    cppdialect "C++11"
    targetdir "Bin/%{prj.name}/%{cfg.buildcfg}"
    objdir    "Bin/objs"
    defines {"GLEW_STATIC"}
    characterset("MBCS")
    buildoptions { "/EHsc" }
    location "Build" 
    project "NWEngineCore" 
                    kind "StaticLib"
                    defines { "NW_GAME_BUILD" } --Doing it thrhough a macro is deprecated see Script.cpp update()

                    includedirs {
                        "./",
                        "dependencies/GLEW/include",
                        "dependencies/GLFW/include",
                        "dependencies/vendor",
                        "dependencies/SND/include",
                        "dependencies/freetype/include",
                        "dependencies/OPENAL/include",
                        "Engine/**",
                        "Engine/"
                    }
                    files {
                        "Engine/**.cpp",
                        "Engine/**.h",  
                        "dependencies/vendor/nwin/*.cpp",
                        "premake5.lua"
                    }
                    filter "configurations:*Debug*"
                        targetname ("NWengineCore32d")
                    filter "configurations:*Release*"
                        targetname ("NWengineCore32")

    project "Sandbox" 
                      kind "ConsoleApp"
                      defines { "NW_GAME_BUILD" } --Doing it thrhough a macro is deprecated see Script.cpp update()
                      targetname ("Sandbox")
                      includedirs {
                        "./",
                        "dependencies/GLEW/include",
                        "dependencies/vendor",
                        "dependencies/SND/include",
                        "dependencies/freetype/include",
                        "dependencies/OPENAL/include",
                        "dependencies/SND/include",
                        "Engine/**",
                        "Engine/",

                        "Sandbox/src"
                    }
                    files {
                        "Engine/**.cpp",
                        "dependencies/vendor/nwin/*.cpp",
                        "Sandbox/src/**.cpp",
                        "Engine/**.h", 
                        "premake5.lua"
                    }
                    links {
                        "dwmapi.lib"
                    }
                    --removefiles { "%{prj.location}/Engine/NWengine.cpp", "%{prj.location}/Engine/Game.cpp", "%{prj.location}/Engine/Source.cpp"}
                    
    workspace "*"
        libdirs { 
                        "dependencies/GLEW/lib/Release/Win32",
                        "dependencies/freetype/lib/win32",
                        "dependencies/SND/lib",
                        "dependencies/OPENAL/win32"
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
