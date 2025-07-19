@echo off
setlocal enabledelayedexpansion

:: Get the directory of the script (Torch/scripts/)
set "SCRIPT_DIR=%~dp0"
:: Go up one level to the project root (Torch/)
for %%I in ("%SCRIPT_DIR%..") do set "SOURCE_DIR=%%~fI"

:: Define build directory inside the root
set "BUILD_DIR=%SOURCE_DIR%\out\build"

:: Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Navigate to build directory
pushd "%BUILD_DIR%"

:: Debug: Print paths
echo Project root: %SOURCE_DIR%
echo Build dir: %BUILD_DIR%

:: Run CMake to generate build files
echo Generating build files with CMake...
cmake -DCMAKE_BUILD_TYPE=Debug "%SOURCE_DIR%"

:: Build the project
echo Building the project with MSBuild...
cmake --build . --config Debug

:: Go back
popd

echo Debug build and configuration completed.
endlocal
pause
