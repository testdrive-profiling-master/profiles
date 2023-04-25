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
// Title : Processor
// Rev.  : 4/25/2023 Tue (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

module SRAM_Single_multicycled #(
	parameter				CYCLE	= 2
) (
	input					CLK,	// clock
	input					nRST,	// reset (active low)
	input					EN,		// chip enable
	input					WE,		// write enable
	output					READY,	// input ready
	input	[7:0]			ADDR,	// address
	input	[63:0]			DIN,	// data input
	output					OE,		// output enable
	output	reg [63:0]		DOUT	// data output
);

// definition & assignment ---------------------------------------------------
wire	[63:0]				w_dout;
reg		[63:0]				r_dout;
reg							r_EN;
reg							r_WE;
reg		[7:0]				r_ADDR;
reg		[63:0]				r_DIN;

// implementation ------------------------------------------------------------
always@(posedge CLK) begin	// for checking clock MHz
	if(!nRST) begin
		r_EN	<= 1'b0;
		r_WE	<= 1'b0;
		r_ADDR	<= 'd0;
		r_DIN	<= 'd0;
	end
	else begin
		r_EN	<= EN;
		r_WE	<= WE;
		r_ADDR	<= ADDR;
		r_DIN	<= DIN;
	end
end

SRAM_Single_Multicycle #(
	.ADDR_WIDTH		(8),
	.DATA_WIDTH		(64),
	.CYCLE			(CYCLE)
) sram (
	.CLK			(CLK),
	.nRST			(nRST),
	.EN				(r_EN),
	.WE				(r_WE),
	.READY			(READY),
	.ADDR			(r_ADDR),
	.DIN			(r_DIN),
	.OE				(OE),
	.DOUT			(w_dout)
);

always@(posedge CLK) begin	// for multi-cycle contraint 'PIN' to 'CELL' search configuration
	if(!nRST) begin
		r_dout	<= 'd0;
		DOUT	<= 'd0;

	end
	else begin
		r_dout	<= w_dout;
		DOUT	<= r_dout;
	end
end

endmodule
