-- verigen run example
RunScript("test_definition.lua")


-- code inception
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")



core	= {}
core.wrapper	= module:new("test_wrapper")
core.top		= module:new("test_core")


core.wrapper:set_param("AA", "12")
core.wrapper:set_param("AB", "AA + 3", true)
core.wrapper:set_param("AC", "14")

core.top:set_param("CORE_ID", "0")

t = core.wrapper:add_interface("apb_ctrl", bus.apb)
t:set_port("m", "S0")
t:set_desc("APB's control bus")


core.top:add_interface("maxi0", bus.maxi4):set_port("m")


x = core.wrapper:add_interface("axi_m", bus.maxi4)

core.wrapper:add_module("top", core.top)

core.wrapper:make_code()
