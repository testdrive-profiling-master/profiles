clock					= {}
clock.__index			= clock
clock.__list			= {}
clock.__constraint		= {}
clock.__desc			= nil
clock.__default			= nil
clock.__rst				= "nRST"	-- default reset
clock.__no_reset		= false
clock.__speed			= 100		-- default clock speed

__clock_edge_enum 			= {}
__clock_edge_enum.high		= true
__clock_edge_enum.low		= false


-- 클럭 찾기
function clock.find(name)
	return load("return clock.__list." .. name)()
end

function clock.get_default()
	return clock.__default
end

-- 클럭 유효성 확인
function clock.is_valid(inst)
	return __meta_is_valid(inst, clock)
end

-- 클럭 생성
function clock:new(name, desc)
	-- name validation
	if type(name) ~= "string" then
		error("Invalid clock name.", 2)
	end
	
	-- create instance
	local	t		= setmetatable({}, self)
	t.__index		= t

	t.name				= name
	t.__active			= false
	
	if desc ~= nil then
		t.__desc		= desc
	end
	
	if clock.__default == nil then
		clock.__default		= t
	end
	
	if self == clock then	-- top clock
		-- clock duplication check
		if clock.find(name) ~= nil then
			error("already existed module : '" .. name .. "'", 2)
		end

		-- add to list
		clock.__list[name]		= t
	end
	
	return t
end

function clock:set_reset(name)
	self.__rst			= name
	self.__no_reset		= (name == nil)
end

function clock:get_reset(name)
	if self.__no_reset then
		return nil
	end

	return self.__rst
end

function clock:set_speed(mhz)
	if mhz <= 0 then
		error("clock speed constraint must be higher than zero.", 2)
	end
	self.__speed		= mhz
end

function clock:set_default()
	clock.__default		= self
end
