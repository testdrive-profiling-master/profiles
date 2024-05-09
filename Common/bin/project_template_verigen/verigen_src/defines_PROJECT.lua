verigen.set_description("PROJECT Definitions")

RunScript("defines_common.lua")

i_mtsp		= {}		-- PROJECT interfaces
i_core		= {}		-- core interfaces

config.thread_size		= 8
config.inst_width		= 128

-------------------------------------------------------------------
-- master bus wrapper
i_mtsp.mbus	= interface:new("mbus")
i_mtsp.mbus:set_clock(clk.MCLK)
i_mtsp.mbus:set_signal("RREQ")
i_mtsp.mbus:set_signal("RGRANT")
i_mtsp.mbus:set_signal("RADDR", config.m_addr_width)
i_mtsp.mbus:set_signal("RSIZE", 8)
i_mtsp.mbus:set_signal("RDATA", config.m_data_width)
i_mtsp.mbus:set_signal("RVALID")
i_mtsp.mbus:set_signal("RREADY")
i_mtsp.mbus:set_signal("RLAST")

i_mtsp.mbus:set_signal("WREQ")
i_mtsp.mbus:set_signal("WGRANT")
i_mtsp.mbus:set_signal("WADDR", config.m_addr_width)
i_mtsp.mbus:set_signal("WSIZE", 8)
i_mtsp.mbus:set_signal("WDATA", config.m_data_width)
i_mtsp.mbus:set_signal("WVALID")
i_mtsp.mbus:set_signal("WREADY")
i_mtsp.mbus:set_signal("WLAST")

i_mtsp.mbus:set_modport("m", {
	["output"] = {"RREQ", "RADDR", "RSIZE", "RREADY",   "WREQ", "WADDR", "WSIZE", "WDATA", "WVALID"},
	["input" ] = {"RGRANT", "RDATA", "RVALID", "RLAST", "WGRANT", "WREADY", "WLAST"}
})

i_mtsp.mbus:set_modport("s", {
	["input" ] = {"RREQ", "RADDR", "RSIZE", "RREADY",   "WREQ", "WADDR", "WSIZE", "WDATA", "WVALID"},
	["output"] = {"RGRANT", "RDATA", "RVALID", "RLAST", "WGRANT", "WREADY", "WLAST"}
})

----------------------------------------------------------------
-- setup & contraint
----------------------------------------------------------------
-- clocks
bus.apb:set_clock(clk.MCLK)
bus.maxi3:set_clock(clk.MCLK)
bus.maxi4:set_clock(clk.MCLK)

-- additional constraints
verigen.set_constraint("Interrupt", "set_false_path -from [get_ports INTR]")

----------------------------------------------------------------
-- verilog function
----------------------------------------------------------------
vfunction("THREAD_MASK", function(i)
	-- fix range
	while i < 0 do
		i = config.thread_size + i
	end

	if i >= config.thread_size then
		i = config.thread_size % i
	end

	if i == 0 then
		return "thread_mask"
	end

	local s			= String("")
	local s_left	= String("")
	local s_right	= String("")

	if i == 1 then
		s_right:Append(config.thread_size-1)
	else
		s_right:Append((config.thread_size-1) .. ":" .. (config.thread_size-i))
	end

	if i == (config.thread_size-1) then
		s_left:Append(0)
	else
		s_left:Append((config.thread_size-1-i) .. ":0")
	end

	s:Append("{thread_mask[" .. s_left.s .. "], thread_mask[" .. s_right.s .. "]}")

	return s.s
end)
