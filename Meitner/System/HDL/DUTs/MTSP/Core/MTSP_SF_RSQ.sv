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

module MTSP_SF_RSQ(	CLK, nRST,
					MO0, DIN0,
					MO1, DIN1,
					PHASE_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_MODESC]		MO0, MO1;				// phase #0,#1 micro operation
input	[`RANGE_DWORD]		DIN0, DIN1;				// phase #0,#1 data input
// output
output	reg [1:0]			PHASE_EN;				// phase enable
//output	reg [`RANGE_DWORD]	DOUT;					// result output
output	[`RANGE_DWORD]		DOUT;					// result output

// register definition & assignment ------------------------------------------
reg 	[`RANGE_DWORD]		DIN;
reg		[1:0]				pipe_en, pipe_phase;
wire						MO0_en = (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_RSQ) & (MO0[`RANGE_MODESC_ALT]),
MO1_en = (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_RSQ) & (MO1[`RANGE_MODESC_ALT]),
MO_en	= MO0_en | MO1_en;

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN				<= 0;
		pipe_en[0]		<= 0;
		pipe_phase[0]	<= 0;
	end
	else begin
		DIN				<= MO_en ? (MO1_en ? DIN1 : DIN0) : DIN;
		pipe_en[0]		<= MO_en;
		pipe_phase[0]	<= MO_en ? MO1_en : pipe_phase[0];
	end
end

// stage #1 : EX0 -----------------------
wire						Sign;			// Sign
wire	[`RANGE_EXP]		Exp_ex0;		// Exponent
wire	[`RANGE_FRACTION]	Frac_ex0;		// Fraction
assign	{Sign, Exp_ex0, Frac_ex0} = DIN[`RANGE_DATA];		// dispatch

// Exponent dispatch
wire						bEven = Exp_ex0[0];			// Exp´Â Â¦¼öÀÓ
wire	[`RANGE_EXP]		Rsq_E_ex0;
assign	Rsq_E_ex0		 =	{1'b0,~Exp_ex0[`MSB_EXP:1]} + {2'b0,~bEven,bEven,bEven,bEven,bEven,bEven};
//	Compute_Exp =  (((~exp)>>1)&0x3F) + ((exp&0x1) ? 0x1F : 0x20);

// Quick Newton Raphson Algorithm X=(X(3-A*X^2))/2 = 3/2*X-A*(X^3)/2		// X = 1 / sqrt(A)
// Reference value : 23bits LUT(average rounded)
wire	[15:0]				Ref;		// x.xxx...
wire	[15:0]				Ref2;		// 0.0xxx...

MTSP_SF_RSQ_LUT		rsq_lut(CLK, {Frac_ex0[`MSB_FRACTION:`MSB_FRACTION-6], bEven}, {Ref, Ref2});

reg		[`RANGE_EXP]		Rsq_E;
reg		[`RANGE_FRACTION]	Frac;		// Fraction
reg							ExpZero_ex0;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		{Rsq_E, Frac}		<= 0;
		pipe_en[1]			<= 0;
		pipe_phase[1]		<= 0;
		ExpZero_ex0			<= 0;
	end
	else begin
		{Rsq_E, Frac}		<= {Rsq_E_ex0, Frac_ex0};
		pipe_en[1]			<= pipe_en[0];
		pipe_phase[1]		<= pipe_phase[0];
		ExpZero_ex0			<= (Exp_ex0==0);
	end
end

// stage #2 : EX1 -----------------------
wire	[31:0]				FMul = {1'b1, Frac[15:1]} * Ref2;	// temp = {1,Frac}*Ref^3

reg		[18:0]				FSum_ex1;
reg							ExpZero_ex1;
reg		[`RANGE_EXP]		Rsq_E_ex1, Rsq_E_n1_ex1;
//wire	[18:0]				FSum = {Ref, 3'b0} - FMul[31:14];	// 3X/2-AX^3/2

//wire	[`RANGE_FRACTION]	Compute_Frac = FSum_ex1[18] ? FSum_ex1[17:2] : FSum_ex1[16:1];
//wire	[`RANGE_EXP]		Compute_Exp =  (FSum_ex1[18] ? Rsq_E : Rsq_E-1);

// Result Out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		FSum_ex1			<= 0;
		ExpZero_ex1			<= 0;
		Rsq_E_ex1			<= 0;
		Rsq_E_n1_ex1		<= 0;
		//DOUT				<= 0;
		PHASE_EN			<= 0;
	end
	else begin
		FSum_ex1			<= {Ref, 3'b0} - FMul[31:14];	// 3X/2-AX^3/2
		ExpZero_ex1			<= ExpZero_ex0;
		Rsq_E_ex1			<= Rsq_E;
		Rsq_E_n1_ex1		<= Rsq_E-1;
		//DOUT				<= {8'b0, 1'b0, (ExpZero_ex0 ? 23'b0 : {Compute_Exp, Compute_Frac})};
		PHASE_EN			<= {pipe_en[1]&(pipe_phase[1]), pipe_en[1]&(~pipe_phase[1])};
	end
end

wire	[`RANGE_FRACTION]	Compute_Frac = FSum_ex1[18] ? FSum_ex1[17:2] : FSum_ex1[16:1];
wire	[`RANGE_EXP]		Compute_Exp =  (FSum_ex1[18] ? Rsq_E_ex1 : Rsq_E_n1_ex1);

assign	DOUT				= {8'b0, 1'b0, ExpZero_ex0 ? 23'b0 : {Compute_Exp, Compute_Frac}};

endmodule
