@echo off
cd %~dp0%
if not defined VisualStudioVersion (
    call vcvars64.bat
)
msbuild ..\\Build\\Sandbox.vcxproj -p:Configuration=Debug;Platform=x64
