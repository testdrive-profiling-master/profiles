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

module SRAM_Single_conventional (
	input					CLK,	// clock
	input					nCE,	// chip enable (active low)
	input					nWE,	// write enable (active low)
	input	[7:0]			ADDR,	// address (256 entries)
	input	[63:0]			DIN,	// data input
	output	reg [63:0]		DOUT	// data output
);

// definition & assignment ---------------------------------------------------
reg							r_nCE;
reg							r_nWE;
reg		[7:0]				r_ADDR;
reg		[63:0]				r_DIN;
wire	[63:0]				w_DOUT;

// implementation ------------------------------------------------------------
always@(posedge CLK) begin	// dummy register F/F for checking clock MHz
	r_nCE	<= nCE;
	r_nWE	<= nWE;
	r_ADDR	<= ADDR;
	r_DIN	<= DIN;
	DOUT	<= w_DOUT;
end

SRAM_Single #(
	.ADDR_WIDTH		(8),
	.DATA_WIDTH		(64)
) sram (
	.CLK			(CLK),
	.nCE			(r_nCE),
	.nWE			(r_nWE),
	.ADDR			(r_ADDR),
	.DIN			(r_DIN),
	.DOUT			(w_DOUT)
);

endmodule
