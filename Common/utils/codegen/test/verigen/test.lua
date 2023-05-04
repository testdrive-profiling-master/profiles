-- verigen run example
RunScript("test_definition.lua")


-- code inception
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")



core	= {}
core.wrapper	= module:new("test_wrapper")
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("test_slave_ctrl")
core.core_if	= module:new("core_if")
core.mem_ctrl	= module:new("mem_ctrl")


core.wrapper:set_param("AA", "12")
core.wrapper:set_param("AB", "AA + 3", true)
core.wrapper:set_param("AC", "14")

--[[
t = core.wrapper:add_interface("apb_ctrl", bus.apb)
t:set_port("m", "S0")
t:set_desc("APB's control bus")--]]


core.slave_ctrl.code:Append("// do something...\n")


core.top:add_interface("maxi0", bus.maxi4):set_port("m")

tt = core.wrapper:add_module(core.mem_ctrl)
tt = core.wrapper:add_module(core.top)
tt = core.wrapper:add_module(core.top)

--tt:set_param("CORE_ID", 3)
--tt:set_port("s_apb_0", "32'h32")

core.top:add_module(core.slave_ctrl)
core.top:add_module(core.core_if)


core.slave_ctrl:set_param("CORE_ID", "0")

i_apb = core.slave_ctrl:add_interface("s_apb", bus.apb)
i_apb:set_port("m")
i_apb:set_desc("APB's control bus")

i_apb = core.slave_ctrl:add_interface("s_apb_0", bus.apb)
i_apb:set_port("m")


--core.top:add_interface("s_apb", bus.apb)

core.wrapper:make_code()
