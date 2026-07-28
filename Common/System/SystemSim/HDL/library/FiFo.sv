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
// Title : Common verilog library
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_FIFO_V__
`define __TESTDRIVE_FIFO_V__
`timescale 1ns/1ns

module FiFo #(
	parameter	FIFO_DEPTH			= 4,
	parameter	DATA_WIDTH			= 32
) (
	// system signals
	input							CLK,			// clock
	input							nCLR,			// clear (active low)

	// push interface
	input							nWE,			// write enable (active low)
	input	[(DATA_WIDTH-1):0]		DIN,			// input data
	output							FULL,			// fifo is full
	// pop interface
	input							nRE,			// read enable (active low)
	output	[(DATA_WIDTH-1):0]		DOUT,			// output data
	output							EMPTY			// fifo is empty
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg [FIFO_DEPTH-1:0]				w_addr;
reg [FIFO_DEPTH-1:0]				r_addr;
reg [FIFO_DEPTH:0]					status_full;
reg [FIFO_DEPTH:0]					status_empty;

wire	valid_we_n					= nWE | FULL;
wire	valid_re_n					= nRE | EMPTY;

assign FULL		= status_full[FIFO_DEPTH];
assign EMPTY	= status_empty[FIFO_DEPTH];

// implementation ------------------------------------------------------------
always @ (posedge CLK) begin
	if (!nCLR) begin
		w_addr			<= {FIFO_DEPTH{1'b0}};
		r_addr			<= {FIFO_DEPTH{1'b0}};
		status_full		<= {(FIFO_DEPTH+1){1'b0}};
		status_empty	<= {1'b1, {(FIFO_DEPTH){1'b0}}};
	end
	else begin
		if (!valid_we_n)
			w_addr	<= w_addr + 1'b1;
		if (!valid_re_n)
			r_addr	<= r_addr + 1'b1;

		if (valid_re_n ^ valid_we_n) begin
			status_full <= status_full + {{FIFO_DEPTH{valid_we_n}}, 1'b1};
		end

		if (valid_re_n ^ valid_we_n) begin
			status_empty <= status_empty + {{FIFO_DEPTH{valid_re_n}}, 1'b1};
		end
	end
end

SRAM_Dual_Distributed #(
	.ADDR_WIDTH			(FIFO_DEPTH),
	.DATA_WIDTH			(DATA_WIDTH)
) mem (
	// System
	.CLK				(CLK),
	// SRAM interface
	.nCE				(valid_we_n & valid_re_n),
	.nWE				(valid_we_n),
	.WADDR				(w_addr),
	.RADDR				(r_addr),
	.DIN				(DIN),
	.DOUT				(DOUT)
);

endmodule

`endif//__TESTDRIVE_FIFO_V__
