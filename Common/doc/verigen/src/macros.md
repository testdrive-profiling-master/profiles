# Macro functions

Provides useful macro functions for organizing in your Lua code.

@<tbl:media/instruction_macros.xlsx;summary;Macro function for Lua>

 
The functions below are predefined in-line functions as vfunction available in verilog. It can be used as "$function_name(~)".

@<tbl:media/instruction_macros.xlsx;vfunction_list;In-line functions for verilog>

;;;

## _V macro
@<tbl:media/instruction_macros.xlsx;_V>
 
ex) _V macro example
```lua
print(_V("assign A = {$(B[#],)};", 0, 3))
```
[Result]
```verilog
assign A = {B[0], B[1], B[2], B[3]};
```
;;;

## vfunction macro
@<tbl:media/instruction_macros.xlsx;vfunction>
:::Note
By default, the "_V" macro is declared as vfunction , so you can use the $_V(...) function equivalently within verilog.
 
ex) vfunction macro example
```lua
vfunction("RANGE", function(size,step)
	return ("[" .. ((size*(step+1))-1) .. ":" .. (size*step) .. "]")
end)
```
[Source input]
```verilog
wire	$RANGE(32,1)	T;
```

[Result]
```verilog
wire	[63:32]		T;
```
;;;

## verigen.set_description function
@<tbl:media/instruction_macros.xlsx;verigen.set_description>
 
ex) verigen.set_description macro example
```lua
verigen.set_description("Your description")
```
;;;

## verigen.add_reference function
@<tbl:media/instruction_macros.xlsx;verigen.add_reference>
 
ex) verigen.add_reference macro example
```lua
verigen.add_reference("__Defines.sv", "Global definitions")
```
;;;

## verigen.add_verilog function
@<tbl:media/instruction_macros.xlsx;verigen.add_verilog>
 
ex) verigen.add_verilog macro example
```lua
verigen.add_verilog("*.sv")
```
;;;

## verigen.set_constraint function
@<tbl:media/instruction_macros.xlsx;verigen.set_constraint>
 
ex) verigen.set_constraint macro example
```lua
verigen.set_constraint("Reset constraint for M/S_nRST", "set_false_path -from [get_ports M_nRST]")
verigen.set_constraint(nil, "set_false_path -from [get_ports S_nRST]")
```
 
Result: ".xdc" constraint output file
```perl
# Reset constraint for M/S_nRST
set_false_path -from [get_ports M_nRST]
set_false_path -from [get_ports S_nRST]
```
;;;

## read_excel_table function
@<tbl:media/instruction_macros.xlsx;read_excel_table>
 
ex) read_excel_table macro example
```lua
isa_table	= excel_read_table("isa_table.xlsx", "ISA", "#opcode", true, "A4")	-- read from sheet "ISA"'s "A4"
```
;;;

## verigen.set_max_showlink function
@<tbl:media/instruction_macros.xlsx;verigen.set_max_showlink>
 
ex.1) set verigen.set_max_showlink to unlimited.
```lua
verigen.set_max_showlink(0)
end)
```
 
Result:
@<img:#media/verigen.set_max_showlink_0.jpg;0.7;verigen.set_max_showlink(unlimited)>
 
ex.2) set verigen.set_max_showlink to 4.
```lua
verigen.set_max_showlink(4)
end)
```
Result:
@<img:#media/verigen.set_max_showlink_4.jpg;0.4;verigen.set_max_showlink(4)>
;;;

## $LOG2 function
@<tbl:media/instruction_macros.xlsx;LOG2>
 
ex) $LOG2 example
```lua
val_a = 16
```
[Source input]
```verilog
localparam		BITS	= $LOG2(val_a);
```

[Result]
```verilog
localparam		BITS	= 4;
```
;;;

## $DEMUX_BY_EN function
@<tbl:media/instruction_macros.xlsx;DEMUX_BY_EN>
 
ex) $DEMUX_BY_EN example
[Source input]
```verilog
wire	[31:0]		a,b,c,d;
wire	[3:0]		en;
wire	[31:0]		odata;

$DEMUX_BY_EN(32, 4, "en", "{a,b,c,d}", "odata")
```

[Result]
```verilog
wire	[31:0]		a,b,c,d;
wire	[3:0]		en;
wire	[31:0]		odata;

demux_by_enable #(
    .WIDTH              (32),
    .CHANNELS           (4),
    .TRISTATE           (1)
) demux_en_pc (
    .EN_BUS             (en),
    .DIN_BUS            ({a,b,c,d}),
    .DOUT               (odata)
);
```
;;;

## $MULTICYCLE function
@<tbl:media/instruction_macros.xlsx;MULTICYCLE>
 
ex) $MULTICYCLE example
[Source input]
```verilog
$MULTICYCLE("MTSP_Synchronize", "mtsp_sync", 2, 1)
```

[Result]
```verilog
genvar i;
// multicycle design for MTSP_Synchronize
i_mtsp_sync mtsp_sync();
wire    mtsp_sync_ie, mtsp_sync_oe, mtsp_sync_iready;
generate
wire    [7:0]   pipe_i;
wire    [1:0]   pipe_o;
wire    [1:0]   __o;

MultiCyclePathEx #(
    .IWIDTH     (8),
    .OWIDTH     (2),
    .CYCLE      (2),
    .COUNT      (1)
) multi_pipe (
    .CLK        (MCLK),
    .nRST       (nRST),
    .IE         (mtsp_sync_ie),
    .IDATA      ({mtsp_sync.sync, mtsp_sync.eop}),
    .IREADY     ({mtsp_sync_iready}),
    .PIPE_I     (pipe_i),
    .PIPE_O     (pipe_o),
    .OE         (mtsp_sync_oe),
    .ODATA      (__o)
);

assign  {mtsp_sync.awake, mtsp_sync.done}   = __o;

for(i=0; i<1; i=i+1) begin
    i_mtsp_sync     __temp;
    assign  {__temp.sync, __temp.eop}   = pipe_i[`BUS_RANGE(8, i)];
    assign  pipe_o[`BUS_RANGE(2,i)] = {__temp.awake, __temp.done};

    MTSP_Synchronize MTSP_Synchronize (
        .mtsp_sync           (__temp)
    );
end
endgenerate
```
;;;
