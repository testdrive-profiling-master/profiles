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

module MTSP_ALU_BW(	CLK, nRST,
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
reg 	[`RANGE_DWORD]	DIN_A, DIN_B;
reg		[`RAMGE_MOEXT]	moext_sm;
reg						molsb_sm;
wire					MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`MSB_MODESC_MO:`LSB_MODESC_MO+1]==`MO_BW),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`MSB_MODESC_MO:`LSB_MODESC_MO+1]==`MO_BW),
MO_en	= MO0_en | MO1_en;

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN_A			<= 0;
		DIN_B			<= 0;
		moext_sm		<= 0;
		molsb_sm		<= 0;
	end
	else begin
		DIN_A			<= MO_en ? (MO1_en ? SRC1A : SRC0A) : DIN_A;
		DIN_B			<= MO_en ? (MO1_en ? SRC1B : SRC0B) : DIN_B;
		moext_sm[`RANGE_MOEXT_EN]		<= MO_en;
		moext_sm[`RANGE_MOEXT_PHASE]	<= MO1_en;	// phase #1 priority is high
		moext_sm[`RANGE_MOEXT_OP]		<= MO_en ? (MO1_en ? MO1[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL] : MO0[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL]) : moext_sm[`RANGE_MOEXT_OP];
		molsb_sm		<= MO_en ? (MO1_en ? MO1[`LSB_MODESC_MO] : MO0[`LSB_MODESC_MO]) : molsb_sm;
	end
end

// stage #1 : EX0 -----------------------
wire	[1:0]			op_bw		= {molsb_sm, moext_sm[`RANGE_MOEXT_ALT]};	// 00 : AND, 01 : OR, 10 : XOR, 11 : shift
wire					op_nNOT		= moext_sm[`RANGE_MOEXT_SEL];
wire	[4:0]			din_nB		= -DIN_B[4:0];
wire	[`RANGE_DWORD]	data_bw		= (op_bw==2'b11) ? (DIN_B[15] ? (DIN_B[7] ? (DIN_A >>> din_nB) : (DIN_A <<< DIN_B[4:0])) :
		(DIN_B[7] ? (DIN_A >>  din_nB) : (DIN_A <<  DIN_B[4:0]))) :
(op_bw==2'b10) ? (DIN_A ^ DIN_B) :
(op_bw==2'b01) ? (DIN_A | DIN_B) :
(DIN_A & DIN_B),
data_bw_ex0	= op_nNOT ? data_bw : ~data_bw;
//	shift capability (op_bw == 2'b11)
//	DIN_B[15]==0, DIN_B[7]==0	: logical shift left
//	DIN_B[15]==0, DIN_B[7]==1	: logical shift right
//	DIN_B[15]==1, DIN_B[7]==0	: arithmetic shift left
//	DIN_B[15]==1, DIN_B[7]==1	: arithmetic shift right

// Result out
assign	DOUT		= {data_bw_ex0};
assign	PHASE_EN	= {moext_sm[`RANGE_MOEXT_EN]&moext_sm[`RANGE_MOEXT_PHASE], moext_sm[`RANGE_MOEXT_EN]&(~moext_sm[`RANGE_MOEXT_PHASE])};

endmodule
