@echo off

echo Objects creation has ended.
g++  -D GLEW_STATIC  -D WIN32 -I C:\Users\HP\Desktop\NWengine^
 -I C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include^
 -I C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include^
 -I C:\Users\HP\Desktop\NWengine\dependencies\vendor^
 -I C:\Users\HP\Desktop\NWengine\dependencies\freetype\include^
 -I C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include^
 -I C:\Users\HP\Desktop\NWengine\Engine^
 -I C:\Users\HP\Desktop\NWengine\dependencies\SND\include^
 -I C:\Users\HP\Desktop\NWengine\Engine\Components^
 -I C:\Users\HP\Desktop\NWengine\Engine\Audio^
 -I C:\Users\HP\Desktop\NWengine\Ressources^
 -I C:\Users\HP\Desktop\NWengine\Engine\Context^
 -I C:\Users\HP\Desktop\NWengine\Engine\Gui^
 -I C:\Users\HP\Desktop\NWengine\Engine\Maths^
 -I C:\Users\HP\Desktop\NWengine\Engine\Rendering -L C:\Users\HP\Desktop\NWengine\dependencies\GLFW\lib-mingw^
 -L C:\Users\HP\Desktop\NWengine\dependencies\GLEW\lib\Release\Win32^
 -L "C:\Users\HP\Desktop\NWengine\dependencies\freetype\release static\vs2015-2022\win32"^
 -L C:\Users\HP\Desktop\NWengine\dependencies\SND\lib^
 -L "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\win32" -lkernel32^
 -luser32^
 -lgdi32^
 -lwinspool^
 -lcomdlg32^
 -ladvapi32^
 -lshell32^
 -lole32^
 -loleaut32^
 -luuid^
 -lodbc32^
 -lodbccp32^
 -lopengl32^
 -lglfw3^
 -lglew32^
 -lOpenAL32^
 C:\Users\HP\Desktop\NWengine\dependencies\SND\lib\libsndfile-1.lib^
 GNUBuild\glm.o^
 GNUBuild\imgui.o^
 GNUBuild\imgui_demo.o^
 GNUBuild\imgui_draw.o^
 GNUBuild\imgui_impl_glfw.o^
 GNUBuild\imgui_impl_opengl3.o^
 GNUBuild\imgui_tables.o^
 GNUBuild\imgui_widgets.o^
 GNUBuild\Audio.o^
 GNUBuild\Collision.o^
 GNUBuild\PostProcessing.o^
 GNUBuild\Script.o^
 GNUBuild\Transform.o^
 GNUBuild\Console.o^
 GNUBuild\NWstd.o^
 GNUBuild\Parser.o^
 GNUBuild\Particles.o^
 GNUBuild\Physics.o^
 GNUBuild\Camera.o^
 GNUBuild\Context.o^
 GNUBuild\GameObject.o^
 GNUBuild\Globals.o^
 GNUBuild\Inputs.o^
 GNUBuild\Primitives.o^
 GNUBuild\NWengine.o^
 GNUBuild\RessourcesLoader.o^
 GNUBuild\Scene.o^
 GNUBuild\Shader.o^
 GNUBuild\Source.o^
 GNUBuild\Text.o^
 GNUBuild\Texture.o^
 GNUBuild\TileMap.o^
 GNUBuild\dllTest.o^
 GNUBuild\player.o^
 GNUBuild\test.o^
 GNUBuild\Sprite.o^
 GNUBuild\StaticGuiVariable.o^
 -o GNUBuild\NWengine-GNU-Build-2022.exe
echo exe generated