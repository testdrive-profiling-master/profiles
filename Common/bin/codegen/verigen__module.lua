module				= {}
module.__index		= module
module.__list		= {}
module.__author		= "testdirve profiling master - verigen"
module.__date		= os.date("%B/%d/%Y %a")
module.__year		= os.date("%Y")
module.__time		= os.date("%X")
module.__title		= "no title"
module.__external	= false
module.__inception	= ""
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
function module.new(name, base)
	-- name validation
	if type(name) ~= "string" then
		LOGE("Invalid module name.")
		os.exit(1)
	end
	
	-- create instance
	local	t = nil
	if base == nil then
		t = setmetatable({}, module)
	elseif interface.is_valid(base) then
		t = setmetatable({}, base)
	else
		LOGE("Module[" .. name .. "] creation is failed : invalid base module instance.")
		os.exit(1)
	end
	t.__index		= t

	-- module duplication check
	if module.find(name) ~= nil then
		LOGE("already existed module : '" .. name .. "'")
		os.exit(1)
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
	__slf_test(self)
	self.__title		= title
end

function module:set_author(name)
	__slf_test(self)
	self.__author		= name
end

function module:set_inception(filename)
	__slf_test(self)
	
	local f = TextFile()
	if f:Open(filename) then
		self.__inception	= String(f:GetAll(false))
		self.__inception:Replace("\r", "", true)
		self.__inception:Trim(" \n")
		self.__inception:Append("\n")
		self.__inception	= self.__inception.s
		f:Close()
	else
		LOGE("Can't find code inception : " .. filename)
		os.exit(1)
	end
	
end

function module:set_external()
	__slf_test(self)
	module.__external	= true
end

function module:set_top()
	__slf_test(self)
	module.__top	= self
end

function module:add_port(name, i, type)
	if interface.is_valid(i) == false then
		LOGE("Invalid interface : " .. name)
		os.exit(1)
	end
	self.port[name]	= {["i"] = i, ["type"] = type}
end

function module:parameter(name, default_value)
	__slf_test(self)
	if load("return module.__list." .. self.name .. ".param." .. name)() ~= nil then
		LOGE("module[" .. self.name .. "] : given parameter[" .. name .. "] is already existed.")
	end

	load("module.__list." .. self.name .. ".param." .. name .. "=" .. default_value)()
end

function module.build_all()
	for name, m in pairs(module.__list) do
		if m.external ~= true then
			local	f = TextFile()
			if f:Create(sOutPath .. "/" .. m.name .. ".sv") == false then
				LOGE("Can't create top design file.")
			os.exit(1)
			end
			-- code inception
			do
				local code_inception = String(m.__inception)
				code_inception:Replace("__YEAR__", m.__year)
				code_inception:Replace("__TITLE__", m.__title)
				code_inception:Replace("__DATE__", m.__date)
				code_inception:Replace("__TIME__", m.__time)
				code_inception:Replace("__AUTHOR__", m.__author)
				f:Put(code_inception.s)
			end
			-- common include
			f:Put(	"`include \"" .. module.__top.name .. "_defines.vh\"\n\n")

			-- module declaration
			f:Put(	"module " .. m.name)

			-- parameters
			do
				local	param_count	= 0
				for name, v in pairs(m.param) do
					if param_count == 0 then
						f:Put(" #(")
					else
						f:Put(",")
					end
					f:Put(string.format("\n    %-20s = %s", name, tostring(v)) .. "")
					param_count	= param_count + 1
				end
				
				if param_count ~= 0 then
					f:Put(	"\n)")
				end
			end

			-- ports
			do
				f:Put(	" (\n")
				for name, port in pairs(m.port) do
				end
				f:Put(	");\n\n")
			end
			f:Put(	"endmodule\n")
			f:Close()
		end
    end
end
