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

-- 출력 폴더 생성
if lfs.attributes(sOutPath, "mode") ~= "directory" then
	lfs.mkdir(sOutPath)
	if lfs.attributes(sOutPath, "mode") ~= "directory" then
		error("Output path can't access : '" .. sOutPath .. "'")
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
	if inst == nil then
		return false
	end

	inst	= getmetatable(inst)
	
	while inst ~= nil do
		if inst == base then
			return true
		end
		inst	= getmetatable(inst)
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
RunScript("verigen__make.lua")

-- 사용자 소스 실행
if RunScript(sInFilename) == false then
	return
end