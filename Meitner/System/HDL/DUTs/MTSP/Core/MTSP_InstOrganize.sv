//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
//             Phase #1로 넘어갈때 31번은 명령어 Enable(active low)로 변경되며,
//             30번 비트는 Arithmetic Enable(active low) 신호로 교체된다.
//----------------------------------------------------------------------------
`include "MTSP_Defines.vh"

module MTSP_InstOrganize(
	// System
	input							CLK, nRST,						// main clock & reset (active low)
	// Input
	input							nEN,							// enable (active low)
	input	[`RANGE_PC]				PC,								// PC
	input	[`RANGE_UINSTx4]		UINSTx4,						// 4 unit instructions end & phase bit
	// Output
	output	reg [`RANGE_PC]			PC_OUT,							// PC output
	output	reg [`RANGE_UINSTx4]	UINSTx4_OUT,					// phase instructions
	// source read
	output	reg						SRC0A_nEN,						// source enable phase #0-A (active low)
	output	reg [`RANGE_GPRs]		SRC0A_ADDR						// source address phase #0-A
);

// definition & assignment ---------------------------------------------------
wire	[`RANGE_UINST]			uin_0, uin_1, uin_2, uin_3;		// input unit instruction
wire	[`RANGE_UINST]			uog_0, uog_1, uog_2, uog_3;		// organized unit instruction
wire	[`RANGE_UINST]			up0_m, up0_s, up1_m, up1_s;		// phase unit instruction (p#0_main/sub, p#1_main/sub)
wire	[2:0]					iend;							// instruction end bit
wire	[1:0]					phase;							// phase bit
wire	[1:0]					p0_nEN, p1_nEN,					// Phase enable
p0_DEC_nEN, p1_DEC_nEN,			// unit instruction decode enable
p0_ALU_nEN, p1_ALU_nEN;			// alu operation enable
// implementation ------------------------------------------------------------
// step #0 : separate unit instruction
// 4개의 명령어를 분해.
assign	{uin_0, uin_1, uin_2, uin_3}	= UINSTx4;

// step #1 : re-arrange unit instruction
// PC값 하위 2비트에 따라 메모리 순서에 맞게 명령어 순서 재정렬
assign	{uog_0, uog_1, uog_2, uog_3}	= (PC[1:0]==2'd0) ? {uin_0, uin_1, uin_2, uin_3} :	// 0 : 0,1,2,3
(PC[1:0]==2'd1) ? {uin_1, uin_2, uin_3, uin_0} :	// 1 : 1,2,3,0
(PC[1:0]==2'd2) ? {uin_2, uin_3, uin_0, uin_1} :	// 2 : 2,3,0,1
{uin_3, uin_0, uin_1, uin_2};	// 3 : 3,0,1,2

// step #2 : check end & phase bits
// END, PHASE 비트 뽑아냄. 이때 마지막 END 비트 무효. 마지막 2개 페이즈비트 무효됨.
assign	iend					= {uog_2[`RANGE_UINST_END]|iend[1],
				   uog_1[`RANGE_UINST_END]|iend[0],
				   uog_0[`RANGE_UINST_END]};
assign	phase					= {uog_1[`RANGE_UINST_PHASE],
					uog_0[`RANGE_UINST_PHASE]};

// step #4 : set phase #0 unit instructions
// Phase #0 명령어 분리
assign	p0_nEN					= {(phase[1] | iend[0]),
					 phase[0]};
assign	{up0_m, up0_s}			= {uog_0, uog_1};

// step #5 : set phase #1 unit instructions
// Phase #1 명령어 분리
assign	p1_nEN					= phase[0] ? {iend[0], 1'b0} :
phase[1] ? iend[1:0] : iend[2:1];

assign	{up1_m, up1_s}			= phase[0] ? {uog_0, uog_1} :
phase[1] ? {uog_1, uog_2} : {uog_2, uog_3};

// decode/alu enables
assign	p0_DEC_nEN				= {nEN|p0_nEN[1], nEN|p0_nEN[0]},
p1_DEC_nEN				= {nEN|p1_nEN[1], nEN|p1_nEN[0]},
p0_ALU_nEN				= {p0_DEC_nEN[1]|(&up0_s[29:28]), p0_DEC_nEN[0]|((&up0_m[29:28]) & (up0_m[`RANGE_UINST_MO]!=`MO_ADDR))},
p1_ALU_nEN				= {p1_DEC_nEN[1]|(&up1_s[29:28]), p1_DEC_nEN[0]|(&up1_m[29:28])};

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		PC_OUT			<= 'd0;
		UINSTx4_OUT		<= 128'hFFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF;
		SRC0A_nEN		<= `nFALSE;
		SRC0A_ADDR		<= 0;
	end
	else begin
		// STEP #3 : PC값을 END 비트에 맞게 증가함
		PC_OUT			<= PC + {~iend[2],(iend[2]&~iend[0]),((iend[2]&~iend[1])|iend[0])}; /* (iend[0] ? 1 : iend[1] ? 2 : iend[2] ? 3 : 4);*/
		// STEP #6 : set output unit instructions
		// 명령어 패치		decode enable  alu enable                  decode enable  alu enable
		UINSTx4_OUT		<= {p0_DEC_nEN[0], p0_ALU_nEN[0], up0_m[29:0], p0_DEC_nEN[1], p0_ALU_nEN[1], up0_s[29:0],
						 p1_DEC_nEN[0], p1_ALU_nEN[0], up1_m[29:0], p1_DEC_nEN[1], p1_ALU_nEN[1], up1_s[29:0]};
		// Source phase#0-A
		SRC0A_nEN		<= nEN | phase[0];
		SRC0A_ADDR		<= {(p0_ALU_nEN[1] ? 2'b0 : up0_s[`RANGE_UINST_SI_A]), up0_m[`RANGE_UINST_SRC]};
	end
end

endmodule
