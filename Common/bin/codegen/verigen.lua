local	sProfilePath	= String(nil)

sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

if lfs.attributes(".verigen", "mode") == "file" then
	RunScript(".verigen")
else
	local	Arg				= ArgTable("Verilog Generator for TestDrive Profiling Master. v1.02")
	
	Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
	Arg:AddOptionFile	("out_path", "./output", nil, nil, "output_path", "output path")
	Arg:AddRemark(nil, "default : ./output")

	if (Arg:DoParse() == false) then
		return
	end

	sInFilename				= Arg:GetOptionFile("in_file", 0)
	sOutPath				= Arg:GetOptionFile("out_path", 0)
end

-- create output directory
if lfs.attributes(sOutPath, "mode") ~= "directory" then
	lfs.mkdir(sOutPath)
	if lfs.attributes(sOutPath, "mode") ~= "directory" then
		error("Output path can't access : '" .. sOutPath .. "'")
	end
end

if lfs.attributes(sOutPath .. "/html", "mode") ~= "directory" then
	lfs.mkdir(sOutPath .. "/html")
	if lfs.attributes(sOutPath .. "/html", "mode") ~= "directory" then
		error("Output html path can't access : '" .. sOutPath .. "/html'")
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

__vfunctions	= {}

function vfunction(name, func)
	if type(name) ~= "string" then
		error("vfunction name must be a string type.", 2)
	end

	__vfunctions[name]	= func
end

function __build_code(s)
	local	sSrc	= String(s.s)
	local	sOut	= String("")

	-- lua vfunction call
	for name, func in pairs(__vfunctions) do
		local	sExp	= "$" .. name .. "(*)"
		
		sOut:Append(sSrc:TokenizeVariable(sExp).s)
		local	sVal	= sSrc:GetVariable()
		
		while sVal:Length() ~= 0 do
			if type(func) == "string" then
				local sResult	= load("return " .. func .."(" .. sVal.s .. ")")()
				if type(sResult) == "string" or type(sResult) == "number" then
					sOut:Append(tostring(sResult))
				end
				local sExtra	= sSrc:TokenizeVariable(sExp)
				sExtra:TrimRight(" \r\n")
				sOut:Append(sExtra.s)
			else
				local sResult	= load("return " .. "__vfunctions[\"" .. name .. "\"](" .. sVal.s .. ")")()
				if type(sResult) == "string" or type(sResult) == "number" then
					sOut:Append(tostring(sResult))
				end
				sOut:Append(sSrc:TokenizeVariable(sExp).s)
			end
			sVal	= sSrc:GetVariable()
		end
		
		sSrc.s = sOut.s
		sOut:clear()
	end

	-- global lua function call
	do
		sOut:Append(sSrc:TokenizeVariable("$(*)").s)
		local	sVal	= sSrc:GetVariable()
		
		while sVal:Length() ~= 0 do
			local sResult	= load("return (" .. sVal.s .. ")")()
			if type(sResult) == "string" or type(sResult) == "number" then
				sOut:Append(tostring(sResult))
			end
			sOut:Append(sSrc:TokenizeVariable("$(*)").s)
			sVal	= sSrc:GetVariable()
		end
	end
	
	-- inline lua code
	sSrc.s = sOut.s
	sOut:clear()
	do
		sOut:Append(sSrc:TokenizeVariable("${*}").s)
		local	sVal	= sSrc:GetVariable()
		
		while sVal:Length() ~= 0 do
			load(sVal.s)()
			sOut:Append(sSrc:TokenizeVariable("${*}").s)
			sVal	= sSrc:GetVariable()
		end
	end

	sOut:TrimLeft(" \t\r\n")
	sOut:Replace("\r", "", true)	-- linux style only

	s.s	= sOut.s
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
