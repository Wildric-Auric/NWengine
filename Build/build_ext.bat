cl /EHsc /DBUILDER_EXT /I ..\Engine\Maths /I ..\Engine\STL Builder_ext.cpp Builder.cpp ..\Engine\STL\Utilities.cpp /link comdlg32.lib /OUT:ext.exe
del Builder.obj
del Builder_ext.obj
del Utilities.obj
