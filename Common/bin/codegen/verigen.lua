RunScript("codegen_utils")

local	sProfilePath	= String(nil)

sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")

if lfs.attributes(".verigen", "mode") == "file" then
	RunScript(".verigen")
else
	local	Arg				= ArgTable("Verilog Generator for TestDrive Profiling Master. v1.05")
	
	Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
	Arg:AddOptionFile	("out_path", "./output", nil, nil, "output_path", "output path")
	Arg:AddRemark(nil, "default : ./output")

	if (Arg:DoParse() == false) then
		return
	end

	sInFilename				= Arg:GetOptionFile("in_file", 0)
	sOutPath				= Arg:GetOptionFile("out_path", 0)
end

verigen				= {}
verigen.lua_files		= {}	-- hook additional user lua script

verigen.graphviz					= {}
verigen.graphviz.max_edge_count	= 8		-- default maximum edge show on same module

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
	local keys = __collect_keys(t, __key_compare)
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
local __vfunction_reserved = {
	["display"] = 1,
	["write"] = 1,
	["strobe"] = 1,
	["monitor"] = 1,
	["monitoron"] = 1,
	["monitoroff"] = 1,
	["displayb"] = 1,
	["writeb"] = 1,
	["strobeb"] = 1,
	["monitorb"] = 1,
	["displayo"] = 1,
	["writeo"] = 1,
	["strobeo"] = 1,
	["monitoro"] = 1,
	["displayh"] = 1,
	["writeh"] = 1,
	["strobeh"] = 1,
	["monitorh"] = 1,
	["fopen"] = 1,
	["fclose"] = 1,
	["frewind"] = 1,
	["fflush"] = 1,
	["fseek"] = 1,
	["ftell"] = 1,
	["fdisplay"] = 1,
	["fwrite"] = 1,
	["swrite"] = 1,
	["fstrobe"] = 1,
	["fmonitor"] = 1,
	["fread"] = 1,
	["fscanf"] = 1,
	["fdisplayb"] = 1,
	["fwriteb"] = 1,
	["swriteb"] = 1,
	["fstrobeb"] = 1,
	["fmonitorb"] = 1,
	["fdisplayo"] = 1,
	["fwriteo"] = 1,
	["swriteo"] = 1,
	["fstrobeo"] = 1,
	["fmonitoro"] = 1,
	["fdisplayh"] = 1,
	["fwriteh"] = 1,
	["swriteh"] = 1,
	["fstrobeh"] = 1,
	["fmonitorh"] = 1,
	["sscanf"] = 1,
	["sdf_annotate"] = 1
}

function vfunction(name, func)
	if type(name) ~= "string" then
		error("vfunction name must be a string type.", 2)
	end
	
	if __vfunction_reserved[name] ~= nil then
		error("vfunction name[" .. name .. "] is reserved by verilog system call.", 2)
	end
	
	if __vfunctions[name] ~= nil then
		error("vfunction name[" .. name .. "] is already defined.", 2)
	end

	__vfunctions[name]	= func
end

function __build_code(s)
	local 	org_size	= #s.s
	local	sSrc		= String(s.s)
	local	sOut		= String("")

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

	if #sSrc.s ~= org_size then		-- seperate from additional codes
		sOut:Append("\n")
	end

	s:erase(0, org_size)
	s:insert(0, sOut.s)
end

-- default script libraries
RunScript("verigen__clock.lua")
RunScript("verigen__interface.lua")
RunScript("verigen__module.lua")
RunScript("verigen__make.lua")

-- hooking RunScript
local hook		= require("hook")

function __RunScript(filename)
	for i, v in ipairs(verigen.lua_files) do	-- delete already existed file name = push back to end reference
		if v.filename == filename then
			v.filename	= nil
		end
	end
	
	local id = #verigen.lua_files + 1
	
	verigen.lua_files[id]				= {}
	verigen.lua_files[id].filename	= filename
	verigen.lua_files[id].desc		= nil
end

RunScript = hook.add(RunScript, __RunScript)

verigen.set_description	= function(desc)
	for i, v in ipairs(verigen.lua_files) do	-- delete already existed file name = push back to end reference
		if v.filename ~= nil and v.filename == codegen.current_file then
			v.desc	= desc
		end
	end
end

verigen.add_reference = function(filename, desc)
	local id = #verigen.lua_files + 1
	
	verigen.lua_files[id]	= {}
	verigen.lua_files[id].filename	= filename
	verigen.lua_files[id].desc		= desc
end

verigen.add_verilog	= function(filename)
	if filename == nil then
		LOGE("must specify the filename of verigen_add_verilog()")
		os.exit(1)
	end

	filename	= String(filename)
	filename:Replace("\\", "/", true)

	local	path	= String(filename.s)
	local	front	= ""
	local	back	= String(filename.s)
	
	if filename:find("*", 0) >= 0 then

		path:CutBack("*")
		back:CutFront("*")
		
		if path:Length() == 0 then
			path.s	= "."
		else
			if path:CompareBack("/") then
				path:DeleteBack("/")
			else
				front	= String(path.s)
				front:CutFront("/", true)
				front:MakeLower()
				front	= front.s
				path:CutBack("/", false)
			end
		end
		
		back:MakeLower()
	else
		module.apply_code(entry)
		return
	end
	
	path	= path.s
	back	= back.s

	for entry in lfs.dir(path) do
		local	s	= String(entry)
		
		if s:CompareFront(".") == false then
			s:MakeLower()
			
			if (#back ~= 0) and s:CompareBack(back) == false then
				goto exit_for
			end
			
			if (#front ~= 0) and s:CompareFront(front) == false then
				goto exit_for
			end
			
			module.apply_code(path .. "/" .. entry)
		end
		
		::exit_for::
	end
end

function verigen_set_max_showlink(n)
	if type(n) == "number" then
		verigen.graphviz.max_edge_count	= n
	end
end

-- 사용자 소스 실행
if RunScript(sInFilename) == false then
	return
end
