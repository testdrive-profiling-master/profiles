@echo off

cd "%TESTDRIVE_DIR%bin\eclipse"

if exist %2 goto RUN_ECLIPSE
echo Preparing workspace for this at first time.
echo Please wait...

"%TESTDRIVE_DIR%bin\eclipse\eclipse.exe" -nosplash -data "%2" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -importAll "%1"

:RUN_ECLIPSE
start %TESTDRIVE_DIR%bin\eclipse\eclipse.exe -data "%2"