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
`ifndef __TESTDRIVE_LFSR_COUNTER_V__
`define __TESTDRIVE_LFSR_COUNTER_V__
`timescale 1ns/1ns

module LFSRCounter #(	// LFSR(Linear feedback shift register) counter
	parameter	WIDTH				= 4				// can accept only 4,8,16,32
)(
	// system signals
	input							CLK,			// clock
	input							CLR,			// clear
	input							INC,			// increase
	output	reg	[(WIDTH-1):0]		COUNT			// count value
);
// synopsys template

// register definition & assignment ------------------------------------------

// implementation ------------------------------------------------------------
// counting...
generate
always@(posedge CLK) begin
	if(CLR) begin
		COUNT			<= {WIDTH{1'b0}};				// start with '0'
	end
	else begin
		if(INC) begin
			COUNT[(WIDTH-1):1]		<= COUNT[(WIDTH-2):0];
			if(WIDTH == 4) begin
				COUNT[0]		<= ~^COUNT[3:2];
			end
			else if(WIDTH == 8) begin
				COUNT[0]		<= ~^{COUNT[7], COUNT[5:3]};
			end
			else if(WIDTH == 16) begin
				COUNT[0]		<= ~^{COUNT[15:14], COUNT[12], COUNT[3]};
			end
			else if(WIDTH == 32) begin
				COUNT[0]		<= ~^{COUNT[31], COUNT[21], COUNT[1:0]};
			end
`ifdef USE_TESTDRIVE
			else begin
				assert(1) $error("Invalid parameter 'WIDTH' = %d", WIDTH);
			end
`endif

		end
	end
end
endgenerate

endmodule

`endif//__TESTDRIVE_LFSR_COUNTER_V__
