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
// Title : Processor
// Rev.  : 7/21/2021 Wed (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"

// testbench example...
module top (
	input							MCLK,			// clock
	input							nRST,			// reset (active low)
	output	reg						BUSY,			// processor is busy
	output							INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------
int		sim_count;

assign	INTR	= 0;

reg		[31:0]		A, B;
wire	[31:0]		O	= A + B;

// implementation ------------------------------------------------------------
// timing counter
always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		sim_count	= 0;
	end
	else begin
		sim_count++;
	end
end
`define ON_TIME(t)	if(sim_count==t)

// testbench example
always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		A		<= 'd0;
		B		<= 'd0;
		BUSY	<= `TRUE;
	end
	else begin
		`ON_TIME(2) begin
			A		<= 'd1;
			B		<= 'd2;
		end
		`ON_TIME(3) begin
			A		<= 'd3;
			B		<= 'd4;
		end
		`ON_TIME(4) begin
			A		<= 'd5;
			B		<= 'd6;
		end
		`ON_TIME(5) begin
			A		<= 'd7;
			B		<= 'd8;
		end
		`ON_TIME(6) begin
			A		<= 'd9;
			B		<= 'd10;
		end
		`ON_TIME(7) begin
			BUSY	<= `FALSE;
		end
	end
end

endmodule
