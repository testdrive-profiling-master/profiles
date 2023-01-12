@echo off

SETLOCAL

cd %TESTDRIVE_PROFILE%common\WDK
set DDK_PATH=%CD%\

if exist bin\setenv.bat goto COMPILE_WDM_DRIVER
@echo *E: WDK tool is not found... Check Microsoft download page( https://www.microsoft.com/en-us/download/details.aspx?id=11800 )
start explorer .
start readme.rtf
cd %~dp0
goto END:

:COMPILE_WDM_DRIVER
call %TESTDRIVE_PROFILE%common\WDK\bin\setenv.bat %DDK_PATH% %1 %2 %3

cd %~dp0

makecert -r -pe -ss PrivateCertStore -n CN=TestDrive(Test) -eku 1.3.6.1.5.5.7.3.3 TestDrive.cer

:END
ENDLOCAL
