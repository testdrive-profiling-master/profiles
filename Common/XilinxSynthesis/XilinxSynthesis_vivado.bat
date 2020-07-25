@echo off
setlocal

rem xilinx 환경 변수 설정
call %1

rem synthesis 실행
cd .XilinxSynthesis.work

if exist synthesis_%3.txt del synthesis_%3.txt
if exist synthesis_%3.rpt del synthesis_%3.rpt
if exist timing_%3.txt del timing_%3.txt
if exist timing_setup_%3.rpt del timing_setup_%3.rpt
if exist timing_hold_%3.rpt del timing_hold_%3.rpt
if exist timing_clock_%3.rpt del timing_clock_%3.rpt
if exist timing_port_%3.rpt del timing_port_%3.rpt

if "%2"=="32" (
	vivado -log synthesis_%3.txt -mode batch -notrace -source XilinxSynthesis.tcl
) else (
	vivado -log synthesis_%3.txt -m64 -mode batch -notrace -source XilinxSynthesis.tcl
)

if exist vivado.jou del vivado.jou

endlocal
