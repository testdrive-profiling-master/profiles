verigen.set_description("Main generator")

-- default definitions
RunScript("defines___PROJECT__.lua")

-- IP design
RunScript("design_hierarchy.lua")

-- add eclipse project link
module:add_document("img/simulation_top.png", "../.eclipse.bat")

----------------------------------------------------------------
-- add all code details
----------------------------------------------------------------
verigen.add_verilog("*.sv")

----------------------------------------------------------------
-- make design
----------------------------------------------------------------
__PROJECT__.top_wrapper:make_code()
