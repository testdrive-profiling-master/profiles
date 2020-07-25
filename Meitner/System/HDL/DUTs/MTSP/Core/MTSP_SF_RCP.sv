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

module MTSP_SF_RCP(	CLK, nRST,
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
wire						MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_RCP) & (~MO0[`RANGE_MODESC_ALT]),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_RCP) & (~MO1[`RANGE_MODESC_ALT]),
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
// Quick Neeton Raphson Algorithm
// Result = Ref*(2-{1,Frac}*Ref) = 2*Ref - {1,Frac}*(Ref^2) = (Ref<<1) - {1,Frac} * Ref^2
wire						Sign_ex0;		// Sign
wire	[`RANGE_EXP]		Exp;			// Exponent
wire	[`RANGE_FRACTION]	Frac_ex0;		// Fraction
assign	{Sign_ex0, Exp, Frac_ex0} = DIN[`RANGE_DATA];	// dispatch

// Exponent dispatch : -(e-63)-1+63 = (~e)+1+63-1+63 = (~e)+(7'b1111110)
wire	[`RANGE_EXP]		Compute_Exp_ex0;
assign	Compute_Exp_ex0 = (Exp==0)	? 7'b1111111 : (~Exp)+{6'b111111, (Frac_ex0==0)};

// Reference value : 23bits LUT(average rounded)
wire	[6:0]				Ref;		// Ref		: 0.1x(xxxxxxx)			(7 bits)
wire	[15:0]				Ref2;		// Ref^2	: 0.(xxxxxxxxxxxxxxx)	(16 bits)
MTSP_SF_RCP_LUT				rcp_lut(CLK, Frac_ex0[`MSB_FRACTION:`MSB_FRACTION-7], {Ref, Ref2});

reg							Sign;
reg 	[`RANGE_FRACTION]	Frac;
reg 	[`RANGE_EXP]		Compute_Exp;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		{Sign, Compute_Exp, Frac} <= 0;
		pipe_en[1]			<= 0;
		pipe_phase[1]		<= 0;
	end
	else begin
		{Sign, Compute_Exp, Frac}
		<= {Sign_ex0, Compute_Exp_ex0, Frac_ex0};
		pipe_en[1]			<= pipe_en[0];
		pipe_phase[1]		<= pipe_phase[0];
	end
end

// stage #2 : EX1 -----------------------
wire	[30:0]				temp = {1'b1, Frac} * Ref2;	// temp = {1,Frac}*Ref^2

reg							FracZero;
reg							Sign_ex1;
reg 	[`RANGE_EXP]		Compute_Exp_ex1;
reg		[`RANGE_FRACTION]	Compute_Frac_ex1;
//wire	[`RANGE_FRACTION]	Compute_Frac = {Ref, 9'b0} - temp[30:30-`SIZE_FRACTION+1];	// (Ref<<1) - temp

// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		FracZero			<= 0;
		Sign_ex1			<= 0;
		Compute_Exp_ex1		<= 0;
		Compute_Frac_ex1	<= 0;
		//DOUT				<= 0;
		PHASE_EN			<= 0;
	end
	else begin
		FracZero			<= (Frac==0);
		Sign_ex1			<= Sign;
		Compute_Exp_ex1		<= Compute_Exp;
		Compute_Frac_ex1	<= {Ref, 9'b0} - temp[30:30-`SIZE_FRACTION+1];	// (Ref<<1) - temp
		//DOUT				<= {8'b0, Sign, Compute_Exp, (Frac==0 ? 16'b0 : Compute_Frac)};
		PHASE_EN			<= {pipe_en[1]&(pipe_phase[1]), pipe_en[1]&(~pipe_phase[1])};
	end
end

assign	DOUT				= {8'b0, Sign_ex1, Compute_Exp_ex1, (FracZero ? 16'b0 : Compute_Frac_ex1)};

endmodule
