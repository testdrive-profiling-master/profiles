# Fast follow

이 단은 예제 중심으로 빠르게 설명하고 있습니다. \
사전 형식으로 class 와 method 를 확인하려면, 다음 단 '@<bookmark:Class and Method>'를 참조하시기 바랍니다.

## Step #1 : 모듈의 생성

아래와 같이 스크립트 코드를 생성하고 실행합니다.

@<b>[main.lua 파일]@</b>
```#lua
-- modules
core_wrapper	= module:new("test_wrapper")		-- top
core			= {}
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("slave_ctrl")
core.core_if	= module:new("core_if")
core.core_ex	= module:new("core_ex")
core.core_wb	= module:new("core_wb")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")
core.busy_ctrl	= module:new("busy_ctrl")

-- make code
core_wrapper:make_code()
```
core_wrapper 모듈과 core 관련 모듈을 @<link:https://www.lua.org/pil/3.6.html;lua table> 에 담아 생성한 것입니다.

@<b>[실행]@</b>
```txt
> verigen main.lua
*I: Build TOP design : test_wrapper.sv
*W: Empty port module : 'test_wrapper' module
*I: Build contraint : test_wrapper_constraint.xdc
*I: Make design hierarchy : test_wrapper_hierarchy.svg
*I: Make common defines : test_wrapper_defines.vh
*I: Make design file list : test_wrapper.f
```

@<bookmark:module:new> 메소드를 통해 사용 되어질 모듈들을 간략히 선언합니다. \
그리고 마지막 @<bookmark:module:make_code> 메소드는 실제 verilog 코드, \
constraint 파일과 hierarchy diagram 을 생성해 냅니다. \
현재는 여러개의 모듈을 선언하였으나, core_wrapper 연결된 모듈이 없기 때문에, \
verilog 디자인은 test_wrapper.sv 파일 하나만 생성합니다.
나머지 test_wrapper_constraint.xdc, test_wrapper_defines.vh 등은 비어있는 상태입니다.

@<b>[결과 : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper ();


endmodule
```
결과 디자인은 말 그대로 빈 모듈 파일이며, 다이어그램도 역시 비어있는 상태입니다.

@<b>[결과 : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step1_test_wrapper_hierarchy.svg;0.2;Step #1 Hierarchy Diagram>
 

## Step #2 : 모듈의 연결

아래와 같이 lua 스크립트를 수정하여 실행합니다.

@<b>[main.lua 파일]@</b>
```#lua
RunScript("test_definition.lua")

-- modules
core_wrapper	= module:new("test_wrapper")		-- top
core			= {}
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("slave_ctrl")
core.core_if	= module:new("core_if")
core.core_ex	= module:new("core_ex")
core.core_wb	= module:new("core_wb")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")
core.busy_ctrl	= module:new("busy_ctrl")

-- module connection
core_wrapper:add_module(core.reg_ctrl)
core_wrapper:add_module(core.mem_ctrl)
core.reg_ctrl:add_module(core.busy_ctrl)
core.reg_ctrl:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

-- multi-core genration
for i = 1, config.core_size, 1 do
   core_wrapper:add_module(core.top)
end

-- make code
core_wrapper:make_code()
```
이제 추가된 15 ~ 28 라인에서 각 모듈을 @<bookmark:module:add_module> 함수로 연결하고, core 도 4개 모듈을 생성하여 연결했습니다. \
미리 정의된 config.core_size 값을 사용하기 위해 상단(line #1)에 "@<bookmark:Appendix : test_definition.lua>" 를 포함하고 있습니다.

@<b>[실행]@</b>
```txt
> verigen main.lua
*I: Build sub design : mem_ctrl.sv
*W: Empty port module : 'mem_ctrl' module
*I: Build sub design : busy_ctrl.sv
*W: Empty port module : 'busy_ctrl' module
*I: Build sub design : slave_ctrl.sv
*W: Empty port module : 'slave_ctrl' module
*I: Build sub design : reg_ctrl.sv
*W: Empty port module : 'reg_ctrl' module
*I: Build sub design : core_ex.sv
*W: Empty port module : 'core_ex' module
*I: Build sub design : core_if.sv
*W: Empty port module : 'core_if' module
*I: Build sub design : core_wb.sv
*W: Empty port module : 'core_wb' module
*I: Build sub design : test_core.sv
*W: Empty port module : 'test_core' module
*I: Build TOP design : test_wrapper.sv
*W: Empty port module : 'test_wrapper' module
*I: Build contraint : test_wrapper_constraint.xdc
*I: Make design hierarchy : test_wrapper_hierarchy.svg
*I: Make common defines : test_wrapper_defines.vh
*I: Make design file list : test_wrapper.f
```
실행된 결과에서 test_wrapper.sv 파일 말고도 포함된 다른 파일들도 자동 생성되며, \
top design 을 본다면 아직 모듈만 추가 선언된 상태이고 포트는 기술되지 않았으므로, \
경고를 발생하지만 아래처럼 각 서브 모듈이 자동으로 추가된 모습을 볼 수 있습니다.

@<b>[결과 : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper ();


mem_ctrl mem_ctrl (
);

reg_ctrl reg_ctrl (
);

test_core test_core_0 (
);

test_core test_core_1 (
);

test_core test_core_2 (
);

test_core test_core_3 (
);

endmodule
```
 
@<b>[결과 : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step2_test_wrapper_hierarchy.svg;0.8;Step #2 Hierarchy Diagram>

test_wrapper.sv 이외에도 다른 reg_ctrl.sv, test_core.sv 안에서도 @<bookmark:Step #2 Hierarchy Diagram>에서 \
볼 수 있듯이, 각각 하위 모듈들을 포함하고 있습니다.
 

## Step #3 : parameter 추가

아래와 같이 lua 스크립트를 수정하여 실행합니다.

@<b>[main.lua 파일]@</b>
```#lua
RunScript("test_definition.lua")

-- modules
core_wrapper	= module:new("test_wrapper")		-- top
core			= {}
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("slave_ctrl")
core.core_if	= module:new("core_if")
core.core_ex	= module:new("core_ex")
core.core_wb	= module:new("core_wb")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")
core.busy_ctrl	= module:new("busy_ctrl")

-- module connection
core_wrapper:add_module(core.reg_ctrl)
core_wrapper:add_module(core.mem_ctrl)
core.reg_ctrl:add_module(core.busy_ctrl)
core.reg_ctrl:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

-- parameters
core.core_if:set_param("CORE_ID", "0")
core.reg_ctrl:set_param("BASE_ADDR", "32'h10000000")

-- multi-core genration
for i = 1, config.core_size, 1 do
   local	core_inst	= core_wrapper:add_module(core.top)
   core_inst:set_param("CORE_ID", i)
end

-- make code
core_wrapper:make_code()
```
추가또는 변경된 코드는 line #26,27,31,32 입니다.
core 의 인스턴스 마다 CORE_ID 를 부여하기 위해, line #26 에서 @<bookmark:module:set_param> 을 통해 지정했습니다. \
또한 line #31 에서 생성한 모든 core module instance 에 각각 CORE_ID 를 지정합니다.
reg_ctrl 의 경우는 BASE_ADDR parameter 를 추가했는데, 상위에서 이를 지정하지 않았기 때문에, \
@<bookmark:Step #3 Hierarchy Diagram>과 같이 그대로 top 까지 parameter 가 전달되어 선언됩니다.

@<b>[결과 : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step3_test_wrapper_hierarchy.svg;1.0;Step #3 Hierarchy Diagram>

아래는 top 의 결과물입니다.

@<b>[결과 : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper #(
	parameter BASE_ADDR        = 32'h10000000
) ();


mem_ctrl mem_ctrl (
);

reg_ctrl #(
	.BASE_ADDR           (BASE_ADDR)
) reg_ctrl (
);

test_core #(
	.CORE_ID             (1)
) test_core_0 (
);

test_core #(
	.CORE_ID             (2)
) test_core_1 (
);

test_core #(
	.CORE_ID             (3)
) test_core_2 (
);

test_core #(
	.CORE_ID             (4)
) test_core_3 (
);

endmodule
```
 
## Step #4 : interface 추가

아래와 같이 lua 스크립트를 수정하여 실행합니다.

@<b>[main.lua 파일]@</b>
```#lua
RunScript("test_definition.lua")

-- modules
core_wrapper	= module:new("test_wrapper")		-- top
core			= {}
core.top		= module:new("test_core")
core.slave_ctrl	= module:new("slave_ctrl")
core.core_if	= module:new("core_if")
core.core_ex	= module:new("core_ex")
core.core_wb	= module:new("core_wb")
core.mem_ctrl	= module:new("mem_ctrl")
core.reg_ctrl	= module:new("reg_ctrl")
core.busy_ctrl	= module:new("busy_ctrl")

-- module connection
core_wrapper:add_module(core.reg_ctrl)
core_wrapper:add_module(core.mem_ctrl)
core.reg_ctrl:add_module(core.busy_ctrl)
core.reg_ctrl:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

-- parameters
core.core_if:set_param("CORE_ID", "0")
core.reg_ctrl:set_param("BASE_ADDR", "32'h10000000")

-- add slave bus interface
i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb")
i_apb:set_port("m")
i_apb:set_desc("APB's control bus")

-- add master bus interface
bus.mbus	= bus.maxi4:new("mbus")
bus.mbus:set_param("DATA_WIDTH", 512)
bus.mbus:set_param("ADDR_WIDTH", 36)
bus.mbus:set_prefix("M#")

core.mem_ctrl:add_interface(bus.mbus, "maxi"):set_port("m")

-- add busy signal interface
core_busy_all	= new_signal("core_busy_all", config.core_size)
core_busy		= new_signal("core_busy")

core.top:add_interface(core_busy):set_port("m")
core_wrapper:add_interface(core_busy_all)
core.busy_ctrl:add_interface(core_busy_all):set_port("s")

-- multi-core genration
for i = 1, config.core_size, 1 do
   local	core_inst	= core_wrapper:add_module(core.top)
   core_inst:set_param("CORE_ID", i)
   core_inst:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
end

-- add instruction interface
core.core_if:add_interface(core_i.inst, "if_inst"):set_port("m")
core.core_ex:add_interface(core_i.inst, "if_inst"):set_port("s")

core.core_ex:add_interface(core_i.inst, "ex_inst"):set_port("m")
core.core_wb:add_interface(core_i.inst, "ex_inst"):set_port("s")

-- make code
core_wrapper:make_code()
```
 

### Slave APB 연결

먼저 line #29~32 사이의 APB 버스 추가입니다.
```lua
...
-- add slave bus interface
i_apb = core.slave_ctrl:add_interface(bus.apb, "s_apb")
i_apb:set_port("m")
i_apb:set_desc("APB's control bus")
...
```
bus.apb 는 "@<bookmark:Appendix : test_definition.lua>"에서 먼저 정의된 APB bus 입니다. \
위와 같이 @<bookmark:module:add_interface> 함수로 APB bus interface 의 instance 를 생성하고, \
@<bookmark:interface_i:set_port> 함수로 master(systemverilog 의 modport) 로 포트로 기술합니다. \
(포트로 선언하지 않으면, 기본은 내부 시그널 라우팅용 선언입니다.)
@<bookmark:interface_i:set_desc> 함수는 주석 설명을 기술하는 방법입니다.

이렇게 생성된 버스 인터페이스 instance는 @<bookmark:Step #4 Hierarchy Diagram> 에서 볼 수 있듯이 \
상위 모듈 reg_ctrl 과 top 모듈에서 별다른 언급이 없기 때문에, 최종 top 의 port 로 연결되집니다.
 

### Master AXI 연결

두번째로 line #34~38 의 master AXI 버스 추가입니다.

```lua
...
-- add master bus interface
bus.mbus	= bus.maxi4:new("mbus")
bus.mbus:set_param("DATA_WIDTH", 512)
bus.mbus:set_param("ADDR_WIDTH", 36)
bus.mbus:set_prefix("M#")

core.mem_ctrl:add_interface(bus.mbus, "maxi"):set_port("m")
...
```
 
이번엔 버스 인터페이스를 바로 추가하지 않고 복제하여, 수정하여 추가하는 방법입니다.
"@<bookmark:Appendix : test_definition.lua>"에서 이미 선언된 bus.maxi4 인터페이스를 'mbus' 이름으로 \
@<bookmark:interface:new> 함수를 통해 bus.mbus 에 복제한 후, \
DATA_WIDTH 와 ADDR_WIDTH 에 원하는 값을 @<bookmark:interface:set_param> 함수로 설정합니다.
@<bookmark:interface:set_prefix> 를 사용하는 이유는 top design 의 포트로 출력될 때, \
내부 시그널들이 input/output 등으로 출력되는데, 이 시그널 앞에 붙이는 문자열을 의미합니다. \
여기서는 'mbus' 이름의 대문자로 "MBUS#" 으로 붙이게 되며, \
이를 강제로 'M#' 으로 변경하게 됩니다.
예를 들어 내부 ARADDR 신호의 경우 M0_ARADDR 로 변경됨을 의미합니다. \
만약 중복된 종류의 다른 인터페이스가 없다면 'M' 뒤의 숫자 '#'는 제거되어 M_ARADDR 로 정의됩니다.

최종으로 mem_ctrl 모듈에 @<bookmark:module:add_interface> 함수를 통해 interface 를 붙인 후, \
@<bookmark:module_i:set_port> 함수를 이용해 master port 로 선언합니다. \
이 인터페이스 역시 상위 모듈에서 언급한 내용이 없으므로 top 의 port 까지 이어집니다.
 

### 하위 모듈간의 시그널 연결

세번째로 하위 모듈간의 시그널 연결하는 예시(line #42~48, 54)입니다.

```lua
...
-- add busy signal interface
core_busy_all	= new_signal("core_busy_all", config.core_size)
core_busy		= new_signal("core_busy")

core.top:add_interface(core_busy):set_port("m")
core_wrapper:add_interface(core_busy_all)
core.busy_ctrl:add_interface(core_busy_all):set_port("s")

for i = 1, config.core_size, 1 do
   ...
   core_inst:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
end
...
```

인터페이스가 아닌 시그널 단일 신호의 경우, new_signal 함수로 생성하지만, \
내부적으로는 interface 로 구현되어집니다. \
이러한 인터페이스를 bared interface 라 칭하며, new signal 함수 형태로 아래와 같이 구현되어 있음을 참고 바랍니다.

```lua
function new_signal(name, width)
	local	signal	= interface:new(name)
	
	if width == nil then
		width	= 1
	end

	signal:set_param("WIDTH", width)
	signal:set_signal(name, "WIDTH")
	signal:set_modport("s", {["input" ] = {name}})
	signal:set_modport("m", {["output"] = {name}})
	signal:set_prefix()		-- none prefix
	signal:set_bared()		-- bared signals
	return signal
end
```

추가된 코드를 설명하자면, 4(config.core_size) bit 의 core_busy_all 신호와, core_busy 신호 인터페이스를 추가합니다.
```lua
core.top:add_interface(core_busy):set_port("m")
```
: 각 코어에 core_busy 를 출력할 수 있도록 core.top 에 core_busy 를 master output으로 선언하고,
```lua
core.busy_ctrl:add_interface(core_busy_all):set_port("s")
```
: 이를 실제 사용할 busy_ctrl 모듈에 core_busy_all 을 slave input으로 선언합니다.

```lua
core_wrapper:add_interface(core_busy_all)
```
: 중간에 연결될 부분 core_wrapper 모듈에 동일한 이름으로 interface를 추가하여, busy_ctrl과 연결되도록 합니다.

```lua
core_inst:set_port("core_busy", "core_busy_all[" .. (i-1) .. "]")
```
: 마지막으로  코어마다 core_busy_all[#] 시그널이 연결되도록 포트 선언을 마치면, 각 core 의 instance 에서 \
busy_ctrl 모듈쪽으로 자동 연결됩니다.
 

### 하위 모듈간의 인터페이스 연결

네번째로 하위 모듈간의 인터페이스 연결하는 예시(line #57~65)입니다.


```lua
...
-- add instruction interface
core.core_if:add_interface(core_i.inst, "if_inst"):set_port("m")
core.core_ex:add_interface(core_i.inst, "if_inst"):set_port("s")

core.core_ex:add_interface(core_i.inst, "ex_inst"):set_port("m")
core.core_wb:add_interface(core_i.inst, "ex_inst"):set_port("s")
...
```
인터페이스 연결은 단일 시그널 연결보다 보다 단순하고 간결합니다. \
가능한 인터페이스 단위로 연결을 하시길 바랍니다.

설명을 하자면 인터페이스 core_i.inst 의 instance 인 if_inst 와 ex_inst 를 각각 core_if -> core_ex 로, \
core_ex -> core_wb 으로 연결한 예입니다. if_inst 하나를 예를 들면 core_if 에 add_interface 로 추가하고 master 포트로 지정합니다. \
이와 똑같은 이름을 core_ex 에 생성하고 여기서는 slave 포트로 지정합니다.
이때 두 인터페이스의 중간 연결 지점(core.top)을 찾아 같은 이름의 interface 가 자동으로 선언되어 연결됩니다.

최종 연결 결과는 @<bookmark:Step #4 Hierarchy Diagram>과 같이 구성됨을 확인 할 수 있습니다.
 
@<b>[최종 결과 : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step4_test_wrapper_hierarchy.svg;1.0;Step #4 Hierarchy Diagram>
 

아래는 소스 일부인 test_wrapper.sv(top) 와 test_core.sv(core) 를 나열합니다.

@<b>[최종 결과 : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper #(
	parameter BASE_ADDR        = 32'h10000000
) (
	// clock & reset
	input                      ACLK,        // AXI clock
	input                      PCLK,        // APB clock
	input                      PRESETn,     // reset of 'PCLK' (active low)
	input                      nRST,        // default global reset (active low)

	// maxi
	input                      M_AWREADY,
	input                      M_WREADY,
	input                      M_BVALID,
	input  [1:0]               M_BRESP,
	input  [3:0]               M_BID,
	input                      M_ARREADY,
	input                      M_RVALID,
	input  [511:0]             M_RDATA,
	input  [1:0]               M_RRESP,
	input  [3:0]               M_RID,
	input  [3:0]               M_ARQOS,
	input  [3:0]               M_ARREGION,
	output                     M_AWVALID,
	output [35:0]              M_AWADDR,
	output [2:0]               M_AWSIZE,
	output [1:0]               M_AWBURST,
	output [3:0]               M_AWCACHE,
	output [2:0]               M_AWPROT,
	output [3:0]               M_AWID,
	output [7:0]               M_AWLEN,
	output                     M_AWLOCK,
	output                     M_WVALID,
	output                     M_WLAST,
	output [511:0]             M_WDATA,
	output [63:0]              M_WSTRB,
	output [3:0]               M_WID,
	output                     M_BREADY,
	output                     M_ARVALID,
	output [35:0]              M_ARADDR,
	output [2:0]               M_ARSIZE,
	output [1:0]               M_ARBURST,
	output [3:0]               M_ARCACHE,
	output [2:0]               M_ARPROT,
	output [3:0]               M_ARID,
	output [7:0]               M_ARLEN,
	output                     M_ARLOCK,
	output                     M_RREADY,
	output                     M_RLAST,
	output [3:0]               M_AWQOS,
	output [3:0]               M_AWREGION,

	// s_apb (APB's control bus)
	input                      S_PREADY,
	input  [31:0]              S_PRDATA,
	input                      S_PSLVERR,
	output [1:0]               S_PSEL,
	output                     S_PENABLE,
	output                     S_PWRITE,
	output [15:0]              S_PADDR,
	output                     S_PWDATA
);

// bared interface : core_busy_all
logic [3:0]                    core_busy_all;

// interface : maxi
i_mbus                 maxi;
assign maxi.AWREADY            = M_AWREADY;
assign maxi.WREADY             = M_WREADY;
assign maxi.BVALID             = M_BVALID;
assign maxi.BRESP              = M_BRESP;
assign maxi.BID                = M_BID;
assign maxi.ARREADY            = M_ARREADY;
assign maxi.RVALID             = M_RVALID;
assign maxi.RDATA              = M_RDATA;
assign maxi.RRESP              = M_RRESP;
assign maxi.RID                = M_RID;
assign maxi.ARQOS              = M_ARQOS;
assign maxi.ARREGION           = M_ARREGION;
assign M_AWVALID               = maxi.AWVALID;
assign M_AWADDR                = maxi.AWADDR;
assign M_AWSIZE                = maxi.AWSIZE;
assign M_AWBURST               = maxi.AWBURST;
assign M_AWCACHE               = maxi.AWCACHE;
assign M_AWPROT                = maxi.AWPROT;
assign M_AWID                  = maxi.AWID;
assign M_AWLEN                 = maxi.AWLEN;
assign M_AWLOCK                = maxi.AWLOCK;
assign M_WVALID                = maxi.WVALID;
assign M_WLAST                 = maxi.WLAST;
assign M_WDATA                 = maxi.WDATA;
assign M_WSTRB                 = maxi.WSTRB;
assign M_WID                   = maxi.WID;
assign M_BREADY                = maxi.BREADY;
assign M_ARVALID               = maxi.ARVALID;
assign M_ARADDR                = maxi.ARADDR;
assign M_ARSIZE                = maxi.ARSIZE;
assign M_ARBURST               = maxi.ARBURST;
assign M_ARCACHE               = maxi.ARCACHE;
assign M_ARPROT                = maxi.ARPROT;
assign M_ARID                  = maxi.ARID;
assign M_ARLEN                 = maxi.ARLEN;
assign M_ARLOCK                = maxi.ARLOCK;
assign M_RREADY                = maxi.RREADY;
assign M_RLAST                 = maxi.RLAST;
assign M_AWQOS                 = maxi.AWQOS;
assign M_AWREGION              = maxi.AWREGION;

// interface : s_apb (APB's control bus)
i_apb                  s_apb;
assign s_apb.PREADY            = S_PREADY;
assign s_apb.PRDATA            = S_PRDATA;
assign s_apb.PSLVERR           = S_PSLVERR;
assign S_PSEL                  = s_apb.PSEL;
assign S_PENABLE               = s_apb.PENABLE;
assign S_PWRITE                = s_apb.PWRITE;
assign S_PADDR                 = s_apb.PADDR;
assign S_PWDATA                = s_apb.PWDATA;


mem_ctrl mem_ctrl (
	.ACLK                (ACLK),
	.nRST                (nRST),
	.maxi                (maxi)
);

reg_ctrl #(
	.BASE_ADDR           (BASE_ADDR)
) reg_ctrl (
	.PCLK                (PCLK),
	.PRESETn             (PRESETn),
	.core_busy_all       (core_busy_all),
	.s_apb               (s_apb)
);

test_core #(
	.CORE_ID             (1)
) test_core_0 (
	.core_busy           (core_busy_all[0])
);

test_core #(
	.CORE_ID             (2)
) test_core_1 (
	.core_busy           (core_busy_all[1])
);

test_core #(
	.CORE_ID             (3)
) test_core_2 (
	.core_busy           (core_busy_all[2])
);

test_core #(
	.CORE_ID             (4)
) test_core_3 (
	.core_busy           (core_busy_all[3])
);

endmodule
```
 
@<b>[최종 결과 : test_core.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_core #(
	parameter CORE_ID          = 0
) (
	// core_busy
	output                     core_busy
);

// interface : ex_inst
i_inst                 ex_inst;

// interface : if_inst
i_inst                 if_inst;


core_ex core_ex (
	.ex_inst             (ex_inst),
	.if_inst             (if_inst)
);

core_if #(
	.CORE_ID             (CORE_ID)
) core_if (
	.if_inst             (if_inst)
);

core_wb core_wb (
	.ex_inst             (ex_inst)
);

endmodule
```