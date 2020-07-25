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

module MTSP_ALU_BA(	CLK, nRST,
					MO0, SRC0A, SRC0B,
					MO1, SRC1A, SRC1B,
					PHASE_EN, DOUT0, DOUT1);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_MODESC]		MO0, MO1;				// phase #0/#1 micro-operation description
input	[`RANGE_DWORD]		SRC0A, SRC0B,			// phase #0 source data
SRC1A, SRC1B;			// phase #1 source data
// output
output	reg [1:0]			PHASE_EN;				// output enable
output	reg [`RANGE_DWORD]	DOUT0, DOUT1;			// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]		DIN_0A, DIN_0B, DIN_1A, DIN_1B;
reg		[1:0]				sm_en, sm_sl;
wire						MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_BA) & (~MO0[`RANGE_MODESC_ALT]),
MO0_sl	= MO0[`RANGE_MODESC_SEL],
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_BA) & (~MO1[`RANGE_MODESC_ALT]),
MO1_sl	= MO1[`RANGE_MODESC_SEL];

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN_0A			<= 0;
		DIN_0B			<= 0;
		DIN_1A			<= 0;
		DIN_1B			<= 0;
		sm_sl			<= 0;
		sm_en			<= 0;
	end
	else begin
		DIN_0A			<= MO0_en ? SRC0A : DIN_0A;
		DIN_0B			<= MO0_en ? SRC0B : DIN_0B;
		DIN_1A			<= MO1_en ? SRC1A : DIN_1A;
		DIN_1B			<= MO1_en ? SRC1B : DIN_1B;
		sm_sl			<= {(MO1_en&MO1_sl), (MO0_en&MO0_sl)};
		sm_en			<= {MO1_en, MO0_en};
	end
end

// stage #1 : EX0 -----------------------
reg		[8:0]			ex0_sum0A, ex0_sum0R, ex0_sum0G, ex0_sum0B,
ex0_sum1A, ex0_sum1R, ex0_sum1G, ex0_sum1B;
reg		[1:0]			ex0_en, ex0_sl;

// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		{ex0_sum0A, ex0_sum0R, ex0_sum0G, ex0_sum0B}	<= 0;
		{ex0_sum1A, ex0_sum1R, ex0_sum1G, ex0_sum1B}	<= 0;

		ex0_sl		<= 2'd0;
		ex0_en		<= `FALSE;
	end
	else begin
		ex0_sum0A	<= DIN_0A[31:24]+DIN_0B[31:24];
		ex0_sum0R	<= DIN_0A[23:16]+DIN_0B[23:16];
		ex0_sum0G	<= DIN_0A[15: 8]+DIN_0B[15: 8];
		ex0_sum0B	<= DIN_0A[ 7: 0]+DIN_0B[ 7: 0];

		ex0_sum1A	<= DIN_1A[31:24]+DIN_1B[31:24];
		ex0_sum1R	<= DIN_1A[23:16]+DIN_1B[23:16];
		ex0_sum1G	<= DIN_1A[15: 8]+DIN_1B[15: 8];
		ex0_sum1B	<= DIN_1A[ 7: 0]+DIN_1B[ 7: 0];

		ex0_sl		<= sm_sl;
		ex0_en		<= sm_en;
	end
end

// stage #2 : EX1 -----------------------
wire	[9:0]			ex1_sumA, ex1_sumR, ex1_sumG, ex1_sumB;
wire	[`RANGE_DWORD]	ex1_avr, ex1_sum0, ex1_sum1;

assign					ex1_sumA	= ex0_sum0A + ex0_sum1A,
ex1_sumR	= ex0_sum0R + ex0_sum1R,
ex1_sumG	= ex0_sum0G + ex0_sum1G,
ex1_sumB	= ex0_sum0B + ex0_sum1B,
ex1_avr		= {ex1_sumA[9:2], ex1_sumR[9:2], ex1_sumG[9:2], ex1_sumB[9:2]},
ex1_sum0	= {(ex0_sum0A[8] ? 8'hFF : ex0_sum0A[`RANGE_BYTE]),
			(ex0_sum0R[8] ? 8'hFF : ex0_sum0R[`RANGE_BYTE]),
			(ex0_sum0G[8] ? 8'hFF : ex0_sum0G[`RANGE_BYTE]),
			(ex0_sum0B[8] ? 8'hFF : ex0_sum0B[`RANGE_BYTE])},
ex1_sum1	= {(ex0_sum1A[8] ? 8'hFF : ex0_sum1A[`RANGE_BYTE]),
			(ex0_sum1R[8] ? 8'hFF : ex0_sum1R[`RANGE_BYTE]),
			(ex0_sum1G[8] ? 8'hFF : ex0_sum1G[`RANGE_BYTE]),
			(ex0_sum1B[8] ? 8'hFF : ex0_sum1B[`RANGE_BYTE])};

// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DOUT0		<= 0;
		DOUT1		<= 0;
		PHASE_EN	<= 2'd0;
	end
	else begin
		DOUT0		<= ex0_sl[0] ? ex1_avr : ex1_sum0;
		DOUT1		<= ex0_sl[1] ? ex1_avr : ex1_sum1;
		PHASE_EN	<= (|ex0_sl) ? ex0_sl : ex0_en;
	end
end
endmodule
