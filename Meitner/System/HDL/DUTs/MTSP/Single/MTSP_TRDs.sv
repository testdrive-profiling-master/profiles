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

module MTSP_TRDs(
	// system
	input						CLK, nRST,			// main clock & reset (active low)

	// input
	input						PC_nEN,				// PC operation enable (active low)
	input	[`RANGE_BR]			PC_BOP,				// branch operation
	input	[`RANGE_PC]			PC_NEXT,			// next PC output
	input						PC_nWAIT,			// thread wait for memory transaction
	input	[`RANGE_TRDs]		PC_nAWAKE,			// thread awake to processing the thread

	// output
	output	reg [`RANGE_TRDs]	nTRD,				// Thread enable (active low)
	output	reg 				TRD_nEN,			// Thread enable
	output	reg [`RANGE_PC]		TRD_PC,				// Thread program counter
	output						TRD_BUSY,			// Thread busy (core is running)

	// multi-core sync
	i_sync.m					core_sync			// sync. of multi-core
);

// register definition & assignment ------------------------------------------
reg		[`RANGE_TRDs]		r_SYNC_nTRD, r_PC_nAWAKE;
wire						IF_run;
wire	[`RANGE_PC]			IF_pc;
wire						w_TRD_SYNC;			// thread sync.

// implementation ------------------------------------------------------------
MTSP_TRDs_Primitive	TRD_Primitive(
	.CLK			(CLK),
	.nRST			(nRST),
	.IF_nEN			({nTRD[(`MSB_TRDs-1):0], nTRD[`MSB_TRDs]}),
	.IF_RUN			(IF_run),
	.IF_PC			(IF_pc),
	.PC_nEN			(PC_nEN),
	.PC_nTRD		({nTRD[(`MSB_TRDs-3):0], nTRD[`MSB_TRDs:(`MSB_TRDs-2)]}),
	.PC_BOP			(PC_BOP),
	.PC_NEXT		(PC_NEXT),
	.PC_nWAIT		(PC_nWAIT),
	.PC_nAWAKE		(r_PC_nAWAKE),
	.TRD_BUSY		(TRD_BUSY),
	.TRD_SYNC		(w_TRD_SYNC)
);

//reg aaa= 0;
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		nTRD			<= `DEFUALT_TRDs;
		TRD_nEN			<= `nFALSE;
		TRD_PC			<= 'd0;

		core_sync.en	<= `FALSE;
		r_SYNC_nTRD		<= `RESET_TRDs;

		r_PC_nAWAKE		<= `RESET_TRDs;
	end
	else begin
		nTRD			<= {nTRD[(`MSB_TRDs-1):0], nTRD[`MSB_TRDs]};
		TRD_nEN			<= ~IF_run;
		TRD_PC			<= IF_pc;

		core_sync.en	<= w_TRD_SYNC;
		r_SYNC_nTRD		<= w_TRD_SYNC	? {nTRD[(`MSB_TRDs-3):0], nTRD[`MSB_TRDs:(`MSB_TRDs-2)]} : r_SYNC_nTRD;

		r_PC_nAWAKE		<= PC_nAWAKE & (core_sync.ack ? r_SYNC_nTRD : `RESET_TRDs);
	end
end

endmodule
