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

build

copy %4\testdrive.sys ..\bin\%2\

@rem SignTool sign /v /ac VeriSign.cer /s my /n CloneX /t http://timestamp.verisign.com/scripts/timstamp.dll ..\bin\%2\testdrive.sys
SignTool sign /v /s PrivateCertStore /n TestDrive(Test) /t http://timestamp.verisign.com/scripts/timstamp.dll ..\bin\%2\testdrive.sys

if "%2"== "x86" goto CODESIGN_X86
if "%2"== "x64" goto CODESIGN_X64

:CODESIGN_X64
inf2cat /driver:..\bin /os:7_X64
goto CODESIGN_END

:CODESIGN_X86
inf2cat /driver:..\bin /os:7_X86
goto CODESIGN_END

:CODESIGN_END
@rem SignTool sign /v /ac VeriSign.cer /s my /n CloneX /t http://timestamp.verisign.com/scripts/timstamp.dll ..\bin\testdrive_pcie_%2.cat
signtool sign /v /s PrivateCertStore /n TestDrive(Test) /t http://timestamp.verisign.com/scripts/timestamp.dll ..\bin\testdrive_pcie_%2.cat

:END
ENDLOCAL
