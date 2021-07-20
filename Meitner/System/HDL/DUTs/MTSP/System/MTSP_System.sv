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

module MTSP_System #(
	parameter	CORE_SIZE			= 4,					// multi-core size
	parameter	COMMAND_QUEUE_DEPTH	= 6						// command queue depth
) (
	// System
	input							CLK, nRST,				// main clock & reset
	output							BUSY,					// system is busy
	output							INTR,					// interrupt
	// bus interfaces
	i_bus_slave.s					bus_slave,				// slave bus
	i_bus_master.m					bus_master,				// master bus
	// external control interface
	output	[`RANGE_DWORD]			VIDEO_BASE				// base address of screen
);
// synopsys template

// definition & assignment ---------------------------------------------------
// External instruction registers interface
i_local_memory_control			local_memory;				// local memory control

i_thread_control				thread_command;				// external thread control
i_memory_command				mem_command_ex;				// memory command (external)
i_memory_command				mem_command_pu;				// memory command (processor unit)
i_mem_command_desc				mem_desc;					// memory description

// memory interface to processor
wire	[2:0]					MEM_BUSY;					// memory operation is busy
// input data
wire							MEM_nIE;					// input enable
wire	[`RANGE_DWORDx8]		MEM_IDATA;					// input data
wire							MEM_nIVALID;				// input valid
// output data
wire							MEM_nOE;					// output enable
wire	[`RANGE_DWORDx8]		MEM_ODATA;					// output data

// Cache LUT interface
wire							cache_lut_clear;			// cache LUT clear
wire							cache_hit;					// cache hit

// busy (processor & memory)
wire							busy_processor;

assign	BUSY					= busy_processor;

// R/W Queue
wire							W_EMPTY, R_EMPTY, W_nRE, R_RE, W_WE;
wire	[1:0]					R_WE;
wire	[`RANGE_DWORDx8]		W_DIN, R_DIN_MASTER, R_DIN_CACHE;
wire	[`RANGE_DWORDx8]		W_DOUT, R_DOUT;

// initialization for simulation system
`ifdef USE_TESTDRIVE
`DPI_FUNCTION int MTSP_Initialize(input int iCoreSize, input int iGMBSize, input int iLMBSize, input int iGPRSize);

initial begin
	SetSystemDescription("Meitner(MTSP)");
	if(!MTSP_Initialize(CORE_SIZE, `COUNT_GMB, `COUNT_LMB, `MEITNER_GPRs_SIZE)) begin
		$error;
	end
end
`endif

// implementation ------------------------------------------------------------
// command controller
MTSP_Command #(
	.CORE_SIZE			(CORE_SIZE),
	.QUEUE_DEPTH		(COMMAND_QUEUE_DEPTH)
) command (
	.CLK				(CLK),
	.nRST				(nRST),
	.BUSY				(BUSY),
	.INTR				(INTR),
	.bus_slave			(bus_slave),
	.mem_command		(mem_command_ex),
	.thread_command		(thread_command),
	.CACHE_LUT_CLEAR	(cache_lut_clear),
	.VIDEO_BASE			(VIDEO_BASE)
);

// bus write queue
FiFo #(
	.FIFO_DEPTH			(7),
	.DATA_WIDTH			(`SIZE_DWORDx8)
) q_write (
	.CLK				(CLK),
	.nCLR				(nRST),
	.nWE				(~W_WE),
	.DIN				(W_DIN),
	.FULL				(),
	.nRE				(W_nRE),
	.DOUT				(W_DOUT),
	.EMPTY				(W_EMPTY)
);

// bus read queue
FiFo #(
	.FIFO_DEPTH			(7),
	.DATA_WIDTH			(`SIZE_DWORDx8)
) q_read (
	.CLK				(CLK),
	.nCLR				(nRST),
	.nWE				(~|R_WE),
	.DIN				((R_WE[0] ? R_DIN_MASTER : R_DIN_CACHE)),
	.FULL				(),
	.nRE				(~R_RE),
	.DOUT				(R_DOUT),
	.EMPTY				(R_EMPTY)
);

// master bus controller
MTSP_BusMaster			bus_master_control(
	.CLK				(CLK),
	.nRST				(nRST),
	.bus_master			(bus_master),
	.mem_desc			(mem_desc),
	.MEM_BUSY			(MEM_BUSY[0]),
	.MEM_nIE			(W_EMPTY),
	.MEM_IDATA			(W_DOUT),
	.MEM_nIVALID		(W_nRE),
	.MEM_OE				(R_WE[0]),
	.MEM_ODATA			(R_DIN_MASTER)
);

// cache memory operation with GMB(global memory block)
MTSP_Cache				cache_controller(
	.CLK				(CLK),
	.nRST				(nRST),
	.QR_RE				(R_RE & (~R_EMPTY)),
	.QR_DOUT			(R_DOUT),
	.QR_WE				(R_WE[1]),
	.QR_DIN				(R_DIN_CACHE),
	.EX_VALID			(local_memory.valid),
	.EX_RDATA			(local_memory.rdata),
	.CACHE_HIT			(cache_hit),
	.mem_desc			(mem_desc),
	.MEM_BUSY			(MEM_BUSY[1])
);

// internal bus controller
MTSP_InterBus			internal_bus(
	.CLK				(CLK),
	.nRST				(nRST),
	.mem_desc			(mem_desc),
	.MEM_BUSY			(MEM_BUSY[2]),
	.QW_WE				(W_WE),
	.QW_DATA			(W_DIN),
	.QR_RE				(R_RE),
	.QR_DATA			(R_DOUT),
	.QR_VALID			(~R_EMPTY),
	.local_memory		(local_memory)
);

// Bus bridge
MTSP_MemoryCommand		memory_command(
	.CLK				(CLK),
	.nRST				(nRST),
	.mem_command_ex		(mem_command_ex),
	.mem_command_pu		(mem_command_pu),
	.CACHE_LUT_CLEAR	(cache_lut_clear),
	.CACHE_HIT			(cache_hit),
	.mem_desc			(mem_desc),
	.MEM_BUSY			((|MEM_BUSY))
);

// multi-thread multi-core stream processor
MTSP_Multi #(
	.CORE_SIZE			(CORE_SIZE)
) multi_core (
	.CLK				(CLK),
	.nRST				(nRST),
	.BUSY				(busy_processor),
	.local_memory		(local_memory),
	.thread_command		(thread_command),
	.mem_command		(mem_command_pu)
);

endmodule
