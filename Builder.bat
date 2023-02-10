@echo off
 if not defined DevEnvDir (call vcvars32)
LINK "Compiled\\ScriptManager.obj"^
 "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build\\Data\\x64\\EngineDll\\NWengine.lib"^
 "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build\\Data\\x64\\EngineDll\\NWengine.obj"^
 "Compiled\\SoundPlayer.obj"^
 "Compiled\\player.obj"^
 /DLL /LIBPATH:"dependencies\\GLFW\\lib-vc2019"^
 /LIBPATH:"dependencies\\GLEW\\lib\\Release\\Win32"^
 /LIBPATH:"dependencies\\freetype\\release static\\vs2015-2022\\win32"^
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
 /OUT:C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\NWengine_temp.dll > log.txt