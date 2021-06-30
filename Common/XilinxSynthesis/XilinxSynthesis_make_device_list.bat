@echo off
setlocal

rem xilinx 환경 변수 설정
call %1

vivado -nolog -nojournal -mode batch -source xlpartinfo.tcl -tclargs %2data/parts/installed_devices.txt

endlocal
