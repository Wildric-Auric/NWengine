@echo off
 if not defined DevEnvDir (call vcvars32)
LINK "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\temp\\P\\Compiled\\ScriptManager.obj"^
 "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build\\Data\\x86\\EngineDll\\NWengine.lib"^
 "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build\\Data\\x86\\EngineDll\\NWengine.obj"^
 "C:\Users\HP\source\repos\Wildric-Auric\temp\P\Compiled\player.obj"^
 /DLL /LIBPATH:"dependencies\\GLFW\\lib-vc2019"^
 /LIBPATH:"dependencies\\GLEW\\lib\\Release\\Win32"^
 /LIBPATH:"dependencies\\freetype\\lib\\win32"^
 /LIBPATH:"dependencies\\SND\\lib"^
 /LIBPATH:"dependencies\\OPENAL\\win32"^
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
 /OUT:C:\\Users\\HP\\source\\repos\\Wildric-Auric\\temp\\P\\NWengine_temp.dll > log.txt