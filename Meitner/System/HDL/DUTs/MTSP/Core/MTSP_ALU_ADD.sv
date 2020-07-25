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

module MTSP_ALU_ADD(	CLK, nRST,
					 MO0, MO0_MASK, SRC0A, SRC0B,
					 MO1, MO1_MASK, SRC1A, SRC1B,
					 PHASE_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_MODESC]		MO0, MO1;				// phase #0/#1 micro-operation description
input						MO0_MASK, MO1_MASK;		// phase #0/#1 micro-operation mask
input	[`RANGE_DWORD]		SRC0A, SRC0B,			// phase #0 source data
SRC1A, SRC1B;			// phase #1 source data

// output
output	reg [1:0]			PHASE_EN;				// output enable
output	reg [`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]		DIN_A, DIN_B;
reg		[`RAMGE_MOEXT]		moext_sm, moext_ex0;
wire						MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_ADD),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_ADD),
MO_en	= MO0_en | MO1_en;

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET
`ON_RESET begin
	DIN_A			<= 0;
	DIN_B			<= 0;
	moext_sm		<= 0;
end
else begin
	DIN_A			<= MO_en ? (MO1_en ? SRC1A : SRC0A) : DIN_A;
	DIN_B			<= MO_en ? (MO1_en ? SRC1B : SRC0B) : DIN_B;
	moext_sm[`RANGE_MOEXT_EN]		<= MO_en;
	moext_sm[`RANGE_MOEXT_PHASE]	<= MO1_en;	// phase #1 priority is high
	moext_sm[`RANGE_MOEXT_OP]		<= MO_en ? (MO1_en ? MO1[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL] : MO0[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL]) : moext_sm[`RANGE_MOEXT_OP];
end

// stage #1 : EX0 -----------------------
// Fraction Input
wire	[`RANGE_FRACTION]	Frac_A_ex0, Frac_B_ex0;		// Fraction
wire	[`RANGE_EXP]		Exp_A_ex0, Exp_B_ex0;		// Exponential
wire						Sign_A, Sign_B;	// Sign
assign	{Sign_A, Exp_A_ex0, Frac_A_ex0}	= DIN_A,
{Sign_B, Exp_B_ex0, Frac_B_ex0}	= DIN_B;

// + - 연산을 결정한다.
wire						Sign_Differ = Sign_A ^ Sign_B;

// Exponent 차이를 구한다.
wire	[`MSB_EXP:0]		Exp_Differ_AB = Exp_A_ex0 - Exp_B_ex0,
Exp_Differ_BA = Exp_B_ex0 - Exp_A_ex0;

// Align을 한다 : No overwhelming Check
wire	[`MSB_FRACTION+2:0]	A_Align_ex0, B_Align_ex0;
assign	A_Align_ex0			= {1'b1,Frac_A_ex0,1'b0} >> Exp_Differ_BA,
B_Align_ex0			= {1'b1,Frac_B_ex0,1'b0} >> Exp_Differ_AB;
// Exponent Pre-calculation
wire	[`RANGE_EXP]		ExpAe_ex0, ExpBe_ex0;
assign	ExpAe_ex0			= Sign_Differ ? Exp_A_ex0-1 : Exp_A_ex0+1,
ExpBe_ex0			= Sign_Differ ? Exp_B_ex0-1 : Exp_B_ex0+1;
// 크기를 비교한다
wire						Exp_MatchenDiffer_ex0;
assign	Exp_MatchenDiffer_ex0	= ((Exp_Differ_AB[`MSB_EXP:1]==0) | (Exp_Differ_BA[`MSB_EXP:1]==0)) & Sign_Differ;
wire						Great_A_ex0;
assign	Great_A_ex0			= DIN_A[`MSB_DATA_EXP:0] > DIN_B[`MSB_DATA_EXP:0];
// Sign Computation
wire						Sign_Compute_ex0;
assign	Sign_Compute_ex0	= Great_A_ex0 ? Sign_A : Sign_B;
wire	[`MSB_FRACTION+2:0]	Matched_Sub_ex0;
assign	Matched_Sub_ex0		= Great_A_ex0 ? (Exp_Differ_AB[0] ? ({1'b1, Frac_A_ex0, 1'b0} - {2'b01, Frac_B_ex0}) : ({1'b0, Frac_A_ex0-Frac_B_ex0, 1'b0})) :
(Exp_Differ_BA[0] ? ({1'b1, Frac_B_ex0, 1'b0} - {2'b01, Frac_A_ex0}) : ({1'b0, Frac_B_ex0-Frac_A_ex0, 1'b0}));
wire	[`RANGE_EXP]		Matched_Sub_Exp_Base_ex0;
assign	Matched_Sub_Exp_Base_ex0	= Great_A_ex0 ? Exp_A_ex0 : Exp_B_ex0;
// 계산 결과는 0이다.
wire						A_ZERO = (Exp_A_ex0==0),
B_ZERO = (Exp_B_ex0==0);
wire						Result_Zero = (DIN_A[`MSB_DATA_EXP:0]==DIN_B[`MSB_DATA_EXP:0]) & Sign_Differ;

reg		[`RANGE_DWORD]		data_int_ex0;
reg							Sign_Differ_0;
reg							except_en;
reg		[`RANGE_DATA]		except_data;
reg		[`RANGE_EXP]		Exp_A, Exp_B, ExpAe, ExpBe;
reg		[`RANGE_FRACTION]	Frac_A, Frac_B;

reg							Great_A, Sign_Compute, Exp_MatchenDiffer;
reg		[`RANGE_EXP]		Matched_Sub_Exp_Base;
reg		[`MSB_FRACTION+2:0]	Matched_Sub;
reg		[`MSB_FRACTION+2:0]	A_Align, B_Align;

`ALWAYS_CLOCK_RESET
`ON_RESET begin
	data_int_ex0	<= 0;
	Sign_Differ_0	<= 0;
	except_en		<= 0;
	except_data		<= 0;
	{Exp_A, Exp_B, ExpAe, ExpBe, Frac_A, Frac_B}	<= 0;
	{Great_A, Sign_Compute, Exp_MatchenDiffer, Matched_Sub_Exp_Base, Matched_Sub, A_Align, B_Align}	<= 0;
	moext_ex0		<= 0;
end
else begin
	data_int_ex0	<= DIN_A + DIN_B;
	Sign_Differ_0	<= Sign_Differ;
	except_en		<= A_ZERO|B_ZERO|Result_Zero;
	except_data		<= A_ZERO ? DIN_B : B_ZERO ? DIN_A : 0;
	{Exp_A, Exp_B, ExpAe, ExpBe, Frac_A, Frac_B}
	<= {Exp_A_ex0, Exp_B_ex0, ExpAe_ex0, ExpBe_ex0, Frac_A_ex0, Frac_B_ex0};
	{Great_A, Sign_Compute, Exp_MatchenDiffer, Matched_Sub_Exp_Base, Matched_Sub, A_Align, B_Align}
	<=	{Great_A_ex0, Sign_Compute_ex0, Exp_MatchenDiffer_ex0, Matched_Sub_Exp_Base_ex0, Matched_Sub_ex0, A_Align_ex0, B_Align_ex0};
	moext_ex0		<= moext_sm;
end
// stage #2 : EX1 -----------------------
// case #1 Addition
// Addition을 수행한다.
wire	[`MSB_FRACTION+3:0] Add = Great_A ? ({1'b1, Frac_A, 1'b0} + B_Align) :
({1'b1, Frac_B, 1'b0} + A_Align);

wire	[`MSB_FRACTION+1:0]	Add_Frac = Add[`MSB_FRACTION+3] ? Add[`MSB_FRACTION+2:1] :
Add[`MSB_FRACTION+1:0];

wire	[`RANGE_EXP]		Add_Exp = Add[`MSB_FRACTION+3] ? (Great_A ? ExpAe : ExpBe) : // Exp+1:
(Great_A ? Exp_A : Exp_B);

wire	[`MSB_DATA_EXP:0]	Add_Result = {Add_Exp, Add_Frac[`MSB_FRACTION+1:1]} + Add_Frac[0];	// Rounding

// case #2 subtraction
// subtraction을 수행한다.
wire	[`MSB_FRACTION+2:0] Sub = Great_A ? ({1'b1, Frac_A, 1'b0} - B_Align) :
({1'b1, Frac_B, 1'b0} - A_Align);

wire	[`MSB_FRACTION:0]	Sub_Frac = Sub[`MSB_FRACTION+2] ? Sub[`MSB_FRACTION+1:1] :
Sub[`MSB_FRACTION  :0];

wire	[`RANGE_EXP]		Sub_Exp = Sub[`MSB_FRACTION+2] ? (Great_A ? Exp_A : Exp_B) :
(Great_A ? ExpAe : ExpBe);	// Exp-1:

wire	[`MSB_DATA_EXP:0]	Sub_Result = {Sub_Exp, Sub_Frac};	// No Rounding Need

// case #3 Exponent matched Addition
// 특수 경우 : Full Normalize 과정이 필요한 Exponent가 일치하거나 1차이일 경우 경우의 뺄샘을 계산한다.
// 상위는 별도로 zlc를 수행하지 않는다.

wire	[`RANGE_FRACTION]	Matched_Sub_Frac
= Matched_Sub[17] ?  Matched_Sub[16:1] :			// 17
Matched_Sub[16] ?  Matched_Sub[15:0] :			// 16
Matched_Sub[15] ? {Matched_Sub[14:0],  1'b0} :	// 15
Matched_Sub[14] ? {Matched_Sub[13:0],  2'b0} :	// 14
Matched_Sub[13] ? {Matched_Sub[12:0],  3'b0} :	// 13
Matched_Sub[12] ? {Matched_Sub[11:0],  4'b0} :	// 12
Matched_Sub[11] ? {Matched_Sub[10:0],  5'b0} :	// 11
Matched_Sub[10] ? {Matched_Sub[ 9:0],  6'b0} :	// 10
Matched_Sub[ 9] ? {Matched_Sub[ 8:0],  7'b0} :	//  9
Matched_Sub[ 8] ? {Matched_Sub[ 7:0],  8'b0} :	//  8
Matched_Sub[ 7] ? {Matched_Sub[ 6:0],  9'b0} :	//  7
Matched_Sub[ 6] ? {Matched_Sub[ 5:0], 10'b0} :	//  6
Matched_Sub[ 5] ? {Matched_Sub[ 4:0], 11'b0} :	//  5
Matched_Sub[ 4] ? {Matched_Sub[ 3:0], 12'b0} :	//  4
Matched_Sub[ 3] ? {Matched_Sub[ 2:0], 13'b0} :	//  3
Matched_Sub[ 2] ? {Matched_Sub[ 1:0], 14'b0} :	//  2
Matched_Sub[ 1] ? {Matched_Sub[   0], 15'b0} :	//  1
16'b0;		//  0

wire	[`RANGE_EXP]		Matched_Sub_Exp = Matched_Sub_Exp_Base +
(Matched_Sub[17] ? 7'd0 :		// 17
 Matched_Sub[16] ? -7'd1 :		// 16
 Matched_Sub[15] ? -7'd2 :		// 15
 Matched_Sub[14] ? -7'd3 :		// 14
 Matched_Sub[13] ? -7'd4 :		// 13
 Matched_Sub[12] ? -7'd5 :		// 12
 Matched_Sub[11] ? -7'd6 :		// 11
 Matched_Sub[10] ? -7'd7 :		// 10
 Matched_Sub[ 9] ? -7'd8 :		//  9
 Matched_Sub[ 8] ? -7'd9 :		//  8
 Matched_Sub[ 7] ? -7'd10:		//  7
 Matched_Sub[ 6] ? -7'd11:		//  6
 Matched_Sub[ 5] ? -7'd12:		//  5
 Matched_Sub[ 4] ? -7'd13:		//  4
 Matched_Sub[ 3] ? -7'd14:		//  3
 Matched_Sub[ 2] ? -7'd15:		//  2
 Matched_Sub[ 1] ? -7'd16:		//  1
 -7'd17);	//  0

// Exponente Computation
wire	[`RANGE_EXP]		Exp_Compute = Exp_MatchenDiffer ? Matched_Sub_Exp :
Sign_Differ_0 ? Sub_Result[`RANGE_DATA_EXP] : Add_Result[`RANGE_DATA_EXP];

wire	[`RANGE_FRACTION]	Frac_Compute = Exp_MatchenDiffer ? Matched_Sub_Frac :
Sign_Differ_0 ? Sub_Result[`RANGE_DATA_FRACTION] : Add_Result[`RANGE_DATA_FRACTION];

// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DOUT		<= 0;
		PHASE_EN	<= 0;
	end
	else begin
		DOUT		<= moext_ex0[`RANGE_MOEXT_SEL] ? data_int_ex0 : {8'b0, (except_en ? except_data : {Sign_Compute, Exp_Compute, Frac_Compute})};
		// phase 1 enable											   phase 0 enable
		PHASE_EN	<= {moext_ex0[`RANGE_MOEXT_EN]&moext_ex0[`RANGE_MOEXT_PHASE], moext_ex0[`RANGE_MOEXT_EN]&(~moext_ex0[`RANGE_MOEXT_PHASE])};
	end
end

endmodule
