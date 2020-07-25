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
`ifndef __TESTDRIVE_CONTROL_SLICE_V__
`define __TESTDRIVE_CONTROL_SLICE_V__
`timescale 1ns/1ns

module ControlSlice #(
	parameter		WIDTH			= 4,				// control width
	parameter		INITIAL			= {WIDTH{1'b0}},	// initial value
	parameter		CHAINED			= 1					// chained control?
) (
	input							CLK,				// clock
	input							nRST,				// reset (active low)
	// control input
	input							IE,					// input enable
	input	[WIDTH-1:0]				IDATA,				// input data
	output							IREADY,				// input ready
	// control output
	output							OE,					// output enable
	output	reg [WIDTH-1:0]			ODATA,				// otuput data
	input							OREADY				// output ready
);
// synopsys template

// register definition & assignment ------------------------------------------
reg							occupied;		// occupied control

assign	OE					= occupied;

// implementation ------------------------------------------------------------
generate if(CHAINED) begin : chained_control
	assign	IREADY				= (~occupied) | OREADY;
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			occupied		<= 1'b0;
		end
		else begin
			if(occupied) begin
				if((~IE) & OREADY) begin
					occupied	<= 1'b0;
				end
			end
			else begin
				if(IE) begin
					occupied	<= 1'b1;
				end
			end
		end
	end
end
else begin : unchained_control
	assign	IREADY				= ~occupied;
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			occupied		<= 1'b0;
		end
		else begin
			if(occupied) begin
				if(OREADY) begin
					occupied	<= 1'b0;
				end
			end
			else begin
				if(IE) begin
					occupied	<= 1'b1;
				end
			end
		end
	end
end
endgenerate

always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		ODATA			<= INITIAL;
	end
	else begin
		if(IE & IREADY) begin
			ODATA			<= IDATA;
		end
	end
end

endmodule

`endif//__TESTDRIVE_CONTROL_SLICE_V__
