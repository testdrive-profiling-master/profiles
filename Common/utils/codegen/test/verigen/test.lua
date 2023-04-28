-- verigen run example
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")
module:set_top("top")

-- clock definition
PCLK	= clock:new("PCLK", "APB clock")
PCLK:set_speed(100)
PCLK:set_reset("PRESETn", "low")

BCLK	= clock:new("CCLK", "core clock")
BCLK:set_speed(1000)

BCLK	= clock:new("ICLK", "interconnection clock")
BCLK:set_speed(1500)

ACLK	= clock:new("ACLK", "AXI clock")
ACLK:set_speed(1000)


top		= module:new("test_wrapper")
test2	= module:new("test2")

axi 	= interface:new("axi")
axi:set_param("DATA_WIDTH", 256)
axi:set_param("ADDR_WIDTH", 32)

apb 	= interface:new("apb")
apb:set_clock(PCLK)
apb:set_param("ADDR_WIDTH", 16)
apb:set_param("DATA_WIDTH", 32)
apb:set_signal("PADDR", "ADDR_WIDTH")
apb:set_signal("PSEL", 2)
apb:set_signal("PENABLE")
apb:set_signal("PWRITE")
apb:set_signal("PWDATA")
apb:set_signal("PREADY")
apb:set_signal("PRDATA", "DATA_WIDTH")
apb:set_signal("PSLVERR")

apb:set_modport("s", {["input"]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["output"]={"PREADY", "PRDATA", "PSLVERR"}})

apb:set_port("s", "P")



axi4	= interface:new("axi4", axi)
axi4:set_param("DATA_WIDTH", 512)
axi4:set_signal("TEN", 4)
axi4:set_clock(ACLK)

axi5	= interface:new("axi5", axi4)
axi5:set_signal("ALEN", 3)
axi5:set_signal("AEN", 4)
axi5:set_signal("WDATA", "DATA_WIDTH")
axi5:set_signal("TEN", 6)
axi5:set_signal("A")

axi5:set_param("DATA_WIDTH", 128)

top:set_param("AA", "12")
top:set_param("AB", "13")
top:set_param("AC", "14")

LOGI("NAME : " .. axi5:full_name())
