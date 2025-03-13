@echo off

set TOOLSET=%1
if "%TOOLSET%"=="" (
    echo No toolset version specified. Set to hardcoded latest
    set TOOLSET=142
    rem for /f "tokens=2 delims==" %%i in ('"wmic environment where "Name='VisualStudioVersion'" get VariableValue /value"') do set TOOLSET=%%i
)

mkdir nwtmp
mkdir ..\\Dependencies\\freetype
mkdir ..\\Dependencies\\freetype\\lib\\win86
mkdir ..\\Dependencies\\freetype\\lib\\win64

mkdir ..\\Dependencies\\glew\\lib\\release\\win86
mkdir ..\\Dependencies\\glew\\lib\\release\\win64
mkdir ..\\Dependencies\\glew\\lib\\debug\\win86
mkdir ..\\Dependencies\\glew\\lib\\debug\\win64
mkdir ..\\Dependencies\\vendor\\stb

cd nwtmp
git clone https://github.com/freetype/freetype.git
git clone https://github.com/Perlmint/glew-cmake.git
git clone https://github.com/nothings/stb.git

copy stb\\stb_image.h ..\\..\\Dependencies\\vendor\\stb\\stb_image.h
copy stb\\stb_image_write.h ..\\..\\Dependencies\\vendor\\stb\\stb_image_write.h

cd freetype
msbuild -p:Configuration="Release Static"  -p:Platform=x64 
msbuild -p:Configuration="Release Static" -p:Platform=Win32
copy "objs\\Win32\\Release Static\\freetype.lib" ..\\..\\..\\Dependencies\\freetype\\lib\\win86\\freetype.lib
copy "objs\\x64\\Release Static\\freetype.lib"   ..\\..\\..\\Dependencies\\freetype\\lib\\win64\\freetype.lib
xcopy include ..\\..\\..\\Dependencies\\freetype\\include /E /I /Y
cd ../glew-cmake/build/vc15
msbuild glew.sln -p:Configuration="Release" -p:Platform=x64 -p:PlatformToolset=v%TOOLSET% -p:GLEW_STATIC=1
msbuild glew.sln -p:Configuration="Release" -p:Platform=Win32 -p:PlatformToolset=v%TOOLSET% -p:GLEW_STATIC=1
msbuild glew.sln -p:Configuration="Debug"   -p:Platform=x64 -p:PlatformToolset=v%TOOLSET% -p:GLEW_STATIC=1
msbuild glew.sln -p:Configuration="Debug"  -p:Platform=Win32 -p:PlatformToolset=v%TOOLSET% -p:GLEW_STATIC=1
copy ..\\..\\lib\\Release\\Win32\\glew32s.lib   ..\\..\\..\\..\\..\\Dependencies\\glew\\lib\\release\\win86\\glew32s.lib
copy ..\\..\\lib\\Release\\x64\\glew32s.lib     ..\\..\\..\\..\\..\\Dependencies\\glew\\lib\\release\\win64\\glew32s.lib
copy ..\\..\\lib\\Debug\\Win32\\glew32sd.lib   ..\\..\\..\\..\\..\\Dependencies\\glew\\lib\\debug\\win86\\glew32s.lib
copy ..\\..\\lib\\Debug\\x64\\glew32sd.lib     ..\\..\\..\\..\\..\\Dependencies\\glew\\lib\\debug\\win64\\glew32s.lib
xcopy ..\\..\\include ..\\..\\..\\..\\..\\Dependencies\\glew\\include /E /I /Y
cd ../../../../...
