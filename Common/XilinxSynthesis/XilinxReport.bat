@echo off
setlocal

rem xilinx ȯ�� ���� ����
if %PROCESSOR_ARCHITECTURE%==x86 (
	call %1\settings32.bat
) else (
	call %1\settings64.bat
)

start xreport %2

endlocal
