@echo off
rem Builds with the VS 2026 toolchain; usage: build.bat [run]
set "PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\Installer"
set "VS=C:\Program Files\Microsoft Visual Studio\18\Community"
call "%VS%\Common7\Tools\VsDevCmd.bat" -arch=x64 >nul
set "PATH=%PATH%;%VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"
cd /d "%~dp0"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
cmake --build build || exit /b 1
if /i "%1"=="run" build\rigid_body_physics_engine.exe

