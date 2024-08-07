# Appendix

## Appendix : test_definition.lua

```lua
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
-- bus interface
---------------------------------------------------------
bus		= {}

-- APB bus
bus.apb 	= interface:new("apb")
bus.apb:set_clock(clk.PCLK)
bus.apb:set_param("ADDR_WIDTH", 16)
bus.apb:set_param("DATA_WIDTH", 32)
bus.apb:set_param("SEL_WIDTH", 2)
bus.apb:set_signal("PADDR", "ADDR_WIDTH")
bus.apb:set_signal("PSEL", "SEL_WIDTH")
bus.apb:set_signal("PENABLE")
bus.apb:set_signal("PWRITE")
bus.apb:set_signal("PWDATA", "DATA_WIDTH")
bus.apb:set_signal("PREADY")
bus.apb:set_signal("PRDATA", "DATA_WIDTH")
bus.apb:set_signal("PSLVERR")

bus.apb:set_modport("m/s", {["output"]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["input" ]={"PREADY", "PRDATA", "PSLVERR"}})

bus.apb:set_prefix("S#")


-- AXI3 master bus
bus.maxi3 	= interface:new("maxi3")
bus.maxi3:set_clock(clk.ACLK)
bus.maxi3:set_param("DATA_WIDTH", 128)
bus.maxi3:set_param("ADDR_WIDTH", 32)
bus.maxi3:set_param("ID_WIDTH", 4)
-- write address
bus.maxi3:set_signal("AWVALID")
bus.maxi3:set_signal("AWREADY")
bus.maxi3:set_signal("AWADDR", "ADDR_WIDTH")
bus.maxi3:set_signal("AWSIZE", 3)
bus.maxi3:set_signal("AWBURST", 2)
bus.maxi3:set_signal("AWCACHE", 4)
bus.maxi3:set_signal("AWPROT", 3)
bus.maxi3:set_signal("AWID", "ID_WIDTH")
bus.maxi3:set_signal("AWLEN", 4)
bus.maxi3:set_signal("AWLOCK", 2)
-- write data
bus.maxi3:set_signal("WVALID")
bus.maxi3:set_signal("WREADY")
bus.maxi3:set_signal("WLAST")
bus.maxi3:set_signal("WDATA", "DATA_WIDTH")
bus.maxi3:set_signal("WSTRB", "DATA_WIDTH/8")
bus.maxi3:set_signal("WID", "ID_WIDTH")
-- write response
bus.maxi3:set_signal("BVALID")
bus.maxi3:set_signal("BREADY")
bus.maxi3:set_signal("BRESP", 2)
bus.maxi3:set_signal("BID", "ID_WIDTH")
-- read address
bus.maxi3:set_signal("ARVALID")
bus.maxi3:set_signal("ARREADY")
bus.maxi3:set_signal("ARADDR", "ADDR_WIDTH")
bus.maxi3:set_signal("ARSIZE", 3)
bus.maxi3:set_signal("ARBURST", 2)
bus.maxi3:set_signal("ARCACHE", 4)
bus.maxi3:set_signal("ARPROT", 3)
bus.maxi3:set_signal("ARID", "ID_WIDTH")
bus.maxi3:set_signal("ARLEN", 4)
bus.maxi3:set_signal("ARLOCK", 2)
-- read data
bus.maxi3:set_signal("RVALID")
bus.maxi3:set_signal("RREADY")
bus.maxi3:set_signal("RLAST")
bus.maxi3:set_signal("RDATA", "DATA_WIDTH")
bus.maxi3:set_signal("RRESP", 2)
bus.maxi3:set_signal("RID", "ID_WIDTH")

bus.maxi3:set_modport("m/s", {
	["output"]={
			"AWVALID", "AWADDR", "AWSIZE", "AWBURST", "AWCACHE", "AWPROT", "AWID", "AWLEN", "AWLOCK",
			"WVALID", "WLAST", "WDATA", "WSTRB", "WID",
			"BREADY",
			"ARVALID", "ARADDR", "ARSIZE", "ARBURST", "ARCACHE", "ARPROT", "ARID", "ARLEN", "ARLOCK",
			"RREADY", "RLAST"
			},
	["input"]={
			"AWREADY", "WREADY", "BVALID", "BRESP", "BID", "ARREADY", "RVALID", "RDATA", "RRESP", "RID"
			}
})

bus.maxi3:set_prefix("M#")

-- AXI4 master bus
bus.maxi4	= bus.maxi3:new("maxi4")
bus.maxi4:set_signal("AWLOCK")			-- modified 2bit to 1bit
bus.maxi4:set_signal("ARLOCK")			-- modified 2bit to 1bit
bus.maxi4:set_signal("AWLEN", 8)		-- modified 4bit to 8bit
bus.maxi4:set_signal("ARLEN", 8)		-- modified 4bit to 8bit
bus.maxi4:set_signal("AWQOS", 4)		-- new on AXI4
bus.maxi4:set_signal("AWREGION", 4)		-- new on AXI4
bus.maxi4:set_signal("ARQOS", 4)		-- new on AXI4
bus.maxi4:set_signal("ARREGION", 4)		-- new on AXI4

bus.maxi4:add_modport("m/s", {
	["input" ]={"ARQOS", "ARREGION"},
	["output"]={"AWQOS", "AWREGION"}
})

bus.maxi4:set_prefix("M#")

---------------------------------------------------------
-- core interface
---------------------------------------------------------
core_i	= {}
core_i.inst		= interface:new("inst")
core_i.inst:set_signal("EN")
core_i.inst:set_signal("INST", 32)
core_i.inst:set_signal("READY")
core_i.inst:set_modport("m/s", {
	["output"]={"EN", "INST"},
	["input"] ={"READY"}
})

core_i.inst:set_clock(clk.MCLK)

---------------------------------------------------------
-- configuration
---------------------------------------------------------
config				= {}
config.core_size	= 4
```
