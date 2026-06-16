if exist system_design.mcs del /F system_design.mcs
if exist system_design.prm del /F system_design.prm
call D:\\AMDDesignTools\\2025.2.1\\Vivado\\bin\\vivado.bat -mode batch -source make_spi_mcs.tcl
if exist *.jou del /F *.jou
if exist *.log del /F *.log
if exist dfx_runtime.txt del /F dfx_runtime.txt
pause
