-----------------------------------------------
-- verigen processor generation example
-- Step 0 : initialize
-----------------------------------------------
-- clock, reset, bus interface declarations
RunScript("test_definition.lua")

-- code inception
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")


-----------------------------------------------
-- Step 1 : module & interface Declaration
-----------------------------------------------
core_wrapper	= module:new("test_wrapper")		-- top
core			= {}
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("slave_ctrl")
core.core_if	= module:new("core_if")
core.core_ex	= module:new("core_ex")
core.core_wb	= module:new("core_wb")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")

-- add master bus
bus.maxi4:set_param("DATA_WIDTH", 512)
bus.maxi4:set_param("ADDR_WIDTH", 36)
bus.maxi4:set_prefix("M#")

-- add busy
core_busy		= new_signal("core_busy")

-----------------------------------------------
-- Step 2 : module interconnection
-----------------------------------------------
core_wrapper:add_module(core.mem_ctrl)
core_wrapper:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

core.slave_ctrl:add_module(core.reg_ctrl)

-- multi-core genration
core.inst	= {}
for i = 1, config.core_size, 1 do
   core.inst[i]	= core_wrapper:add_module(core.top)
end

----------------------------------------------------------------
-- Step 3 : add all code details
----------------------------------------------------------------
for entry in lfs.dir("src/") do
	local	s	= String(entry)
	if s:CompareBack(".sv") then
		module.apply_code("src/" .. entry)
	end
end

-----------------------------------------------
-- Step last : module codes out
-----------------------------------------------
core_wrapper:make_code()
