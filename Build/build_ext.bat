cl /EHsc /DBUILDER_EXT /I ..\Engine\Maths /I ..\Engine\STL Builder_ext.cpp Builder.cpp /link /OUT:ext.exe
del Builder.obj
del Builder_ext.obj