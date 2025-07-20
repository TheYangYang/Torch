@echo off
setlocal EnableDelayedExpansion

:: Use Vulkan SDK environment variable to find glslc
set GLSLC=%VULKAN_SDK%\Bin\glslc.exe
set SHADER_DIR=%~dp0shader
set OUT_DIR=%~dp0compiled_shaders

:: Check if glslc exists
if not exist "%GLSLC%" (
    echo ERROR: glslc not found at %GLSLC%
    echo Make sure the Vulkan SDK is installed and VULKAN_SDK environment variable is set.
    pause
    exit /b
)

:: Create output directory
if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

:: Compile shaders
call :compile_shader "%SHADER_DIR%\shader.vert" "%OUT_DIR%\vert.spv"
call :compile_shader "%SHADER_DIR%\shader.frag" "%OUT_DIR%\frag.spv"

pause
exit /b

:compile_shader
set "SRC_FILE=%~1"
set "OUT_FILE=%~2"

:: Check if output doesn't exist
if not exist "%OUT_FILE%" (
    echo Compiling %SRC_FILE%...
    "%GLSLC%" "%SRC_FILE%" -o "%OUT_FILE%"
    exit /b
)

:: Get file modification timestamps
for %%F in ("%SRC_FILE%") do set "SRC_TIME=%%~tF"
for %%F in ("%OUT_FILE%") do set "OUT_TIME=%%~tF"

:: Compare timestamps
if "!SRC_TIME!" GTR "!OUT_TIME!" (
    echo Recompiling %SRC_FILE%...
    "%GLSLC%" "%SRC_FILE%" -o "%OUT_FILE%"
) else (
    echo %OUT_FILE% is up to date.
)

exit /b
