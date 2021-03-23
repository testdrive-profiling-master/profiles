@echo off
if not defined TESTDRIVE_PROFILE (
	echo *E: Check testdrive run environment first!
	goto EXIT
)

if "%1"=="" (
:SHOW_USAGE
	echo.
	echo * Create project template with TestDrive Profiling Master.
	echo Usage : create_project project_type project_name
	echo.
	echo    project_type
	echo        'cpp'         : C++ project
	echo    project_name      : making project name
	echo.
	echo ex. create_project cpp test
	goto EXIT
)

@rem if "%2"=="" goto SHOW_USAGE

if "%1"=="cpp" (
	mkdir %2
	cp -rf %TESTDRIVE_PROFILE%Common/bin/project_template_cpp/. %2/
) else (
	echo *E: Project type does not exist : "%1"
	goto SHOW_USAGE
)

:EXIT
