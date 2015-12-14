@echo off
setlocal
if "%~1" equ "" (
  set BUILD_TYPE=Debug
  goto :GotBuildType
)
if "%~1" equ "debug" (
  set BUILD_TYPE=Debug
  goto :GotBuildType
)
if "%~1" equ "Debug" (
  set BUILD_TYPE=Debug
  goto :GotBuildType
)
if "%~1" equ "release" (
  set BUILD_TYPE=Release
  goto :GotBuildType
)
if "%~1" equ "release" (
  set BUILD_TYPE=Release
  goto :GotBuildType
)

echo Unsupported build type
exit /b 1

:GotBuildType
call :SetPaths "%~fp0\.." "%~fp0\..\.."
goto :Eof

:SetPaths
set SCRIPTDIR=%~fp1
set REPODIR=%~fp2
set CMAKEDIR=%REPODIR%\cmake

if not exist %CMAKEDIR%\%BUILD_TYPE% (
  mkdir %CMAKEDIR%\%BUILD_TYPE%
)

if "%CMAKE%" equ "" (
  set CMAKE=cmake
)

cd %CMAKEDIR%\%build_type%
%CMAKE% ^
  --no-warn-unused-cli ^
  -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -DCMAKE_INSTALL_PREFIX:PATH=%CMAKEDIR%\%BUILD_TYPE%\install ^
  %REPODIR%
