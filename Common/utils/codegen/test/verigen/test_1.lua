verigen.set_description("Test project")

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

-- make code
core_wrapper:make_code()
