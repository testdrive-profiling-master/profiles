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
	echo        'c', 'c++', 'cpp'   : C++ project
	echo        'v', 'verilog'      : verilog project
	echo    project_name            : making project name
	echo.
	echo ex. create_project cpp test
	goto EXIT
)

if "%2"=="" goto SHOW_USAGE

if exist %2 (
	echo *E: Already project folder is existed. : '%2'
	goto EXIT
)

setlocal
set project_type=none

if "%1"=="c"       set project_type=cpp
if "%1"=="c++"     set project_type=cpp
if "%1"=="cpp"     set project_type=cpp
if "%1"=="v"       set project_type=verilog
if "%1"=="verilog" set project_type=verilog

if "%project_type%"=="cpp" (
	mkdir %2
	cp -rf %TESTDRIVE_PROFILE%Common/bin/project_template_cpp/. %2/
	sed "s/Test/%2/" -i ./%2/.project
	sed "s/Test/%2/" -i ./%2/Makefile
	sed "s/Test/%2 project/" -i ./%2/.inception
) else if "%project_type%"=="verilog" (
	mkdir %2
	cp -rf %TESTDRIVE_PROFILE%Common/bin/project_template_verilog/. %2/
	mv ./%2/dpi_private/.src.cpp ./%2/dpi_private/%2.cpp
	sed "s/HDL/%2/" -i ./%2/.project
	sed "s/HDL/%2 project/" -i ./%2/.inception
) else (
	echo *E: Project type does not exist : "%1"
	goto SHOW_USAGE
)
endlocal
echo Done.

:EXIT
