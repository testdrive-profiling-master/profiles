//================================================================================
// Copyright (c) 2013 ~ 2018. HyungKi Jeong(clonextop@gmail.com)
// 
// MIT License
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Title : Simulation HDL module
// Rev.  : 12/21/2018 Fri (clonextop@gmail.com)
//================================================================================
`include "includes.vh"

module top(
	input					CLK,			// clock
	input					nRST,			// reset
	// input
	input					EN,				// input enable
	input	[31:0]			DIN,			// data in
	// output
	output	reg				OE,				// output enable
	output	reg [31:0]		DOUT,			// data out
	output	reg [31:0]		DOUT_GOLDEN		// golden data out
);

import "DPI-C" function bit [31:0] dpi_sine (input bit [31:0] DIN);		// golden function

always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		OE				<= 1'b0;	// FALSE
		DOUT			<= 'd0;
		DOUT_GOLDEN		<= 'd0;
	end else begin
		OE				<= EN;
		DOUT			<= DIN;						// customized ALU output
		if(EN) begin
			DOUT_GOLDEN		<= dpi_sine(DIN);		// golden output
		end
	end
end

endmodule
