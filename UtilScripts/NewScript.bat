@echo off
setlocal enabledelayedexpansion

REM Check if the user passed the necessary arguments
if "%~1"=="" (
    echo Usage: createUserScripts.bat "ClassName" "Directory"
    exit /b
)


REM Set the class name and directory
set className=%~1
set directory=%~2
if "%~2"=="" (
    set directory=./
)

REM Call the Python script with class name and directory
python "%~dp0create_user_scripts.py" "%className%" "%directory%"

if errorlevel 1 (
    echo Error: Python script execution failed.
    exit /b
)

echo Files for class "%className%" created successfully in "%directory%".
