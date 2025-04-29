@echo off

if "%~1" == "" (
    echo "Usage:   Pass arguments to directories which files will be formated"
    echo "Example: format-all.bat Dir0 \"Dir 1\" Dir2"
    goto :eof
)

for %%a in (%*) do (
    if exist "%%a" (
        pushd "%%~a"
        for /R %%f in (*.cpp *.h *.hpp *.shader) do clang-format -i "%%f"
        popd
    ) else (
        echo Directory "%%a" not found.
    )
)
