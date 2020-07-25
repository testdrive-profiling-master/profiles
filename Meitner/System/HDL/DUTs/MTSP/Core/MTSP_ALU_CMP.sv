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

module MTSP_ALU_CMP(	CLK, nRST,
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
reg		[`RANGE_DWORD]	DIN_A, DIN_B;
reg		[`RAMGE_MOEXT]	moext_sm;
wire					MO0_en = (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_CMP),
MO1_en = (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_CMP),
MO_en	= MO0_en | MO1_en;

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
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
end

// stage #1 : EX0 -----------------------
wire				bInt		= moext_sm[`RANGE_MOEXT_SEL],
EQ			= (DIN_A[`RANGE_DATA] == DIN_B[`RANGE_DATA]),			// equal
As			= DIN_A[`RANGE_DATA_SIGN],	// A's sign
Bs			= DIN_B[`RANGE_DATA_SIGN],	// B's sign
GT			= DIN_A[`MSB_DATA-1:0] > DIN_B[`MSB_DATA-1:0];	// A greater than B without sign bit
wire	[3:0]		cmp			= {((~cmp[2])&cmp[1]),
					 (((~As)&Bs) | ((~As)&(~Bs)&GT) | (As&Bs&(~EQ)&(bInt^(~GT)))),
					 ~EQ,
					 EQ};

// Result out
assign	DOUT		= {28'b0, cmp};
assign	PHASE_EN	= {moext_sm[`RANGE_MOEXT_EN]&moext_sm[`RANGE_MOEXT_PHASE], moext_sm[`RANGE_MOEXT_EN]&(~moext_sm[`RANGE_MOEXT_PHASE])};

endmodule
