verigen.set_description("Test project")

RunScript("test_definition.lua")

-- modules
core_wrapper = module:new("test_wrapper")	-- top
core = {}
core.top = module:new("test_core")
core.slave_ctrl = module:new("slave_ctrl")
 
core.core_if = module:new("core_if")
core.core_ex = module:new("core_ex")
core.core_wb = module:new("core_wb")
core.mem_ctrl = module:new("mem_ctrl")
core.reg_ctrl = module:new("reg_ctrl")
core.busy_ctrl = module:new("busy_ctrl")

-- add master bus
bus.maxi4:set_param("DATA_WIDTH", 512)
bus.maxi4:set_param("ADDR_WIDTH", 36)
bus.maxi4:set_prefix("M#")

-- add busy
core_busy = new_signal("core_busy")

-- module connection
core_wrapper:add_module(core.mem_ctrl)
core_wrapper:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

core.slave_ctrl:add_module(core.reg_ctrl)

-- multi-core genration
for i = 1, config.core_size do
	core_wrapper:add_module(core.top)
end

 -- add verilog codes
verigen.add_verilog("src/*.sv")

-- make code
core_wrapper:make_code()
