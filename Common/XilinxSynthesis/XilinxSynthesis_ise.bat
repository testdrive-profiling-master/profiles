@echo off
setlocal

rem xilinx 환경 변수 설정
call %1

rem synthesis 실행
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
