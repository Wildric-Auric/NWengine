@echo off
cd %~dp0%
mv ..\Build\Bin\Sandbox\Debug64\Sandbox.exe ..\Build\Sandbox.exe
start cmd /k ..\Build\Sandbox.exe
