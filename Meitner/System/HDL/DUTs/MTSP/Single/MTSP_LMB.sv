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

module MTSP_LMB #(
	parameter	CORE_ID			= 0
) (
	// system
	input							CLK, nRST,				// main clock & reset (active low)

	i_local_memory_control.s		local_memory,			// local memory control (LMB)

	// core interface
	input							MEM_nEN,				// enable (active low)
	input							MEM_WRITE,				// write enable
	input	[`RANGE_LMB]			MEM_ADDR,				// address
	input	[`RANGE_DWORDx8]		MEM_WDATA,				// write data
	input	[`RANGE_GPRs]			MEM_SRC,				// destination address

	// extended write
	output	reg 					EW_nEN,					// write enable (active low) - phase #0/#1 common signal
	output	reg	[`RANGE_GPRs]		EW0_ADDR, EW1_ADDR,		// address
	output	reg [`RANGE_DWORDx4]	EW0_DATA, EW1_DATA		// data
);

// register definition & assignment ------------------------------------------
wire						n_ce				= (~local_memory.en) & MEM_nEN;
wire						n_we				= MEM_nEN ? (~local_memory.we) : (~MEM_WRITE);
wire	[`RANGE_LMB]		addr				= MEM_nEN ? local_memory.addr : MEM_ADDR;
wire	[`RANGE_DWORDx8]	din					= MEM_nEN ? local_memory.wdata : MEM_WDATA;
wire	[`RANGE_DWORDx8]	dout;

assign						local_memory.ready	= MEM_nEN;	// core priority is higher

wire						mem_n_ren			= MEM_nEN | MEM_WRITE;
reg							r_ex_valid, r_mem_n_ren;
reg		[`RANGE_GPRs]		r_mem_addr;

// implementation ------------------------------------------------------------
// Global registers bank
SRAM_Single #(	// 256 bit single sram
	.ADDR_WIDTH		(`SIZE_LMB),
	.DATA_WIDTH		(`SIZE_DWORDx8)
) sram (
	.CLK			(CLK),
	.nCE			(n_ce),
	.nWE			(n_we),
	.ADDR			(addr),
	.DIN			(din),
	.DOUT			(dout)
);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_ex_valid			<= `FALSE;			// external valid output
		local_memory.rdata	<= 256'd0;
		local_memory.valid	<= `FALSE;

		r_mem_n_ren			<= `nFALSE;			// core read enable (active low)
		r_mem_addr			<= 0;
		EW_nEN				<= `nFALSE;
		EW0_ADDR			<= 0;
		EW1_ADDR			<= 0;
		EW0_DATA			<= 128'b0;
		EW1_DATA			<= 128'b0;
	end
	else begin
		r_ex_valid			<= local_memory.ready & (~local_memory.we) & local_memory.en;
		local_memory.rdata	<= r_ex_valid ? dout : local_memory.rdata;
		local_memory.valid	<= r_ex_valid;

		r_mem_n_ren			<= mem_n_ren;
		r_mem_addr			<= mem_n_ren ? r_mem_addr : MEM_SRC;

		EW_nEN				<= r_mem_n_ren;
		EW0_ADDR			<= r_mem_addr;
		EW1_ADDR			<= r_mem_addr + 1;
		{EW0_DATA, EW1_DATA}	<= r_mem_n_ren ? {EW0_DATA, EW1_DATA} : dout;
	end
end

`ifdef USE_TESTDRIVE
int t;
`DPI_FUNCTION void MTSP_set_LMB(input int unsigned dwCoreID, input int unsigned dwIndex, input bit [`RANGE_DWORDx8] DATA);

initial begin
	for(t=0;t<`COUNT_LMB;t++) begin
		MTSP_set_LMB(CORE_ID, t, sram.mem[t]);
	end
end

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
	end
	else begin
		if(!MEM_nEN && MEM_WRITE)
			MTSP_set_LMB(CORE_ID, MEM_ADDR, MEM_WDATA);
	end
end
`endif

endmodule
