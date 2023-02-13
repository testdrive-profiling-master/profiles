@echo off
title TestDrive Installer
setlocal

echo ******************************************************
echo ****        TestDrive Automatic Installer.        ****
echo ******************************************************

echo.

set /p install_path=Set your install path (default[C:\Project]) : 

if "%install_path%" == "" set install_path="C:\Project"

TestDrive_Installer.exe %install_path%

endlocal
