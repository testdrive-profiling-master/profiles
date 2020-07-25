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
`include "MTSP_Defines.vh"

module MTSP_ALU_FLFR(	CLK, nRST,
					  MO0, MO0_MASK, SRC0A, SRC0B,
					  MO1, MO1_MASK, SRC1A, SRC1B,
					  PHASE_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input					CLK, nRST;				// main clock & reset (active low)
// input
input	[`RANGE_MODESC]	MO0, MO1;				// phase #0/#1 micro-operation description
input					MO0_MASK, MO1_MASK;		// phase #0/#1 micro-operation mask
input	[`RANGE_DWORD]	SRC0A, SRC0B,			// phase #0 source data
SRC1A, SRC1B;			// phase #1 source data
// output
output	[1:0]			PHASE_EN;				// output enable
output	[`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]	DIN;
reg		[`RAMGE_MOEXT]	moext_sm, moext_ex0;
wire					MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_FLFR),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_FLFR),
MO_en	= MO0_en | MO1_en;
// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN				<= 0;
		moext_sm		<= 0;
	end
	else begin
		DIN				<= MO_en ? (MO1_en ? (MO1_MASK ? SRC1B : SRC1A) : (MO0_MASK ? SRC0B : SRC0A)) : DIN;
		moext_sm[`RANGE_MOEXT_EN]		<= MO_en;
		moext_sm[`RANGE_MOEXT_PHASE]	<= MO1_en;	// phase #1 priority is high
		moext_sm[`RANGE_MOEXT_OP]		<= MO_en ? (MO1_en ? MO1[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL] : MO0[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL]) : moext_sm[`RANGE_MOEXT_OP];
	end
end

// stage #1 : EX0 -----------------------
wire						ex0_sign	= DIN[`RANGE_DATA_SIGN];
wire	[`RANGE_EXP]		ex0_exp		= DIN[`RANGE_DATA_EXP];
wire	[`RANGE_FRACTION]	ex0_frac	= DIN[`RANGE_DATA_FRACTION];
wire	[`MSB_FRACTION+1:0]	ex0_fMask	= {(ex0_exp<=7'd62),	// 16	// 소수부분 마스킹: 소수일경우 1, 정수일 경우 0
									  (ex0_exp<=7'd63),	// 15
									  (ex0_exp<=7'd64),	// 14
									  (ex0_exp<=7'd65),	// 13
									  (ex0_exp<=7'd66),	// 12
									  (ex0_exp<=7'd67),	// 11
									  (ex0_exp<=7'd68),	// 10
									  (ex0_exp<=7'd69),	// 9
									  (ex0_exp<=7'd70),	// 8
									  (ex0_exp<=7'd71),	// 7
									  (ex0_exp<=7'd72),	// 6
									  (ex0_exp<=7'd73),	// 5
									  (ex0_exp<=7'd74),	// 4
									  (ex0_exp<=7'd75),	// 3
									  (ex0_exp<=7'd76),	// 2
									  (ex0_exp<=7'd77),	// 1
									  (ex0_exp<=7'd78)};	// 0
wire	[`MSB_FRACTION+1:0]	ex0_mfrac	= {1'b1, ex0_frac} & (moext_sm[`RANGE_MOEXT_SEL] ? ex0_fMask : ~ex0_fMask);	// 마스킹된 fraction
wire						ex0_bInt	= (ex0_exp >= `EXP_BIAS+`SIZE_FRACTION),	// 완전한 정수값
ex0_bFrac	= (ex0_exp < `EXP_BIAS);					// 완전한 소수값

// Floor 계산 : 소수부분을 자름, 단 음수일 경우 소수가 존재할 때 정수부를 1 증가 시켜야 함(실제로 -1을 더함)
wire	[`MSB_DATA_EXP:0]	ex0_FlrC	= {ex0_exp, ex0_frac}+(ex0_sign ? ex0_fMask[`RANGE_FRACTION] : 0);
wire	[`RANGE_DATA]		ex0_Flr		= (ex0_bInt|(DIN[`RANGE_DATA_EXP]==0)) ? DIN[`RANGE_DATA] :				// 모두 정수일 때에는 bypass
ex0_bFrac	? (ex0_sign ? `DATA_nONE : `DATA_ZERO) :		// 모두 실수일 경우는 (양수 => 0, 음수 => -1)
{ex0_sign, ex0_FlrC[`RANGE_DATA_EXP], (ex0_FlrC[`RANGE_DATA_FRACTION]&(~ex0_fMask[`RANGE_FRACTION]))};
// Fraction 계산 : 소수부분만을 구함 단 음수일 경우 (1-|소수|)를 구함, 부호는 항상 양수
wire	[`MSB_FRACTION+1:0]	ex0_FrcC	= (ex0_sign ? -{1'b1, ex0_frac} : {1'b1, ex0_frac}) & ex0_fMask;
wire						ex0_FrcZero	= ex0_bInt | (~|ex0_FrcC);	// 정수이거나 소수부분이 전혀 없을때 결과는 0을 가리킴


reg 						ex1_sign;
reg 	[`RANGE_EXP]		ex1_exp;
reg 	[`MSB_FRACTION+1:0]	ex1_FrcC;
reg 						ex1_FrcZero;
reg 	[`RANGE_DATA]		ex1_Flr;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		ex1_Flr			<= 0;
		{ex1_sign, ex1_exp, ex1_FrcC, ex1_FrcZero} <= 0;
		moext_ex0		<= 0;
	end
	else begin
		ex1_Flr			<= ex0_Flr;
		{ex1_sign, ex1_exp, ex1_FrcC, ex1_FrcZero}
		<= {ex0_sign, ex0_exp, ex0_FrcC, ex0_FrcZero};
		moext_ex0		<= moext_sm;
	end
end

// stage #2 : EX1 -----------------------
// Fraction 계산 2차
wire	[`RANGE_FRACTION]	ex1_pFrac	= ex1_FrcC[16] ? ex1_FrcC[15:0] :
ex1_FrcC[15] ? {ex1_FrcC[14:0], 1'b0} :
ex1_FrcC[14] ? {ex1_FrcC[13:0], 2'b0} :
ex1_FrcC[13] ? {ex1_FrcC[12:0], 3'b0} :
ex1_FrcC[12] ? {ex1_FrcC[11:0], 4'b0} :
ex1_FrcC[11] ? {ex1_FrcC[10:0], 5'b0} :
ex1_FrcC[10] ? {ex1_FrcC[ 9:0], 6'b0} :
ex1_FrcC[ 9] ? {ex1_FrcC[ 8:0], 7'b0} :
ex1_FrcC[ 8] ? {ex1_FrcC[ 7:0], 8'b0} :
ex1_FrcC[ 7] ? {ex1_FrcC[ 6:0], 9'b0} :
ex1_FrcC[ 6] ? {ex1_FrcC[ 5:0], 10'b0} :
ex1_FrcC[ 5] ? {ex1_FrcC[ 4:0], 11'b0} :
ex1_FrcC[ 4] ? {ex1_FrcC[ 3:0], 12'b0} :
ex1_FrcC[ 3] ? {ex1_FrcC[ 2:0], 13'b0} :
ex1_FrcC[ 2] ? {ex1_FrcC[ 1:0], 14'b0} :
ex1_FrcC[ 1] ? {ex1_FrcC[0], 15'b0} : 16'b0,
ex1_nFrac	= ex1_exp==62 ? ex1_FrcC[`MSB_FRACTION:0] :
ex1_exp==61 ? ex1_FrcC[`MSB_FRACTION+1:1] :
ex1_exp==60 ? {1'b1, ex1_FrcC[`MSB_FRACTION+1:2]} :
ex1_exp==59 ? {2'b11, ex1_FrcC[`MSB_FRACTION+1:3]} :
ex1_exp==58 ? {3'b111, ex1_FrcC[`MSB_FRACTION+1:4]} :
ex1_exp==57 ? {4'b1111, ex1_FrcC[`MSB_FRACTION+1:5]} :
ex1_exp==56 ? {5'b11111, ex1_FrcC[`MSB_FRACTION+1:6]} :
ex1_exp==55 ? {6'b111111, ex1_FrcC[`MSB_FRACTION+1:7]} :
ex1_exp==54 ? {7'b1111111, ex1_FrcC[`MSB_FRACTION+1:8]} :
ex1_exp==53 ? {8'b11111111, ex1_FrcC[`MSB_FRACTION+1:9]} :
ex1_exp==52 ? {9'b111111111, ex1_FrcC[`MSB_FRACTION+1:10]} :
ex1_exp==51 ? {10'b1111111111, ex1_FrcC[`MSB_FRACTION+1:11]} :
ex1_exp==50 ? {11'b11111111111, ex1_FrcC[`MSB_FRACTION+1:12]} :
ex1_exp==49 ? {12'b111111111111, ex1_FrcC[`MSB_FRACTION+1:13]} :
ex1_exp==48 ? {13'b1111111111111, ex1_FrcC[`MSB_FRACTION+1:14]} :
ex1_exp==47 ? {14'b11111111111111, ex1_FrcC[`MSB_FRACTION+1:15]} :
ex1_exp==46 ? {15'b111111111111111, ex1_FrcC[`MSB_FRACTION+1]} : 16'b1111111111111111;
wire	[`RANGE_EXP]		ex1_pExp	= ex1_exp +
(ex1_FrcC[16] ? 7'd0 :
 ex1_FrcC[15] ? -7'd1 :
 ex1_FrcC[14] ? -7'd2 :
 ex1_FrcC[13] ? -7'd3 :
 ex1_FrcC[12] ? -7'd4 :
 ex1_FrcC[11] ? -7'd5 :
 ex1_FrcC[10] ? -7'd6 :
 ex1_FrcC[ 9] ? -7'd7 :
 ex1_FrcC[ 8] ? -7'd8 :
 ex1_FrcC[ 7] ? -7'd9 :
 ex1_FrcC[ 6] ? -7'd10 :
 ex1_FrcC[ 5] ? -7'd11 :
 ex1_FrcC[ 4] ? -7'd12 :
 ex1_FrcC[ 3] ? -7'd13 :
 ex1_FrcC[ 2] ? -7'd14 :
 ex1_FrcC[ 1] ? -7'd15 : -7'd16);

assign	DOUT		= {8'b0, moext_ex0[`RANGE_MOEXT_ALT] ? (ex1_FrcZero ? `DATA_ZERO :
				{1'b0, (ex1_sign & (ex1_exp < `FRAC_BIAS)) ? {7'd62, ex1_nFrac} : {ex1_pExp, ex1_pFrac}}) :
				ex1_Flr};
assign	PHASE_EN	= {moext_ex0[`RANGE_MOEXT_EN]&moext_ex0[`RANGE_MOEXT_PHASE], moext_ex0[`RANGE_MOEXT_EN]&(~moext_ex0[`RANGE_MOEXT_PHASE])};

endmodule
