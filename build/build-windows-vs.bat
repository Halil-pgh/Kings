@echo off
setlocal

:: Get the directory of the batch file
set SOURCE_DIR=%~dp0..

if "%1"=="debug" (
    set CMAKE_BUILD_TYPE=Debug
    set BUILD_DIR=%SOURCE_DIR%\cmake-build-debug
) else (
    set CMAKE_BUILD_TYPE=Release
    set BUILD_DIR=%SOURCE_DIR%\cmake-build-release
)

cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% -DBUILD_SHARED_LIBS=OFF -S %SOURCE_DIR% -B %BUILD_DIR%
