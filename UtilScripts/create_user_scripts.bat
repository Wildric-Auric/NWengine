@echo off
setlocal

REM — check args —
if "%~1"=="" goto usage
if "%~2"=="" goto usage

set "CLASS=%~1"
set "DIR=%~2"

REM — ensure directory exists —
if not exist "%DIR%" mkdir "%DIR%"

set "HFILE=%DIR%\%CLASS%.h"
set "CPPFILE=%DIR%\%CLASS%.cpp"

REM — write header file —
echo #pragma once                       >  "%HFILE%"
echo #include "NWengineCore.h"          >> "%HFILE%"
echo.                                   >> "%HFILE%"
echo class %CLASS%: public Scriptable { >> "%HFILE%"
echo public:                            >> "%HFILE%"
echo     SCRIPT_CONSTR(%CLASS%);        >> "%HFILE%"
echo     void Start() override;         >> "%HFILE%"
echo     void Update() override;        >> "%HFILE%"
echo };                                 >> "%HFILE%"

REM — write source file —
echo #include "%CLASS%.h"      >  "%CPPFILE%"
echo.                          >> "%CPPFILE%"
echo void %CLASS%::Start() {   >> "%CPPFILE%"
echo }                         >> "%CPPFILE%"
echo.                          >> "%CPPFILE%"
echo void %CLASS%::Update() {  >> "%CPPFILE%"
echo }                         >> "%CPPFILE%"

echo Created "%HFILE%" and "%CPPFILE%"
exit /B 0

:usage
echo Usage: %~nx0 ClassName Directory
exit /B 1
