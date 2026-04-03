local Arg = ArgTable("EDK2 manager")

Arg:AddOptionString		("cmd", nil, nil, nil, "command", "EDK2 command")
Arg:AddRemark			(nil, "install : reinstall EDK2")
Arg:AddRemark			(nil, "build   : build project")
Arg:AddRemark			(nil, "clean   : clean project")
Arg:AddOptionString		("target_arch", "X64", "a", nil, "TARGETARCH", "Target architecture (default:X64)")
Arg:AddRemark			(nil, "ARCHS is one of list: IA32, X64, AARCH64, RISCV64")
Arg:AddOptionString		("build_target", "RELEASE", "b", nil, "BUILDTARGET", "Build Target (default:RELEASE)")
Arg:AddOptionString		("platform_file", nil, "p", nil, "PLATFORMFILE", "platform file")
Arg:AddOptionString		("module_file", nil, "m", nil, "MODULEFILE", "module file")

if (Arg:DoParse() == false) then
	return
end

local cmd			= Arg:GetOptionString("cmd")
local opt			= {}
opt.target_arch		= Arg:GetOptionString("target_arch")
opt.build_target	= Arg:GetOptionString("build_target")
opt.platform_file	= Arg:GetOptionString("platform_file")
opt.module_file		= Arg:GetOptionString("module_file")

local testdrive_path = String()
testdrive_path:GetEnvironment("TESTDRIVE_DIR")
testdrive_path = testdrive_path.s

local profile_path = String()
profile_path:GetEnvironment("TESTDRIVE_PROFILE")
profile_path = profile_path.s .. "Common/bin/"
local edk_path = profile_path .. "edk2"

do	-- set EDK2 environment
	local mingw_path = testdrive_path .. "bin/msys64/ucrt64/"
	local env = String(mingw_path)
	env:SetEnvironment("BASETOOLS_MINGW_PATH")
	env:SetEnvironment("CYGWIN_HOME")
	env.s = "X64"
	env:SetEnvironment("HOST_ARCH")
	env.s = "python3"
	env:SetEnvironment("PYTHON_COMMAND")
	env.s = "mingw32-"
	env:SetEnvironment("CLANG_HOST_BIN")
	
	env.s = mingw_path .. "bin/"
	env:SetEnvironment("CLANG_BIN")
	env:SetEnvironment("NASM_PREFIX")
	
	env.s = profile_path .. "iasl/"
	env:SetEnvironment("IASL_PREFIX")
	
	env.s = profile_path .. "edk2/BaseTools/Bin/Win64"
	env:SetEnvironment("EDK_TOOLS_BIN")
	env.s = profile_path .. "edk2"
	env:SetEnvironment("PACKAGES_PATH")
	
	env.s = lfs.FullPath(".")
	env:SetEnvironment("WORKSPACE")
	opt.workspace_path = env.s
end

local reinstall = (cmd == "install")

-- check EDK2 tool
if (lfs.IsExist(profile_path .. "edk2/BaseTools/") == false) or reinstall then
	LOGI("Installing EDK2...\n")
	exec("rm -rf \"" .. edk_path .. "\"")
	if run("git clone https://github.com/tianocore/edk2 \"" .. edk_path .. "\"") ~= 0 then
		LOGE("Can't access to internet... Please try again later.")
		os.exit(1)
	end
	exec("cd \"" .. edk_path .. "\"&" .. "git checkout tags/edk2-stable202602")
	run("cd \"" .. edk_path .. "\"&" .. "git submodule update --init")
	run("cd \"" .. edk_path .. "\"&" .. "edksetup.bat Mingw-w64 ForceRebuild")
	LOGI("EDK2 is installed successfully!")
end

-- check IASL tool
if (lfs.IsExist(profile_path .. "iasl/iasl.exe") == false) or reinstall then
	LOGI("Installing Intel IASL...\n")
	exec("rm -rf \"" .. profile_path .. "iasl\"")
	if run("git clone https://github.com/testdrive-profiling-master/iasl.git \"" .. profile_path .. "iasl\"") ~= 0 then
		LOGE("Can't access to internet... Please try again later.")
		os.exit(1)
	end
	LOGI("IASL is installed successfully!")
end

if reinstall then
	os.exit(0)
end

-- find project file if not specified
if #opt.platform_file == 0 then
	local file_list = CreateFileList(".", 0, true, "dsc")
	if file_list:Size() == 0 then
		LOGE("Any project file is not found.")
		os.exit(1)
	end
	opt.platform_file = file_list:Pop().data
end

-- find module file if not specified
if #opt.module_file == 0 then
	local file_list = CreateFileList(".", 0, true, "inf")
	if file_list:Size() == 0 then
		LOGE("Any module file is not found.")
		os.exit(1)
	end
	opt.module_file = file_list:Pop().data
end

-- check project file existance & get build output directory
do
	local f = TextFile()
	if f:Open(opt.platform_file) then
		local contents = String(f:GetAll())
		f:Close()
		contents:CutFront("[Defines]")
		contents:CutFront("OUTPUT_DIRECTORY")
		contents:CutBack("\n", true)
		contents:Trim(" \r\t=")
		opt.output_dir = contents.s
	else
		LOGE("Can't access project file.")
		os.exit(1)
	end
end

-- check module name
do
	local sName = String(opt.module_file)
	sName:CutBack(".")
	sName:CutFront("/", true)
	sName:CutFront("\\", true)
	opt.module_name = sName.s
end

if cmd == "build" then
	local iret = run(edk_path .. "/edksetup.bat Mingw-w64 >nul&build -t CLANGDWARF -a " .. opt.target_arch ..
		" -b " .. opt.build_target .. 
		" -p " .. opt.platform_file ..
		" -m " .. opt.module_file)
	if iret ~= 0 then
		LOGE("Build EFI is failed.")
		os.exit(iret)
	end
	
	-- get base name
	local sEnv = String()
	if sEnv:GetEnvironment("BASE_NAME@Defines@" .. opt.module_file) then
		opt.base_name = sEnv.s
	end
	
	-- copy efi file to bin dir.
	if lfs.IsExist("bin/") == false then
		lfs.mkdir("bin")
	end
	exec("cp -f ./" ..
			opt.output_dir .. "/" .. opt.build_target .. "_CLANGDWARF/" .. opt.target_arch ..
			"/" .. opt.base_name .. ".efi bin/")
	
	-- get vendor/device id
	local sEnv = String()
	if sEnv:GetEnvironment("VENDOR_ID@Defines@" .. opt.module_file) then
		opt.vendor_id = sEnv.s
	end
	if sEnv:GetEnvironment("DEVICE_ID@Defines@" .. opt.module_file) then
		opt.device_id = sEnv.s
	end
	sEnv:GetEnvironment("ROM_COMPRESSION@Defines@" .. opt.module_file)
	opt.rom_compression = (sEnv.s == "TRUE")

	-- make ROM binary file
	if (opt.vendor_id ~= nil) and (opt.device_id ~= nil) then
		iret = run(edk_path .. "/edksetup.bat Mingw-w64 >nul&EfiRom" ..
			" -f " .. opt.vendor_id .. " -i " .. opt.device_id ..
			" -o ./bin/" .. opt.base_name .. ".rom -e" .. (opt.rom_compression and "c" or "") .. " ./bin/" .. opt.base_name .. ".efi")
			
		if iret ~= 0 then
			LOGE("ROM file generation is failed")
			os.exit(iret)
		end
		
		LOGI("ROM file : ./bin/" .. opt.base_name .. ".rom")
	else
		LOGW("ROM file creation was skipped because Vendor/Device ID was not specified.")
	end

	os.exit(iret)
end

if cmd == "clean" then
	local output_dir = String(opt.output_dir)
	output_dir:Trim("/\\")
	output_dir:CutBack("/", true)
	output_dir:CutBack("\\", true)
	exec("rm -rf " .. output_dir.s)
	LOGI("Done!")
	os.exit(0)
end
