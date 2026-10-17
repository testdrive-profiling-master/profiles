-- set DPI awareness mode
--    unaware           : 96 DPI fixed(default, you can select this for more performance with small size screen)
--    system_aware      : main monitor's DPI
--    per_monitor_aware : each monitor's DPI auto change
SetDpiAwareness("per_monitor_aware")

local sProjectPath = String()
local sSubSystemPath = String()
sProjectPath:GetEnvironment("PROJECT")
sSubSystemPath:GetEnvironment("SUB_SYSTEM_PATH")

if sProjectPath:IsEmpty() then
	error("The TestDrive device will be activated under the testdrive execution. You should run the TestDrive project first.")
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

-- Get testdrive device
local dev = TestDriveDevice()

-- setup module implementation
if dev:LoadSystemModule(sSubSystemPath.s) == false then
	os.exit(1)
end
-------------------------------------------------------------------
--                       PCIe Device setup                       --
-------------------------------------------------------------------
-- Device full description
LOGI("Device name : " .. dev:GetDescription())

-- setup BAR (type, byte_size, bind_address, 64bit_address, prefetchable)
--            type      : "memory", "io"
--            byte_size : 2^N size
dev:CreateBAR("memory", 1024*64, 0x20000, true)									-- BAR #0/1 : registers
dev:CreateBAR("memory", 1024*1024*32, 0x10000000, true)							-- BAR #2/3 : just sample
dev:CreateBAR("memory", dev:GetMemorySize(), dev:GetMemoryBase(), true, true)	-- BAR #4/5 : zero copy memory area

-- setup MSI
dev:EnableMSI(4, false)	-- iVectorCount, bMaskPerVector
--dev:EnableDisplay(640, 480)	-- for VGA device
