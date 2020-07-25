system.clear

system.msg "¢Â HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)\n"
system.call	".\\build_hw.bat", "", "..\\..\\System\\HDL\\", "Error:"

system.msg "¢Â Simulation HDL module build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Simulation\\SimHDL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Simulation Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Simulation\\SimSystem\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Emulation Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Emulation\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Driver(PCIe) Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\PCIeDriver\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Testbench build\n"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"-j 8\"", "..\\..\\Application\\Example\\Testbench\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n¢Â Build all sequence is completed!\n"