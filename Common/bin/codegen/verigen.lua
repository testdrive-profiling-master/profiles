local	Arg				= ArgTable("Verilog Generator for TestDrive Profiling Master. v1.00")
local	sProfilePath	= String(nil)

sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
Arg:AddOptionFile	("out_path", "./output", nil, nil, "output_path", "output path")
Arg:AddRemark(nil, "default : ./output")

if (Arg:DoParse() == false) then
	return
end

sInFilename				= Arg:GetOptionFile("in_file", 0)
sOutPath				= Arg:GetOptionFile("out_path", 0)

-- ��� ���� ����
if lfs.attributes(sOutPath, "mode") ~= "directory" then
	lfs.mkdir(sOutPath)
	if lfs.attributes(sOutPath, "mode") ~= "directory" then
		LOGE("Output path can't access : '" .. sOutPath .. "'")
		os.exit(1)
	end
end

-- �߸��� member ��� �׽�Ʈ
function __slf_test(s)
	if s == nil then
		LOGE("invalid use.")
		os.exit(1)
	end
end

function __meta_is_valid(inst, base)
	local meta = getmetatable(inst)
	while meta ~= nil do
		if meta == base then
			return true
		end
		meta	= getmetatable(meta)
	end
	return false
end

-- default script libraries
RunScript("verigen__clock.lua")
RunScript("verigen__interface.lua")
RunScript("verigen__module.lua")

-- ����� �ҽ� ����
if RunScript(sInFilename) == false then
	return
end

-- top design ���� ���� �Ǵ�
do
	if module.__top == nil then
		LOGE("Top design name is not declared.")
		os.exit(1)
	end

	LOGI("Top design : " .. module.__top.name)
end

-- design ����
module.build_all()

-- configuration ����
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.__top.name .. "_config.vh") == false then
		LOGE("Can't create configuration file.")
		os.exit(1)
	end
	f:Put(	"`ifndef __" .. module.__top.name:upper() .. "_CONFIG_VH__\n"..
			"`define __" .. module.__top.name:upper() .. "_CONFIG_VH__\n\n")
			
	f:Put(	"`endif//__" .. module.__top.name:upper() .. "_DEFINES_VH__\n")
end

-- defines ����
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.__top.name .. "_defines.vh") == false then
		LOGE("Can't create configuration file.")
		os.exit(1)
	end

	f:Put(	"`ifndef __" .. module.__top.name:upper() .. "_DEFINES_VH__\n"..
			"`define __" .. module.__top.name:upper() .. "_DEFINES_VH__\n"..
			"`include \"testdrive_system.vh\"		// default system defines\n"..
			"`include \"" .. module.__top.name:upper() .. "_config.vh\"		// current system configurations\n\n")
			
	f:Put(	"`endif//__" .. module.__top.name:upper() .. "_DEFINES_VH__\n")
end

-- constraint ���� ����
RunScript("verigen__constraint.lua")
