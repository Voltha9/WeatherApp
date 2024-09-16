@echo off
setlocal

:: Ask the user if they want to build in Debug or Release
set /p build_type="Choose the build type (Debug/Release): "

if /I "%build_type%"=="Debug" (
    set CONFIG=Debug
) else if /I "%build_type%"=="Release" (
    set CONFIG=Release
) else (
    echo Unrecognized build type. Defaulting to Debug.
    set CONFIG=Debug
)

:: Define paths
set "SCRIPT_DIR=%~dp0"
set "BUILD_DIR=%SCRIPT_DIR%build"

:: Check if VCPKG_ROOT is set, if yes, use it; otherwise, install locally
if defined VCPKG_ROOT (
    if exist "%VCPKG_ROOT%\vcpkg.exe" (
        set "VCPKG_DIR=%VCPKG_ROOT%"
        echo Using vcpkg from VCPKG_ROOT: %VCPKG_DIR%
    ) else (
        echo VCPKG_ROOT is set but vcpkg.exe was not found in %VCPKG_ROOT%. Falling back to local installation.
        set "VCPKG_DIR=%SCRIPT_DIR%vcpkg"
    )
) else (
    set "VCPKG_DIR=%SCRIPT_DIR%vcpkg"
    echo VCPKG_ROOT not found, using local vcpkg at %VCPKG_DIR%
)

:: Install vcpkg if it's not already present in the specified directory
if not exist "%VCPKG_DIR%\vcpkg.exe" (
    echo Installing vcpkg...
    git clone https://github.com/microsoft/vcpkg.git "%VCPKG_DIR%"
    cd /d "%VCPKG_DIR%"
    call bootstrap-vcpkg.bat
    if errorlevel 1 (
        echo Error during vcpkg installation.
        goto EndScript
    )
)

echo vcpkg installed or found successfully.
pause

:: Return to the script directory for further operations
cd /d "%SCRIPT_DIR%"

:: Install dependencies with vcpkg
echo Installing dependencies (SFML, cpprestsdk) with vcpkg...
call "%VCPKG_DIR%\vcpkg" install sfml cpprestsdk
if errorlevel 1 (
    echo Error during dependency installation.
    goto EndScript
)

echo Dependencies installed successfully.
pause

:: Create the build directory and configure with CMake
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"
echo Configuring the project in %CONFIG% mode with CMake...
call cmake -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_TOOLCHAIN_FILE="%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake" "%SCRIPT_DIR%"
if errorlevel 1 (
    echo Error during CMake configuration.
    goto EndScript
)

echo Project configured successfully in %CONFIG% mode.
pause

:: Build the project
echo Building the project in %CONFIG% mode...
call cmake --build . --config %CONFIG%
if errorlevel 1 (
    echo Error during the build process.
    goto EndScript
)

echo Project built successfully in %CONFIG% mode.
pause

:: Copy resources to the build directory
if exist "%SCRIPT_DIR%Resources" (
    echo Copying Resources directory...
    xcopy /E /I /Y "%SCRIPT_DIR%Resources" "%BUILD_DIR%\Release\Resources"
    if errorlevel 1 (
        echo Error while copying resources.
        goto EndScript
    )
    echo Resources directory copied successfully.
)

:EndScript
pause
endlocal
