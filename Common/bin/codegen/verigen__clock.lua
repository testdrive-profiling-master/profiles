clock					= {}
clock.__index			= clock
clock.__list			= {}
clock.__constraint		= {}
clock.__edge			= true		-- true(active high), false(active low)
clock.__private_reset	= false
clock.__speed			= 100		-- default clock speed
clock.__enable			= false

active_high			= true
active_low			= false

__clock_edge_enum 			= {}
__clock_edge_enum.high		= true
__clock_edge_enum.low		= true


-- 클럭 찾기
function clock.find(name)
	return load("return clock.__list." .. name)()
end

-- 클럭 유효성 확인
function clock.is_valid(inst)
	return __meta_is_valid(inst, clock)
end

-- 클럭 생성
function clock.new(name, base)
	-- name validation
	if type(name) ~= "string" then
		LOGE("Invalid clock name.")
		os.exit(1)
	end
	
	-- create instance
	local	t = nil
	if base == nil then
		t = setmetatable({}, clock)
	elseif interface.is_valid(base) then
		t = setmetatable({}, base)
	else
		LOGE("Clock[" .. name .. "] creation is failed : invalid base module instance.")
		os.exit(1)
	end
	t.__index		= t

	-- clock duplication check
	if clock.find(name) ~= nil then
		LOGE("already existed module : '" .. name .. "'")
		os.exit(1)
	end
	
	t.name	= name
	
	-- add to list
	clock.__list[name]		= t
	
	return t
end

function clock:set_edge(edge_type)
	__slf_test(self)
	local ret = load("return __clock_edge_enum." .. edge_type)()
	if ret == nil then
		LOGE("clock edge type must be 'high' or 'low'")
		os.exit(1)
	end
	self.__edge	= ret
end

function clock:constraint_speed(mhz)
	__slf_test(self)
	if mhz <= 0 then
		LOGE("clock speed constraint must be higher than zero.")
		os.exit(1)
	end
	self.__speed		= mhz
end
