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
`ifndef __TESTDRIVE_GRAY_COUNTER_V__
`define __TESTDRIVE_GRAY_COUNTER_V__
`timescale 1ns/1ns

module GrayCounter #(
	parameter	WIDTH				= 4
)(
	// system signals
	input							CLK,			// clock
	input							nRST,			// reset (active low)
	input							INC,			// increase
	output	reg	[(WIDTH-1):0]		COUNT			// count value
);
// synopsys template

// register definition & assignment ------------------------------------------
reg		[(WIDTH-1):0]			binary_count;

// implementation ------------------------------------------------------------
// counting...
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		binary_count	<= {{(WIDTH-1){1'b0}}, 1'b1};	// start with '1'
		COUNT			<= {WIDTH{1'b0}};		    	// start with '0'
	end
	else begin
		if(INC) begin
			binary_count	<= binary_count + 1'b1;
			COUNT			<= {binary_count[WIDTH-1], binary_count[(WIDTH-1):1] ^ binary_count[(WIDTH-2):0]};
		end
	end
end

endmodule

`endif//__TESTDRIVE_GRAY_COUNTER_V__
