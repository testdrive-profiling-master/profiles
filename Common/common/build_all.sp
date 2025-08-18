#lua
LOG_CLEAR()

do
	local sProssorCount = String()
	sProssorCount:GetEnvironment("NUMBER_OF_PROCESSORS")
	sProssorCount = sProssorCount.s
	local iHDLBuildCount = math.tointeger(tonumber(sProssorCount) / 4)
	if iHDLBuildCount < 1 then
		iHDLBuildCount = 1
	elseif iHDLBuildCount > 4 then
		iHDLBuildCount = 4	-- suppressing to low, because hardware C++ sources can be very large in size.
	end
	
	if lfs.attributes(System.ProjectPath .. "System\\HDL\\", "mode") == "directory" then
		LOGI("¢Â HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)")
		System.Execute(".\\build_hw.bat", "", "%PROJECT%System\\HDL\\", {"Error:", {"fatal:","warning"}, "*E: ", {"*I: ", 0}, {"*W: ","warning"}})
	end

	if lfs.attributes(System.ProjectPath .. "System\\SubSystems\\Simulation\\SimHDL\\", "mode") == "directory" then
		LOGI("¢Â Simulation HDL module build")
		System.Execute("mingw32-make", "-j " .. iHDLBuildCount, "%PROJECT%System\\SubSystems\\Simulation\\SimHDL\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
	end

	if lfs.attributes(System.ProjectPath .. "System\\SubSystems\\Simulation\\SimSystem\\", "mode") == "directory" then
		LOGI("¢Â Simulation Sub-system build")
		System.Execute("mingw32-make", "-j " .. sProssorCount, "%PROJECT%System\\SubSystems\\Simulation\\SimSystem\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
	end

	if lfs.attributes(System.ProjectPath .. "System\\SubSystems\\PCIeDriver\\", "mode") == "directory" then
		LOGI("¢Â Driver(PCIe) Sub-system build")
		System.Execute("mingw32-make", "-j " .. sProssorCount, "%PROJECT%System\\SubSystems\\PCIeDriver\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
	end

	LOGI("¢Â Testbench build")
	System.Execute("HierarchicalMake", ". -arg \"-j " .. sProssorCount .. "\"", "%PROJECT%Application\\Example\\Testbench\\", {"*E: ", "error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
end

LOGI("¢Â Build all sequence is completed!")
