@echo off
setlocal

rem xilinx 환경 변수 설정
call %1

vivado -mode batch -source xlpartinfo.tcl -tclargs %2data/parts/installed_devices.txt

if exist vivado.jou del vivado.jou
if exist vivado.log del vivado.log

endlocal
