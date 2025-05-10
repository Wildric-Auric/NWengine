@echo off
setlocal

set "BASE=%~dp0"

set "OUTDIR=%BASE%Template"
set "LIBDIR=%OUTDIR%\lib"
set "HDRDIR=%OUTDIR%\include"
set "ENGDIR=%BASE%Engine"
set "PROJ=%BASE%Build\NWengineCore.vcxproj"
set "BINDBG=%BASE%Build\Bin\NWengineCore\Debug64"
set "BINRLS=%BASE%Build\Bin\NWengineCore\Release64"

REM 1) Create folders
md "%OUTDIR%" 2>nul
md "%LIBDIR%" 2>nul
md "%HDRDIR%" 2>nul

echo Building Debug.
msbuild "%PROJ%" -p:Configuration=Debug;Platform=x64

echo Building Release.
msbuild "%PROJ%" -p:Configuration=Release;Platform=x64

copy /Y "%BINDBG%\NWengineCore64d.idb" "%LIBDIR%\NWengineCore64d.idb"
copy /Y "%BINDBG%\NWengineCore64d.lib" "%LIBDIR%\NWengineCore64d.lib"
copy /Y "%BINDBG%\NWengineCore64d.pdb" "%LIBDIR%\NWengineCore64d.pdb"
copy /Y "%BINRLS%\NWengineCore64.lib"  "%LIBDIR%\NWengineCore64.lib"

md "%HDRDIR%\temp" 2>nul

for /R "%ENGDIR%" %%F in (*.h)   do copy /Y "%%~fF" "%HDRDIR%\temp\"
for /R "%ENGDIR%" %%F in (*.hpp) do copy /Y "%%~fF" "%HDRDIR%\temp\"

hdrasm "%HDRDIR%\temp\" "%HDRDIR%\" NWengineCore.h NWengine.h

rmdir /S /Q "%HDRDIR%\temp"

echo.
echo ===== Build Complete =====
endlocal
exit /B 0

