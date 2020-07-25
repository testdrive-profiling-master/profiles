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

module MTSP_MO_Dispatch(	UINSTx4, REF_ADDR_DEST,
						 MO0_X, MO0_Y, MO0_Z, MO0_W, MO0_MASK, MO0_WADDR,
						 MO1_X, MO1_Y, MO1_Z, MO1_W, MO1_MASK, MO1_WADDR);

// port list -----------------------------------------------------------------
// input
input	[`RANGE_UINSTx4]		UINSTx4;							// unit instructionx4
input	[`RANGE_GPRs]			REF_ADDR_DEST;						// dest(phase #1) relative address
// output
output	[`RANGE_MODESC]			MO0_X, MO0_Y, MO0_Z, MO0_W,			// phase #0 micro-operation description
MO1_X, MO1_Y, MO1_Z, MO1_W;			// phase #1 micro-operation description
output	[`RANGE_MASK4D]			MO0_MASK, MO1_MASK;					// phase #0/#1 micro-operation mask
output	[`RANGE_GPRs]			MO0_WADDR, MO1_WADDR;				// phase #0/#1 GPRs write address

// definition & assignment ---------------------------------------------------
wire	[`RANGE_UINST]			ui0, ui1, ui2, ui3;					// unit instructions
assign	{ui0, ui1, ui2, ui3}	= UINSTx4;							// unit instruction patch
wire	[3:0]					ui0_mo = ui0[`RANGE_UINST_MO],
ui2_mo = ui2[`RANGE_UINST_MO];
wire							nEN0_main	= (ui0[`RANGE_UINST_nEN]|ui0[`RANGE_UINST_nALU]),	// phase #0 main instruction enable
nEN0_sub	= (ui1[`RANGE_UINST_nEN]|ui1[`RANGE_UINST_nALU]),	// phase #0 sub instruction enable
nEN1_main	= (ui2[`RANGE_UINST_nEN]|ui2[`RANGE_UINST_nALU]),	// phase #1 main instruction enable
nEN1_sub	= (ui3[`RANGE_UINST_nEN]|ui3[`RANGE_UINST_nALU]);	// phase #1 sub instruction enable
wire	[`RANGE_MO]				MO0_main	= ui0[`RANGE_UINST_MO],
MO0_sub		= ui1[`RANGE_UINST_MO],
MO1_main	= ui2[`RANGE_UINST_MO],
MO1_sub		= ui3[`RANGE_UINST_MO];
wire							ALT0_main	= ui0[`RANGE_UINST_ALTERNATE],	// phase #0 main alternate
ALT0_sub	= ui1[`RANGE_UINST_ALTERNATE],	// phase #0 sub alternate
ALT1_main	= ui2[`RANGE_UINST_ALTERNATE],	// phase #1 main alternate
ALT1_sub	= ui3[`RANGE_UINST_ALTERNATE],	// phase #1 sub alternate
SEL0_main	= ui0[`RANGE_UINST_SELECT],		// phase #0 main select
SEL0_sub	= ui1[`RANGE_UINST_SELECT],		// phase #0 sub select
SEL1_main	= ui2[`RANGE_UINST_SELECT],		// phase #1 main select
SEL1_sub	= ui3[`RANGE_UINST_SELECT];		// phase #1 sub select
// coordinate와 process 명령어는 직접 GPRs에 쓰지 않는다.
// 연산 명령어일 경우 WMASK가 활성된다. (addr 명령어 제외)
wire	[`RANGE_MASK4D]			WMASK0	= ui0[`RANGE_UINST_nALU] ? `MASK4D_ALLMASKED : ui0[`RANGE_UINST_WMASK],		// phase #0 write mask
WMASK1	= ui2[`RANGE_UINST_nALU] ? `MASK4D_ALLMASKED : ui2[`RANGE_UINST_WMASK],		// phase #1 write mask

MOMASK0	= nEN0_sub ? `MASK4D_ALLUNMASKED : ui1[`RANGE_UINST_MO_MASK],	// phase #0 MO mask
MOMASK1	= nEN1_sub ? `MASK4D_ALLUNMASKED : ui3[`RANGE_UINST_MO_MASK];	// phase #1 MO mask

// implementation ------------------------------------------------------------
// MO description
assign	MO0_X		= {nEN0_main|WMASK0[`SELECT_X],
				 MOMASK0[`SELECT_X] ? ALT0_sub	: ALT0_main,
				 MOMASK0[`SELECT_X] ? SEL0_sub	: SEL0_main,
				 MOMASK0[`SELECT_X] ? MO0_sub		: MO0_main},
MO0_Y		= {nEN0_main|WMASK0[`SELECT_Y],
		  MOMASK0[`SELECT_Y] ? ALT0_sub	: ALT0_main,
		  MOMASK0[`SELECT_Y] ? SEL0_sub	: SEL0_main,
		  MOMASK0[`SELECT_Y] ? MO0_sub		: MO0_main},
MO0_Z		= {nEN0_main|WMASK0[`SELECT_Z],
		  MOMASK0[`SELECT_Z] ? ALT0_sub	: ALT0_main,
		  MOMASK0[`SELECT_Z] ? SEL0_sub	: SEL0_main,
		  MOMASK0[`SELECT_Z] ? MO0_sub		: MO0_main},
MO0_W		= {nEN0_main|WMASK0[`SELECT_W],
		  MOMASK0[`SELECT_W] ? ALT0_sub	: ALT0_main,
		  MOMASK0[`SELECT_W] ? SEL0_sub	: SEL0_main,
		  MOMASK0[`SELECT_W] ? MO0_sub		: MO0_main},
MO1_X		= {nEN1_main|WMASK1[`SELECT_X],
		  MOMASK1[`SELECT_X] ? ALT1_sub	: ALT1_main,
		  MOMASK1[`SELECT_X] ? SEL1_sub	: SEL1_main,
		  MOMASK1[`SELECT_X] ? MO1_sub		: MO1_main},
MO1_Y		= {nEN1_main|WMASK1[`SELECT_Y],
		  MOMASK1[`SELECT_Y] ? ALT1_sub	: ALT1_main,
		  MOMASK1[`SELECT_Y] ? SEL1_sub	: SEL1_main,
		  MOMASK1[`SELECT_Y] ? MO1_sub		: MO1_main},
MO1_Z		= {nEN1_main|WMASK1[`SELECT_Z],
		  MOMASK1[`SELECT_Z] ? ALT1_sub	: ALT1_main,
		  MOMASK1[`SELECT_Z] ? SEL1_sub	: SEL1_main,
		  MOMASK1[`SELECT_Z] ? MO1_sub		: MO1_main},
MO1_W		= {nEN1_main|WMASK1[`SELECT_W],
		  MOMASK1[`SELECT_W] ? ALT1_sub	: ALT1_main,
		  MOMASK1[`SELECT_W] ? SEL1_sub	: SEL1_main,
		  MOMASK1[`SELECT_W] ? MO1_sub		: MO1_main};
// output
assign	MO0_MASK	= MOMASK0,
MO1_MASK	= MOMASK1,
MO0_WADDR	= {(nEN0_sub|(~ui1[`RANGE_UINST_SWITCH]) ? 2'b0 : {ui1[`RANGE_UINST_MODIFIER], ui1[`RANGE_UINST_NEGATE]}), ui0[`RANGE_UINST_DEST]},
MO1_WADDR	= {(nEN1_sub|(~ui3[`RANGE_UINST_SWITCH]) ? 2'b0 : {ui3[`RANGE_UINST_MODIFIER], ui3[`RANGE_UINST_NEGATE]}), ui2[`RANGE_UINST_DEST]} + REF_ADDR_DEST;

endmodule
