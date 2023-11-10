----------------------------------------------------------------
-- module container
----------------------------------------------------------------
PROJECT	= {}

----------------------------------------------------------------
-- mtsp top
----------------------------------------------------------------
PROJECT.top_wrapper		= module:new("PROJECT_wrapper")
PROJECT.top				= module:new("PROJECT")
PROJECT.sbus			= module:new("PROJECT_BusSlave")
PROJECT.mbus			= module:new("PROJECT_BusMaster")
PROJECT.mbus_wrapper	= module:new("PROJECT_BusMasterWrapper")


PROJECT.top_wrapper:add_module(PROJECT.top)
PROJECT.top_wrapper:add_module(PROJECT.sbus)
PROJECT.top_wrapper:add_module(PROJECT.mbus)

PROJECT.mbus_list		= {"bus_inst", "bus_data", "bus_stack"}	-- instruction & data
PROJECT.mbus_inst		= {}

for i=1, #PROJECT.mbus_list do
	PROJECT.mbus_inst[i]	= PROJECT.mbus:add_module(PROJECT.mbus_wrapper)
end

----------------------------------------------------------------
-- set colored to design map
----------------------------------------------------------------
PROJECT.sbus.fill_color		= "00D0FF"
PROJECT.mbus.fill_color		= "00D0FF"
