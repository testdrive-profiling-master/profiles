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
`ifndef __TESTDRIVE_SRAM_SINGLE_V__
`define __TESTDRIVE_SRAM_SINGLE_V__
`timescale 1ns/1ns

module SRAM_Single #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 4
) (
	// System
	input							CLK,			// clock
	// SRAM interface
	input							nCE,			// Chip enable (active low)
	input							nWE,			// write enable (active low)
	input	[(ADDR_WIDTH-1):0]		ADDR,			// address
	input	[(DATA_WIDTH-1):0]		DIN,			// input data
	output	reg [(DATA_WIDTH-1):0]	DOUT			// output data
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0];

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nCE) begin
		if(!nWE)
			mem[ADDR]	<= DIN;
		DOUT	<= mem[ADDR];
	end
end

endmodule

`endif//__TESTDRIVE_SRAM_SINGLE_V__
