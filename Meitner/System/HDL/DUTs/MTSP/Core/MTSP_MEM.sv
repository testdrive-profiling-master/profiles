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

module MTSP_MEM(
	// System
	input							CLK, nRST,					// main clock & reset (active low)
	// input
	input	[`RANGE_UINST]			UINST,						// input instructions : phase #1's main
	input	[`RANGE_GPRs]			REF_ADDR_DEST,				// reference address : dest
	input	[`RANGE_INDEX]			REF_ADDR_RELATIVE,			// reference address : relative
	input	[`RANGE_GPRs]			REF_MEM_SRC,				// reference source address for memory operation
	input	[`RANGE_DWORDx4]		SRC0, SRC1,					// sources input : phase #1's source

	// output
	output	reg	[`RANGE_MEMID]		MEM_nEN,					// memory operatio enable (active low)
	output	reg						MEM_WRITE,					// memory write enable
	output	reg [`RANGE_WORD]		MEM_ADDR,					// memory address
	output	reg [`RANGE_GPRs]		MEM_SRC,					// memory source
	output	reg [`RANGE_DWORDx4]	MEM_DATA_0, MEM_DATA_1		// memory source data
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
wire					sm_nEN		= UINST[`RANGE_UINST_nEN] | (UINST[`RANGE_UINST_MO]!=`MO_MEM),
sm_WRITE	= UINST[21];
wire	[3:0]			sm_ID		= UINST[25:22];
wire	[`RANGE_GPRs]	sm_SRC		= REF_MEM_SRC + REF_ADDR_DEST;
wire	[`RANGE_INDEX]	sm_ADDR		= UINST[`RANGE_INDEX] + REF_ADDR_RELATIVE;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		MEM_nEN		<= `MEMID_nRESET;	// memory enable
		MEM_WRITE	<= 0;				// write
		MEM_ADDR	<= 0;				// address
		MEM_SRC		<= 0;				// source index
		MEM_DATA_0	<= 128'b0;			// output data
		MEM_DATA_1	<= 128'b0;			// output data
	end
	else begin
		MEM_nEN[`MEMID_LMB]			<= sm_nEN | (sm_ID!=`MEMID_LMB);
		MEM_nEN[`MEMID_SCRATCH]		<= sm_nEN | (sm_ID!=`MEMID_SCRATCH);
		MEM_nEN[`MEMID_STREAM]		<= sm_nEN | (sm_ID!=`MEMID_STREAM);
		MEM_nEN[`MEMID_PACKUNPACK]	<= sm_nEN | (sm_ID!=`MEMID_PACKUNPACK);
		if(!sm_nEN) begin
			MEM_WRITE	<= sm_WRITE;
			MEM_ADDR	<= {UINST[15:`SIZE_INDEX], sm_ADDR};
			MEM_SRC		<= sm_SRC;
			MEM_DATA_0	<= SRC0;
			MEM_DATA_1	<= SRC1;
		end
	end
end

endmodule
