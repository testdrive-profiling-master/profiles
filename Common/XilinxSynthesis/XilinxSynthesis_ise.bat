@echo off
setlocal

rem xilinx ȯ�� ���� ����
call %1

rem synthesis ����
cd .XilinxSynthesis.work

if "%2"=="1" goto ISE_REBUILD_PROJECT

if exist Project.xise (
	goto ISE_RUN_PROCESS
) else (
	goto ISE_REBUILD_PROJECT
)

:ISE_REBUILD_PROJECT
xtclsh "%TESTDRIVE_PROFILE%common/XilinxSynthesis/XilinxSynthesis.tcl" rebuild_project
goto EXIT

:ISE_RUN_PROCESS
xtclsh "%TESTDRIVE_PROFILE%common/XilinxSynthesis/XilinxSynthesis.tcl" run_process
goto EXIT

:EXIT
endlocal
