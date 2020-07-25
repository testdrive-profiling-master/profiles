@echo off
if "%programfiles(x86)%XXX"=="XXX" goto 32BIT
:64BIT
echo 64-bit Windows system
call build_driver.bat chk x64 WIN7 objchk_win7_amd64\amd64
goto END

:32BIT
echo 32-bit Windows system
call build_driver.bat chk x86 WIN7 objchk_win7_x86\i386
goto END

:END
