local Arg = ArgTable("QEMU manager")

Arg:AddOptionString		("cmd", nil, nil, nil, "command", "QEMU command")
Arg:AddRemark			(nil, "install  : install QEMU binaries")
Arg:AddRemark			(nil, "create   : Create new QEMU project")
Arg:AddRemark			(nil, "boot     : run QEMU for testdrive")
Arg:AddRemark			(nil, "refresh  : Refresh QEMU project's hard-disk image")

if (Arg:DoParse() == false) then
	return
end

local sEnvQEMU		= "@QEMU@qemu_testdrive.ini"
local cmd			= Arg:GetOptionString("cmd")
local opt			= {}

local testdrive_path = String()
testdrive_path:GetEnvironment("TESTDRIVE_DIR")
testdrive_path = testdrive_path.s

local profile_path = String()
profile_path:GetEnvironment("TESTDRIVE_PROFILE")
profile_path = profile_path.s .. "Common/bin/"
local sEnvQEMU_Config = "@QEMU@" .. profile_path .. "qemu.ini"	-- QEMU configuration file

-- Prepare QEMU development project
if cmd == "devel" then
	if lfs.IsExist("qemu_testdrive") == false then
		run("git clone https://github.com/testdrive-profiling-master/qemu_testdrive.git qemu_testdrive")
	end
	os.exit(0)
end

function IsNeedToUpdate()
	local bRet	= false
	local sEnv				= String()
	sEnv:GetEnvironment("DATE" .. sEnvQEMU_Config)
	
	local iPrevTimeStamp	= sEnv:IsEmpty() and 0 or tonumber(sEnv.s)
	local iCurTimeStamp		= math.floor(os.time() / (60*60*24))
	
	if (iCurTimeStamp - iPrevTimeStamp) >= 30 then	-- check every 1 month
		-- Get 'QEMU for TestDrive' latest commit number
		local sCurCommit	= String()
		sCurCommit.s = exec("git ls-remote https://github.com/testdrive-profiling-master/qemu_testdrive.git HEAD")
		sCurCommit:Trim("\r\n\t ")
		if sCurCommit:CompareBack("HEAD") then
			sCurCommit:CutBack("HEAD")
			sCurCommit:Trim("\t ")
		
			-- commit check
			local sPrevCommit = String()
			sPrevCommit:GetEnvironment("COMMIT" .. sEnvQEMU_Config)
			
			if sPrevCommit.s ~= sCurCommit.s then
				LOGI("New QEMU for TestDrive is released!")
				sCurCommit:SetEnvironment("COMMIT" .. sEnvQEMU_Config)
				bRet = true
			end
			sEnv.s = tostring(iCurTimeStamp)
			sEnv:SetEnvironment("DATE" .. sEnvQEMU_Config)
		end
	end
	
	return bRet
end

-- check QEMU for TestDrive tool
if (lfs.IsExist(profile_path .. "qemu-system-x86_64.exe") == false) or IsNeedToUpdate() or (cmd == "install") then
	if lfs.IsExist(profile_path .. "qemu-system-x86_64.exe") ~= false then
		LOGI("There is already an installed QEMU.")
		LOGI("@1    Do you want to continue? (y/n) : ")
		local answer = io.read():lower()
		if (answer ~= "y") and (answer ~= "yes") then
			os.exit(1)
		end
	else -- 'install' command
		-- install required libraries, but not original qemu
		os.require("mingw-w64-ucrt-x86_64-qemu mingw-w64-ucrt-x86_64-gtk-vnc mingw-w64-ucrt-x86_64-spice-gtk mingw-w64-ucrt-x86_64-virt-viewer")
		exec("pacman -R --noconfirm mingw-w64-ucrt-x86_64-qemu")
	end
	LOGI("Installing QEMU for TestDrive...\n")
	if lfs.IsExist("qemu_testdrive") == false then
		run("git clone https://github.com/testdrive-profiling-master/qemu_testdrive.git qemu_testdrive")
	end
	-- configure
	if lfs.IsExist("qemu_testdrive/.qemu/build/Makefile") == false then
		run("cd qemu_testdrive&&run_as_admin scripts\\build_qemu.bat")
	end
	-- build & install
	local num_processor = String()
	num_processor:GetEnvironment("NUMBER_OF_PROCESSORS")

	run("cd qemu_testdrive&&run_as_admin scripts\\build_qemu.bat -j " .. num_processor.s .. " install")
	LOGI("Clean up devel. misc...")
	exec("rm -rf qemu_testdrive")
	LOGI("Done!")
end

if cmd == "create" then
	if lfs.IsExist("qemu_testdrive.ini") or lfs.IsExist("qemu_testdrive.qcow2") then
		LOGE("Already another QEMU project existed here...")
		os.exit(1)
	end
	
	LOGI("Prepare default QEMU project for TestDrive.")
	exec("cp \"" .. profile_path .. "codegen/qemu/qemu_testdrive_default.ini\" qemu_testdrive.ini")
	exec("echo @qemu boot > qemu_testdrive.bat")
	
	LOGI("For the initial installation,\n" ..
	"    you must download preferred OS installation CD image\n" ..
	"    and specify the 'CDROM_IMAGE' variable from 'qemu_testdrive.ini'.\n")
	LOGI("Now Type 'qemu_testdrive' to start.")
	os.exit(0)
end

function DoRefresh()
	local sImagePath = String()
	local sImageSize = String()
	if sImagePath:GetEnvironment("HARD_DISK_IMAGE" .. sEnvQEMU) and sImageSize:GetEnvironment("HARD_DISK_SIZE" .. sEnvQEMU) then
		if lfs.IsExist(sImagePath.s) then
			-- change to requested size
			run("qemu-img resize " .. sImagePath.s .. " " .. sImageSize.s)
			-- refresh image
			run("qemu-img convert -O qcow2 -p " .. sImagePath.s .. " " .. sImagePath.s .. ".reduced")
			exec("mv -f " .. sImagePath.s .. ".reduced " .. sImagePath.s)
			run("qemu-img info " .. sImagePath.s)
		else
			local sBootFromCDROM = String()
			sBootFromCDROM:GetEnvironment("BOOT_FROM_CDROM" .. sEnvQEMU)
			sBootFromCDROM:MakeLower()
			if (sBootFromCDROM.s ~= "true") and lfs.IsExist("README.md") then
				run("explorer .")
				LOGI("Please read the instruction('README.md') first...")
				os.exit(1)
			end
			
			exec("qemu-img create -f qcow2 " .. sImagePath.s .. " " .. sImageSize.s)
			LOGI("New disk(" .. sImageSize.s .. ") is created.")
		end
		
		local sCountDays = String()
		if sCountDays:GetEnvironment("AUTO_REFRESH_DAYS" .. sEnvQEMU) then
			if tonumber(sCountDays.s) > 0 then
				local latest_refresh_date = String(os.time())
				latest_refresh_date:SetEnvironment("LATEST_REFRESH_TIME" .. sEnvQEMU)
			end
		end
	else
		LOGE("No project information.")
		return false
	end
	return true
end

if cmd == "refresh" then
	os.exit(DoRefresh() and 0 or 1)
end

if cmd == "boot" then
	if lfs.IsExist("qemu_testdrive.ini") == false then
		LOGI("No QEMU project.")
		os.exit(1)
	end
	
	local cmd = String()
	local sEnv = String()
	local bBootForInstall = false
	local sSystem = "x86_64"
	
	if sEnv:GetEnvironment("TITLE" .. sEnvQEMU) then
		sEnv:Replace("\"", "\\\"", true)
		cmd:Append(" -name \"" .. sEnv.s .. "\"")
		LOGI("Boot QEMU(" .. sEnv.s .. ")");
	else
		LOGI("Boot QEMU");
	end
	
	if sEnv:GetEnvironment("SYSTEM" .. sEnvQEMU) then
		sSystem = sEnv.s
	end
	
	if sEnv:GetEnvironment("ACCELERATION" .. sEnvQEMU) then
		cmd:Append(" -accel " .. sEnv.s)
	end
	
	if sEnv:GetEnvironment("SMP_SIZE" .. sEnvQEMU) then
		local iSMP = tonumber(sEnv.s)
		if iSMP > 0 then
			cmd:Append(" -smp " .. iSMP)
		end
	end
	
	if sEnv:GetEnvironment("MEMORY_SIZE" .. sEnvQEMU) then
		cmd:Append(" -m " .. sEnv.s)
	end
	
	if sEnv:GetEnvironment("KERNEL_IMAGE" .. sEnvQEMU) then
		cmd:Append(" -kernel " .. sEnv.s)
	end
	
	if sEnv:GetEnvironment("RAM_DISK_IMAGE" .. sEnvQEMU) then
		cmd:Append(" -initrd " .. sEnv.s)
	end
	
	if sEnv:GetEnvironment("HARD_DISK_IMAGE" .. sEnvQEMU) then
		cmd:Append(" -hda " .. sEnv.s)
		
		if lfs.IsExist(sEnv.s) == false then
			DoRefresh()
		end
		
		if sEnv:GetEnvironment("AUTO_REFRESH_DAYS" .. sEnvQEMU) then
			local refresh_days = tonumber(sEnv.s)
			
			if sEnv:GetEnvironment("LATEST_REFRESH_TIME" .. sEnvQEMU) then
				elapsed_days = (os.time() - tonumber(sEnv.s)) / (24 * 60 * 60)
				
				if elapsed_days > refresh_days then
					DoRefresh()
				end
			else
				-- never booted yet...
				sEnv.s = tostring(os.time())
				sEnv:SetEnvironment("LATEST_REFRESH_TIME" .. sEnvQEMU)
			end
		end
	end
	
	if sEnv:GetEnvironment("BOOT_COMMAND" .. sEnvQEMU) then
		sEnv:Replace("\"", "\\\"", true)
		cmd:Append(" -append \"" .. sEnv.s .. "\"")
	end
	
	if sEnv:GetEnvironment("BOOT_FROM_CDROM" .. sEnvQEMU) then
		sEnv:MakeLower()
		if sEnv.s == "true" then
			bBootForInstall = true
			if sEnv:GetEnvironment("CDROM_IMAGE" .. sEnvQEMU) then
				if lfs.IsExist(sEnv.s) == false then
					LOGE("Can't find CDROM image : " .. sEnv.s)
					os.exit(1)
				end
				cmd:Append(" -cdrom " .. sEnv.s)
			else
				LOGE("You must set the 'CDROM_IMAGE' of qemu_testdrive.ini file.")
				os.exit(1)
			end
			cmd:Append(" -boot d")
		end
		sEnv.s = "false"
		sEnv:SetEnvironment("BOOT_FROM_CDROM" .. sEnvQEMU)
	end
	
	if bBootForInstall == false then
		if sEnv:GetEnvironment("VGA" .. sEnvQEMU) then
			cmd:Append(" -vga " .. sEnv.s)
		end
	end
	
	if sEnv:GetEnvironment("PROJECT") and sEnv:GetEnvironment("SUB_SYSTEM_PATH") then
		cmd:Append(" -device testdrive")
	end
	
	if sEnv:GetEnvironment("DISPLAY" .. sEnvQEMU) then
		cmd:Append(" -display " .. sEnv.s)
	end
	
	if sEnv:GetEnvironment("EXTRA_OPTIONS" .. sEnvQEMU) then
		cmd:Append(" " .. sEnv.s)
	end
	
	run("qemu-system-" .. sSystem .. " " .. cmd.s)
end
