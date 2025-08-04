system.clear

system.msg "¢Â Simulation Sub-system build\n"
system.call	"mingw32-make", "-j 12", "%PROJECT%System\\SubSystems\\Simulation\\SimSystem\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Driver(PCIe) Sub-system build\n"
system.call	"mingw32-make", "-j 12", "%PROJECT%System\\SubSystems\\PCIeDriver\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "¢Â Testbench build\n"
system.call	"HierarchicalMake", ". -arg \"-j 12\"", "%PROJECT%Application\\Example\\Testbench\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n¢Â S/W build sequence is completed!\n"