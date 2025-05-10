pushd "%~dp0"
call setup_vendor.bat
cd ../
premake5 vs2022
popd
