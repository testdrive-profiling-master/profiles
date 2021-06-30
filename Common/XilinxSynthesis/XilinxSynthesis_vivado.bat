@echo off
setlocal

rem setup xilinx environment variables
call %1

rem run synthesis
cd .XilinxSynthesis.work

if exist synthesis_%2.txt del synthesis_%2.txt
if exist synthesis_%2.rpt del synthesis_%2.rpt
if exist timing_%2.txt del timing_%2.txt
if exist timing_setup_%2.rpt del timing_setup_%2.rpt
if exist timing_hold_%2.rpt del timing_hold_%2.rpt
if exist timing_clock_%2.rpt del timing_clock_%2.rpt
if exist timing_port_%2.rpt del timing_port_%2.rpt

vivado -log synthesis_%2.txt -mode batch -notrace -source XilinxSynthesis.tcl

if exist vivado*.jou del /Q vivado*.jou

endlocal
