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
// Title : Meitner processor v1.1
// Rev.  : 7/20/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_Command #(
	parameter	CORE_SIZE			= 1,				// core size
	parameter	QUEUE_DEPTH			= 5					// command queue bit depth(2^N, default 32 entries)
) (
	// System
	input							CLK, nRST,			// main clock & reset
	input							BUSY,				// system is busy
	output	reg						INTR,				// interrupt
	// bus
	i_bus_slave.s					bus_slave,			// slave bus interface
	// controls
	i_memory_command.m				mem_command,		// memory control
	i_thread_control.m				thread_command,		// thread control
	output	reg						CACHE_LUT_CLEAR,	// cache LUT clear
	output	reg [`RANGE_DWORD]		VIDEO_BASE			// screen base address
);

// definition & assignment ---------------------------------------------------
reg		[7:0]					q_WCOUNT;				// command write count
reg		[7:0]					q_RCOUNT;				// command read-out count
reg								q_WAIT;					// command wait status
reg		[1:0]					q_WAIT_MASK;			// command wait mask : {memory, busy}
wire							q_FULL, q_EMPTY;
wire	[3:0]					q_CMD;					// Q command id
wire	[`RANGE_DWORD]			q_DATA;					// Q command data

wire							q_EN					= (~q_WAIT) & (~q_EMPTY);
wire	[3:0]					q_DEPTH					= QUEUE_DEPTH;		// current command queue bit depth
wire	[7:0]					q_CORE_SIZE				= CORE_SIZE;

assign	bus_slave.ready			= ~q_FULL | (bus_slave.en & (~bus_slave.we));
assign	bus_slave.rdata			= {q_CORE_SIZE, q_WCOUNT, q_RCOUNT, q_DEPTH, q_FULL, q_EMPTY, mem_command.en, BUSY};	// command status

`ifdef USE_TESTDRIVE
`DPI_FUNCTION void MTSP_WriteCommand(input int iCommand, input int unsigned dwData);
`define	MTSP_WriteCommand(iCommand, dwData)		MTSP_WriteCommand(iCommand, dwData)
`else
`define	MTSP_WriteCommand(iCommand, dwData)
`endif

// implementation ------------------------------------------------------------
// command queue entries
FiFo #(
	.FIFO_DEPTH		(QUEUE_DEPTH),
	.DATA_WIDTH		((`SIZE_DWORD+4))
) command_queue (
	.CLK			(CLK),
	.nCLR			(nRST),
	.nWE			(~(bus_slave.en & (bus_slave.we))),
	.DIN			({bus_slave.addr, bus_slave.wdata}),
	.FULL			(q_FULL),
	.nRE			(q_WAIT),
	.DOUT			({q_CMD, q_DATA}),
	.EMPTY			(q_EMPTY)
);

// command controller
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		// command queue status & wait control
		q_WCOUNT				<= 'd0;
		q_RCOUNT				<= 'd0;
		q_WAIT					<= `FALSE;
		q_WAIT_MASK				<= 2'd0;
		// bus control
		mem_command.en			<= `FALSE;
		mem_command.desc		<= 128'b0;
		// thread control
		thread_command.en		<= `FALSE;
		thread_command.op		<= 1'b0;
		thread_command.pc		<= 'd0;
		// etc.
		INTR					<= `FALSE;
		VIDEO_BASE				<= 'd0;
		CACHE_LUT_CLEAR			<= `TRUE;		// initially clear cache LUT
	end
	else begin
		// command queue status
		if(bus_slave.en & (bus_slave.we) & (~q_FULL)) begin
			q_WCOUNT		<= q_WCOUNT + 1'b1;
		end
		if(q_EN) begin
			q_RCOUNT		<= q_RCOUNT + 1'b1;
			`MTSP_WriteCommand(q_CMD, q_DATA);	// for tracing...
		end

		// set memory description data
		if(q_EN && (q_CMD==4'd0))
			mem_command.desc[`BUS_RANGE(`SIZE_DWORD, 0)]	<= q_DATA;
		if(q_EN && (q_CMD==4'd1))
			mem_command.desc[`BUS_RANGE(`SIZE_DWORD, 1)]	<= q_DATA;
		if(q_EN && (q_CMD==4'd2))
			mem_command.desc[`BUS_RANGE(`SIZE_DWORD, 2)]	<= q_DATA;
		if(q_EN && (q_CMD==4'd3))
			mem_command.desc[`BUS_RANGE(`SIZE_DWORD, 3)]	<= q_DATA;

		// start program
		if(q_EN && (q_CMD==4'd4)) begin
			thread_command.en	<= `TRUE;			// thread control
			thread_command.pc	<= q_DATA[31:2];	// 32bit aligned PC
			//@TODO wait system busy when q_DATA[1] == 1
			thread_command.op	<= q_DATA[0];		// thread invoke option
		end
		else begin
			thread_command.en	<= `FALSE;
		end

		// set video base address
		if(q_EN && (q_CMD==4'd5)) begin
			VIDEO_BASE			<= q_DATA;
		end

		// memory control
		if(mem_command.en) begin
			if(mem_command.valid)
				mem_command.en		<= `FALSE;
		end
		else if(q_EN && (q_CMD==4'd3)) begin
			mem_command.en		<= `TRUE;
		end

		// signal assertion control
		if(q_EN && (q_CMD==4'd9)) begin
			INTR				<= q_DATA[0];		// invoke interrupt
			CACHE_LUT_CLEAR		<= q_DATA[1];		// cache LUT clear
		end
		else begin
			INTR				<= `FALSE;
			CACHE_LUT_CLEAR		<= `FALSE;
		end

		// command control
		if(q_EN && (q_CMD==4'd11)) begin
			q_WAIT				<= `TRUE;			// command wait
			q_WAIT_MASK			<= q_DATA[1:0];
		end
		else begin
			if(~|(q_WAIT_MASK & {mem_command.en, BUSY})) begin
				q_WAIT				<= `FALSE;
			end
		end
	end
end

endmodule
