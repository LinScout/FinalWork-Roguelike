@echo off
echo ========================================
echo XYZ Roguelike - Release Build Script
echo ========================================
echo.

REM Путь к MSBuild (Visual Studio 2022)
set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

REM Проверка существования MSBuild
if not exist %MSBUILD% (
    echo ERROR: MSBuild not found!
    echo Please install Visual Studio 2022 or update the path in this script.
    pause
    exit /b 1
)

echo [1/5] Cleaning solution...
%MSBUILD% Game.sln /t:Clean /p:Configuration=Release /p:Platform=x64 /v:minimal
if errorlevel 1 (
    echo ERROR: Clean failed!
    pause
    exit /b 1
)
echo Clean completed successfully.
echo.

echo [2/5] Building XYZEngine (Release x64)...
%MSBUILD% XYZEngine\XYZEngine.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /v:minimal
if errorlevel 1 (
    echo ERROR: XYZEngine build failed!
    pause
    exit /b 1
)
echo XYZEngine built successfully.
echo.

echo [3/5] Building XYZRoguelike (Release x64)...
%MSBUILD% XYZRoguelike\XYZRoguelike.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /v:minimal
if errorlevel 1 (
    echo ERROR: XYZRoguelike build failed!
    pause
    exit /b 1
)
echo XYZRoguelike built successfully.
echo.

echo [4/5] Creating release folder...
if exist "Release_Package" rmdir /s /q "Release_Package"
mkdir "Release_Package"

REM Копирование .exe
echo Copying executable...
copy "XYZRoguelike\x64\Release\XYZRoguelike.exe" "Release_Package\" >nul
if errorlevel 1 (
    echo ERROR: Failed to copy executable!
    pause
    exit /b 1
)

REM Копирование ресурсов
echo Copying resources...
xcopy "Resources" "Release_Package\Resources\" /E /I /Y >nul
if errorlevel 1 (
    echo ERROR: Failed to copy resources!
    pause
    exit /b 1
)

REM Копирование документации
echo Copying documentation...
copy "README_GAME.txt" "Release_Package\" >nul
copy "ЗАЩИТА_ПРОЕКТА.txt" "Release_Package\" >nul 2>nul
copy "DESIGN_DOC.txt" "Release_Package\" >nul 2>nul

echo Release folder created successfully.
echo.

echo [5/5] Creating ZIP archive...
powershell -command "Compress-Archive -Path 'Release_Package\*' -DestinationPath 'XYZRoguelike_v1.0.zip' -Force"
if errorlevel 1 (
    echo WARNING: Failed to create ZIP archive (PowerShell required)
    echo You can manually create ZIP from Release_Package folder
) else (
    echo ZIP archive created: XYZRoguelike_v1.0.zip
)
echo.

echo ========================================
echo BUILD COMPLETED SUCCESSFULLY!
echo ========================================
echo.
echo Release files location: Release_Package\
echo Executable: Release_Package\XYZRoguelike.exe
echo ZIP archive: XYZRoguelike_v1.0.zip
echo.
echo You can now distribute the Release_Package folder or ZIP file.
echo.
pause
