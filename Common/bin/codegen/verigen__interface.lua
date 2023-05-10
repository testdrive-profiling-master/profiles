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
		error("Invalid interface name.", 2)
	end

	-- create instance
	local	t		= setmetatable({}, self)
	t.__index		= t

	-- interface duplication check
	if interface.find(name) ~= nil then
		error("already existed interface : '" .. name .. "'", 2)
	end
	
	-- add to list
	interface.__list[name] = t

	-- set variables
	t.name			= name
	t.param			= {}
	t.signal		= {}
	t.modport		= {}
	t.prefix		= name:upper() .. "#"
	t.__active		= false
	t.__bared		= false
	
	-- copy construct
	if self ~= interface then
		for name, param in pairs(self.param) do
			t.param[name]	= param
		end
		for name, signal in pairs(self.signal) do
			t.signal[name]	= {["name"]=name, ["width"] = signal.width}
		end
		for mp_name, mp in pairs(self.modport) do
			t.modport[mp_name]			= {}
			t.modport[mp_name].name		= mp_name
			t.modport[mp_name].data		= {}
			for io_type, io_list in pairs(mp.data) do
				local	io_data		= {}
				for i, io_name in pairs(io_list) do
					io_data[#io_data + 1]	= io_name
				end
				t.modport[mp_name].data[io_type]	= io_data
			end
		end
	end
	
	return t
end

function new_signal(name, width)
	local	signal	= interface:new(name)
	
	if width == nil then
		width	= 1
	end

	signal:set_param("WIDTH", width)
	signal:set_signal(name, "WIDTH")
	signal:set_modport("s", {["input" ] = {name}})
	signal:set_modport("m", {["output"] = {name}})
	signal:set_prefix()		-- none prefix
	signal:set_bared()		-- bared signals
	return signal
end

function interface:set_multicycle(modport_name, cycle, inst_count)
	if self:get_clock() == nil then
		error("Multicycle interface[" .. i.name .. "] must have a clock.", 2)
	end
	
	if cycle <= 1 then
		error("Multicycle interface[" .. i.name .. "]'s cycle(required : " .. cycle .. ") must greater than 2.", 2)
	elseif cycle > 12 then
		error("Multicycle interface[" .. i.name .. "]'s cycle(required : " .. cycle .. ") is too high.", 2)
	end
	
	if inst_count == nil then
		inst_count	= cycle
	end
	
	if inst_count < 1 then
		error("Multicycle interface[" .. i.name .. "]'s instance count(required : " .. inst_count .. ") must greater than 1.", 2)
	elseif inst_count > cycle then
		error("Multicycle interface[" .. i.name .. "]'s instance count(required : " .. inst_count .. ") must be less than cycle(" .. cycle .. ").", 2)
	end
	
	if self.__muticycle ~= nil then
		error("Already interface[" .. i.name .. "] is defined multicycled.", 2)
	end
	
	local	modport	= self:get_modport(modport_name)
	
	if modport == nil then
		error("Interface[" .. i.name .. "]'s Multicycle modport(" .. modport_name .. ") is not existed.", 2)
	end
	
	self.set_signal("IE")
	self.set_signal("IREADY")
	self.set_signal("OE")
	self.set_signal("OREADY")
	
	self.__muticycle	= {["modport"] = modport, ["cycle"] = cycle, ["count"] = inst_count}
end

function interface:set_prefix(prefix)
	self.prefix		= (prefix == nil) and "" or prefix
end

function interface:set_bared(bared)
	self.__bared	= (bared == nil) and true or bared
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
		error("invalid clock setting on interface '" .. self.name .. "'", 2)
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
	self.modport[name].data		= modport
	self.modport[name].name		= name
end

function interface:add_modport(name, modport)
	if self.modport[name] == nil then
		error("Can't find mod port[" .. name .. "] on interface[" .. self.name .. "]", 2)
	end
	
	for mp_name, mp_list in pairs(modport) do
		local	io_base	= self.modport[name].data[mp_name]
		
		if io_base == nil then
			error("Can't find modport[" .. mp_name .. "] on interface[" .. self.name .. "]", 2)
		end
		
		for i, signal_name in pairs(mp_list) do
			io_base[#io_base + 1]	= signal_name
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

function interface.__build()
	-- build bit width
	for i_name, i in key_pairs(interface.__list) do
		for signal_name, signal in key_pairs(i.signal) do
			if type(signal.width) ~= "number" then
				signal.width	= __retrieve_param(i.param, signal.width)
				signal.width	= tonumber(signal.width) and math.tointeger(tonumber(signal.width)) or signal.width
			end
		end
	end
end

-- 인터페이스 코드 생성
function interface.__make_code(f)
	f:Put("//---------------------------------------------\n")
	f:Put("// interfaces\n")
	f:Put("//---------------------------------------------\n")
	
	for i_name, i in key_pairs(interface.__list) do
		if i.__active and (i.__bared == false) then
			f:Put("interface i_" .. i_name .. ";\n")
			
			-- signal list
			for signal_name, signal in key_pairs(i.signal) do
				if signal.width > 0 then
					--f:Put("	logic ")
					-- bitwidth
					if signal.width > 1 then
						f:Put(string.format("	logic %-16s" .. signal_name .. ";\n", "[" .. tostring(signal.width - 1) .. ":0] "))
					elseif signal.width == 1 then
						f:Put(string.format("	logic %-16s" .. signal_name .. ";\n", ""))
					end
					
					--f:Put(signal_name .. ";\n")
				end
			end
			
			-- modport list
			for mp_name, mp in key_pairs(i.modport) do
				f:Put("	modport " .. mp_name .. " (\n")
				local sModport	= String("")
				for io_type, io_list in key_pairs(mp.data) do
					sModport:Append("\t\t" .. string.format("%-7s ", io_type))
					for io_num, io_name in ipairs(io_list) do
						sModport:Append(io_name .. ", ")
					end
					sModport:Append("\n")
				end
				
				sModport:DeleteBack(", ")

				f:Put(sModport.s .. "\t);\n")
			end
			
			f:Put("endinterface\n\n")
		end
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
		error("Invalid instance of interface name.", 2)
	end

	-- create instance
	local	t	= setmetatable({}, self)
	t.__index	= t
	
	-- set default
	t.name			= name
	
	if self == interface_i then
		if module.is_valid(parent) == false then
			error("Not a module.", 2)
		end
	
		t.parent		= parent
		t.interface		= i
		t.lookup		= nil
		t.prefix		= nil
		t.desc			= nil
		t.modport		= nil
		t.__bared		= i.__bared
	end

	return t
end

function interface_i:set_desc(desc)
	self.desc			= desc
end

function interface_i:get_prefix()
	local	prefix		= String((self.prefix == nil) and self.interface.prefix or self.prefix)
	
	if prefix:Length() > 0 then
		local	inst_id		= 0
		local	inst_count	= 0
	
		for i_name, i in key_pairs(self.parent.interfaces) do
			if i.modport ~= nil and i.interface == self.interface then
				if i == self then
					inst_id	= inst_count
				end
				inst_count	= inst_count + 1
			end
		end
		
		if inst_count > 1 then
			prefix:Replace("#", tostring(inst_id))
		else
			prefix:Replace("#", "")		-- only one interface port
		end
		
		prefix:Append("_")
	end
	
	return prefix.s
end

function interface_i:set_prefix(prefix)
	self.prefix			= prefix
end

function interface_i:set_port(modport_name)
	if modport_name ~= nil then
		if self.interface:get_modport(modport_name) == nil then
			error("Can't find modport '" .. modport_name .. "' on interface '" .. self.interface.name .. "'", 2)
		end
	end

	self.modport	= self.interface:get_modport(modport_name)
end
