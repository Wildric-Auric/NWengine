@echo off
call vcvars32

echo Set cl

cl /D_USRDLL /D_WINDLL^
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
  Ressources\Scripts\dllTest.cpp^
 /link /DLL /OUT:ScriptDlls\dllTest.dll

del *.obj
del ScriptDlls\dllTest.lib