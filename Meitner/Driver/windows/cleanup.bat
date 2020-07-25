@echo off
taskkill /f /im oacrmonitor.exe

cd %TESTDRIVE_PROFILE%common\WDK
rmdir /S /Q oacr
del /Q build.dat

cd %~dp0

cd src
rmdir /S /Q objchk_win7_amd64
rmdir /S /Q objchk_win7_x86
rmdir /S /Q objfre_win7_amd64
rmdir /S /Q objfre_win7_x86
del /S /Q *.log
del /S /Q *.err
cd ..

cd vs_project
del /Q *.sdf
del /Q *.user
rmdir /S /Q Debug
rmdir /S /Q Release
rmdir /S /Q ipch
cd ..

cd %~dp0
