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
`ifndef __TESTDRIVE_SRAM_DUAL_ASYNC_V__
`define __TESTDRIVE_SRAM_DUAL_ASYNC_V__
`timescale 1ns/1ns

module SRAM_Dual_async #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 4
) (
	// write side
	input							W_CLK,			// write clock
	input							W_nEN,			// write enable (active low)
	input	[(ADDR_WIDTH-1):0]		W_ADDR,			// write address
	input	[(DATA_WIDTH-1):0]		W_DATA,			// write data

	// read side
	input							R_CLK,			// read clock
	input							R_nEN,			// read enable (active low)
	input	[(ADDR_WIDTH-1):0]		R_ADDR,			// read address
	output	reg [(DATA_WIDTH-1):0]	R_DATA			// read data
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0];

// implementation ------------------------------------------------------------
always@(posedge W_CLK) begin
	if(!W_nEN)
		mem[W_ADDR]	<= W_DATA;
end

always@(posedge R_CLK) begin
	if(!R_nEN)
		R_DATA		<= mem[R_ADDR];
end

endmodule

`endif//__TESTDRIVE_SRAM_DUAL_ASYNC_V__
