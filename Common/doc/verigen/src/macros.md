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
