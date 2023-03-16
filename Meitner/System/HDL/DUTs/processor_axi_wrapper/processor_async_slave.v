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
// Title : processor AXI wrapper
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

module processor_async_slave #(
	parameter	ADDR_WIDTH			= 4,
	parameter	WFIFO_DEPTH			= 2,
	parameter	RFIFO_DEPTH			= 2
) (
	// bus slave side
	input							S_CLK,				// clock
	input							S_nRST,				// reset (active low)
	input							S_EN,				// enable
	input							S_WE,				// write enable
	input	[ADDR_WIDTH-1:0]		S_ADDR,				// address
	input	[`RANGE_DWORD]			S_WDATA,			// write data
	output	[`RANGE_DWORD]			S_RDATA,			// read data
	output							S_READY,			// ready
	// processor side
	input							P_CLK,				// clock
	input							P_nRST,				// reset (active low)
	output							P_EN,				// slave enable
	output							P_WE,				// write enable
	output	[ADDR_WIDTH-1:0]		P_ADDR,				// address
	output	[`RANGE_DWORD]			P_WDATA,			// write data
	input	[`RANGE_DWORD]			P_RDATA,			// read data
	input							P_READY				// data ready
);

// definition & assignment ---------------------------------------------------
wire					wfifo_full, wfifo_empty, rfifo_full, rfifo_empty;
reg						read_phase;			// read phase operation

assign	S_READY			= S_EN & (S_WE ? (~wfifo_full) : (~rfifo_empty));
assign	P_EN			= ~wfifo_empty;

// implementation ------------------------------------------------------------
// write bus FIFO
FiFo_Async #(
	.FIFO_DEPTH		(WFIFO_DEPTH),
	.DATA_WIDTH		(`SIZE_DWORD + ADDR_WIDTH + 1)
) write_fifo (
	// push interface
	.W_CLK			(S_CLK),
	.W_nRST			(S_nRST & P_nRST),
	.W_nEN			(~(S_EN & (~read_phase))),
	.W_DATA			({S_WE, S_ADDR, S_WDATA}),
	.W_FULL			(wfifo_full),
	// pop interface
	.R_CLK			(P_CLK),
	.R_nRST			(P_nRST),
	.R_nEN			(~P_READY),
	.R_DATA			({P_WE, P_ADDR, P_WDATA}),
	.R_EMPTY		(wfifo_empty)
);

// read bus FIFO
FiFo_Async #(
	.FIFO_DEPTH		(RFIFO_DEPTH),
	.DATA_WIDTH		(`SIZE_DWORD)
) read_fifo (
	// push interface
	.W_CLK			(P_CLK),
	.W_nRST			(P_nRST),
	.W_nEN			(~(P_EN & (~P_WE) & P_READY)),
	.W_DATA			(P_RDATA),
	.W_FULL			(rfifo_full),
	// pop interface
	.R_CLK			(S_CLK),
	.R_nRST			(S_nRST & P_nRST),
	.R_nEN			((~S_EN) | S_WE),
	.R_DATA			(S_RDATA),
	.R_EMPTY		(rfifo_empty)
);

// read phase operation
always@(posedge S_CLK, negedge S_nRST) begin
	if(!S_nRST) begin
		read_phase		<= 1'b0;
	end
	else begin
		if(!read_phase) begin
			read_phase		<= (~wfifo_full) & S_EN & (~S_WE);
		end
		else begin
			read_phase		<= (~rfifo_empty) & (~wfifo_empty) & (~P_WE);
		end
	end
end

endmodule
