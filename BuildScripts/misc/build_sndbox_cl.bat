@echo off
REM build_sandbox_clang.bat — compile only Sandbox with native clang++, linker flags

SETLOCAL ENABLEDELAYEDEXPANSION

:: Usage: build_sandbox_clang.bat [Debug|Release] [x86|x64]
IF /I "%~1"=="Release" (
  SET CONFIG=Release
) ELSE (
  SET CONFIG=Debug
)
IF /I "%~2"=="x86" (
  SET PLATFORM=x86
  SET PLATSUF=86
  SET ARCHFLAG=-m32
) ELSE (
  SET PLATFORM=x64
  SET PLATSUF=64
  SET ARCHFLAG=-m64
)

::— config‐specific flags from your premake
IF /I "%CONFIG%"=="Debug" (
  SET CXXFLAGS=-std=c++14 -g -w %ARCHFLAG%
  SET DEFINES=-DGLEW_STATIC -DNW_DEBUG -D_DEBUG -DNW_VERSION=\"0.9.090125\"
  SET GLEWDIR=Dependencies\glew\lib\debug\win%PLATSUF%
  SET FREETYPEDIR=Dependencies\freetype\lib\win%PLATSUF%
  SET OUTDIR=Build\Bin\Sandbox\Debug%PLATSUF%
) ELSE (
  SET CXXFLAGS=-std=c++11 -O2 -DNDEBUG -w %ARCHFLAG%
  SET DEFINES=-DGLEW_STATIC -DNW_NDEBUG -DNW_RELEASE -DNW_VERSION=\"0.9.090125\"
  SET GLEWDIR=Dependencies\glew\lib\release\win%PLATSUF%
  SET FREETYPEDIR=Dependencies\freetype\lib\win%PLATSUF%
  SET OUTDIR=Build\Bin\Sandbox\Release%PLATSUF%
)

::— include dirs
SET INCDIRS=-I. -IDependencies\glew\include -IDependencies\vendor -IDependencies\freetype\include -IEngine -IEngine\Audio -IEngine\Components -IEngine\Context -IEngine\Components -IEngine\Front -IEngine\Graphics  -IEngine\Systems -IEngine\Systems\Scripting -IEngine\Components\UI -IEngine\Util -IEngine\Context\NWin -IEngine\Audio\AudioLib -ISandbox\src

::— tell the MSVC linker where to look
SET LINKFLAGS=-Xlinker "/LIBPATH:%CD%\%GLEWDIR%" -Xlinker "/LIBPATH:%CD%\%FREETYPEDIR%"

::— exactly the .lib files your premake links
SET LIBFILES=-lopengl32.lib -lfreetype.lib -lglew32s.lib -lxaudio2.lib -ldwmapi.lib -lLIBCMTd.lib -lShell32.lib -lComdlg32.lib -lOle32.lib -luser32.lib -lgdi32
:: make sure output dir exists
IF NOT EXIST "%OUTDIR%" mkdir "%OUTDIR%"

:: compile resources, if any
SET RESFILES=
FOR %%F IN ("*.rc") DO (
  echo [RC] %%~nxF
  rc.exe "%%~F" /fo "%OUTDIR%\%%~nF.res"
  SET RESFILES=!RESFILES! "%OUTDIR%\%%~nF.res"
)

:: gather cpp’s
SET SOURCES=
FOR %%F IN ("UnityBuild\*.cpp") DO (
  SET SOURCES=!SOURCES! "%%~F"
)
FOR /R "Sandbox\src" %%F IN (*.cpp) DO (
  SET SOURCES=!SOURCES! "%%~F"
)

echo.
echo === Building Sandbox (%CONFIG%, %PLATFORM%) ===
clang++ %CXXFLAGS% %DEFINES% %INCDIRS% !SOURCES! !RESFILES! %LINKFLAGS% %LIBFILES% -o "%OUTDIR%\Sandbox.exe"
IF ERRORLEVEL 1 (
  echo.
  echo *** BUILD FAILED ***
  EXIT /B 1
)
echo.
echo *** BUILD SUCCEEDED: "%OUTDIR%\Sandbox.exe" ***
ENDLOCAL
EXIT /B 0

