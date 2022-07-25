@echo off
call vcvars32
@echo off

cl /MD /D_USRDLL /D_WINDLL^
 -I C:\Users\HP\Desktop\NWengine^
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
 -I C:\Users\HP\Desktop\NWengine\Engine\Rendering^
 -I C:\Users\HP\Desktop\NWengine\Engine\STL^
 -I "C:\Users\HP\Desktop\NWengine\Engine\Native Scripting"^
 Ressources\Scripts\player.cpp^
 Release\Inputs.obj^
 Release\Transform.obj^
 Release\Globals.obj^
 Release\Console.obj^
 Release\NWscripting.obj^
 /link^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\GLFW\lib-vc2019"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\GLEW\lib\Release\Win32"^
 /LIBPATH:"C:\Users\HP\Desktop\NWengine\dependencies\IRRKLANG\lib\Win32-visualStudio"^
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
 /DLL /OUT:ScriptDlls\player.dll

del *.obj
del ScriptDlls\player.lib