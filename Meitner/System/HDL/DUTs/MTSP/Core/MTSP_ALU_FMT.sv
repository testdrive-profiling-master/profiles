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

module MTSP_ALU_FMT(	CLK, nRST,
					 MO, SRCA, SRCB,
					 PHASE_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_MODESC]		MO;						// micro-operation description
input	[`RANGE_DWORD]		SRCA, SRCB;				// source data

// output
output	reg 				PHASE_EN;				// output enable
output	reg [`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]		DIN_A, DIN_B;
reg		[`RAMGE_MOEXT]		sm_en;
wire						MO_en	= (~MO[`RANGE_MODESC_nEN]) & (MO[`RANGE_MODESC_MO]==`MO_FMT) & (MO[`RANGE_MODESC_ALT]);

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN_A			<= 0;
		DIN_B			<= 0;
		sm_en			<= 0;
	end
	else begin
		DIN_A			<= MO_en ? SRCA : DIN_A;
		DIN_B			<= MO_en ? SRCB : DIN_B;
		sm_en			<= MO_en;
	end
end

// stage #1 : EX0 -----------------------
wire	[4:0]			r_0, b_0, r_1, b_1;
wire	[5:0]			g_0, g_1;

assign	r_0				= DIN_A[23:19],
g_0				= DIN_A[15:10],
b_0				= DIN_A[ 7: 3],
r_1				= DIN_B[23:19],
g_1				= DIN_B[15:10],
b_1				= DIN_B[ 7: 3];

wire	[`RANGE_DWORD]	rcolor	= {r_0, g_0, b_0, r_1, g_1, b_1};
//wire	[`RANGE_DWORD]	rcolor	= {r_1, g_1, b_1, r_0, g_0, b_0};

// Result out
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DOUT		<= 0;
		PHASE_EN	<= 0;
	end
	else begin
		DOUT		<= sm_en ? rcolor : DOUT;
		PHASE_EN	<= sm_en;
	end
end

endmodule
