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

module MTSP_SF_MAN(	CLK, nRST,
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
output	reg [`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg 	[`RANGE_DWORD]		DIN;
reg		[1:0]				pipe_en, pipe_phase;
wire						MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_LOG) & (~MO0[`RANGE_MODESC_ALT]),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_LOG) & (~MO1[`RANGE_MODESC_ALT]),
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
wire						ex0_zero	= (DIN[`RANGE_DATA_EXP]==0);
wire						ex0_sign	= DIN[`SIGN_DATA];
wire	[`RANGE_FRACTION]	ex0_frac	= DIN[`RANGE_DATA_FRACTION];

reg							ex1_zero, ex1_sign;
reg		[`RANGE_FRACTION]	ex1_frac;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		{ex1_zero, ex1_sign, ex1_frac} <= 0;
		pipe_en[1]		<= 0;
		pipe_phase[1]	<= 0;
	end
	else begin
		{ex1_zero, ex1_sign, ex1_frac}
		<= {ex0_zero, ex0_sign, ex0_frac};
		pipe_en[1]		<= pipe_en[0];
		pipe_phase[1]	<= pipe_phase[0];
	end
end

// stage #2 : EX1 -----------------------
// Result Out
wire	[`RANGE_EXP]	ex1_exp		= `EXP_BIAS;
wire	[`RANGE_DATA]	ex1_data	= ex1_zero ? `DATA_ZERO : {ex1_sign, ex1_exp, ex1_frac};

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DOUT			<= 0;
		PHASE_EN		<= 0;
	end
	else begin
		DOUT			<= {8'b0, ex1_data};
		PHASE_EN		<= {pipe_en[1]&(pipe_phase[1]), pipe_en[1]&(~pipe_phase[1])};
	end
end

endmodule
