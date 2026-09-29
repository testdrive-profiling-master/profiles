@echo off
echo Pre-update.

echo Update CI...
SETLOCAL
set CURPATH=%cd%

cd /d %TESTDRIVE_DIR%
git pull origin master
cd /d %TESTDRIVE_PROFILE%
git pull origin master
ENDLOCAL

echo run...
start "%TESTDRIVE_DIR%TestDrive.exe" project.profile
