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
-- Step 0 : modules Declaration
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
core.busy_ctrl	= module:new("busy_ctrl")

-----------------------------------------------
-- Step 1 : modules Interfaces
-----------------------------------------------
-- add master bus
bus.mbus	= bus.maxi4:new("mbus")
bus.mbus:set_param("DATA_WIDTH", 512)
bus.mbus:set_param("ADDR_WIDTH", 36)
bus.mbus:set_prefix("M#")

core.mem_ctrl:add_interface(bus.mbus, "maxi"):set_port("m")

-- add slave bus
i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb")
i_apb:set_port("m")
i_apb:set_desc("APB's control bus")

i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb_0")
i_apb:set_port("m")

-- add busy
core_busy_all	= new_signal("core_busy_all", config.core_size)
core_busy		= new_signal("core_busy")

core.top:add_interface(core_busy):set_port("m")
core_wrapper:add_interface(core_busy_all)
core.busy_ctrl:add_interface(core_busy_all):set_port("s")

-- add instruction
core.core_if:add_interface(core_i.inst, "if_inst"):set_port("m")
core.core_ex:add_interface(core_i.inst, "if_inst"):set_port("s")

core.core_ex:add_interface(core_i.inst, "ex_inst"):set_port("m")
core.core_wb:add_interface(core_i.inst, "ex_inst"):set_port("s")

-----------------------------------------------
-- Step 2 : modules parameters
-----------------------------------------------
-- core ID
core.core_if:set_param("CORE_ID", "0")

core.reg_ctrl:set_param("BASE_ADDR", "32'h10000000")

-----------------------------------------------
-- Step 3 : module interconnection
-----------------------------------------------
core_wrapper:add_module(core.reg_ctrl)
core_wrapper:add_module(core.mem_ctrl)
core.reg_ctrl:add_module(core.busy_ctrl)
core.reg_ctrl:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

-- multi-core genration
for i = 1, config.core_size, 1 do
   local	core_inst	= core_wrapper:add_module(core.top)
   core_inst:set_param("CORE_ID", i)
   core_inst:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
end

-----------------------------------------------
-- Step last : module codes out
-----------------------------------------------
core_wrapper:make_code()
