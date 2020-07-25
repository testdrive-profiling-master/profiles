@echo off
setlocal

rem xilinx 환경 변수 설정
if %PROCESSOR_ARCHITECTURE%==x86 (
	call %1\settings32.bat
) else (
	call %1\settings64.bat
)

start xreport %2

endlocal
