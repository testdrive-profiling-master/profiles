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

module MTSP_InterBus #(
	parameter	HIGH_ADDRESS		= 4'h8
) (
	// System
	input							CLK, nRST,					// main clock & reset

	// control interface
	i_mem_command_desc.s			mem_desc,					// memory description
	output	reg						MEM_BUSY,					// memory operation is busy

	// Queue data interface
	output	reg						QW_WE,						// write enable
	output	reg [`RANGE_DWORDx8]	QW_DATA,					// write data
	output							QR_RE,						// read enable
	input	[`RANGE_DWORDx8]		QR_DATA,					// read data
	input							QR_VALID,					// read valid

	// memory control
	i_local_memory_control.m		local_memory				// local memory control
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[7:0]					r_SIZE;
reg								r_EI_TOGGLE;
reg								r_EX_R_nEN;

wire	w_EI_REQ				= mem_desc.req_ext;
wire	w_EI_nRE				= ~QR_VALID;
wire	w_EI_nEN				= w_EI_nRE;

wire	w_EX_REQ				= mem_desc.req_ext;
wire	w_EX_nRE				= ((~local_memory.en)|local_memory.ready) ? ((~QR_VALID)|(~local_memory.we)) : (~local_memory.en);
wire	w_EX_nQWE				= (~local_memory.valid) | mem_desc.cache_only;

wire	[7:0]					w_SIZE	= (
	mem_desc.req_ext											? mem_desc.size : r_SIZE

);
wire	w_MEM_BUSY				= mem_desc.req_ext|(|w_SIZE);

assign	QR_RE					= ~(local_memory.en & (~local_memory.ready));

// implementation ------------------------------------------------------------
// transaction for instruction registers
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_EI_TOGGLE			<= 0;
	end
	else begin
		r_EI_TOGGLE			<= (w_EI_nEN ? r_EI_TOGGLE : ~r_EI_TOGGLE) & (~w_EI_REQ);
	end
end

// transaction for external registers
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		local_memory.en		<= `FALSE;
		local_memory.we		<= `FALSE;
		local_memory.addr	<= 'd0;
		local_memory.wdata	<= 256'b0;

		// Q write
		QW_WE				<= `FALSE;
		QW_DATA				<= 256'b0;
	end
	else begin
		local_memory.en		<= ~((
								  mem_desc.req_ext		? (~mem_desc.we) :
								  (~local_memory.we)	? ~w_MEM_BUSY :		// LMBs read case
								  (((~local_memory.en)|local_memory.ready) ? (~QR_VALID) : (~local_memory.en))));			// LMBs write ÀÏ °æ¿ì

		local_memory.we		<= w_EX_REQ		? (~mem_desc.we) : local_memory.we;	// if write command, we proceed external read
		local_memory.addr	<= w_EX_REQ		? mem_desc.index[`RANGE_INDEX] :
		(~local_memory.en)|(~local_memory.ready)	? local_memory.addr : (local_memory.addr + 1'b1);
		local_memory.wdata	<= ((~QR_RE)|(~QR_VALID)) ? local_memory.wdata : QR_DATA;

		// Q write
		QW_WE			<= ~w_EX_nQWE;
		QW_DATA			<= w_EX_nQWE	? QW_DATA : local_memory.rdata;
	end
end

// manage lifecycle of transaction
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_SIZE			<= 0;
		MEM_BUSY		<= `FALSE;
	end
	else begin
		r_SIZE			<= w_SIZE;
		MEM_BUSY		<= w_MEM_BUSY;
	end
end

endmodule
