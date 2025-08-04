system.clear

system.msg "�� HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)\n"
system.call	".\\build_hw.bat", "", "%PROJECT%System\\HDL\\", "Error:"

system.msg "�� Simulation HDL module build\n"
system.call	"mingw32-make", "-j 4", "%PROJECT%System\\SubSystems\\Simulation\\SimHDL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "�� Simulation Sub-system build\n"
system.call	"mingw32-make", "-j 12", "%PROJECT%System\\SubSystems\\Simulation\\SimSystem\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "�� Driver(PCIe) Sub-system build\n"
system.call	"mingw32-make", "-j 12", "%PROJECT%System\\SubSystems\\PCIeDriver\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "�� Testbench build\n"
system.call	"HierarchicalMake", ". -arg \"-j 12\"", "%PROJECT%Application\\Example\\Testbench\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n�� Build all sequence is completed!\n"