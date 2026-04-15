local dev = TestDriveDevice()

local sProjectPath = String()
local sSubSystemPath = String()
sProjectPath:GetEnvironment("PROJECT")
sSubSystemPath:GetEnvironment("SUB_SYSTEM_PATH")

if sProjectPath:IsEmpty() then
	error("Project is not ready! You must run your TestDrive project first.")
end

do	-- check 'simulation' mode
	local sSubSystemName = String()
	sSubSystemName:GetEnvironment("SUB_SYSTEM_NAME")
	if sSubSystemName.s ~= "Simulation" then
		error("Subsystem must be 'Simulation' mode.")
	end
end

-- set work folder to 'Program' output path
lfs.chdir(sProjectPath.s .. "Program")

-- setup BAR# (type, byte_size, 64bit_address, prefetchable, bind_address)
dev:CreateBAR("memory", 1024*64, true, false, 0x20000)				-- BAR #0/1
dev:CreateBAR("memory", 1024*1024*32, true, false, 0x10000000)		-- BAR #2/3
dev:CreateBAR("memory", 1024*1024*256, true, false, 0x80000000)		-- BAR #4/5

-- setup MSI
dev:EnableMSI(1, false)	-- iVectorCount, bMaskPerVector

-- setup module implementation
dev:SetSystemModule(sSubSystemPath.s)
