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

-- 모델 찾기
function module.find(name)
	return load("return module.__list." .. name)()
end

-- 모듈 유효성 확인
function module.is_valid(inst)
	return __meta_is_valid(inst, module)
end

-- 모델 생성
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
	t.interfaces	= {}
	t.constraint	= {}
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

function module:add_interface(i, name)
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

function module:get_module(name)
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
		
		local	conflict_module	= self:get_module(name)
		if conflict_module ~= nil then
			conflict_module.name	= name .. "_0"
			
			local	i = 1
			
			while self:get_module(name .. "_" .. tostring(i)) ~= nil do
				i = i + 1
			end
			
			name	= name .. "_" .. tostring(i)
		end
	end

	if module.is_valid(m) == nil then
		error("Not a module instance : '" .. name .. "'", 2)
	end
	
	if self:get_module(name) then
		error("already same module[" .. self.name .. "] instance[" .. name .. "] is existed.", 2)
	end

	local	moudule_inst	= module_i:new(name, m, self)
	self.modules[name]		= moudule_inst
	
	return moudule_inst
end

function module:add_code(s, t, v_end, v_step)
	local	code	= String(s)
	
	if type(t) == "number" then
		if type(v_end) == "number" then
			-- step check
			if type(v_step) ~= "number" then
				if t > v_end then
					v_step	= -1
				else
					v_step	= 1
				end
			else
				if v_step == 0 then
					error("step size is zero.", 2)
				end
				if (t > v_end and v_step > 0) or (t < v_end and v_step < 0) then
					error("invalid step size.", 2)
				end
			end
			
			local	s_start		= code:TokenizeVariable("$*$").s
			local	s_variable	= code:GetVariable().s
			local	s_end		= code:Tokenize("").s
			
			if #s_variable == 0 then
				error("Can't find variable text", 2)
			end
			
			do
				local s_code = String("")
				for i = t, v_end, v_step do
					local s_val = String(s_variable)
					s_val:Replace("#", tostring(i), true)
					s_code:Append(" " .. s_val.s)
				end
				
				s_code:Trim(" ,")
				
				code.s	= s_start .. s_code.s .. s_end
			end
		else
			code:Replace("#", tostring(t), true)
		end
	elseif type(t) == "table" then
		for tag, val in pairs(t) do
			code:Replace(tostring(tag), tostring(val), true)
		end
	end
	
	if code:CompareBack(";") then
		code:Append("\n")
	end
	self.code:Append(code.s)
end

-- module instance
module_i				= {}
module_i.__index		= module_i

-- 인터페이스 유효성 확인
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
	end

	return t
end

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