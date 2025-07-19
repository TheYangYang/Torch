@echo off
setlocal enabledelayedexpansion

:: Get absolute path to the project root (one level above this script)
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..") do set "SOURCE_DIR=%%~fI"

:: Define build directory path
set "BUILD_DIR=%SOURCE_DIR%\out\build"

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Navigate to build directory
pushd "%BUILD_DIR%"

:: Run CMake to configure the project
echo === Generating Release build files ===
cmake -DCMAKE_BUILD_TYPE=Release "%SOURCE_DIR%"

:: Build the project
echo === Building the project in Release mode ===
cmake --build . --config Release

:: Optionally, run the executable (edit path if needed)
:: echo === Running the application ===
:: "%BUILD_DIR%\bin\Sandbox.exe"

:: Return to original directory
popd

echo === Release build and configuration completed ===
endlocal
pause
