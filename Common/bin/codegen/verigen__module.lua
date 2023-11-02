module				= {}
module.__index		= module
module.__list		= {}
module.__author		= "testdrive profiling master - verigen"
module.__date		= os.date("%B/%d/%Y %a")
module.__year		= os.date("%Y")
module.__time		= os.date("%X")
module.__title		= "no title"
module.__top		= nil
module.__inception	= ""

__m				= nil

-- find module
function module.find(name)
	return load("return module.__list." .. name)()
end

-- check module validation
function module.is_valid(inst)
	return __meta_is_valid(inst, module)
end

-- create module
function module:new(name)
	-- name validation
	if type(name) ~= "string" then
		error("Invalid module name.", 2)
	end
	
	-- module duplication check
	if module.find(name) ~= nil then
		error("already existed module : '" .. name .. "'", 2)
	end
	
	-- create instance
	local			t = setmetatable({}, self)
	t.__index		= t

	-- add to list
	module.__list[name] = t


	-- set name
	name	= String(name)
	name:Replace(" ", "_", true)
	name:Replace(".", "_", true)
	name:Replace("__", "_", true)
	name:Trim("_ \t")
	t.name			= name.s
	
	-- construction
	t.modules		= {}
	t.params		= {}
	t.clocks		= {}
	t.interfaces	= {}
	t.constraint	= {}
	t.fill_color	= "00000010"
	t.code			= String("")
	t.enable		= true
	
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
		error("Can't find code inception : " .. filename, 2)
	end
end

function module:get_inception(void)
	local code_inception = String(self.__inception)
	code_inception:Replace("__YEAR__", self.__year)
	code_inception:Replace("__TITLE__", self.__title)
	code_inception:Replace("__DATE__", self.__date)
	code_inception:Replace("__TIME__", self.__time)
	code_inception:Replace("__AUTHOR__", self.__author)
	code_inception:Replace("__CHECK__", "", true)			-- for global code inception format
	return code_inception.s
end

function module:get_interface(name)
	return self.interfaces[name]
end

function module:get_port(name)
	local	i = get_interface(name)
	return (i.modport == nil) and nil or i
end

function module:add_clock(clk)
	if clock.is_valid(clk) == false then
		error("Not a valid clock", 2)
	end
	
	if self.clocks[clk.name] == nil then
		self.clocks[clk.name]	= clk
	end
end

function module:add_interface(i, name, modport)
	if i == nil then
		error("NULLed interface.", 2)
	end
	
	if name == nil then
		name	= i.name
	end

	if interface.is_valid(i) == false then
		error("Not a interface of '" .. name .. "'", 2)
	end
	
	if self:get_interface(name) ~= nil then
		error("Already interface instance '" .. name .. "' is existed.", 2)
	end
	
	if is_port == nil then
		is_port	= false
	end
	
	local	t				= interface_i:new(name, i, self)
	self.interfaces[name]	= t
	
	if modport ~= nil then
		t:set_port(modport)
	end
	
	return t
end

function module:get_param(name, is_local)
	local param	= self.params[name]
	
	if is_local ~= nil then
		if param.is_local ~= is_local then
			return nil
		end
	end
	
	return param
end

function module:set_param(name, value, is_local)
	if self.params[name] ~= nil then
		error("module[" .. self.name .. "] : given parameter[" .. name .. "] is already existed.", 2)
	end
	
	if is_local == nil then
		is_local	= false
	end

	self.params[name] = {["default"] = value, ["is_local"] = is_local}
end

function module:get_module(name, nilAsError)
	if nilAsError == nil or nilAsError == true then
		if self.modules[name] == nil then
			error("Can't find submodule '" .. name .. "' in module '" .. self.name .. "'", 2)
		end
	end

	return self.modules[name]
end

function module:add_module(m, name)
	if m == nil then
		error("Nulled module added to module[" .. self.name .. "]", 2)
	end

	if m == self then
		error("Can't include self module instance : '" .. self.name .. "'", 2)
	end

	if name == nil then
		name	= m.name
		
		local	conflict_module	= self:get_module(name, false)
		if conflict_module ~= nil then
			conflict_module.name	= name .. "_0"
			
			local	i = 1
			
			while self:get_module(name .. "_" .. tostring(i), false) ~= nil do
				i = i + 1
			end
			
			name	= name .. "_" .. tostring(i)
		end
	end

	if module.is_valid(m) == nil then
		error("Not a module instance : '" .. name .. "'", 2)
	end
	
	if self:get_module(name, false) then
		error("already same module[" .. self.name .. "] instance[" .. name .. "] is existed.", 2)
	end

	local	moudule_inst	= module_i:new(name, m, self)
	self.modules[name]		= moudule_inst
	
	return moudule_inst
end

function _V(s, v_start, v_end, v_step)
	local	code	= String(s)
	
	if type(v_start) == "number" then
		if type(v_end) == "number" then
			-- step check
			if type(v_step) ~= "number" then
				if v_start > v_end then
					v_step	= -1
				else
					v_step	= 1
				end
			else
				if v_step == 0 then
					error("step size is zero.", 2)
				end
				if (v_start > v_end and v_step > 0) or (v_start < v_end and v_step < 0) then
					error("invalid step size.", 2)
				end
			end
			
			local	s_start		= code:TokenizeVariable("$(*)").s
			local	s_variable	= code:GetVariable().s
			local	s_end		= code:Tokenize("").s
			
			if #s_variable == 0 then
				s_variable	= s
				s_start		= ""
				s_end		= ""
			end
			
			do
				local s_code = String("")
				for i = v_start, v_end, v_step do
					local s_val = String(s_variable)
					s_val:Replace("#", tostring(i), true)
					s_code:Append(" " .. s_val.s)
				end
				
				s_code:Trim(" ,")
				
				code.s	= s_start .. s_code.s .. s_end
			end
		else
			code:Replace("#", tostring(v_start), true)
		end
	end
	
	return code.s
end
vfunction("_V", _V)

function _PIPE(bit_width, name, source, en, default_value, clk, reset)
	local	code	= String("reg	")
	if type(bit_width) ~= "number" then
		error("bit_width is not a number. : " .. name)
	end
	
	if bit_width ~= 1 then
		code.s	= code.s .. "[" .. (bit_width-1) .. ":0]	"
	end
	
	code.s	= code.s .. name .. ";\n"
	
	code.s	= code.s ..
		"always@(poesdge " .. clk .. ", negedge " .. reset .. ") begin\n" ..
		"	if(!" .. reset .. ") begin\n" ..
		"		" .. name	.. "	<= " .. default_value .. ";\n"
	if en ~= nil then
		code.s	= code.s ..
			"	end else if(" .. en .. ") begin\n"
	else
		code.s	= code.s ..
			"	end else begin\n"
	end
	code.s	= code.s ..
		"		" .. name	.. "	<= " .. source .. ";\n" ..
		"	end\n" ..
		"end\n"
		
end
vfunction("_PIPE", _PIPE)

function module:add_code(s)
	local	code	= String(s)
	
	code:TrimRight(" \t")
	
	if code:CompareBack(";") then
		code:Append("\n")
	end
	self.code:Append(code.s)
end

function module.apply_code(filename)
	local	f = TextFile()
	local	cur_module	= nil
	if f:Open(filename) then
		local	codes		= String("")
		
		function add_code(m, code)
			if m ~= nil then
				code:TrimLeft("\r\n")
				code:TrimRight(" \t\r\n")
				code:Replace("\r", "", true)
				code:Replace("\n\n\n", "\n\n", true)	-- all 3~ empty lines to 2 empty line.
				m:add_code(code.s .. "\n")
			end
			code:clear()
		end
		
		while f:IsEOF() == false do
			local	s	= String(f:Get())
			
			if s == nil then
				break
			end
			
			do	-- '//#' 으로 시작하는 주석 처리는 유지하지 않는다.
				local iPos = s:find("//#", 0)
				if iPos >= 0 then
					s:erase(iPos, -1)
				end
			end
			
			local sTag	= s:Tokenize(" \t\r\n").s
			
			if sTag == "module" then
				s:CutBack("//", true)	-- comment out
				s:Trim(" \t\r\n")
				s:CutFront("module")
				s:Trim(" \t")
				
				if (s:Length() > 0) then
					local	bEnable		= true
					
					add_code(cur_module, codes) -- flush previous codes
					if s:CompareBack(")") and (s:find("(", 0) > 0) then
						local	sOption	= String(s.s)
						
						sOption:CutFront("(")
						sOption:CutBack(")")
						sOption:Trim(" \t")
						
						if sOption:Length() > 0 then
							bEnable	= load("return (" .. sOption.s .. ")")()
							
							if type(bEnable) ~= "boolean" then
								error("Invalid boolean result in code descriptor : " .. s.s)
							end
						end
						
						s:CutBack("(", true)
					end
					
					if bEnable then
						cur_module	= module.find(s.s)
						if cur_module == nil then
							error("module[" .. s.s .. "] is not found.", 2)
						end
					else
						cur_module	= nil
					end
				end
			elseif sTag == "endmodule" then
				add_code(cur_module, codes)
				cur_module	= nii
			else
				codes:Append(s.s)
			end
		end
		
		add_code(cur_module, codes)
		f:Close()
	else
		error("Can't open code file : " .. filename)
	end
end

-- module instance
module_i				= {}
module_i.__index		= module_i

-- check module instance validation
function module_i.is_valid(inst)
	return __meta_is_valid(inst, module_i)
end

function module_i:new(name, m, parent)
	-- name validation
	if type(name) ~= "string" then
		error("Invalid instance of module name.", 2)
	end

	-- create instance
	local	t	= setmetatable({}, self)
	t.__index	= t
	
	-- set default
	t.name			= name
	
	if self == module_i then
		if module.is_valid(parent) == false then
			error("Not a module.", 2)
		end
	
		t.parent		= parent
		t.module		= m
		t.param			= {}
		t.port			= {}
		t.desc			= nil
		t.code			= {}
		t.code.prefix	= ""
		t.code.postfix	= ""
		t.graphviz		= {}
	end

	return t
end

-- verilog inline function
vfunction("set_author", "__m:set_author")
vfunction("set_inception", "__m:set_inception")
vfunction("set_param", "__m:set_param")
vfunction("add_clock", "__m:add_clock")
vfunction("add_interface", "__m:add_interface")

-- module instance
function module_i:get_param(name)
	return self.param[name]
end

function module_i:set_param(name, val)
	self.param[name]	= val
end

function module_i:get_port(name)
	return self.port[name]
end

function module_i:set_port(name, val)
	self.port[name]		= val
end

-- global definitions
__global_defines__	= module:new("GLOBAL_DEFINES")
