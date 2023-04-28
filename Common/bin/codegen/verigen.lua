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

function __ERROR(msg)
	LOGE(msg)
	os.exit(1)
end

-- 출력 폴더 생성
if lfs.attributes(sOutPath, "mode") ~= "directory" then
	lfs.mkdir(sOutPath)
	if lfs.attributes(sOutPath, "mode") ~= "directory" then
		__ERROR("Output path can't access : '" .. sOutPath .. "'")
	end
end

local function __collect_keys(t, sort)
	local _k = {}
	for k in pairs(t) do
		_k[#_k+1] = k
	end
	table.sort(_k, sort)
	return _k
end

function key_pairs(t)
	local keys = __collect_keys(t, function(a,b) return a < b end)
	local i = 0
	return function()
		i = i+1
		if keys[i] then
			return keys[i], t[keys[i]]
		end
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

function __retrieve_param(param_list, t)
	local	s	= String(t)
	
	for name, val in pairs(param_list) do
		s:ReplaceVariable(name, tostring(val))
	end
	
	return load("return (" .. s.s .. ")")()
end

-- default script libraries
RunScript("verigen__clock.lua")
RunScript("verigen__interface.lua")
RunScript("verigen__module.lua")

-- 사용자 소스 실행
if RunScript(sInFilename) == false then
	return
end

-- defines 생성
do
	local	f = TextFile()
	if f:Create(sOutPath .. "/" .. module.name .. "_defines.vh") == false then
		__ERROR("Can't create configuration file.")
	end

	f:Put(	"`ifndef __" .. module.name:upper() .. "_DEFINES_VH__\n"..
			"`define __" .. module.name:upper() .. "_DEFINES_VH__\n"..
			"`include \"testdrive_system.vh\"		// default system defines\n"..
			"`include \"" .. module.name .. "_config.vh\"		// current system configurations\n\n")
	interface.__make_code(f)
	f:Put(	"`endif//__" .. module.name:upper() .. "_DEFINES_VH__\n")
end

-- design 생성
module.build_all()

-- constraint 파일 생성
RunScript("verigen__constraint.lua")
