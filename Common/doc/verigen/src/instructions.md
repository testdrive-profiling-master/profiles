# Class and Method

There are three object types as shown below.

* clock
** You can generate clocks and assign them to interfaces. When the corresponding interface is used, the automatically assigned clock and reset matching the clock are declared to the port. If reset is not declared, the default reset nRST signal is automatically generated.
* interface
** Create an interface to be used in the module. Interfaces can be created by inheriting from other interfaces. Instances requested by add_interface to a module can only call interface_i:* functions.
* module
** You can create a module, include other submodules via the module:add_module function, or call module:add_interface to create an interface instance.

;;;

## clock
Generates or manages clocks to be assigned to interfaces. \
The assigned clock is automatically declared according to the port of the module when the corresponding interface is used.
In addition, the speed of the corresponding clock is defined in the contraint, and false_path is automatically designated for registers between heterogeneous clocks and reset set for the clock.

@<tbl:media/instruction_clock.xlsx;summary;clock object summary>

;;;

### clock:new
@<tbl:media/instruction_clock.xlsx;new>
 
ex) creation example
```lua
new_clock	= clock:new("CLK")			-- generated from the base clock
new_clock:set_reset("GRSTn")

new_clock2	= new_clock:new("ACLK")		-- Clock cloned from new_clock. It inherits reset and speed.
```
;;;

### clock:set_reset
@<tbl:media/instruction_clock.xlsx;set_reset>
  
ex) Example of specifying reset
```lua
aclock	= clock:new("ACLK")
aclock:set_reset("ARSTn")				-- Assign reset ARSTn
```
;;;

### clock:get_reset
@<tbl:media/instruction_clock.xlsx;get_reset>

;;;

### clock:set_speed
@<tbl:media/instruction_clock.xlsx;set_speed>
  
ex) Example of motion speed designation
```lua
aclock	= clock:new("ACLK")
aclock:set_speed(1000)					-- Set 1GHz to ACK
```
;;;

### clock:set_default
@<tbl:media/instruction_clock.xlsx;set_default>
  
ex) Basic clock setting example
```lua
aclock	= clock:new("MCLK")
aclock:set_default()
```
;;;

### clock.find
@<tbl:media/instruction_clock.xlsx;find>
  
ex) clock find example
```lua
aclock	= clock.find("ACLK")			-- Find ACLK

if aclock ~= nil then
	LOGI("ACLK is found.")				-- found.
end
```
;;;

### clock.is_valid
@<tbl:media/instruction_clock.xlsx;is_valid>
 
ex) Example of checking the clock object
```lua
aclock	= clock:new("ACLK")

if clock.is_valid(aclock) then
	LOGI("aclock is clock object.")		-- clock object is correct.
end
```
;;;

### clock.get_default
@<tbl:media/instruction_clock.xlsx;get_default>

;;;

## interface
The interface object works identically to systemverilog's interface technology. \
If you look at the interface syntax of systemverilog, it is as follows.

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
A detailed description of the systemverilog interface can be found in external links. \
See @<link:https://verificationguide.com/systemverilog/systemverilog-modport/;systemverilog modport description>.
 
Among them, port configuration is attempted using the modport function that can be synthesized, \
and it is largely divided into the interface_i object created through add_interface to the interface object, \
which is the basic object, and the module object.

@<tbl:media/instruction_interface.xlsx;summary;interface object summary>
 
@<tbl:media/instruction_interface.xlsx;summary_i;interface_i object summary>

;;;

### interface:new
@<tbl:media/instruction_interface.xlsx;new>
 
ex) Example of interface creation
```lua
i_apb		= interface:new("APB")		-- APB interface creation
i_apb:set_signal("RARDDR", 32)
```
;;;

### new_signal
@<tbl:media/instruction_interface.xlsx;new_signal>
 
The actual implementation inside creates a bared interface as shown below, \
setting modport 's' to input and modport 'm' to output. \
Also, because it is a bared interface, \
it is not even logged as an interface in the [top_module]_include.vh header.
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
 
ex) signal creation example
```lua
s_BUSY		= new_signal("BUSY_ALL", 4)
```
;;;

### interface.find
@<tbl:media/instruction_interface.xlsx;find>
 
ex) Example of finding an interface
```lua
i_APB		= interface:new("APB")

if interface.find("APB") ~= nil then
	LOGI("APB interface is existed.")
end
```
;;;

### interface.is_valid
@<tbl:media/instruction_interface.xlsx;is_valid>
 
ex) Example of checking interface object
```lua
i_APB	= interface:new("APB")

if interface.is_valid(i_APB) then
	LOGI("i_APB is interface object.")		-- interface object is correct.
end
```
;;;

### interface:set_clock
@<tbl:media/instruction_interface.xlsx;set_clock>
 
ex) Example of setting clock on interface object
```lua
i_APB	= interface:new("APB")

PCLK	= clock:new("PCLK", "APB's clock")
PCLK:set_reset("PRSTn")

i_APB:set_clock(PCLK)			-- PCLK setting
```
;;;

### interface:get_clock
@<tbl:media/instruction_interface.xlsx;get_clock>
 
ex) Example of setting clock on interface object
```lua
i_APB	= interface:new("APB")

PCLK	= clock:new("PCLK", "APB's clock")
PCLK:set_reset("PRSTn")

i_APB:set_clock(PCLK)

LOGI("APB's clock is " .. i_APB:get_clock().name)	-- print clock name
```
;;;

### interface:set_signal
@<tbl:media/instruction_interface.xlsx;set_signal>
 
ex) Example of adding signal to interface object
```lua
i_axi3	= interface:new("AXI3")

-- parameter setting
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- signal setting
i_axi3:set_signal("AWVALID")
i_axi3:set_signal("AWREADY")
i_axi3:set_signal("AWADDR", "ADDR_WIDTH")
i_axi3:set_signal("AWSIZE", 3)
i_axi3:set_signal("AWBURST", 2)
i_axi3:set_signal("WSTRB", "DATA_WIDTH/8")
```
;;;

### interface:signal_count
@<tbl:media/instruction_interface.xlsx;signal_count>
;;;

### interface:set_param
@<tbl:media/instruction_interface.xlsx;set_param>
 
ex) Example of adding parameter to interface object
```lua
i_axi3	= interface:new("AXI3")

-- parameter setting
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- Modify parameter
i_axi3:set_param("DATA_WIDTH", 256)
```
;;;

### interface:get_param
@<tbl:media/instruction_interface.xlsx;get_param>
 
ex) An example of getting parameters to an interface object
```lua
i_axi3	= interface:new("AXI3")

-- parameter setting
i_axi3:set_param("ADDR_WIDTH", 16)
i_axi3:set_param("DATA_WIDTH", 128)

-- get parameter and print
LOGI("i_axi3's data width = " .. tostring(i_axi3:get_param("DATA_WIDTH")))
```
;;;

### interface:set_modport
@<tbl:media/instruction_interface.xlsx;set_modport>
 
ex) set_modport example
```lua
-- some bus
some_bus 	= interface:new("some_bus")
some_bus:set_param("ADDR_WIDTH", 16)
some_bus:set_param("DATA_WIDTH", 32)
some_bus:set_signal("en")
some_bus:set_signal("addr", "ADDR_WIDTH")
some_bus:set_signal("idata", "DATA_WIDTH")
some_bus:set_signal("oe")
some_bus:set_signal("odata", "DATA_WIDTH")
some_bus:set_signal("ext")

some_bus:set_modport("m", {["input" ]={"en", "addr", "idata"}, ["output"]={"oe", "odata"}})
some_bus:set_modport("s", {["output"]={"en", "addr", "idata"}, ["input" ]={"oe", "odata"}})
```
 
All below three cases of code are identically same.

case #1) 2 seperated modport script
```lua
...
some_bus:set_modport("m", {["input" ]={"en", "addr", "idata"}, ["output"]={"oe", "odata"}})
some_bus:set_modport("s", {["output"]={"en", "addr", "idata"}, ["input" ]={"oe", "odata"}})
```
 
case #2) dual inverted modport script
```lua
...
some_bus:set_modport("m/s", {["input" ]={"en", "addr", "idata"}, ["output"]={"oe", "odata"}})
```
 
case #3) dual inverted modport script and delete 'ext' signal
```lua
...
some_bus:set_modport("m/s", {["input" ]={"en", "addr", "idata", "ext"}, ["output"]={"oe", "odata"}})
some_bus:set_modport("m/s", {["-input" ]={"ext"})	-- delete 'm' input and 's' output as named 'ext'
```

;;;

### interface:get_modport
@<tbl:media/instruction_interface.xlsx;get_modport>

ex) add_modport example
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

-- List 'input' of modeport 's'
for i, signal_name in ipairs(bus_apb:get_modport("s").input) do
	LOGI("modport 's' input : " .. signal_name)
end
```
;;;

### interface:set_prefix
@<tbl:media/instruction_interface.xlsx;set_prefix>
 
ex) set_prefix example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

inst:set_prefix("I#")	-- Signals start with I#_*.

m		= module:new("top")
m:add_interface(inst, "inst_0", "m")
m:add_interface(inst.modport.m, "inst_1")		-- same as : m:add_interface(inst, "inst_1", "m")

m:make_code()
```
 
@<b>[execution result : top_defines.vh]@<b>
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
 
@<b>[execution result : top.sv]@<b>
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
 
Used when configuring bared signals.
 
ex) set_bared example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

inst:set_bared()			-- bared interface setting
```
;;;

### interface:set_top_uppercase
@<tbl:media/instruction_interface.xlsx;set_top_uppercase>
 
;;;

### interface_i:set_port
@<tbl:media/instruction_interface.xlsx;i_set_port>
 
When a basic interface is added to a module, \
the port output (input, output, inout) is determined through this function.
 
ex) interface_i:set_port example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

top:add_interface(inst):set_port("m")	-- Set inst interface to top output as modport 'm'
```
;;;

### interface_i:set_desc
@<tbl:media/instruction_interface.xlsx;i_set_desc>
 
ex) interface_i:set_desc example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")	-- Set inst interface to top output as modport 'm'
i_int:set_desc("main instruction")	-- comment description
```
;;;

### interface_i:set_prefix
@<tbl:media/instruction_interface.xlsx;i_set_prefix>
 
ex) interface_i:set_prefix example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")		-- Set inst interface to top output as modport 'm'
i_int:set_prefix("IF")	-- Specify prefix
```
This interface instance is converted to IF_EN, IF_INST and output as port.
;;;

### interface_i:get_prefix
@<tbl:media/instruction_interface.xlsx;i_get_prefix>
 
ex) interface_i:get_prefix example
```lua
-- interface example
inst 	= interface:new("inst")
inst:set_signal("EN")
inst:set_signal("INST", 32)
inst:set_modport("s", {["input" ]={"EN", "INST"}})
inst:set_modport("m", {["output"]={"EN", "INST"}})

top			= module:new("top")

i_int = top:add_interface(inst)
i_int:set_port("m")		-- Set inst interface to top output as modport 'm'

-- default prefix output
LOGI("PREFIX : " .. i_int:get_prefix())
```
;;;

## module

An object that matches a verilog module declaration. \
It is created with the @<bookmark:module:new> function, \
and the final result source is output through the @<bookmark:module:make_code> function. \
At this time, the declaration of sub modules included in the lower level and interfaces used at least once are also made.
The port of the top module is converted into single signals in the form of input/output, \
not the systemverilog interface syntax, and the internal sub modules are described according to the interface syntax.
The sub module object added with the @<bookmark:module:add_module> function is used as the module_i interface.
 
@<tbl:media/instruction_module.xlsx;summary;module object summary>
 
@<tbl:media/instruction_module.xlsx;summary_i;module_i(sub module) object summary>
;;;

### module:new
@<tbl:media/instruction_module.xlsx;new>

ex) Module creation example
```lua
top			= module:new("top")		-- Module creation example
```
;;;

### module:make_code
@<tbl:media/instruction_module.xlsx;make_code>
;;;

### module:set_inception
@<tbl:media/instruction_module.xlsx;set_inception>
 
This inception text will be placed at the top of each .sv source. \
You can separately insert sentences such as license by specifying :set_inception, \
:set_title, :set_author functions for each module separately.
If you call it with module:set_inception, all generated modules will use the code inception of the base module unless otherwise specified.

ex) set_inception example
```lua
-- code inception setting
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
 
execution result
@<b>[result file : top.sv]@</b>
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

### module:add_document
@<tbl:media/instruction_module.xlsx;add_document>
 
This function only affects "Design map". Display it as a hyperlink in "Design map" so that users can click to edit it.

ex) add_document example
```lua
top	= module:new("top")		-- Module creation example
top:add_document("Configuration", "configuration@config.xlsx")
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
 
ex) module:set_param example
```lua
top	= module:new("top")		-- Module creation example

-- port parameter setting
top:set_param("DATA_WIDTH", 32)

-- local parameter setting
top:set_param("BYTE_WIDTH", "DATA_WIDTH/8", true)
```
;;;

### module:get_param
@<tbl:media/instruction_module.xlsx;get_param>
;;;

### module:add_interface
@<tbl:media/instruction_module.xlsx;add_interface>
;;;

ex) module:add_interface example
```lua
m = module:new("top")
m:add_interface(inst.modport.s, "inst_0")	-- slave port interface 'inst_0'
m:add_interface(inst.modport.m, "inst_1") 	-- master port interface 'inst_1'
m:add_interface(inst.modport)				-- logic interface 'inst'
```

### module:add_clock
@<tbl:media/instruction_module.xlsx;add_clock>
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
 
ex) module.apply_code example (When you want to add code to Core and ALU modules.)
```lua
module.apply_code("__core.sv")
```
 
[__core.sv]
```verilog
:Core
assign	A = B;		// Core's code
assign	C = D;		// Core's code

:ALU (config.core_size > 4)
assign	E = F;		// ALU's code
assign	G = H;		// ALU's code
wire	[15:0]	CORE_SIZE	= $(config.core_size);
```
:::NoteHeading
You can execute lua code by writing '$(*)' or '${*}' in the middle of verilog code. '$(*)' is a string or number returned code, and '${*}' can describe lua code execution without return.

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
