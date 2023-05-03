interface				= {}
interface.__index		= interface
interface.__list		= {}
interface.__active		= false

-- 인터페이스 찾기
function interface.find(name)
	return load("return interface.__list." .. name)()
end

function interface.find_port(name)
	for i_name, i in pairs(interface.__list) do
		if i.__port_name == name then
			return i
		end
	end
	
	return nil
end

-- 인터페이스 유효성 확인
function interface.is_valid(inst)
	return __meta_is_valid(inst, interface)
end

-- 인터페이스 생성
function interface:new(name)
	-- name validation
	if type(name) ~= "string" then
		__ERROR("Invalid interface name.")
	end

	-- create instance
	local	t		= setmetatable({}, self)
	t.__index		= t

	-- interface duplication check
	if interface.find(name) ~= nil then
		__ERROR("already existed interface : '" .. name .. "'")
	end
	
	-- add to list
	interface.__list[name] = t

	-- set variables
	t.name			= name
	t.param			= {}
	t.signal		= {}
	t.modport		= {}
	t.__active		= false
	
	-- copy construct
	if self ~= interface then
		for name, param in pairs(self.param) do
			t.param[name]	= param
		end
		for name, signal in pairs(self.signal) do
			t.signal[name]	= {["name"]=name, ["width"] = signal.width}
		end
		for name, modport in pairs(self.modport) do
			t.modport[name]	= modport
		end
	end
	
	return t
end

function interface:get_param(name)
	return load("return interface.__list." .. self.name .. ".param." .. name)()
end

function interface:set_param(name, default_value)
	self.param[name]	= default_value
end

function interface:get_clock()
	return self.__clock
end

function interface:set_clock(clk)
	if clock.is_valid(clk) == false then
		__ERROR("invalid clock setting on interface '" .. self.name .. "'")
	end
	
	self["__clock"]		= clk
end

function interface:set_signal(name, bit_width)
	if bit_width == nil then
		bit_width	= 1
	end
	
	if load("return interface.__list." .. self.name .. ".signal." .. name .. " ~= nil")() then
		self.signal[name].width	= bit_width
	else
		self.signal[name]		= {["name"]=name, ["width"] = bit_width}
	end
end

function interface:get_modport(name)
	return self.modport[name]
end

function interface:set_modport(name, modport)
	self.modport[name]			= {}
	self.modport[name].data		= {}
	self.modport[name].name		= name
	
	for io_name, io_list in key_pairs(modport) do
		if io_name ~= "i" then
			self.modport[name].data.input	= io_list
		elseif io_name ~= "o" then
			self.modport[name].data.output	= io_list
		elseif io_name ~= "io" then
			self.modport[name].data.inout	= io_list
		else
			__ERROR("Interface [" .. self.name .. "]'s modport[" .. name .. "]'s I/O type must be 'i', 'o' or 'io'")
		end
	end
	
end

function interface:full_name()
	local	name	= self.name
	local	parent	= getmetatable(self)
	
	while parent ~= nil do
		if parent.name == nil then
			break
		end
		name	= parent.name .. "_" .. name
		parent	= getmetatable(parent)
	end

	return name
end

-- 인터페이스 코드 생성
function interface.__make_code(f)
	-- build bit width
	for i_name, i in key_pairs(interface.__list) do
		for signal_name, signal in key_pairs(i.signal) do
			if type(signal.width) ~= "number" then
				signal.width	= __retrieve_param(i.param, signal.width)
			end
		end
	end

	f:Put("//---------------------------------------------\n")
	f:Put("// interfaces\n")
	f:Put("//---------------------------------------------\n")
	
	for i_name, i in key_pairs(interface.__list) do
		f:Put("interface i_" .. i_name .. ";\n")
		-- signal list
		for signal_name, signal in key_pairs(i.signal) do
			if signal.width > 0 then
				f:Put("    logic ")
				-- bitwidth
				if signal.width > 1 then
					f:Put(string.format("%-16s", "[" .. tostring(signal.width - 1) .. ":0] "))
				else
					f:Put(string.format("%-16s", ""))
				end
				
				f:Put(signal_name .. ";\n")
			end
		end
		f:Put("endinterface\n\n")
	end
end

-- interface instance
interface_i				= {}
interface_i.__index		= interface_i

-- 인터페이스 유효성 확인
function interface_i.is_valid(inst)
	return __meta_is_valid(inst, interface_i)
end

function interface_i:new(name, i, parent)
	-- name validation
	if type(name) ~= "string" then
		__ERROR("Invalid instance of interface name.")
	end

	-- create instance
	local	t	= setmetatable({}, self)
	t.__index	= t
	
	-- set default
	t.name			= name
	
	if self == interface_i then
		if module.is_valid(parent) == false then
			__ERROR("Not a module.")
		end
	
		t.parent		= parent
		t.interface		= i
		t.lookup		= nil
		t.prefix		= ""
		t.desc			= nil
		t.modport		= nil
	end

	return t
end

function interface_i:set_desc(desc)
	self.desc			= desc
end

function interface_i:set_port(modport_name, prefix)
	if modport_name ~= nil then
		if self.interface:get_modport(modport_name) == nil then
			__ERROR("Can't find modport '" .. modport_name .. "' on interface '" .. self.interface.name .. "'")
		end
	end

	self.modport	= self.interface:get_modport(modport_name)
	
	if prefix ~= nil then
		self.prefix	= prefix .. "_"
	end
end
