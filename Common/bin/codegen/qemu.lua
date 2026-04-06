local Arg = ArgTable("QEMU manager")

Arg:AddOptionString		("cmd", nil, nil, nil, "command", "QEMU command")
Arg:AddRemark			(nil, "install : reinstall QEMU")
Arg:AddRemark			(nil, "run     : run QEMU")


if (Arg:DoParse() == false) then
	return
end

local cmd			= Arg:GetOptionString("cmd")
local opt			= {}

local testdrive_path = String()
testdrive_path:GetEnvironment("TESTDRIVE_DIR")
testdrive_path = testdrive_path.s

local profile_path = String()
profile_path:GetEnvironment("TESTDRIVE_PROFILE")
profile_path = profile_path.s .. "Common/bin/"

-- check EDK2 tool
if (lfs.IsExist(profile_path .. "qemu-system-x86_64.exe") == false) or (cmd == "install") then
	LOGI("Installing QEMU for TestDrive...\n")
	if lfs.IsExist("qemu_src") == false then
		run("git clone https://github.com/testdrive-profiling-master/qemu_testdrive.git qemu_testdrive")
	end
	-- configure
	if lfs.IsExist("qemu_testdrive/.qemu/build/Makefile") == false then
		run("cd qemu_testdrive&&run_as_admin .build_qemu.bat")
	end
	-- build & install
	local num_processor = String()
	num_processor:GetEnvironment("NUMBER_OF_PROCESSORS")

	run("cd qemu_testdrive&&run_as_admin .build_qemu.bat -j " .. num_processor.s .. " install")
	LOGI("Clean up devel. misc...")
	exec("rm -rf qemu_testdrive")
	LOGI("Done!")
end

