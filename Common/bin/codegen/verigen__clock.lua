clock					= {}
clock.__index			= clock
clock.__list			= {}
clock.__constraint		= {}
clock.__desc			= nil
clock.__default_rst		= "nRST"
clock.__rst				= nil		-- reset name
clock.__private_reset	= false
clock.__speed			= 100		-- default clock speed

__clock_edge_enum 			= {}
__clock_edge_enum.high		= true
__clock_edge_enum.low		= false


-- 클럭 찾기
function clock.find(name)
	return load("return clock.__list." .. name)()
end

-- 클럭 유효성 확인
function clock.is_valid(inst)
	return __meta_is_valid(inst, clock)
end

-- 클럭 생성
function clock:new(name, desc, base)
	-- name validation
	if type(name) ~= "string" then
		__ERROR("Invalid clock name.")
	end
	
	-- create instance
	local	t = nil
	if base == nil then
		t = setmetatable({}, clock)
	elseif interface.is_valid(base) then
		t = setmetatable({}, base)
	else
		__ERROR("Clock[" .. name .. "] creation is failed : invalid base module instance.")
	end
	t.__index		= t

	-- clock duplication check
	if clock.find(name) ~= nil then
		__ERROR("already existed module : '" .. name .. "'")
	end
	
	t.name				= name
	t.__active			= false
	
	if desc ~= nil then
		t.__desc		= desc
	end
	
	-- add to list
	clock.__list[name]		= t
	
	return t
end

function clock:set_reset(name)
	self.__rst	= name
	
	if name == __default_rst then
		__ERROR("reset '" .. name .. "' is same as global reset.")
	end
end

function clock:set_speed(mhz)
	if mhz <= 0 then
		__ERROR("clock speed constraint must be higher than zero.")
	end
	self.__speed		= mhz
end
