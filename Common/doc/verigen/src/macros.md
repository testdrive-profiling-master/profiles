# Macro functions

코드 구성에 유용한 메크로 함수를 제공합니다.

@<tbl:media/instruction_macros.xlsx;summary;메크로 함수 요약>

 
아래 함수는 verilog에서 사용 가능한 vfunction으로 미리 정의된 함수들입니다. "$함수명(~)"으로 사용할 수 있습니다.

@<tbl:media/instruction_macros.xlsx;vfunction_list;미리 정의된 vfunction 목록>

;;;

## _V 메크로
@<tbl:media/instruction_macros.xlsx;_V>
 
ex) _V 메크로 예시
```lua
print(_V("assign A = {$(B[#],)};", 0, 3))
```
[출력]
```verilog
assign A = {B[0], B[1], B[2], B[3]};
```
;;;

## vfunction 메크로
@<tbl:media/instruction_macros.xlsx;vfunction>
:::NoteHeading
기본적으로 "_V" 메크로가 vfunction 으로 선언되어 있어서, verilog 내에서 $_V(...) 함수를 동일하게 사용할 수 있습니다.
 
ex) vfunction 메크로 예시
```lua
vfunction("RANGE", function(size,step)
	return ("[" .. ((size*(step+1))-1) .. ":" .. (size*step) .. "]")
end)
```
[소스 입력]
```verilog
wire	$RANGE(32,1)	T;
```

[출력]
```verilog
wire	[63:32]		T;
```
;;;

## $LOG2 함수
@<tbl:media/instruction_macros.xlsx;LOG2>
 
ex) $LOG2 예시
```lua
val_a = 16
```
[소스 입력]
```verilog
localparam		BITS	= $LOG2(val_a);
```

[출력]
```verilog
localparam		BITS	= 4;
```
;;;

## $DEMUX_BY_EN 함수
@<tbl:media/instruction_macros.xlsx;DEMUX_BY_EN>
 
ex) $DEMUX_BY_EN 예시
[소스 입력]
```verilog
wire	[31:0]		a,b,c,d;
wire	[3:0]		en;
wire	[31:0]		odata;

$DEMUX_BY_EN(32, 4, "en", "{a,b,c,d}", "odata")
```

[출력]
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

## $MULTICYCLE 함수
@<tbl:media/instruction_macros.xlsx;MULTICYCLE>
 
ex) $MULTICYCLE 예시
[소스 입력]
```verilog
$MULTICYCLE("MTSP_Synchronize", "mtsp_sync", 2, 1)
```

[출력]
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
