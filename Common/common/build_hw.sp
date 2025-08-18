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
	
	LOGI("¢Â HDL to C++ conversion (Verilator : https://www.veripool.org/wiki/verilator)")
	System.Execute(".\\build_hw.bat", "", "%PROJECT%System\\HDL\\", {"Error:", {"fatal:","warning"}, "*E: ", {"*I: ", 0}, {"*W: ","warning"}})

	LOGI("¢Â Simulation HDL module build")
	System.Execute("mingw32-make", "-j " .. iHDLBuildCount, "%PROJECT%System\\SubSystems\\Simulation\\SimHDL\\", {"error:", "] Error", "undefined reference to ", {"Build succeeded.",0}})
end

LOGI("\n¢Â H/W build sequence is completed!")
