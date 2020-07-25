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

module MTSP_TRDs_Primitive(
	// system
	input						CLK, nRST,			// main clock & reset (active low)

	// instruction fetch
	input	[`RANGE_TRDs]		IF_nEN,				// enable (active low)
	output						IF_RUN,				// run instruction
	output	[`RANGE_PC]			IF_PC,				// program counter

	// input
	input						PC_nEN,				// PC operation enable (active low)
	input	[`RANGE_TRDs]		PC_nTRD,			// Thread enable (active low)
	input	[`RANGE_BR]			PC_BOP,				// local branch operation
	input	[`RANGE_PC]			PC_NEXT,			// next PC output
	input						PC_nWAIT,			// thread wait for memory transaction
	input	[`RANGE_TRDs]		PC_nAWAKE,			// threa awake to processing the thread

	// output
	output	reg					TRD_BUSY,			// some threads is busy
	output						TRD_SYNC			// thread sync.
);

// register definition & assignment ------------------------------------------
genvar i;

wire	[`RANGE_PC]			pc[`RANGE_TRDs];
wire	[`RANGE_TRDs]		busy, run;
wire						nSolitude;
wire						w_PC_nWAIT;

assign	IF_PC				= !IF_nEN[0] ? pc[0] :
!IF_nEN[1] ? pc[1] :
!IF_nEN[2] ? pc[2] :
!IF_nEN[3] ? pc[3] :
!IF_nEN[4] ? pc[4] :
!IF_nEN[5] ? pc[5] :
!IF_nEN[6] ? pc[6] :
!IF_nEN[7] ? pc[7] :
!IF_nEN[8] ? pc[8] :
!IF_nEN[9] ? pc[9] :
!IF_nEN[10] ? pc[10] : pc[11];
assign	IF_RUN				= |((~IF_nEN) & run);
//assign	TRD_BUSY			= |busy;
assign	nSolitude			= |(PC_nTRD & busy);
assign	w_PC_nWAIT			= PC_nWAIT & (nSolitude|(~PC_BOP[2]));
assign	TRD_SYNC			= (~nSolitude) & PC_BOP[2] & (~PC_nEN);

// implementation ------------------------------------------------------------

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		TRD_BUSY	<= `FALSE;
	end
	else begin
		TRD_BUSY	<= |busy;
	end
end

//BOP	00 : jmp (jump current to PC_NEXT)
//		01 : all (revolt all thread to PC_NEXT)
//		10 : end (end current thread)
//		11 : seq (sequencing thread)

wire			EN_ALL		= (~PC_nEN) & (PC_BOP[1:0]==2'b01);

generate
for(i=0;i<12;i=i+1) begin : thread_status
	MTSP_TRDs_Status status (
		.CLK			(CLK),
		.nRST			(nRST),
		.EN_ALL			(EN_ALL),
		.nEN			((PC_nEN|PC_nTRD[i])),
		.BOP			(PC_BOP[1:0]),
		.PC_NEXT		(PC_NEXT),
		.nWAIT			(w_PC_nWAIT),
		.nAWAKE			(PC_nAWAKE[i]),
		.nSOLITUDE		(nSolitude),
		.PC				(pc[i]),
		.BUSY			(busy[i]),
		.RUN			(run[i])
	);
end
endgenerate

endmodule
