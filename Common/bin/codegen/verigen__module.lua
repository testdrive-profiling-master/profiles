module				= {}
module.__index		= module
module.__list		= {}
module.__port		= {}
module.__author		= "testdirve profiling master - verigen"
module.__date		= os.date("%B/%d/%Y %a")
module.__year		= os.date("%Y")
module.__time		= os.date("%X")
module.__title		= "no title"
module.__current	= module
module.__inception	= ""
module.name			= "top"
module.enable		= false

-- 모델 찾기
function module.find(name)
	return load("return module.__list." .. name)()
end

-- 모듈 유효성 확인
function module.is_valid(inst)
	return __meta_is_valid(inst, module)
end

-- 모델 생성
function module:new(name, base)
	-- name validation
	if type(name) ~= "string" then
		__ERROR("Invalid module name.")
	end
	
	-- create instance
	local	t = nil
	if base == nil then
		t = setmetatable({}, module)
	elseif interface.is_valid(base) then
		t = setmetatable({}, base)
	else
		__ERROR("Module[" .. name .. "] creation is failed : invalid base module instance.")
	end
	
	-- construction 
	t.__port		= {}
	t.__index		= t

	-- module duplication check
	if module.find(name) ~= nil then
		__ERROR("already existed module : '" .. name .. "'")
	end
	
	-- add to list
	module.__list[name] = t


	-- set name
	name	= String(name)
	name:MakeLower()
	name:Replace(" ", "_", true)
	name:Replace(".", "_", true)
	name:Replace("__", "_", true)
	name:Trim("_ \t")
	t.name			= name.s
	
	-- add details
	t.port		= {}
	t.param		= {}
	
	return t
end

function module:set_title(title)
	self.__title		= title
end

function module:set_author(name)
	self.__author		= name
end

function module:set_inception(filename)
	local f = TextFile()
	if f:Open(filename) then
		self.__inception	= String(f:GetAll(false))
		self.__inception:Replace("\r", "", true)
		self.__inception:Trim(" \n")
		self.__inception:Append("\n")
		self.__inception	= self.__inception.s
		f:Close()
	else
		__ERROR("Can't find code inception : " .. filename)
	end
	
end

function module:make_current(m)
	module.__current	= self
end

function module:add_port(name, i, type)
	if interface.is_valid(i) == false then
		__ERROR("Invalid interface : " .. name)
	end
	self.port[name]	= {["i"] = i, ["type"] = type}
end

function module:set_param(name, default_value)
	if load("return module.__list." .. self.name .. ".param." .. name)() ~= nil then
		__ERROR("module[" .. self.name .. "] : given parameter[" .. name .. "] is already existed.")
	end

	load("module.__list." .. self.name .. ".param." .. name .. "=" .. default_value)()
end

function module.build_all()
	local f = TextFile()
	
	LOGI("TOP design name : " .. module.name)
	
	-- create top design file
	if f:Create(sOutPath .. "/" .. module.name .. ".sv") == false then
		__ERROR("Can't create top design file.")
	end
	
	-- code inception
	do
		local code_inception = String(module.__inception)
		code_inception:Replace("__YEAR__", module.__year)
		code_inception:Replace("__TITLE__", module.__title)
		code_inception:Replace("__DATE__", module.__date)
		code_inception:Replace("__TIME__", module.__time)
		code_inception:Replace("__AUTHOR__", module.__author)
		f:Put(code_inception.s)
	end
	
	-------------------------------------------------------------------
	-- common include
	f:Put(	"`include \"" .. module.name .. "_defines.vh\"\n\n")

	-------------------------------------------------------------------
	-- module declaration
	f:Put(	"module " .. module.name .. " (\n")
	do
		-- clocks
		f:Put(	"\t// clocks\n")
		for clk_name, clk in key_pairs(clock.__list) do
			if clk.__desc == nil then
				f:Put("\t" .. string.format("input  %16s%s,", "", clk_name) .. "\n")
			else
				f:Put(string.format("\t%-40s// %s\n", string.format("input  %16s%s,", "", clk_name), clk.__desc))
			end
		end
		-- resets
		f:Put(	"\t// resets\n")
		local	use_default_rst = false
		for clk_name, clk in key_pairs(clock.__list) do
			if clk.__rst ~= nil then
				f:Put(string.format("\t%-40s// reset of '%s' (active low)\n", string.format("input  %16s%s,", "", clk.__rst), clk.name))
			else
				use_default_rst	= true
			end
		end
		if use_default_rst then
			f:Put(string.format("\t%-40s// default global reset (active low)\n", string.format("input  %16s%s,", "", clock.__default_rst)))
		end
		
		-- interface ports
		for i_name, i in key_pairs(interface.__list) do
			if i.__port_type ~= nil then
				local	sCode	= String("\n\t// " .. i.name .. "\n")
				
				local	modport	= i.modport[i.__port_type]
				
				if modport == nil then
					__ERROR("Can't find modport '" .. i.__port_type .. "' on interface '" .. i_name .. "'")
				end
				
				for io_type, io_list in key_pairs(modport) do
					for id, io_name in key_pairs(io_list) do
						local	signal	= i.signal[io_name]
						
						sCode:Append("\t" .. string.format("%-6s ", io_type) ..
							(string.format("%-16s", (signal.width > 1) and string.format("[%d:0]", signal.width - 1) or "")) ..
							i.__port_name .. "_" .. io_name .. ",\n")
					end
				end
				f:Put(sCode.s)
			end
		end
	end
	f:Put(	");\n\n")
	
	-------------------------------------------------------------------
	-- active interface
	for i_name, i in key_pairs(interface.__list) do
		if i.__active then
			f:Put("// " .. i.name .. "\n")
			f:Put("i_" .. i.name .. " \n")
		end
	end

	f:Put(	"endmodule\n")
	f:Close()
end
