//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common verilog library
// Rev.  : 5/25/2023 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_FIFO_ASYNC_V__
`define __TESTDRIVE_FIFO_ASYNC_V__
`timescale 1ns/1ns
/* TOP_MODULE = FiFo_Async */

module FiFo_Async_Status #(
	parameter	FIFO_DEPTH			= 4,
	parameter	DEFAULT				= 1'b0
) (
	input							CLK, nRST,		// clock & reset (active low)
	input							nEN,			// increase enable (active low)
	output [FIFO_DEPTH-1:0]			ADDR,			// fifo address
	output reg [FIFO_DEPTH :0]		PTR,			// pointer (gray code)
	input [FIFO_DEPTH :0]			PTR_CMP,		// check ptr
	output reg						STATUS			// status
);
// synopsys template

// register definition & assignment ------------------------------------------
reg		[FIFO_DEPTH:0]			bin;
wire	[FIFO_DEPTH:0]			ptr_next, bin_next;
wire							status_next;
wire							inc;
// implementation ------------------------------------------------------------
always @(posedge CLK, negedge nRST) begin
	if (!nRST)
		{bin, PTR} <= {((FIFO_DEPTH+1) * 2){1'b0}};
	else
		{bin, PTR} <= {bin_next, ptr_next};
end

assign	ADDR			= bin[FIFO_DEPTH-1:0];			// current address
assign	inc				= ~(nEN | STATUS);				// increase
assign	bin_next		= bin + {{(FIFO_DEPTH-1){1'b0}}, inc};		// next address
assign	ptr_next		= {bin_next[FIFO_DEPTH], (bin_next[FIFO_DEPTH:1] ^bin_next[FIFO_DEPTH-1:0])};		// next pointer

assign	status_next		= (ptr_next == PTR_CMP);

always @(posedge CLK, negedge nRST) begin
	if (!nRST)
		STATUS		<= DEFAULT;
	else
		STATUS		<= status_next;
end

endmodule

module FiFo_Async #(
	parameter FIFO_DEPTH		= 4,				// (FIFO_DEPTH >= 2)
	parameter DATA_WIDTH		= 32				// data width
) (
	// write side
	input							W_CLK,			// write clock
	input							W_nRST,			// reset (active low)
	input							W_nEN,			// write enable (active low)
	input [DATA_WIDTH-1:0]			W_DATA,			// write data
	output							W_FULL,			// fifo is full
	// read side
	input							R_CLK,			// read clock
	input							R_nRST,			// reset (active low)
	input							R_nEN,			// read enable (active low)
	output [DATA_WIDTH-1:0]			R_DATA,			// read data
	output							R_EMPTY			// fifo is empty
);
// synopsys template

// register definition & assignment ------------------------------------------
wire [FIFO_DEPTH-1:0]		w_addr, r_addr;			// index
wire [FIFO_DEPTH:0]			w_ptr, r_ptr;			// pointer (gray code)
wire [FIFO_DEPTH:0]			w2r_ptr, r2w_wptr;		// pipelined pointer (gray code)

// implementation ------------------------------------------------------------
SyncPipe #(
	.WIDTH			(FIFO_DEPTH+1),
	.LEVEL			(2)
) sync_r2w (
	.CLK			(W_CLK),
	.nRST			(W_nRST),
	.DIN			(r_ptr),
	.DOUT			(r2w_wptr)
);

SyncPipe #(
	.WIDTH			(FIFO_DEPTH+1),
	.LEVEL			(2)
) sync_w2r (
	.CLK			(R_CLK),
	.nRST			(R_nRST),
	.DIN			(w_ptr),
	.DOUT			(w2r_ptr)
);

SRAM_Dual_Distributed #(
	.ADDR_WIDTH		(FIFO_DEPTH),
	.DATA_WIDTH		(DATA_WIDTH)
) mem (
	.CLK			(W_CLK),
	.nCE			(W_FULL),
	.nWE			(W_nEN),
	.WADDR			(w_addr),
	.RADDR			(r_addr),
	.DIN			(W_DATA),
	.DOUT			(R_DATA)
);

FiFo_Async_Status #(
	.FIFO_DEPTH		(FIFO_DEPTH),
	.DEFAULT		(1'b1)
) empty_status (
	.CLK			(R_CLK),
	.nRST			(R_nRST),
	.nEN			(R_nEN),
	.ADDR			(r_addr),
	.PTR			(r_ptr),
	.PTR_CMP		(w2r_ptr),
	.STATUS			(R_EMPTY)
);

FiFo_Async_Status #(
	.FIFO_DEPTH		(FIFO_DEPTH),
	.DEFAULT		(1'b0)
) full_status (
	.CLK			(W_CLK),
	.nRST			(W_nRST),
	.nEN			(W_nEN),
	.ADDR			(w_addr),
	.PTR			(w_ptr),
	.PTR_CMP		({~r2w_wptr[FIFO_DEPTH:FIFO_DEPTH-1], r2w_wptr[FIFO_DEPTH-2:0]}),
	.STATUS			(W_FULL)
);

endmodule

`endif//__TESTDRIVE_FIFO_ASYNC_V__
