call D:\\AMDDesignTools\\2025.2.1\\Vivado\\bin\\vivado.bat -mode batch -source program_spi.tcl
if exist *.jou del /F *.jou
if exist *.log del /F *.log
if exist dfx_runtime.txt del /F dfx_runtime.txt
rm -rf .Xil
pause
