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

module MTSP_BRC(
	// System
	input							CLK, nRST,				// main clock & reset (active low)
	// External thread control
	i_thread_control.s				thread_command,			// thread control

	// input
	input							nEN_BRC,				// branch enable (active low)
	input	[`RANGE_UINST]			UINST,					// input instructions
	input	[`RANGE_PC]				PC,						// program counter input
	input	[`RANGE_INDEX]			REF_ADDR_RELATIVE,		// reference address : dest, relative

	// output
	output	reg						PC_nEN,					// PC operation enable (active low)
	output	reg [`RANGE_BR]			PC_BO,					// branch operation
	output	reg [`RANGE_PC]			PC_NEXT					// next PC output
);

// definition & assignment ---------------------------------------------------
wire					pipe_en;
wire	[`RANGE_PC]		pipe_addr_0, pipe_addr_1;

// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
// branch enable
wire					sm_enable	= (~UINST[`RANGE_UINST_nEN]) & (UINST[`RANGE_UINST_MO]==`MO_BRC);
// branch operation
wire	[`RANGE_BR]		sm_BO		= sm_enable ? UINST[24:21] : `BR_DEFAULT;
// branch address
wire	[`RANGE_PC]		sm_pc		= {{(`SIZE_PC-12){UINST[11]}}, UINST[11:0]} + {{(`SIZE_PC-`SIZE_INDEX){REF_ADDR_RELATIVE[`SIZE_INDEX-1]}}, REF_ADDR_RELATIVE} + (UINST[25] ? PC : 0);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		PC_nEN		<= `nFALSE;			// enable (active low)
		PC_BO		<= 0;				// branch operation
		PC_NEXT		<= 'd0;				// PC
	end
	else begin
		PC_nEN		<= nEN_BRC & (~thread_command.en);
		PC_BO		<= nEN_BRC ? {2'b0, ~thread_command.op, thread_command.op} : sm_BO;	// 0000(JMP), 0001(ALL), 0010(END), 0011(SEQ)
		PC_NEXT		<= nEN_BRC ? thread_command.pc : (sm_enable ? sm_pc: PC);
	end
end

endmodule
