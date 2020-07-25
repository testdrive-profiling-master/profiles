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

module MTSP_ID_3(
	// System
	input							CLK, nRST,						// main clock & reset (active low)
	// Input
	input	[`RANGE_PC]				PC,								// PC
	input	[`RANGE_UINSTx4]		UINSTx4,						// 4 unit instructions end & phase bit
	input	[`RANGE_DWORDx4]		SRC0A, SRC0B, SRC1A, SRC1B,
	input	[`RANGE_GPRs]			REF_ADDR_DEST,					// reference address : destination
	// Output
	output	[`RANGE_PC]				PC_OUT,							// PC output
	output	[`RANGE_UINSTx4]		UINSTx4_OUT,					// phase instructions
	output	[`RANGE_GPRs]			REF_MEM_SRC,					// reference source address for memory operation
	output	[`RANGE_DWORDx4]		SRC0A_OUT, SRC0B_OUT, SRC1A_OUT, SRC1B_OUT,
	// micro-operations
	output	[`RANGE_MODESC]			MO0_X, MO0_Y, MO0_Z, MO0_W,		// phase #0 micro-operation description
	output	[`RANGE_MODESC]			MO1_X, MO1_Y, MO1_Z, MO1_W,		// phase #1 micro-operation description
	output	[`RANGE_MASK4D]			MO0_MASK, MO1_MASK,				// phase #0/#1 micro-operation mask
	output	[`RANGE_GPRs]			MO0_WADDR, MO1_WADDR			// phase #0/#1 GPRs write address
);

// definition & assignment ---------------------------------------------------
wire	[`RANGE_UINST]			p0_m, p0_s, p1_m, p1_s;		// input unit instruction
wire	[`RANGE_DWORDx4]		src1b_coord;

assign	{p0_m, p0_s, p1_m, p1_s}	= UINSTx4;

// implementation ------------------------------------------------------------
MTSP_SrcCoordinate			src_coord(1'b1, 1'b1, p1_s, SRC1B, src1b_coord);

assign	PC_OUT								= PC;
assign	UINSTx4_OUT							= UINSTx4;
assign	{SRC0A_OUT, SRC0B_OUT, SRC1A_OUT}	= {SRC0A, SRC0B, SRC1A};
assign	SRC1B_OUT							= src1b_coord;
assign	REF_MEM_SRC							= {(p1_s[`RANGE_UINST_nALU] ? 2'b0 : p1_s[`RANGE_UINST_SI_A]), p1_m[`RANGE_UINST_SRC]};

MTSP_MO_Dispatch			mo_dispatch
(	UINSTx4,								// UINSTx4,
  REF_ADDR_DEST,							// dest(phase #1) relative address
  MO0_X, MO0_Y, MO0_Z, MO0_W, MO0_MASK, MO0_WADDR,		// MO0_X, MO0_Y, MO0_Z, MO0_W, MO0_MASK, MO0_WADDR,
  MO1_X, MO1_Y, MO1_Z, MO1_W, MO1_MASK, MO1_WADDR);		// MO1_X, MO1_Y, MO1_Z, MO1_W, MO1_MASK, MO1_WADDR

endmodule
