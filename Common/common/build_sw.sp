#lua
LOG_CLEAR()
do
	local sProssorCount = String()
	sProssorCount:GetEnvironment("NUMBER_OF_PROCESSORS")
	sProssorCount = sProssorCount.s
	
	if lfs.attributes(System.ProjectPath .. "System\\SubSystems\\Simulation\\SimSystem\\", "mode") == "directory" then
		LOGI("¢Â Simulation Sub-system build")
		System.Execute("mingw32-make", "-j " .. sProssorCount, "%PROJECT%System\\SubSystems\\Simulation\\SimSystem\\", {"error:", "] Error", "undefined reference to "})
	end

	if lfs.attributes(System.ProjectPath .. "System\\SubSystems\\PCIeDriver\\", "mode") == "directory" then
		LOGI("¢Â Driver(PCIe) Sub-system build")
		System.Execute("mingw32-make", "-j " .. sProssorCount, "%PROJECT%System\\SubSystems\\PCIeDriver\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.", 0}})
	end

	LOGI("¢Â Testbench build")
	System.Execute("HierarchicalMake", ". -arg \"-j " .. sProssorCount .. "\"", "%PROJECT%Application\\Example\\Testbench\\", {"*E: ", "error:", "] Error", "undefined reference to ", {"Build succeeded.", 0}})
end

LOGI("\n¢Â S/W build sequence is completed!")
