system.clear

system.msg "�� HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)\n"
system.call	".\\build_hw.bat", "", "..\\..\\System\\HDL\\", "Error:", "*E: ", "*I: ":0

system.msg "�� Simulation HDL module build\n"
system.call	"mingw32-make", "", "..\\..\\System\\SubSystems\\Simulation\\SimHDL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n�� H/W build sequence is completed!\n"