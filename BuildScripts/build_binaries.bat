@echo off
setlocal

set "BASE=%~dp0.."

set "OUTDIR=%BASE%\Template"
set "LIBDIR=%OUTDIR%\lib"
set "HDRDIR=%OUTDIR%\include"
set "ENGDIR=%BASE%\Engine"
set "PROJ=%BASE%\Build\NWengineCore.vcxproj"
set "BINDBG=%BASE%\Build\Bin\NWengineCore\Debug64"
set "BINRLS=%BASE%\Build\Bin\NWengineCore\Release64"

REM 1) Create folders
mkdir "%OUTDIR%" 2>nul
mkdir "%LIBDIR%" 2>nul
mkdir "%HDRDIR%" 2>nul

echo Building Debug.
msbuild "%PROJ%" -p:Configuration=Debug;Platform=x64

echo Building Release.
msbuild "%PROJ%" -p:Configuration=Release;Platform=x64

copy /Y "%BINDBG%\NWengineCore64d.idb" "%LIBDIR%\NWengineCore64d.idb" > nul 2>&1
copy /Y "%BINDBG%\NWengineCore64d.lib" "%LIBDIR%\NWengineCore64d.lib" > nul 2>&1
copy /Y "%BINDBG%\NWengineCore64d.pdb" "%LIBDIR%\NWengineCore64d.pdb" > nul 2>&1
copy /Y "%BINRLS%\NWengineCore64.lib"  "%LIBDIR%\NWengineCore64.lib"  > nul 2>&1

mkdir "%HDRDIR%\temp"

for %%D in ("%ENGDIR%") do (
  pushd "%%~D"
  for /R %%F in (*.h *.hpp) do (
    copy /Y "%%~F" "%HDRDIR%\temp" >nul 2>&1
  )
  popd
)

if not defined VisualStudioVersion (call vcvars64.bat)
set "OBJDIR=%BASE%\BuildScripts\Hdrasm\obj\"
if not exist %OBJDIR% (mkdir %OBJDIR%)
cl %BASE%\BuildScripts\Hdrasm\*.cpp /Fo"%OBJDIR%\" /link /Out:%BASE%\BuildScripts\hdrasm.exe

%BASE%\BuildScripts\hdrasm "%HDRDIR%/temp/" "%HDRDIR%/" NWengineCore.h NWengine.h

rmdir /S /Q "%HDRDIR%\temp"

echo.
echo ===== Build Complete =====
endlocal
exit /B 0
