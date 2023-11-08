# Fast follow

This section is a quick, example-oriented explanation. \
To check the class and method in dictionary format, see the next step '@<bookmark:Class and Method>'.

The example below describes the implementation at @<link:https://github.com/testdrive-profiling-master/profiles/tree/master/Common/utils/codegen/test/verigen;github example>. \
You can achieve the same result by running do_test.bat in that folder.

## Step #1 : Creation of module

Generate and run the script code as shown below.

@<b>[main.lua file]@</b>
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

-- make code
core_wrapper:make_code()
```
It was created by putting the core_wrapper module and core related modules in @<link:https://www.lua.org/pil/3.6.html;lua table>.

@<b>[Run command]@</b>
```txt
> verigen main.lua
*I: Build TOP design : test_wrapper.sv
*W: Empty port module : 'test_wrapper' module
*I: Build contraint : test_wrapper_constraint.xdc
*I: Make design hierarchy : test_wrapper_hierarchy.svg
*I: Make common defines : test_wrapper_defines.vh
*I: Make design file list : test_wrapper.f
```

Briefly declare the modules to be used through the @<bookmark:module:new> method. \
And the last @<bookmark:module:make_code> method generates the actual verilog code, \
constraint files and hierarchy diagram. \

Currently, we have declared several modules, but since there is no module associated with core_wrapper, \
verilog design only creates one test_wrapper.sv file.

The rest of the test_wrapper_constraint.xdc, test_wrapper_defines.vh, etc. are empty.

@<b>[Result : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper ();

endmodule
```
The resulting design is literally an empty module file, and the hierarchy diagram(test_wrapper_hierarchy.svg) is also empty.

@<b>[Result : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step1_test_wrapper_hierarchy.svg;0.3;Step #1 Hierarchy Diagram>

:::NoteHeading
You can view the actual verilog codes by clicking on the module name in this image.
 

## Step #2 : module interconnections

Modify and run the Lua script as shown below.

@<b>[main.lua file]@</b>
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
core_wrapper:add_module(core.mem_ctrl)
core_wrapper:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

core.slave_ctrl:add_module(core.reg_ctrl)

-- multi-core genration
core.inst	= {}
for i = 1, config.core_size, 1 do
   core.inst[i]	= core_wrapper:add_module(core.top)
end

-- make code
core_wrapper:make_code()
```
Now, in the added lines 15 to 29, each module is connected with the @<bookmark:module:add_module> function, and four modules are also created and connected to the core. \
Include "@<bookmark:Appendix : test_definition.lua>" at the top (line #1) to use the predefined config.core_size value.

@<b>[Run command]@</b>
```txt
> verigen main.lua
*I: Build sub design : mem_ctrl.sv
*W: Empty port module : 'mem_ctrl' module
*I: Build sub design : reg_ctrl.sv
*W: Empty port module : 'reg_ctrl' module
*I: Build sub design : slave_ctrl.sv
*W: Empty port module : 'slave_ctrl' module
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
*I: Make common defines : test_wrapper_defines.vh
*I: Make design hierarchy : test_wrapper_hierarchy.svg
*I: Make design file list : test_wrapper.f
```

In the execution result, other files included in addition to the test_wrapper.sv file are automatically created, and if you look at the top design, \
only the module is added and the port is not described, so a warning is generated, but each submodule is automatically You can see what has been added.

@<b>[Result : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper ();


/* no ports in module. (commented out for DRC.)
mem_ctrl mem_ctrl (
);*/

/* no ports in module. (commented out for DRC.)
slave_ctrl slave_ctrl (
);*/

/* no ports in module. (commented out for DRC.)
test_core test_core_0 (
);*/

/* no ports in module. (commented out for DRC.)
test_core test_core_1 (
);*/

/* no ports in module. (commented out for DRC.)
test_core test_core_2 (
);*/

/* no ports in module. (commented out for DRC.)
test_core test_core_3 (
);*/
endmodule
```
:::NoteHeading
In the code above, since the submodule has no input/output at all, it is commented out to avoid 'DRC (Design Rule Check)' errors.
 
@<b>[Result : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step2_test_wrapper_hierarchy.svg;0.8;Step #2 Hierarchy Diagram>

In addition to test_wrapper.sv, other slave_ctrl.sv and test_core.sv also contain submodules, \
as seen in @<bookmark:Step #2 Hierarchy Diagram>.
 

## Step #3 : Verilog code insertion

Modify and run the Lua script as shown below.

@<b>[main.lua file]@</b>
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

-- add master bus
bus.maxi4:set_param("DATA_WIDTH", 512)
bus.maxi4:set_param("ADDR_WIDTH", 36)
bus.maxi4:set_prefix("M#")

-- add busy
core_busy		= new_signal("core_busy")

-- module connection
core_wrapper:add_module(core.mem_ctrl)
core_wrapper:add_module(core.slave_ctrl)

core.top:add_module(core.core_if)
core.top:add_module(core.core_ex)
core.top:add_module(core.core_wb)

core.slave_ctrl:add_module(core.reg_ctrl)

-- multi-core genration
core.inst	= {}
for i = 1, config.core_size, 1 do
   core.inst[i]	= core_wrapper:add_module(core.top)
end

-- add verilog codes
for entry in lfs.dir("src/") do
	local	s	= String(entry)
	if s:CompareBack(".sv") then
		module.apply_code("src/" .. entry)
	end
end

-- make code
core_wrapper:make_code()
```
The code added to the existing Lua script is line #15~21 and #39~45.
The first changes the bit width of data and address of axi4, \
and the second one adds the two files below through the module.apply_code() function to all *.sv files in the subfolder "./src".
 

@<b>[src/__wrapper.sv]@</b>
```#verilog
//#---------------------------------------------------------------------
module test_core

//#---------------------------------------------------------------------
module core_if
$set_param("CORE_ID", "0")
$add_interface(core_i.inst, "if_inst", "m")
$add_interface(core_busy, nil, "m")

assign	core_busy	= 1'b0;


//#---------------------------------------------------------------------
module core_ex
$add_interface(core_i.inst, "if_inst", "s")
$add_interface(core_i.inst, "ex_inst", "m")


//#---------------------------------------------------------------------
module core_wb
$add_interface(core_i.inst, "ex_inst", "s")
```
:::NoteHeading
As above, by declaring 'modport' paired with the name of the same type of interface through the $add_interface() function, \
the same interfaces declared in two different modules are automatically connected.


@<b>[src/__wrapper.sv]@</b>
```#verilog
//#---------------------------------------------------------------------
module test_wrapper
wire	$RANGE(config.core_size)	core_busy_all;

${	-- It's Lua codes
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
```
:::NoteHeading
You can connect directly through the "@<bookmark:module_i:set_param>()" and "@<bookmark:module_i:set_port>()" functions without automatically connecting interfaces or parameters.

After declaring "@<color:0000FF>module@</color> @<color:FF0000><module_name>@</color>"(The use of "@<color:0000FF>endmodule@</color>" can be omitted.) in the added .sv file, the file can be described using both Verilog and Lua grammars. \
You can either declare I/O via @@set_param() function and @@@<bookmark:module:add_interface>() function, respectively, or use Verilog syntax directly.
In addition, if you want to directly access a Lua variable or function, you can access it with $(*), or you can execute a Lua statement by describing it with ${*}.


@<b>[Result : test_wrapper_hierarchy.svg]@</b>
@<img:#media/step3_test_wrapper_hierarchy.svg;1.0;Step #3 Hierarchy Diagram>

Below is the output of the top design.

@<b>[Final result : test_wrapper.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_wrapper #(
	parameter BASE_ADDR        = 32'h10000000
) (
	// clock & reset
	input                      ACLK,        // AXI clock
	input                      CLK,         // main clock
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

	// s_apb
	input                      S0_PREADY,
	input  [31:0]              S0_PRDATA,
	input                      S0_PSLVERR,
	output [1:0]               S0_PSEL,
	output                     S0_PENABLE,
	output                     S0_PWRITE,
	output [15:0]              S0_PADDR,
	output [31:0]              S0_PWDATA,

	// s_apb_0
	input                      S1_PREADY,
	input  [31:0]              S1_PRDATA,
	input                      S1_PSLVERR,
	output [1:0]               S1_PSEL,
	output                     S1_PENABLE,
	output                     S1_PWRITE,
	output [15:0]              S1_PADDR,
	output [31:0]              S1_PWDATA
);

// synopsys template
// interface : maxi
i_maxi4                maxi();
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

// interface : s_apb
i_apb                  s_apb();
assign s_apb.PREADY            = S0_PREADY;
assign s_apb.PRDATA            = S0_PRDATA;
assign s_apb.PSLVERR           = S0_PSLVERR;
assign S0_PSEL                 = s_apb.PSEL;
assign S0_PENABLE              = s_apb.PENABLE;
assign S0_PWRITE               = s_apb.PWRITE;
assign S0_PADDR                = s_apb.PADDR;
assign S0_PWDATA               = s_apb.PWDATA;

// interface : s_apb_0
i_apb                  s_apb_0();
assign s_apb_0.PREADY          = S1_PREADY;
assign s_apb_0.PRDATA          = S1_PRDATA;
assign s_apb_0.PSLVERR         = S1_PSLVERR;
assign S1_PSEL                 = s_apb_0.PSEL;
assign S1_PENABLE              = s_apb_0.PENABLE;
assign S1_PWRITE               = s_apb_0.PWRITE;
assign S1_PADDR                = s_apb_0.PADDR;
assign S1_PWDATA               = s_apb_0.PWDATA;


mem_ctrl mem_ctrl (
	.ACLK                (ACLK),
	.nRST                (nRST),
	.maxi                (maxi)
);

slave_ctrl #(
	.BASE_ADDR           (BASE_ADDR)
) slave_ctrl (
	.PCLK                (PCLK),
	.PRESETn             (PRESETn),
	.core_busy           (|core_busy_all),
	.s_apb               (s_apb),
	.s_apb_0             (s_apb_0)
);

test_core #(
	.CORE_ID             (1)
) test_core_0 (
	.CLK                 (CLK),
	.nRST                (nRST),
	.core_busy           (core_busy_all[0])
);

test_core #(
	.CORE_ID             (2)
) test_core_1 (
	.CLK                 (CLK),
	.nRST                (nRST),
	.core_busy           (core_busy_all[1])
);

test_core #(
	.CORE_ID             (3)
) test_core_2 (
	.CLK                 (CLK),
	.nRST                (nRST),
	.core_busy           (core_busy_all[2])
);

test_core #(
	.CORE_ID             (4)
) test_core_3 (
	.CLK                 (CLK),
	.nRST                (nRST),
	.core_busy           (core_busy_all[3])
);

wire	[3:0]	core_busy_all;



endmodule
```
 
@<b>[Final result : test_core.sv]@</b>
```verilog
`include "test_wrapper_defines.vh"

module test_core #(
	parameter CORE_ID          = 0
) (
	// clock & reset
	input                      CLK,         // main clock
	input                      nRST,        // default global reset (active low)

	// core_busy
	output logic               core_busy
);

// synopsys template
// interface : ex_inst
i_inst                 ex_inst();

// interface : if_inst
i_inst                 if_inst();


core_ex core_ex (
	.CLK                 (CLK),
	.nRST                (nRST),
	.ex_inst             (ex_inst),
	.if_inst             (if_inst)
);

core_if #(
	.CORE_ID             (CORE_ID)
) core_if (
	.CLK                 (CLK),
	.nRST                (nRST),
	.core_busy           (core_busy),
	.if_inst             (if_inst)
);

core_wb core_wb (
	.CLK                 (CLK),
	.nRST                (nRST),
	.ex_inst             (ex_inst)
);
endmodule
```