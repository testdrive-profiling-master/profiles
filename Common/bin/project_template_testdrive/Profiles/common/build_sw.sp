system.clear

system.msg "�� Simulation Sub-system build\n"
system.call	"mingw32-make", "-j 12", "..\\..\\System\\SubSystems\\Simulation\\SimSystem\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "�� Driver(PCIe) Sub-system build\n"
system.call	"mingw32-make", "-j 12", "..\\..\\System\\SubSystems\\PCIeDriver\\", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "�� Testbench build\n"
system.call	"HierarchicalMake", ". -arg \"-j 12\"", "..\\..\\Application\\Example\\Testbench\\", "*E: ", "error:", "] Error", "undefined reference to " "Build succeeded.":0

system.msg "\n�� Debug S/W build sequence is completed!\n"