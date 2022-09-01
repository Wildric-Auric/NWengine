@echo off
call vcvars32
set "macros=/DGLEW_STATIC /DWIN32 "
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\glm.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\glm\detail\glm.cpp"
echo glm object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui.cpp"
echo imgui object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_demo.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_demo.cpp"
echo imgui_demo object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_draw.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_draw.cpp"
echo imgui_draw object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_impl_glfw.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_impl_glfw.cpp"
echo imgui_impl_glfw object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_impl_opengl3.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_impl_opengl3.cpp"
echo imgui_impl_opengl3 object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_tables.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_tables.cpp"
echo imgui_tables object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\imgui_widgets.obj" "C:\Users\HP\Desktop\NWengine\dependencies\vendor\imgui\imgui_widgets.cpp"
echo imgui_widgets object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Audio.obj" "C:\Users\HP\Desktop\NWengine\Engine\Audio\Audio.cpp"
echo Audio object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Collision.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Collision.cpp"
echo Collision object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\PostProcessing.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\PostProcessing.cpp"
echo PostProcessing object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Script.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Script.cpp"
echo Script object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Transform.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Transform.cpp"
echo Transform object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Console.obj" "C:\Users\HP\Desktop\NWengine\Engine\Console.cpp"
echo Console object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\NWstd.obj" "C:\Users\HP\Desktop\NWengine\Engine\STL\NWstd.cpp"
echo NWstd object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Parser.obj" "C:\Users\HP\Desktop\NWengine\Engine\STL\Parser.cpp"
echo Parser object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Particles.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Particles.cpp"
echo Particles object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Physics.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Physics.cpp"
echo Physics object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Camera.obj" "C:\Users\HP\Desktop\NWengine\Engine\Rendering\Camera.cpp"
echo Camera object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Context.obj" "C:\Users\HP\Desktop\NWengine\Engine\Context\Context.cpp"
echo Context object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\GameObject.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\GameObject.cpp"
echo GameObject object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Globals.obj" "C:\Users\HP\Desktop\NWengine\Engine\Globals.cpp"
echo Globals object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Inputs.obj" "C:\Users\HP\Desktop\NWengine\Engine\Inputs.cpp"
echo Inputs object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Primitives.obj" "C:\Users\HP\Desktop\NWengine\Engine\Context\Primitives.cpp"
echo Primitives object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\NWengine.obj" "C:\Users\HP\Desktop\NWengine\Engine\NWengine.cpp"
echo NWengine object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\RessourcesLoader.obj" "C:\Users\HP\Desktop\NWengine\Engine\RessourcesLoader.cpp"
echo RessourcesLoader object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Scene.obj" "C:\Users\HP\Desktop\NWengine\Engine\Scene.cpp"
echo Scene object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Shader.obj" "C:\Users\HP\Desktop\NWengine\Engine\Shader.cpp"
echo Shader object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Source.obj" "C:\Users\HP\Desktop\NWengine\Engine\Source.cpp"
echo Source object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Text.obj" "C:\Users\HP\Desktop\NWengine\Engine\Text.cpp"
echo Text object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Texture.obj" "C:\Users\HP\Desktop\NWengine\Engine\Texture.cpp"
echo Texture object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\TileMap.obj" "C:\Users\HP\Desktop\NWengine\Engine\TileMap.cpp"
echo TileMap object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\player.obj" "C:\Users\HP\Desktop\NWengine\Ressources\Scripts\player.cpp"
echo player object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Sprite.obj" "C:\Users\HP\Desktop\NWengine\Engine\Components\Sprite.cpp"
echo Sprite object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\StaticGuiVariable.obj" "C:\Users\HP\Desktop\NWengine\Engine\Gui\StaticGuiVariable.cpp"
echo StaticGuiVariable object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\Animation.obj" "C:\Users\HP\Desktop\NWengine\Engine\Animation.cpp"
echo Animation object created
cl /c /I "C:\Users\HP\Desktop\NWengine\dependencies\GLEW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\GLFW\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\vendor"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\freetype\include"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\OPENAL\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine"^
 /I "C:\Users\HP\Desktop\NWengine\dependencies\SND\include"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Components"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Audio"^
 /I "C:\Users\HP\Desktop\NWengine\Ressources"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Context"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Gui"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Maths"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Rendering"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 /I "C:\Users\HP\Desktop\NWengine\Engine\STL"^
 %macros% /Fo"C:\Users\HP\Desktop\NWengine\Build\Engine Build\Data\ScriptManager.obj" "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting\ScriptManager.cpp"
echo ScriptManager object created



echo Objects creation has ended.
