//#---------------------------------------------------------------------
module test_wrapper

wire	$RANGE(config.core_size)	core_busy_all;

${	-- It's Lua codes
	module:set_title("Fast Follow")
	
	for i = 1, config.core_size, 1 do
		core.inst[i]:set_param("CORE_ID", i)
		core.inst[i]:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
	end

	__m:get_module("slave_ctrl"):set_port("core_busy", "|core_busy_all")
}

//#---------------------------------------------------------------------
module slave_ctrl
$set_param("BASE_ADDR", "32'h10000000")
$add_interface(bus.apb, "s_apb", "m")
$add_interface(bus.apb, "s_apb_0", "m")

//#---------------------------------------------------------------------
module mem_ctrl
$add_interface(bus.maxi4, "maxi", "m")

//#---------------------------------------------------------------------
module reg_ctrl
$add_interface(core_busy, nil, "s")
