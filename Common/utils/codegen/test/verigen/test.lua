-- verigen run example
module:set_inception("code_inception.txt")
module:set_author("HyungKi Jeong")
module:set_title("Some title")

-- clock definition
PCLK	= clock.new("PCLK")
PCLK:constraint_speed(100)

BCLK	= clock.new("BCLK")
BCLK:constraint_speed(1000)

ACLK	= clock.new("ACLK")
ACLK:constraint_speed(1500)


top		= module.new("test_wrapper")
x		= module.new("test2")

axi 	= interface.new("axi")
axi:parameter("DATA_WIDTH", 256)
axi:parameter("ADDR_WIDTH", 32)

apb 	= interface.new("apb")
apb:set_clock(PCLK)


axi4	= interface.new("axi4", axi)
axi4:parameter("DATA_WIDTH", 512)

axi5	= interface.new("axi5", axi4)

LOGI("axi:params.DATA_WIDTH :" .. tostring(axi.param.DATA_WIDTH))
LOGI("axi:params.DATA_WIDTH :" .. tostring(axi4.param.DATA_WIDTH))

top:parameter("AA", "12")
top:parameter("AB", "13")
top:parameter("AC", "14")

top:add_port("APB", apb, "s")

top:set_top()

LOGI("NAME : " .. top.name)
