@echo off

:: BatchGotAdmin
REM  --> Check for permissions
 >nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.
 if '%errorlevel%' NEQ '0' (
     echo Requesting administrative privileges...
     goto UACPrompt
 ) else ( goto gotAdmin )

:UACPrompt
     echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
     echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
     exit /B

:gotAdmin
     if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
     pushd "%CD%"
     CD /D "%~dp0"


REM build main tools
make

REM build make release
cd MakeRelease
Make
cd ..

MakeRelease\MakeRelease.exe

iexpress TestDriveInstaller.sed

if exist %TESTDRIVE_PROFILE%../testdrive-profiling-master.github.io/ (
	cp -f TestDrive_Profiling_Master.exe %TESTDRIVE_PROFILE%../testdrive-profiling-master.github.io/wiki/
)