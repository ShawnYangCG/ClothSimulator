@echo off
setlocal
if not "%~1"=="" set "QTDIR=%~1"
if not exist "%QTDIR%\bin\qmake.exe" (
  echo Usage: build-release.cmd "C:\Qt\5.13.0\msvc2017"
  echo Or set QTDIR to a Qt 5 MSVC x86 SDK directory.
  exit /b 1
)
set "QTSDK=%QTDIR%"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
  echo Visual Studio Installer not found. Install Visual Studio C++ build tools.
  exit /b 1
)
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%i"
if not defined VSROOT exit /b 1
call "%VSROOT%\VC\Auxiliary\Build\vcvars32.bat"
if errorlevel 1 exit /b 1
set "PATH=%QTSDK%\bin;%PATH%"
cd /d "%~dp0"
if not exist build mkdir build
cd build
"%QTSDK%\bin\qmake.exe" "..\ClothSimulator.pro" CONFIG+=release
if errorlevel 1 exit /b 1
nmake /NOLOGO
if errorlevel 1 exit /b 1
cd ..
if not exist dist mkdir dist
copy /y "build\ClothSimulator_version_1.exe" "dist\" >nul
if errorlevel 1 exit /b 1
"%QTSDK%\bin\windeployqt.exe" --release --no-translations --no-compiler-runtime "dist\ClothSimulator_version_1.exe"
if errorlevel 1 exit /b 1
set "CRT="
for /d %%i in ("%VCToolsRedistDir%x86\Microsoft.VC*.CRT") do set "CRT=%%i"
if not defined CRT (
  echo Cannot find x86 Visual C++ runtime files under %VCToolsRedistDir%.
  exit /b 1
)
copy /y "%CRT%\*.dll" "dist\" >nul
if errorlevel 1 exit /b 1
xcopy /e /i /y "testObj" "dist\testObj" >nul
if errorlevel 1 exit /b 1
if not exist dist\output mkdir dist\output
echo Ready: %CD%\dist\ClothSimulator_version_1.exe
endlocal
