@echo off
 if not defined DevEnvDir (call vcvars32)
cl /EHsc /MD /c  /DGLEW_STATIC  /DWIN32  /DWIN_32  /D_CRT_SECURE_NO_WARNINGS  /D_CONSOLE  /D_MBCS ^
 /Fo"Compiled\\" /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\GLEW\\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\GLFW\\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\vendor"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\IRRKLANG\\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\freetype\\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\OPENAL\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\dependencies\\SND\\include"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Components"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Audio"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Ressources"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Context"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Gui"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Maths"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Rendering"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\STL"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Native Scripting"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Engine\\Reflection"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Build"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Scripts\\"^
 /I "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Ressources\\Scripts\\" "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Scripts\\PostProcessing.cpp"^
 "C:\\Users\\HP\\source\\repos\\Wildric-Auric\\NWengine\\Ressources\\Scripts\\player.cpp"^
 > log.txt