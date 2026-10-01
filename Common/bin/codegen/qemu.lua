local Arg = ArgTable("QEMU(https://www.qemu.org) for TestDrive Profiling Master.")

Arg:AddOptionString		("cmd", nil, nil, nil, "command", "QEMU command")
Arg:AddRemark			(nil, "update   : Check for update of QEMU binaries")
Arg:AddRemark			(nil, "install  : Force to re-install QEMU binaries")
Arg:AddRemark			(nil, "create   : Create new QEMU project")
Arg:AddRemark			(nil, "boot     : run QEMU for Testdrive")
Arg:AddRemark			(nil, "refresh  : Try create or reduce&resize disk image")
Arg:AddRemark			(nil, "devel    : Prepare QEMU open-source project")

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

local bCheckUpdate = false
if cmd == "update" then
	bCheckUpdate = true
end

local bReInstall = false
if cmd == "install" then
	bReInstall = true
end

function IsNeedToUpdate()
	local bRet	= bReInstall
	local sEnv	= String()
	sEnv:GetEnvironment("DATE" .. sEnvQEMU_Config)
	
	local iPrevTimeStamp	= sEnv:IsEmpty() and 0 or tonumber(sEnv.s)
	local iCurTimeStamp		= math.floor(os.time() / (60*60*24))
	
	if ((iCurTimeStamp - iPrevTimeStamp) >= 7) or bCheckUpdate then	-- check every week
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
			
			if (#sCurCommit.s == 40) and (sPrevCommit.s ~= sCurCommit.s) then
				LOGI("*1New QEMU for TestDrive is released!")
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
if IsNeedToUpdate() or (lfs.IsExist(profile_path .. "qemu/qemu-system-x86_64.exe") == false) then
	if lfs.IsExist(profile_path .. "qemu/qemu-system-x86_64.exe") ~= false then
		if bReInstall == false then
			LOGI("There is a new update for QEMU. Attempting the re-compilation procedure...")
		end
	else -- 'install' command
		-- install required libraries, but not original qemu
		os.require("mingw-w64-ucrt-x86_64-qemu mingw-w64-ucrt-x86_64-gtk-vnc mingw-w64-ucrt-x86_64-spice-gtk mingw-w64-ucrt-x86_64-virt-viewer")
		exec("pacman -R --noconfirm mingw-w64-ucrt-x86_64-qemu")
	end
	LOGI("Installing QEMU for TestDrive...\n")
	if lfs.IsExist("qemu_testdrive") then
		LOGE("Already 'qemu_testdrive' project folder is existed.")
		os.exit(1)
	else
		run("git clone https://github.com/testdrive-profiling-master/qemu_testdrive.git qemu_testdrive")
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
	do
		local f = TextFile()
		f:Create("qemu_testdrive.bat")
		f:Put("@echo off\ncall qemu boot\necho *I: QEMU is down!\n")
		f:Close()
	end
	
	LOGI("For the initial installation,\n" ..
	"    you must download preferred OS installation CD image\n" ..
	"    and specify the 'CDROM_IMAGE' variable from 'qemu_testdrive.ini'.\n")
	LOGI("Now Type 'qemu_testdrive' to start.")
	os.exit(0)
end

function DoRefresh(bCreate)
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
			if bCreate ~= true then
				LOGE("No installed disks found!")
				if lfs.IsExist("README.md") then
					run("explorer .")
					LOGI("Please read the instruction('README.md') first...")
				end
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
	os.exit(DoRefresh(true) and 0 or 1)
end

if cmd == "boot" then
	if lfs.IsExist("qemu_testdrive.ini") == false then
		LOGI("No QEMU project.")
		os.exit(1)
	end
	
	local cmd = String()
	local sEnv = String()
	local bDiskNotInitialized = false
	local sSystem = "x86_64"	-- default system
	
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
			bDiskNotInitialized = true
		elseif sEnv:GetEnvironment("AUTO_REFRESH_DAYS" .. sEnvQEMU) then
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

	-- check CDROM & first install from CDROM
	local bCDROMSpecified = false
	if sEnv:GetEnvironment("CDROM_IMAGE" .. sEnvQEMU) then
		if sEnv.s == "auto" then
			-- find .iso file list
			local iso_list = CreateFileList(".", -1, true, "iso")
			
			if iso_list:Size() == 1 then
				local sIsoFileName = iso_list:Pop().data
				LOGI("ISO image(" .. sIsoFileName .. ") is found.")
				cmd:Append(" -cdrom " .. sIsoFileName)
				bCDROMSpecified = true
			elseif iso_list:Size() > 1 then
				LOGW("Too many .iso images are existed in project folder. This 'CDROM_IMAGE' option is ignored.")
			end
		elseif lfs.IsExist(sEnv.s) then
			cmd:Append(" -cdrom " .. sEnv.s)
			bCDROMSpecified = true
		else
			LOGE("Can't find CDROM image (" .. sEnv.s .. ")")
			os.exit(1)
		end
	end

	-- check boot from CDROM
	if sEnv:GetEnvironment("BOOT_FROM_CDROM" .. sEnvQEMU) then
		sEnv:MakeLower()
		if sEnv.s == "auto" then
			if bDiskNotInitialized then
				if bCDROMSpecified then
					DoRefresh(true)
					cmd:Append(" -boot d")
				else
					LOGE("No installed disks found!")
					if lfs.IsExist("README.md") then
						run("explorer .")
						LOGW("Please read the instruction('README.md') first...")
					end
					os.exit(1)
				end
			end
		elseif sEnv.s == "true" then
			cmd:Append(" -boot d")
		end
	end
	
	if sEnv:GetEnvironment("VGA" .. sEnvQEMU) then
		cmd:Append(" -vga " .. sEnv.s)
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
	
	os.execute("%TESTDRIVE_PROFILE%Common/bin/qemu/qemu-system-" .. sSystem .. " " .. cmd.s)
end
