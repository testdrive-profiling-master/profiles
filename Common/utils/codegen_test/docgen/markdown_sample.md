[외부 문서의 시작입니다.]

### 하위 문서 title #1
* MD 문서입니다.
** 하위 목차 1
** 하위 목차 2
** 하위 목차 3

### 하위 문서 title #2
@<u>DocGen@</u>은 @<b>@<i>"Lua"@</b>@</i>의 '@<color:FF0000>프로그래머블(directive, string manipulation, excel 동기 등...)의 확장성@</color>'과, @<b>@<i>"DocX 템플릿"@</b>@</i>의 '@<color:0000FF>서식 변경 편이성@</color>'을 갖추고 있으며, markdown 의 단순함과 HTML 의 강력한 표현식, M$ 의 sub document 기능을 제공하여 복잡한 신규 표현도 그대로 이용할 수 있습니다.
@<i>@<color:3F3F3F>@<s>뭔소린지 몰라도@</s>@</color>@</i> 써먹기 괜찮다고 할 수 있음.

### 수식
단일 수식 표현입니다.
$$\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$

문장 안의 수식 표현 $$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$ 입니다.
=== 주석 표현입니다.
ㅁㄴㅇㄹ

### 코드 표현
```#v
=== 주석 표현입니다.
`ifndef __TESTDRIVE_SRAM_DUAL_V__
`define __TESTDRIVE_SRAM_DUAL_V__
`timescale 1ns/1ns

module SRAM_Dual #(
	parameter	TITLE				= "untitled",
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 4
) (
	// System
	input							CLK,			// clock
	// SRAM interface
	input							nCE,			// Chip enable (active low)
	input							nWE,			// write enable (active low)
	input	[(ADDR_WIDTH-1):0]		WADDR,			// write address
	input	[(ADDR_WIDTH-1):0]		RADDR,			// read address
	input	[(DATA_WIDTH-1):0]		DIN,			// input data
	output	reg	[(DATA_WIDTH-1):0]	DOUT			// output data
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0];

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nCE) begin
		if(!nWE)
			mem[WADDR]	<= DIN;
		DOUT		<= mem[RADDR];
	end
end

endmodule

`endif//__TESTDRIVE_SRAM_DUAL_V__
```
임베디드 이미지 입니다.
@<img:#test.jpg;0.5;Test image is embedded>

그림 구현은 @<bookmark:Test image is embedded>를 참조하세요.
그림 "Test image is embedded"의 페이지 번호는 @<bookmark:&Test image is embedded> 입니다.
