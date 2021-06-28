system.clear

system.msg "¢Â HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)\n"
system.call	".\\build_hw.bat", "", "..\\..\\System\\HDL\\", "Error:"

system.msg "¢Â Simulation HDL module build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Simulation\\SimHDL\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â GTKWave process filter build\n"
system.call	"%TESTDRIVE_DIR%bin\\HierarchicalMake.exe", ". -arg \"-j 8\"", "..\\..\\Program\\process_filter\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n¢Â H/W build sequence is completed!\n"