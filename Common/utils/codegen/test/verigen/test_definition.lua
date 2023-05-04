---------------------------------------------------------
-- clock definition
---------------------------------------------------------
clk		= {}

clk.MCLK	= clock:new("CLK", "main clock")	-- for core
clk.MCLK:set_speed(1000)

clk.PCLK	= clock:new("PCLK", "APB clock")
clk.PCLK:set_speed(100)
clk.PCLK:set_reset("PRESETn", "low")

clk.BCLK	= clock:new("ICLK", "interconnection clock")
clk.BCLK:set_speed(1500)

clk.ACLK	= clock:new("ACLK", "AXI clock")
clk.ACLK:set_speed(1000)


---------------------------------------------------------
-- bus definitions
---------------------------------------------------------
bus		= {}

-- APB bus
bus.apb 	= interface:new("apb")
bus.apb:set_clock(clk.PCLK)
bus.apb:set_param("ADDR_WIDTH", 16)
bus.apb:set_param("DATA_WIDTH", 32)
bus.apb:set_signal("PADDR", "ADDR_WIDTH")
bus.apb:set_signal("PSEL", 2)
bus.apb:set_signal("PENABLE")
bus.apb:set_signal("PWRITE")
bus.apb:set_signal("PWDATA")
bus.apb:set_signal("PREADY")
bus.apb:set_signal("PRDATA", "DATA_WIDTH")
bus.apb:set_signal("PSLVERR")

bus.apb:set_modport("s", {["i"]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["o"]={"PREADY", "PRDATA", "PSLVERR"}})
bus.apb:set_modport("m", {["o"]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["i"]={"PREADY", "PRDATA", "PSLVERR"}})

bus.apb:set_prefix("S#")


-- AXI3 master bus
bus.maxi3 	= interface:new("maxi3")
bus.maxi3:set_clock(clk.ACLK)
bus.maxi3:set_param("DATA_WIDTH", 256)
bus.maxi3:set_param("ADDR_WIDTH", 32)
bus.maxi3:set_signal("ARADDR")
bus.maxi3:set_signal("AWADDR")

bus.maxi3:set_modport("s", {["i"]={"ARADDR"}, ["o"]={"AWADDR"}})
bus.maxi3:set_modport("m", {["o"]={"ARADDR"}, ["i"]={"AWADDR"}})

bus.maxi3:set_prefix("M#")

-- AXI4 master bus
bus.maxi4	= bus.maxi3:new("maxi4")
bus.maxi4:set_signal("TEN", 4)

bus.maxi4:set_prefix("M#")
