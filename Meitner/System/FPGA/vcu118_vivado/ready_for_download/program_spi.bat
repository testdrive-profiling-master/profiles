call D:\\Xilinx\\Vivado\\2024.2\\bin\\vivado.bat -mode batch -source program_spi.tcl
if exist *isWriteableTest*.tmp del /F *isWriteableTest*.tmp
if exist *pid*.* del /F *pid*.*
if exist vivado_*.str del /F vivado_*.str
if exist *isWriteableTest*.tmp del /F *isWriteableTest*.tmp
if exist *.jou del /F *.jou
if exist *.log del /F *.log
rm -rf .Xil
pause
