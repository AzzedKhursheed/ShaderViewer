@echo off
rem Build configuration script for ShaderViewer on Windows

echo === ShaderViewer Build Configuration ===
echo.

echo Detected platform: Windows
echo.

echo Available build configurations:
echo   1) windows-debug        - Debug build with Visual Studio
echo   2) windows-release      - Release build with Visual Studio  
echo   3) windows-mingw-debug  - Debug build with MinGW/MSYS2
echo   4) windows-mingw-release - Release build with MinGW/MSYS2
echo   5) custom               - Manual configuration
echo.

set /p choice="Choose configuration (1-5): "

if "%choice%"=="1" (
    set PRESET=windows-debug
    goto configure
)
if "%choice%"=="2" (
    set PRESET=windows-release
    goto configure
)
if "%choice%"=="3" (
    set PRESET=windows-mingw-debug
    goto configure
)
if "%choice%"=="4" (
    set PRESET=windows-mingw-release
    goto configure
)
if "%choice%"=="5" (
    goto custom
)

echo Invalid choice
pause
exit /b 1

:custom
echo Manual configuration selected.
echo Create a build directory and run cmake manually:
echo   mkdir build ^&^& cd build
echo   cmake .. -G "Visual Studio 17 2022" -A x64
echo   cmake --build . --config Debug
pause
exit /b 0

:configure
echo.
echo Configuring with preset: %PRESET%
cmake --preset %PRESET%

if errorlevel 1 (
    echo Configuration failed!
    pause
    exit /b 1
)

echo.
echo Configuration complete!
echo To build, run:
echo   cmake --build --preset %PRESET%
echo.
pause
