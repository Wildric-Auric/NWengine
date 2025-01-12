@echo off
setlocal enabledelayedexpansion

REM Define available shader types
set shaderTypes=Basic BasicB Text TextB 

REM Check if the user passed any arguments
if "%~1"=="" (
    echo Usage: newShader --type "directory" "name"
    echo Use newShader --help for more information.
    exit /b
)

REM Help command
if "%~1"=="--help" (
    echo Shader Creation Script
    echo -----------------------
    echo Usage:
    echo     newShader --type "directory" "name"
    echo     Valid types: Basic, BasicB, Text, TextB 
    echo Example:
    echo     newShader --textured "C:\shaders\new" "CustomShader"
    exit /b
)

REM Extract shader type from argument
set shaderType=%~1
set shaderType=!shaderType:--=!

REM Verify shader type
set validType=0
for %%t in (%shaderTypes%) do (
    if /i "!shaderType!"=="%%t" set validType=1
)

if "!validType!"=="0" (
    echo Invalid shader type: %shaderType%
    echo Valid types: %shaderTypes%
    exit /b
)

REM Extract directory name
set directory=%~2
if "%directory%"=="" (
    echo Error: No directory specified.
    echo Usage: newShader --%shaderType% "directory" "name"
    exit /b
)

REM Extract shader name
set shaderName=%~3
if "%shaderName%"=="" (
    echo Error: No shader name specified.
    echo Usage: newShader --%shaderType% "directory" "name"
    exit /b
)

REM Ensure directory exists
if not exist "%directory%" (
    mkdir "%directory%"
    if errorlevel 1 (
        echo Error: Could not create directory "%directory%".
        exit /b
    )
)

REM Check if the shader template file exists
set templateFile=%shaderType%.shader
set templateFileDir=%~dp0%shaderType%.shader
if not exist "%templateFileDir%" (
    echo Error: Template shader file "%templateFile%" not found in the directory.
    exit /b
)

REM Copy the template file to the target directory
copy "%templateFileDir%" "%directory%" > nul
if errorlevel 1 (
    echo Error: Could not copy template file to "%directory%".
    exit /b
)

REM Rename the file in the target directory
set newShaderName=%shaderName%.shader
ren "%directory%\%templateFile%" "%newShaderName%"
if errorlevel 1 (
    echo Error: Could not rename shader file in "%directory%".
    exit /b
)

echo Shader "%newShaderName%" successfully created in "%directory%".
exit /b

