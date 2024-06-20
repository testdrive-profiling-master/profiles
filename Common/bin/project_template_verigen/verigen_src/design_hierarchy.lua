verigen.set_description("Design hierarchy")

----------------------------------------------------------------
-- module container
----------------------------------------------------------------
__PROJECT__	= {}

----------------------------------------------------------------
-- mtsp top
----------------------------------------------------------------
__PROJECT__.top_wrapper		= module:new("__PROJECT___wrapper")
__PROJECT__.top				= module:new("__PROJECT__")
__PROJECT__.sbus			= module:new("__PROJECT___BusSlave")
__PROJECT__.mbus			= module:new("__PROJECT___BusMaster")
__PROJECT__.mbus_wrapper	= module:new("__PROJECT___BusMasterWrapper")


__PROJECT__.top_wrapper:add_module(__PROJECT__.top)
__PROJECT__.top_wrapper:add_module(__PROJECT__.sbus)
__PROJECT__.top_wrapper:add_module(__PROJECT__.mbus)

__PROJECT__.mbus_list		= {"bus_inst", "bus_data", "bus_stack"}	-- instruction & data
__PROJECT__.mbus_inst		= {}

for i=1, #__PROJECT__.mbus_list do
	__PROJECT__.mbus_inst[i]	= __PROJECT__.mbus:add_module(__PROJECT__.mbus_wrapper)
end

----------------------------------------------------------------
-- set colored to design map
----------------------------------------------------------------
__PROJECT__.sbus.fill_color		= "00D0FF"
__PROJECT__.mbus.fill_color		= "00D0FF"
