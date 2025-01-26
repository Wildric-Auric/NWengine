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
                        "Dependencies/GLEW/include",
                        "Dependencies/vendor",
                        "Dependencies/freetype/include",
                        "Engine/**",
                        "Engine/"
                    }
                    files {
                        "Engine/**.cpp",
                        "Engine/**.h",  
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
                        "Dependencies/GLEW/include",
                        "Dependencies/vendor",
                        "Dependencies/freetype/include",
                        "Engine/**",
                        "Engine/",
                        "Sandbox/src"
                    }
                    files {
                        "Engine/**.cpp",
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
                        "Dependencies/GLEW/lib/Release/Win32",
                        "Dependencies/freetype/lib/win32",
            }
            links {
                "opengl32.lib",
                "freetype.lib",
                "glew32s.lib"
            }
        
        filter "architecture:x64"
            libdirs {
                        "Dependencies/GLEW/lib/Release/x64",
                        "Dependencies/freetype/lib/win64",
            }
            links {
                "opengl32.lib",
                "freetype.lib",
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
