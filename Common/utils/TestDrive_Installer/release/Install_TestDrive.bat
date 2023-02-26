@echo off
title TestDrive Installer
setlocal

echo ******************************************************
echo ****        TestDrive Automatic Installer.        ****
echo ******************************************************

echo.

set /p install_path=Set your install path (default[C:\Project]) : 

if "%install_path%" == "" set install_path="C:\Project"

if not exist %install_path% (
	mkdir %install_path%
)

TestDrive_Installer.exe %install_path%

endlocal
