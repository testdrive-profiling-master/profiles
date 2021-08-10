system.clear

system.msg "¢Â Simulation Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Simulation\\SimSystem\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Emulation Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\Emulation\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Driver(PCIe) Sub-system build\n"
system.call	"mingw32-make", "-j 8", "..\\..\\System\\SubSystems\\PCIeDriver\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Testbench build\n"
system.call	"HierarchicalMake", ". -arg \"-j 8\"", "..\\..\\Application\\Example\\Testbench\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n¢Â Debug S/W build sequence is completed!\n"