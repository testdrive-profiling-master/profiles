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
