@echo off

cd %TESTDRIVE_PROFILE%common\util\WDK
set DDK_PATH=%CD%\

call bin\setenv.bat %DDK_PATH% fre x86 WIN7

cd %~dp0

pvk2pfx -pvk VERI_CODE_~~~.pvk -pi 1q2w3e4r -spc VERI_CODE_~~~.spc -pfx ~~~.pfx -po 1q2w3e4r -f

