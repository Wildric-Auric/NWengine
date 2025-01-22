Nw_version = '"0.9.090125"'

workspace "NWengine"
    configurations {"Debug", "Release"}
    platforms {"x86","x64"}
    filter "platforms:*86*"
        architecture "x86"
        pltrm = "32"
    filter "platforms:*64*"
        architecture "x64"
        pltrm = "64"
    filter {}
    location "Build"
    language   "C++"
    cppdialect "C++11"
    targetdir "%{wks.location}/Bin/%{prj.name}/%{cfg.buildcfg}%{pltrm}"
    objdir    "%{wks.location}/Bin/objs"
    defines {"GLEW_STATIC"}
    characterset("MBCS")
    buildoptions { "/EHsc"}

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
                        targetname ("NWengineCore%{pltrm}d")
                    filter "configurations:*Release*"
                        targetname ("NWengineCore%{pltrm}")

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
                        "premake5.lua",
                        "*.rc",
                    }
                    links {
                        "dwmapi.lib"
                    }
                    --removefiles { "%{prj.location}/Engine/NWengine.cpp", "%{prj.location}/Engine/Game.cpp", "%{prj.location}/Engine/Source.cpp"}
                    
    workspace "*"
        filter "architecture:x86" 
            libdirs {
                        "dependencies/GLEW/lib/Release/Win32",
                        "dependencies/freetype/lib/win32",
                        "dependencies/SND/lib/win32",
                        "dependencies/OPENAL/win32"
            }
            links {
                "opengl32.lib",
                "freetype.lib",
                "OpenAL32.lib",
                "sndfile.lib",
                "glew32s.lib"
            }
        
        filter "architecture:x64"
            libdirs {
                        "dependencies/GLEW/lib/Release/x64",
                        "dependencies/freetype/lib/win64",
                        "dependencies/SND/lib/win64",
                        "dependencies/OPENAL/win64"
            }
            links {
                "opengl32.lib",
                "freetype.lib",
                "OpenAL32.lib",
                "sndfile.lib",
                "glew32s.lib"
            }

        filter "configurations:*Debug*"
            defines {"NW_DEBUG", "_DEBUG", "NW_VERSION=%{Nw_version}"}
            staticruntime "off"
            runtime "Debug"
            symbols "On"
        filter "configurations:*Release*"
            staticruntime "on"
            runtime "Release"
            defines {"NW_NDEBUG", "NW_RELEASE", "NW_VERSION=%{Nw_version}"}
            optimize "On"
        filter {}
