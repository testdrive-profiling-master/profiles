@echo off

cd %TESTDRIVE_PROFILE%common\util\WDK
set DDK_PATH=%CD%\

call bin\setenv.bat %DDK_PATH% chk x64 WIN7

cd %~dp0

bcdedit -set TESTSIGNING ON

CertMgr /add TestDrive.cer /s /r localMachine root

