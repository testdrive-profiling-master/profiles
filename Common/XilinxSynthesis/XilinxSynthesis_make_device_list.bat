@echo off
setlocal

rem xilinx ȯ�� ���� ����
call %1

vivado -nolog -nojournal -mode batch -source xlpartinfo.tcl -tclargs %2data/parts/installed_devices.txt

endlocal
