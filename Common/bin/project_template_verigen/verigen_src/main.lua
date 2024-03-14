verigen_description("Main generator")

-- default definitions
RunScript("defines_PROJECT.lua")

-- IP design
RunScript("design_hierarchy.lua")

-- add eclipse project link
module:add_document("img/simulation_top.png", "../.eclipse.bat")

----------------------------------------------------------------
-- add all code details
----------------------------------------------------------------
verigen_add_verilog("*.sv")

----------------------------------------------------------------
-- make design
----------------------------------------------------------------
PROJECT.top_wrapper:make_code()
