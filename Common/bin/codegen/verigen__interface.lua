interface			= {}
interface.__index	= interface
interface.__list	= {}
interface.__clock	= nil
interface.__enable	= false

-- 인터페이스 찾기
function interface.find(name)
	return load("return interface.__list." .. name)()
end

-- 인터페이스 유효성 확인
function interface.is_valid(inst)
	return __meta_is_valid(inst, interface)
end

-- 인터페이스 생성
function interface.new(name, base)
	-- name validation
	if type(name) ~= "string" then
		LOGE("Invalid interface name.")
		os.exit(1)
	end

	-- create instance
	local	t = nil
	if base == nil then
		t = setmetatable({}, interface)
	elseif interface.is_valid(base) then
		t = setmetatable({}, base)
	else
		LOGE("Interface[" .. name .. "] creation is failed : invalid base module instance.")
		os.exit(1)
	end
	t.__index		= t

	-- interface duplication check
	if interface.find(name) ~= nil then
		LOGE("already existed interface : '" .. name .. "'")
		os.exit(1)
	end
	
	-- add to list
	interface.__list[name] = t

	-- set name
	t.name			= name
	t.param			= {}
	
	return t
end

function interface:parameter(name, default_value)
	__slf_test(self)
	if load("return interface.__list." .. self.name .. ".param." .. name)() ~= nil then
		LOGE("Interfrace[" .. self.name .. "] : given parameter[" .. name .. "] is already existed.")
	end
	
	--load("interface.__list." .. self.name .. ".param." .. name .. "=" .. default_value)()
	load("interface.__list." .. self.name .. ".param." .. name .. "=" .. default_value)()
end

function interface:set_clock(clk)
	__slf_test(self)
	
	if clock.is_valid(clk) == false then
		LOGE("invalid clock setting on interface '" .. self.name .. "'")
		os.exit(1)
	end
	
	self.__clock	= clk
end