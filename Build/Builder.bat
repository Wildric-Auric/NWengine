@echo off
call vcvars32
LINK "Engine Build\Data\Animation.obj"^
 "Engine Build\Data\Animator.obj"^
 "Engine Build\Data\Audio.obj"^
 "Engine Build\Data\Camera.obj"^
 "Engine Build\Data\Collision.obj"^
 "Engine Build\Data\Console.obj"^
 "Engine Build\Data\Context.obj"^
 "Engine Build\Data\GameObject.obj"^
 "Engine Build\Data\glm.obj"^
 "Engine Build\Data\Globals.obj"^
 "Engine Build\Data\imgui.obj"^
 "Engine Build\Data\imgui_demo.obj"^
 "Engine Build\Data\imgui_draw.obj"^
 "Engine Build\Data\imgui_impl_glfw.obj"^
 "Engine Build\Data\imgui_impl_opengl3.obj"^
 "Engine Build\Data\imgui_tables.obj"^
 "Engine Build\Data\imgui_widgets.obj"^
 "Engine Build\Data\Inputs.obj"^
 "Engine Build\Data\NWengine.obj"^
 "Engine Build\Data\NWstd.obj"^
 "Engine Build\Data\Parser.obj"^
 "Engine Build\Data\Particles.obj"^
 "Engine Build\Data\Physics.obj"^
 "Engine Build\Data\player.obj"^
 "Engine Build\Data\PostProcessing.obj"^
 "Engine Build\Data\Primitives.obj"^
 "Engine Build\Data\RessourcesLoader.obj"^
 "Engine Build\Data\Scene.obj"^
 "Engine Build\Data\Script.obj"^
 "Engine Build\Data\ScriptManager.obj"^
 "Engine Build\Data\Shader.obj"^
 "Engine Build\Data\Source.obj"^
 "Engine Build\Data\Sprite.obj"^
 "Engine Build\Data\StaticGuiVariable.obj"^
 "Engine Build\Data\Text.obj"^
 "Engine Build\Data\Texture.obj"^
 "Engine Build\Data\TileMap.obj"^
 "Engine Build\Data\Transform.obj"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\GLFW\lib-vc2019"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\GLEW\lib\Release\Win32"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\freetype\release static\vs2015-2022\win32"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\SND\lib"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\win32"^
 opengl32.lib^
 freetype.lib^
 OpenAL32.lib^
 libsndfile-1.lib^
 glew32s.lib^
 glfw3.lib^
 kernel32.lib^
 user32.lib^
 gdi32.lib^
 winspool.lib^
 comdlg32.lib^
 advapi32.lib^
 shell32.lib^
 ole32.lib^
 oleaut32.lib^
 uuid.lib^
 odbc32.lib^
 odbccp32.lib^
 MSVCRT.LIB^
 /OUT:NWengine-MSVC.exe
 /DLL^
 /OUT:C:\Users\HP\source\repos\Wildric-Auric\NWengine\Build\Data\BuildDLLTest.dll
echo exe generated