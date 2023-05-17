# Class and Method

아래와 같이 총 3개의 객체 종류가 존재합니다.

* clock
** 클럭을 생성하고, interface 에 할당할 수 있습니다. 해당 interface 가 사용될 때, 자동으로 할당된 클럭과 클럭에 매칭되는 reset 이 port 로 선언됩니다. 만약 reset 이 선언되어 있지 않는다면, 자동으로 기본 리셋 nRST 신호가 생성됩니다.
* interface
** module 에서 사용할 interface 를 생성합니다. interface 는 다른 interface 로 부터 상속을 받아 생성될 수 있습니다. 모듈에 add_interface 로 부탁된 instance 는 interface_i:* 함수만 호출 가능합니다.
* module
** 모듈을 생성하고, module:add_module 함수를 통해 다른 하위 모듈을 포함하거나, module:add_interface 를 호출하여 interface instance 를 생성할 수 있습니다.

;;;

## clock
interface 에 할당할 클럭을 생성하거나 관리합니다. \
할당된 클럭은 해당 interface 가 사용될 때 자동으로 module의 port에 따라 선언되게 됩니다.
또한 contraint 에 해당 클럭의 speed 가 정의되며, 이종 클럭간의 register 들과 클럭에 설정된 reset 에 대해서도 false_path 를 자동으로 지정하게 됩니다.

@<tbl:media/instruction_clock.xlsx;summary;clock 객체 요약>

;;;

### clock:new
@<tbl:media/instruction_clock.xlsx;new>
 
ex) 생성 예시
```lua
new_clock	= clock:new("CLK")			-- 기본 클럭으로 부터 생성
new_clock:set_reset("GRSTn")

new_clock2	= new_clock:new("ACLK")		-- new_clock을 복제한 클럭. reset, speed 를 상속 받는다.
```
;;;

### clock:set_reset
@<tbl:media/instruction_clock.xlsx;set_reset>
  
ex) reset 지정 예시
```lua
aclock	= clock:new("ACLK")
aclock:set_reset("ARSTn")				-- 리셋 ARSTn 지정
```
;;;

### clock:set_speed
@<tbl:media/instruction_clock.xlsx;set_speed>
  
ex) 동작 속도 지정 예시
```lua
aclock	= clock:new("ACLK")
aclock:set_speed(1000)					-- ACK에 1GHz 설정
```
;;;

### clock.find
@<tbl:media/instruction_clock.xlsx;find>
  
ex) clock 찾기 예시
```lua
aclock	= clock.find("ACLK")			-- ACLK 찾기

if aclock ~= nil then
	LOGI("ACLK is found.")				-- 찾았음.
end
```
;;;

### clock.is_valid
@<tbl:media/instruction_clock.xlsx;is_valid>
 
ex) clock 객체 확인 예시
```lua
aclock	= clock:new("ACLK")

if clock.is_valid(aclock) then
	LOGI("aclock is clock object.")		-- clock 객체가 맞음.
end
```
;;;

## interface
interface 객체는 systemverilog 의 interface 기술과 동일하게 작동합니다. \
systemverilog 의 interface 문법을 살펴보면 아래와 같습니다.

@<b>[systemverilog interface 선언]@</b>
```verilog
interface my_intface;
	logic			a;
	logic	[3:0]	b;

	// modport example
	modport s (input a, output b);		// slave modport
	modport m (input a, input  b);		// master modport
endinterface
```
:::NoteHeading
systemverilog interface의 상세한 설명은 외부링크 \
@<link:https://verificationguide.com/systemverilog/systemverilog-modport/;systemverilog modport 설명>을 참조 바랍니다.
 
그 중 합성 가능한 modport 기능을 사용하여, port 구성을 시도하며, \
기본 객체인 interface 와 \
module 객체에 add_interface 를 통해 생성된 interface_i 객체로 크게 나뉩니다.

@<tbl:media/instruction_interface.xlsx;summary;interface 객체 요약>
 
@<tbl:media/instruction_interface.xlsx;summary_i;interface_i 객체 요약>

;;;

### interface:new
@<tbl:media/instruction_interface.xlsx;new>
 
ex) interface 생성 예시
```lua
i_apb		= interface:new("APB")		-- APB interface 생성
i_apb:set_signal("RARDDR", 32)
```
;;;

### new_signal
@<tbl:media/instruction_interface.xlsx;new_signal>
 
내부의 실제 구현은 아래와 같이 bared interface 를 생성하고, \
modport 's'는 input으로 modport 'm'은 output으로 설정합니다. \
또한 bared interface 이기 때문에 [top_module]_include.vh 헤더에도 \
interface 로 기록되지 않습니다.

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
 
ex) signal 생성 예시
```lua
s_BUSY		= new_signal("BUSY_ALL", 4)
```
;;;

### interface.find
@<tbl:media/instruction_interface.xlsx;find>
 
ex) interface 찾기 예시
```lua
i_APB		= interface:new("APB")

if interface.find("APB") ~= nil then
	LOGI("APB interface is existed.")
end
```
;;;

### interface.is_valid
@<tbl:media/instruction_interface.xlsx;is_valid>
 
ex) interface 객체 확인 예시
```lua
i_APB	= interface:new("APB")

if interface.is_valid(i_APB) then
	LOGI("i_APB is interface object.")		-- interface 객체가 맞음.
end
```
;;;

### interface:set_clock
@<tbl:media/instruction_interface.xlsx;set_clock>
 
ex) interface 객체에 클럭 설정 예시
```lua
i_APB	= interface:new("APB")

PCLK	= clock:new("PCLK", "APB's clock")
PCLK:set_reset("PRSTn")

i_APB:set_clock(PCLK)			-- PCLK 설정
```
;;;

### interface:get_clock
@<tbl:media/instruction_interface.xlsx;get_clock>
 
ex) interface 객체에 클럭 설정 예시
```lua
i_APB	= interface:new("APB")

PCLK	= clock:new("PCLK", "APB's clock")
PCLK:set_reset("PRSTn")

i_APB:set_clock(PCLK)

LOGI("APB's clock is " .. i_APB:get_clock().name)	-- 클럭 이름 출력
```
;;;

### interface:set_signal
@<tbl:media/instruction_interface.xlsx;set_signal>
 
ex) interface 객체에 signal 추가 예시
```lua
i_axi3	= interface:new("AXI3")

-- parameter 설정
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- signal 설정
i_axi3:set_signal("AWVALID")
i_axi3:set_signal("AWREADY")
i_axi3:set_signal("AWADDR", "ADDR_WIDTH")
i_axi3:set_signal("AWSIZE", 3)
i_axi3:set_signal("AWBURST", 2)
i_axi3:set_signal("WSTRB", "DATA_WIDTH/8")
```
;;;

### interface:set_param
@<tbl:media/instruction_interface.xlsx;set_param>
 
ex) interface 객체에 parameter 추가 예시
```lua
i_axi3	= interface:new("AXI3")

-- parameter 설정
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- prameter 수정
i_axi3:set_param("DATA_WIDTH", 256)
```
;;;

### interface:get_param
@<tbl:media/instruction_interface.xlsx;get_param>
 
ex) interface 객체에 parameter 얻기 예시
```lua
i_axi3	= interface:new("AXI3")

-- parameter 설정
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- parameter 얻어 출력하기
LOGI("i_axi3's data width = " .. tostring(i_axi3:get_param("DATA_WIDTH")))
```
;;;

### interface:set_modport
@<tbl:media/instruction_interface.xlsx;set_modport>
 
ex) set_modport 예시
```lua
-- APB bus
bus_apb 	= interface:new("apb")
bus_apb:set_param("ADDR_WIDTH", 16)
bus_apb:set_param("DATA_WIDTH", 32)
bus_apb:set_param("SEL_WIDTH", 2)
bus_apb:set_signal("PADDR", "ADDR_WIDTH")
bus_apb:set_signal("PSEL", "SEL_WIDTH")
bus_apb:set_signal("PENABLE")
bus_apb:set_signal("PWRITE")
bus_apb:set_signal("PWDATA", "DATA_WIDTH")
bus_apb:set_signal("PREADY")
bus_apb:set_signal("PRDATA", "DATA_WIDTH")
bus_apb:set_signal("PSLVERR")

bus_apb:set_modport("s", {["input" ]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["output"]={"PREADY", "PRDATA", "PSLVERR"}})
bus_apb:set_modport("m", {["output"]={"PSEL", "PENABLE", "PWRITE", "PADDR", "PWDATA"}, ["input" ]={"PREADY", "PRDATA", "PSLVERR"}})
```
;;;

### interface:add_modport
@<tbl:media/instruction_interface.xlsx;add_modport>
 
ex) add_modport 예시
```lua
-- APB bus
bus_apb 	= interface:new("apb")
bus_apb:set_param("ADDR_WIDTH", 16)
bus_apb:set_param("DATA_WIDTH", 32)
bus_apb:set_param("SEL_WIDTH", 2)
bus_apb:set_signal("PADDR", "ADDR_WIDTH")
bus_apb:set_signal("PSEL", "SEL_WIDTH")
bus_apb:set_signal("PENABLE")
bus_apb:set_signal("PWRITE")
bus_apb:set_signal("PWDATA", "DATA_WIDTH")
bus_apb:set_signal("PREADY")
bus_apb:set_signal("PRDATA", "DATA_WIDTH")
bus_apb:set_signal("PSLVERR")

bus_apb:set_modport("s", {["input" ]={"PSEL", "PENABLE", "PWRITE"}, ["output"]={"PREADY", "PRDATA", "PSLVERR"}})
bus_apb:set_modport("m", {["output"]={"PSEL", "PENABLE", "PWRITE"}, ["input" ]={"PREADY", "PRDATA", "PSLVERR"}})

bus_apb:add_modport('s', {["input" ]={"PADDR", "PWDATA"}})
bus_apb:add_modport('m', {["output"]={"PADDR", "PWDATA"}})
```
;;;

### interface:get_modport
@<tbl:media/instruction_interface.xlsx;get_modport>

ex) add_modport 예시
```lua
-- APB bus
bus_apb 	= interface:new("apb")
bus_apb:set_param("ADDR_WIDTH", 16)
bus_apb:set_param("DATA_WIDTH", 32)
bus_apb:set_param("SEL_WIDTH", 2)
bus_apb:set_signal("PADDR", "ADDR_WIDTH")
bus_apb:set_signal("PSEL", "SEL_WIDTH")
bus_apb:set_signal("PENABLE")
bus_apb:set_signal("PWRITE")
bus_apb:set_signal("PWDATA", "DATA_WIDTH")
bus_apb:set_signal("PREADY")
bus_apb:set_signal("PRDATA", "DATA_WIDTH")
bus_apb:set_signal("PSLVERR")

bus_apb:set_modport("s", {["input" ]={"PSEL", "PENABLE", "PWRITE"}, ["output"]={"PREADY", "PRDATA", "PSLVERR"}})
bus_apb:set_modport("m", {["output"]={"PSEL", "PENABLE", "PWRITE"}, ["input" ]={"PREADY", "PRDATA", "PSLVERR"}})

-- modeport 's' 의 'input' 나열
for i, signal_name in ipairs(bus_apb:get_modport("s").input) do
	LOGI("modport 's' input : " .. signal_name)
end
```
;;;

### interface:set_prefix
@<tbl:media/instruction_interface.xlsx;set_prefix>
 
ex) set_prefix 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

inst:set_prefix("I#")	-- 시그널에 I#_* 로 시작하게 된다.

m		= module:new("top")
m:add_interface(inst, "inst_0", "m")
m:add_interface(inst, "inst_1", "m")

m:make_code()
```
 
@<b>[실행 결과 : top_defines.vh]@<b>
```#verilog
`ifndef __TOP_DEFINES_VH__
`define __TOP_DEFINES_VH__
`include "testdrive_system.vh"		// default system defines

//---------------------------------------------
// interfaces
//---------------------------------------------
interface i_inst;
	logic                 EN;
	logic [31:0]          INST;
	modport m (
		output  EN, INST
	);
	modport s (
		input   EN, INST
	);
endinterface

`endif//__TOP_DEFINES_VH__
```
 
@<b>[실행 결과 : top.sv]@<b>
```#verilog
`include "top_defines.vh"

module top (
	// inst_0
	output                     I0_EN,
	output [31:0]              I0_INST,

	// inst_1
	output                     I1_EN,
	output [31:0]              I1_INST
);

// interface : inst_0
i_inst                 inst_0;
assign I0_EN                   = inst_0.EN;
assign I0_INST                 = inst_0.INST;

// interface : inst_1
i_inst                 inst_1;
assign I1_EN                   = inst_1.EN;
assign I1_INST                 = inst_1.INST;


endmodule
```
;;;

### interface:set_bared
@<tbl:media/instruction_interface.xlsx;set_bared>
 
bared signal 구성시 사용됩니다.
 
ex) set_bared 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

inst:set_bared()			-- bared interface 설정
```
;;;

### interface_i:set_port
@<tbl:media/instruction_interface.xlsx;i_set_port>
 
기본적인 interface 를 module에 추가하였을 때, \
이 함수를 통해 port 출력(input, output, inout)이 결정되게 됩니다.
 
ex) interface_i:set_port 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

top:add_interface(inst):set_port("m")	-- inst interface를 modport 'm' 으로 top 출력 지정
```
;;;

### interface_i:set_desc
@<tbl:media/instruction_interface.xlsx;i_set_desc>
 
ex) interface_i:set_desc 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")	-- inst interface를 modport 'm' 으로 top 출력 지정
i_int:set_desc("main instruction")	-- 주석 설명
```
;;;

### interface_i:set_prefix
@<tbl:media/instruction_interface.xlsx;i_set_prefix>
 
ex) interface_i:set_prefix 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")		-- inst interface를 modport 'm' 으로 top 출력 지정
i_int:set_prefix("IF")	-- prefix 지정
```
이 interface instance 는 IF_EN, IF_INST 로 전환되어 port 로 출력됩니다.
;;;

### interface_i:get_prefix
@<tbl:media/instruction_interface.xlsx;i_get_prefix>
 
ex) interface_i:get_prefix 예시
```lua
-- interface 예제
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")		-- inst interface를 modport 'm' 으로 top 출력 지정

-- 기본 설정된 prefix 출력
LOGI("PREFIX : " .. i_int:get_prefix())
```
;;;

## module

verilog module 선언과 매칭되는 객체입니다. \
@<bookmark:module:new> 함수로 생성되며, @<bookmark:module:make_code> 함수를 통해 \
최종 결과 소스를 출력합니다. \
이때 하위에 포함된 sub module 들과 한번이라도 사용된 interface 들의 선언도 함께 이루워집니다.
top module 의 port 는 systemverilog 의 interface 문법이 아닌 input/output 형태의 단일 signal들로 \
전환되어 출력되며, 내부 sub module 들은 interface 문법에 따라 기술됩니다.
@<bookmark:module:add_module>함수로 추가된 sub module 객체는 module_i 인터페이스로 사용하게 됩니다.
 
@<tbl:media/instruction_module.xlsx;summary;module 객체 요약>
 
@<tbl:media/instruction_module.xlsx;summary_i;module_i(sub module) 객체 요약>
;;;

### module:new
@<tbl:media/instruction_module.xlsx;new>

ex) module 생성 예시
```lua
top			= module:new("top")		-- 모듈 생성 예
```
;;;

### module:make_code
@<tbl:media/instruction_module.xlsx;make_code>
;;;

### module:set_inception
@<tbl:media/instruction_module.xlsx;set_inception>
 
이 inception 문구는 각 .sv 소스의 상단에 위치하게 됩니다. \
각 module 마다 별도로 :set_inception, :set_title, :set_author 함수로 지정하여 별도로 license 와 같은 \
문장을 삽입할 수 있습니다.
만약 module:set_inception 으로 호출하면, 모든 생성 모듈에서 별도로 지정하지 않는 한 기본 모듈의 code inception을 사용하게 됩니다.

ex) set_inception 예시
```lua
-- code inception 설정
module:set_inception("code_inception.txt")
module:set_title("some title")
module:set_author("me")

top			= module:new("top")

top:make_code()
```
 
@<b>[code_inception.txt]@</b>
```verilog
//================================================================================
// Copyright (c) 2013 ~ __YEAR__. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Title : __TITLE__
// Rev.  : __DATE__ __TIME__ (__AUTHOR__)
//================================================================================
```
 
실행 결과
@<b>[결과 파일 : top.sv]@</b>
```verilog
//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Title : some title
// Rev.  : May/09/2023 Tue 14:10:16 (me)
//================================================================================
`include "top_defines.vh"

module top ();


endmodule
```
;;;

### module:get_inception
@<tbl:media/instruction_module.xlsx;get_inception>
;;;

### module:set_title
@<tbl:media/instruction_module.xlsx;set_title>
;;;

### module:set_author
@<tbl:media/instruction_module.xlsx;set_author>
;;;

### module:set_param
@<tbl:media/instruction_module.xlsx;set_param>
 
ex) module:set_param 예시
```lua
top	= module:new("top")		-- 모듈 생성 예

-- port parameter 설정
top:set_param("DATA_WIDTH", 32)

-- local parameter 설정
top:set_param("BYTE_WIDTH", "DATA_WIDTH/8", true)
```
;;;

### module:get_param
@<tbl:media/instruction_module.xlsx;get_param>
;;;

### module:add_interface
@<tbl:media/instruction_module.xlsx;add_interface>
;;;

### module:get_interface
@<tbl:media/instruction_module.xlsx;get_interface>
;;;

### module:get_port
@<tbl:media/instruction_module.xlsx;get_port>
;;;

### module:add_module
@<tbl:media/instruction_module.xlsx;add_module>
;;;

### module:get_module
@<tbl:media/instruction_module.xlsx;get_module>
;;;

### module:add_code
@<tbl:media/instruction_module.xlsx;add_code>
;;;

### module.find
@<tbl:media/instruction_module.xlsx;find>
;;;

### module.is_valid
@<tbl:media/instruction_module.xlsx;is_valid>
;;;

### module.apply_code
@<tbl:media/instruction_module.xlsx;apply_code>
 
ex) module.apply_code 예시 (Core, ALU 모듈에 코드를 추가하고자 할 때.)
```lua
module.apply_code("__core.sv")
```
 
[__core.sv]
```verilog
:Core
assign	A = B;		// Core's code
assign	C = D;		// Core's code

:ALU
assign	E = F;		// ALU's code
assign	G = H;		// ALU's code
```
;;;

### module_i:set_param
@<tbl:media/instruction_module.xlsx;i_set_param>
;;;

### module_i:get_param
@<tbl:media/instruction_module.xlsx;i_get_param>
;;;

### module_i:set_port
@<tbl:media/instruction_module.xlsx;i_set_port>
;;;

### module_i:get_port
@<tbl:media/instruction_module.xlsx;i_get_port>
;;;

### module_i.is_valid
@<tbl:media/instruction_module.xlsx;i_is_valid>
;;;
