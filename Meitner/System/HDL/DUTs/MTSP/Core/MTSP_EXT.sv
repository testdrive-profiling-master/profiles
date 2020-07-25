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

module MTSP_EXT(
	// System
	input							CLK, nRST,					// main clock & reset (active low)
	// input
	input							nEN_BRC,					// branch enable (active low)
	input	[`RANGE_PC]				PC,							// program counter input
	input	[`RANGE_UINST]			UINST,						// input instructions
	input	[`RANGE_GPRs]			REF_ADDR_DEST,				// reference address : dest
	input	[`RANGE_INDEX]			REF_ADDR_RELATIVE,			// reference address : relative
	input	[`RANGE_GPRs]			REF_MEM_SRC,				// reference source address for memory operation
	input	[`RANGE_DWORDx4]		SRC0, SRC1,					// phase #1 source input
	// External thread control
	i_thread_control.s				thread_command,				// thread control
	// output
	output							PC_nEN,						// PC operation enable (active low)
	output	[`RANGE_BR]				PC_BO,						// branch operation
	output	[`RANGE_PC]				PC_NEXT,					// next output
	output	[`RANGE_MEMID]			MEM_nEN,					// memory operatio enable (active low)
	output							MEM_WRITE,					// memory write enable
	output	[`RANGE_WORD]			MEM_ADDR,					// memory address
	output	[`RANGE_GPRs]			MEM_SRC,					// memory source
	output	[`RANGE_DWORDx4]		MEM_DATA_0, MEM_DATA_1		// memory source data
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
MTSP_BRC	brc(
	.CLK				(CLK),
	.nRST				(nRST),
	.thread_command		(thread_command),
	.nEN_BRC			(nEN_BRC),
	.UINST				(UINST),
	.PC					(PC),
	.REF_ADDR_RELATIVE	(REF_ADDR_RELATIVE),
	.PC_nEN				(PC_nEN),
	.PC_BO				(PC_BO),
	.PC_NEXT			(PC_NEXT)
);

MTSP_MEM	mem(
	.CLK				(CLK),
	.nRST				(nRST),
	.UINST				(UINST),
	.REF_ADDR_DEST		(REF_ADDR_DEST),
	.REF_ADDR_RELATIVE	(REF_ADDR_RELATIVE),
	.REF_MEM_SRC		(REF_MEM_SRC),
	.SRC0				(SRC0),
	.SRC1				(SRC1),
	.MEM_nEN			(MEM_nEN),
	.MEM_WRITE			(MEM_WRITE),
	.MEM_ADDR			(MEM_ADDR),
	.MEM_SRC			(MEM_SRC),
	.MEM_DATA_0			(MEM_DATA_0),
	.MEM_DATA_1			(MEM_DATA_1)
);

endmodule
