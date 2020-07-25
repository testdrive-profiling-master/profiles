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

module MTSP_ALU_MUL(	CLK, nRST,
					 MO0, SRC0A, SRC0B,
					 MO1, SRC1A, SRC1B,
					 DOUT_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_MODESC]		MO0, MO1;				// micro-operation description
input	[`RANGE_DWORD]		SRC0A, SRC0B,			// source data
SRC1A, SRC1B;
// output
output	reg [1:0]			DOUT_EN;				// output enable
output	reg [`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]		DIN_A, DIN_B;
reg							MSB_DATA, mo_en_sm, mo_phase_sm;
wire						MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_MUL);
wire						MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_MUL);
wire						MO_en	= MO0_en | MO1_en;
wire						MO_sel	= MO0_en ? MO0[`RANGE_MODESC_SEL] : MO1[`RANGE_MODESC_SEL];
// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN_A			<= 0;
		DIN_B			<= 0;
		MSB_DATA		<= 1'b0;
		mo_en_sm		<= 1'b0;
		mo_phase_sm		<= 1'b0;
	end
	else begin
		mo_en_sm		<= MO_en;
		if(MO_en) begin
			DIN_A		<= MO0_en ? SRC0A : SRC1A;
			DIN_B		<= MO0_en ? SRC0B : SRC1B;
			MSB_DATA	<= ~MO_sel;
			mo_phase_sm	<= MO1_en;
		end
	end
end

// stage #1 : EX0 -----------------------
// Fraction Input
wire	[`RANGE_FRACTION]	Frac_A, Frac_B;			// Fraction
wire	[`RANGE_EXP]		Exp_A_ex0, Exp_B_ex0;	// Exponential
wire						Sign_A, Sign_B;			// Sign
assign	{Sign_A, Exp_A_ex0, Frac_A}	= DIN_A[`RANGE_DATA],
{Sign_B, Exp_B_ex0, Frac_B}	= DIN_B[`RANGE_DATA];

// Definitions for Fraction Multiplier
`define FRACTION_REAL_SIZE	(`SIZE_FRACTION+1)
`define FRACTION_Rx2_SIZE	(`FRACTION_REAL_SIZE*2)
`define FRACTION_Rx2_MSB	(`FRACTION_Rx2_SIZE-1)

// Sign Computation : Sign_A xor Sign_B
wire						Sign_Compute_ex0;
assign	Sign_Compute_ex0	= Sign_A ^ Sign_B;

// Fraction Computation 0 : Real Fraction Multiplier. (Real_Fraction_A * Real_Fraction_B)
wire	[`FRACTION_Rx2_MSB:0]	Frac_Multiply_ex0 = {MSB_DATA, Frac_A} * {MSB_DATA, Frac_B};

reg							Sign_Compute;
reg		[`RANGE_EXP]		Exp_Sum;
reg							Result_nZero;
reg		[`FRACTION_Rx2_MSB:0]	Frac_Multiply;
reg							mo_en_ex0, mo_sel_ex0, mo_phase_ex0;
// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		Sign_Compute	<= 0;
		Exp_Sum			<= 0;
		Frac_Multiply	<= 0;
		Result_nZero	<= 0;
		mo_en_ex0		<= 1'b0;
		mo_sel_ex0		<= 1'b0;
		mo_phase_ex0	<= 1'b0;
	end
	else begin
		Exp_Sum			<= {~Exp_B_ex0[`MSB_EXP], Exp_B_ex0[`MSB_EXP-1:0]} + Exp_A_ex0;
		Frac_Multiply	<= Frac_Multiply_ex0;
		Result_nZero	<= (|Exp_A_ex0) & (|Exp_B_ex0);
		Sign_Compute	<= Sign_Compute_ex0;
		mo_en_ex0		<= mo_en_sm;
		mo_sel_ex0		<= ~MSB_DATA;
		mo_phase_ex0	<= mo_phase_sm;
	end
end

// stage #2 : EX1 -----------------------
// Non Overflow version
// Fraction Computation 1 : Patch the fraction & Rounding check addition.
wire	[`MSB_FRACTION+1:0]		Frac_Compute = Frac_Multiply[`FRACTION_Rx2_MSB] ? Frac_Multiply[`FRACTION_Rx2_MSB-1:`FRACTION_Rx2_MSB-`SIZE_FRACTION] + Frac_Multiply[`FRACTION_Rx2_MSB-`SIZE_FRACTION-1]:
Frac_Multiply[`FRACTION_Rx2_MSB-2:`FRACTION_Rx2_MSB-`SIZE_FRACTION-1]+ Frac_Multiply[`FRACTION_Rx2_MSB-`SIZE_FRACTION-2];

// Exponent Computation : (Exp_A-halfExpMax) + (Exp_B-halfExpMax) + halfExpMax + Fraction_Overflow
wire	[`MSB_EXP:0]			Exp_Compute = Exp_Sum
+ {Frac_Multiply[`FRACTION_Rx2_MSB], ~Frac_Multiply[`FRACTION_Rx2_MSB]}	// Fraction Overflow Optimized : = 1+Frac_Multiply[`FRACTION_Rx2_MSB]
+ Frac_Compute[`MSB_FRACTION+1];	// Post Normalize
// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DOUT			<= 32'b0;
		DOUT_EN			<= 1'b0;
	end
	else begin
		DOUT			<= mo_sel_ex0	? Frac_Multiply[`RANGE_DWORD] : 										// inteager result
		Result_nZero	? {8'b0, Sign_Compute, Exp_Compute, Frac_Compute[`RANGE_FRACTION]} :	// float result
		0;
		DOUT_EN			<= {mo_en_ex0&mo_phase_ex0, mo_en_ex0&(~mo_phase_ex0)};
	end
end

endmodule
