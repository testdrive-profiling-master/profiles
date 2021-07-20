//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : MTSP(Multi-thread Stream Processor) 'Meitner' v1.1
// Rev.  : 7/20/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_Bus(
	// system
	input							CLK, nRST,					// main clock & reset (active low)
	input	[`RANGE_TRDs]			nTRD,						// Thread enable (active low)

	// input
	input							MEM_nEN,					// Enable (active low)
	input							MEM_WRITE,					// Write enable (active low)
	input	[`RANGE_WORD]			MEM_ADDR,					// address
	input	[`RANGE_DWORDx4]		MEM_DATA,					// Data input
	output							MEM_STALL,					// memory stall

	// thread control
	output							PC_nWAIT,					// thread wait for memory transaction
	output	reg [`RANGE_TRDs]		PC_nAWAKE,					// thread awake to processing the thread

	// External bus control
	i_memory_command.m				mem_command					// memory command
);
// register definition & assignment ------------------------------------------
reg		[`RANGE_TRDs]			r_prefetch_nTRD;								// prefetched thread mask

wire	[1:0]					m_OPT			= MEM_ADDR[15:14];				// 00 : normal, 01 : bypass, 10 : prefetch, 11 : wait
wire							w_nPREFETCH		= (m_OPT!=2'b10);
wire	[`RANGE_TRDs]			awake_nTRD, q_awake_nTRD;
wire							q_FULL, q_EMPTY, q_nPREFETCH;
wire							w_pf_nSTALL		= &(nTRD|r_prefetch_nTRD);		// prefetch stall
wire							q_nWE			= MEM_nEN | (&m_OPT) | q_FULL;	// not a Q full & not a wait command

assign	MEM_STALL				= q_FULL & (~MEM_nEN);							// Q is full at any command
assign	PC_nWAIT				= MEM_nEN | q_FULL |							// Q is full at any command
((|m_OPT) & 									// normal command,
 ((~&m_OPT) | (w_pf_nSTALL)));					// wait command & prefetch stall

assign	awake_nTRD				= (m_OPT!=2'b01) ? nTRD : `RESET_TRDs;

assign	mem_command.en			= ~q_EMPTY;

// implementation ------------------------------------------------------------
// Stream buffer external bus command FIFO (16 entries)
FiFo #(
	.FIFO_DEPTH		(4),
	.DATA_WIDTH		((1+`SIZE_TRDs+`SIZE_DWORDx4))
) bus_queue (
	.CLK			(CLK),
	.nCLR			(nRST),
	.nWE			(q_nWE),
	.DIN			({w_nPREFETCH, awake_nTRD, 1'b0, MEM_WRITE, MEM_ADDR[13:0], MEM_DATA[`RANGE_DWORDx4_XL], MEM_DATA[`RANGE_DWORDx4_Y], MEM_DATA[`RANGE_DWORDx4_Z], MEM_DATA[`RANGE_DWORDx4_W]}),
	.FULL			(q_FULL),
	.nRE			(~mem_command.valid),
	.DOUT			({q_nPREFETCH, q_awake_nTRD, mem_command.desc}),
	.EMPTY			(q_EMPTY)
);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_prefetch_nTRD		<= `RESET_TRDs;
		PC_nAWAKE			<= `RESET_TRDs;
	end
	else begin
		r_prefetch_nTRD		<= (r_prefetch_nTRD & ((MEM_nEN|w_nPREFETCH) ? `RESET_TRDs : nTRD)) | (((~mem_command.valid)|q_nPREFETCH) ? 0 : (~q_awake_nTRD));
		PC_nAWAKE			<= mem_command.valid ? q_awake_nTRD : `RESET_TRDs;
	end
end

endmodule
