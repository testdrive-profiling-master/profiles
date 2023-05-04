-----------------------------------------------
-- verigen processor generation example
-----------------------------------------------

-- clock, reset, bus interface declarations
RunScript("test_definition.lua")

-- code inception
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")


-- module 
core	= {}
core.wrapper	= module:new("test_wrapper")
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("test_slave_ctrl")
core.core_if	= module:new("core_if")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")


--[[
t = core.wrapper:add_interface(bus.apb, "apb_ctrl")
t:set_port("m", "S0")
t:set_desc("APB's control bus")--]]

core.wrapper:add_module(core.reg_ctrl)



-- add master bus
bus.mbus	= bus.maxi4:new("mbus")
bus.mbus:set_param("DATA_WIDTH", 512)
bus.mbus:set_param("ADDR_WIDTH", 36)
bus.mbus:set_prefix("M#")

core.mem_ctrl:add_interface(bus.mbus, "maxi"):set_port("m")

--
core_busy_all	= new_signal("core_busy_all", config.core_size)
core_busy		= new_signal("core_busy")
core.top:add_interface(core_busy):set_port("m")
core.wrapper:add_interface(core_busy_all)

core.reg_ctrl:add_interface(core_busy_all):set_port("m")


-- multi-core genration
for i = 1, config.core_size, 1 do
   local	core_inst	= core.wrapper:add_module(core.top)
   core_inst:set_param("CORE_ID", i)
   core_inst:set_port("core_busy", core_busy_all.name .. "[" .. (i-1) .. "]")
end


tt = core.wrapper:add_module(core.mem_ctrl)

core.top:add_module(core.slave_ctrl)
core.top:add_module(core.core_if)


core.slave_ctrl:set_param("CORE_ID", "0")

i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb")
i_apb:set_port("m")
i_apb:set_desc("APB's control bus")

i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb_0")
i_apb:set_port("m")


--core.top:add_interface(bus.apb, "s_apb")

core.wrapper:make_code()
