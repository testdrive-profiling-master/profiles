# Macro functions

코드 구성에 유용한 메크로 함수를 제공합니다.

@<tbl:media/instruction_macros.xlsx;summary;메크로 함수 요약>
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
