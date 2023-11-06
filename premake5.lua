workspace "NWengineWorkspace"
    configurations {"Debug32", "Release32", "Static-Editor-Debug32", "Static-Editor-Release32" }
    architecture "x86"

    project "NWengineSolution"
        kind "ConsoleApp"
        language   "C++"
        cppdialect "C++11"
        targetdir "Bin/%{cfg.buildcfg}-%{cfg.architecture}"
        targetname ("NWengine")
        objdir    "Bin/%{cfg.buildcfg}-%{cfg.architecture}/objs"
        location "./"
        defines {"GLEW_STATIC"}
        characterset("MBCS")
        buildoptions { "/EHsc" }
        files {
            "%{prj.location}/Engine/**.cpp",
            "%{prj.location}/Gui/**.cpp",
            "%{prj.location}/Build/Builder.cpp",
            "%{prj.location}/Gui/**.cpp",
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

        libdirs { 
            "%{prj.location}/dependencies/GLFW/lib-vc2019",
            "%{prj.location}/dependencies/GLEW/lib/Release/Win32",
            "%{prj.location}/dependencies/IRRKLANG/lib/Win32-visualStudio",
            "%{prj.location}/dependencies/freetype/lib/win32",
            "%{prj.location}/dependencies/SND/lib",
            "%{prj.location}/dependencies/OPENAL/win32"
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

        filter "configurations:Debug"
            defines {"DEBUG", "_DEBUG"}
            symbols "On"
        filter "configurations:Release"
            defines {"NDEBUG", "Release"}
            optimize "On"

        filter "configurations:Static-Editor-Debug"
            defines {"DEBUG", "_DEBUG, NW_GUI_APP"} 
            symbols "On"
        filter "configurations:Static-Editor-Release"
            defines {"NDEBUG", "Release", "NW_GUI_APP"}
            optimize "On"
        filter {}