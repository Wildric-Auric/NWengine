Nw_version = '"0.9.090125"'

workspace "NWengine"
    configurations {"Debug", "Release"}
    platforms {"x86","x64"}
    filter "platforms:*86*"
        architecture "x86"
    filter "platforms:*64*"
        architecture "x64"
    filter "action:vs*"
        buildoptions { "/MP" }
    filter {}
    location "Build"
    language   "C++"
    cppdialect "C++11"
    targetdir "%{wks.location}/Bin/%{prj.name}/%{cfg.buildcfg}%{cfg.platform:sub(2)}"
    objdir    "%{wks.location}/Bin/objs"
    defines {"GLEW_STATIC"}
    characterset("MBCS")
    filter "system:windows"
    buildoptions { "/EHsc"}
    filter {}
    warnings "Off"

    project "NWengineCore"
                    kind "StaticLib"
                    includedirs {
                        "./",
                        "Dependencies/glew/include",
                        "Dependencies/vendor",
                        "Dependencies/freetype/include",
                        "Engine/**",
                        "Engine/"
                    }
                    files {
                        "Engine/**.cpp",
                        "Engine/**.c",
                        "Engine/**.h",
                        "premake5.lua"
                    }
                    filter "configurations:*Debug*"
                        targetname ("NWengineCore%{cfg.platform:sub(2)}d")
                    filter "configurations:*Release*"
                        targetname ("NWengineCore%{cfg.platform:sub(2)}")

    project "NWengineCoreUnityBuild"
                    kind "StaticLib"
                    includedirs {
                        "./",
                        "Dependencies/glew/include",
                        "Dependencies/vendor",
                        "Dependencies/freetype/include",
                        "Engine/**",
                        "Engine/"
                    }
                    files {
                        "UnityBuild/*.cpp",
                        "premake5.lua"
                    }
                    filter "configurations:*Debug*"
                        targetname ("NWengineCore%{cfg.platform:sub(2)}d")
                    filter "configurations:*Release*"
                        targetname ("NWengineCore%{cfg.platform:sub(2)}")

    project "Sandbox"
                      kind "ConsoleApp"
                      targetname ("Sandbox")
                      includedirs {
                        "./",
                        "Dependencies/glew/include",
                        "Dependencies/vendor",
                        "Dependencies/freetype/include",
                        "Engine/**",
                        "Engine/",
                        "Sandbox/src"
                       }
                    files {
                        --"Engine/**.cpp",
                        "UnityBuild/*.cpp",
                        "Sandbox/src/**.cpp",
                        "Sandbox/src/**.c",
                        "Engine/**.h",
                        "premake5.lua",
                        "*.rc",
                    }
                    filter "system:windows"
                    links {
                        "dwmapi"
                    }
    workspace "*"
        filter "system:windows"
        defines {"WIN", "WIN32", "PLTFRM_WIN32"}
        links {
            "opengl32",
            "freetype",
            "glew32s",
            "xaudio2"
        }
        filter "system:linux"
        defines {"PLTFRM_LINUX"}
        links {
            ":glew32s.a",
            ":freetype.a",
            "X11",
            "GL"
        }
        filter "system:windows"
        filter "configurations:*Debug*"
            libdirs {
                 "Dependencies/glew/lib/debug/win%{cfg.platform:sub(2)}",
                 "Dependencies/freetype/lib/win%{cfg.platform:sub(2)}",
                 "Dependencies/glew/lib/debug/linux%{cfg.platform:sub(2)}",
                 "Dependencies/freetype/lib/linux%{cfg.platform:sub(2)}",
            }
        filter "configurations:*Release*"
            libdirs {
                 "Dependencies/glew/lib/release/win%{cfg.platform:sub(2)}",
                 "Dependencies/freetype/lib/win%{cfg.platform:sub(2)}",
            } 
        filter "system:linux"
        filter "configurations:*Debug*"
            libdirs {
                 "Dependencies/glew/lib/debug/lnx%{cfg.platform:sub(2)}",
                 "Dependencies/freetype/lib/lnx%{cfg.platform:sub(2)}",
            }
        filter "configurations:*Release*"
            libdirs {
                 "Dependencies/glew/lib/release/lnx%{cfg.platform:sub(2)}",
                 "Dependencies/freetype/lib/lnx%{cfg.platform:sub(2)}",
            }
        filter {}
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
