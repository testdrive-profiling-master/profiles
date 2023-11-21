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
// Title : processor AXI wrapper
// Rev.  : 4/24/2023 Mon (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

module processor_clkgen_slave (
	// port list -----------------------------------------------------------------
	// clock in/out
	input				CLK, nRST,		// system clock & reset (active low)
	output				PCLK, nPRST,	// processing clock & reset (active low)

	// slave interface
	input				EN,				// enable
	input				WE,				// write enable
	input	[31:0]		WDATA,			// write data
	output	[31:0]		RDATA,			// read data

	// processor states
	input				BUSY			// processor is busy
);

// definition & assignment ---------------------------------------------------
// clkgen
reg						clkgen_EN, clkgen_WE, clkgen_RST;
reg		[6:0]			clkgen_ADDR;
reg		[15:0]			clkgen_WDATA;
wire	[15:0]			clkgen_RDATA;
wire					clkgen_READY, clkgen_LOCKED;

assign	RDATA			= {
	BUSY,
	13'd0,
	clkgen_READY,
	clkgen_LOCKED,
	clkgen_RDATA
};

reg		[3:0]			reset_pipe;
assign	nPRST			= reset_pipe[3];

// implementation ------------------------------------------------------------
`ifdef USE_TESTDRIVE
initial
	reset_pipe = 4'd0;
`endif

always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		clkgen_EN			<= 1'b0;
		clkgen_WE			<= 1'b0;
		clkgen_RST			<= 1'b1;
		clkgen_ADDR			<= 7'd0;
		clkgen_WDATA		<= 16'd0;
	end
	else begin
		if(EN & WE) begin
			clkgen_RST		<= WDATA[25];
			clkgen_EN		<= WDATA[24];
			clkgen_WE		<= WDATA[23];
			clkgen_ADDR		<= WDATA[22:16];
			clkgen_WDATA	<= WDATA[15:0];
		end
	end
end

always@(posedge PCLK, negedge nRST) begin
	if(!nRST) begin
		reset_pipe			<= 4'b0;
	end
	else begin
		reset_pipe			<= {reset_pipe[2:0], clkgen_LOCKED};
	end
end

processor_clkgen testdrive_clock_gen (
	.REF_CLK		(CLK),
	.CLK_OUT		(PCLK),
	.EN				(clkgen_EN),
	.WE				(clkgen_WE),
	.RST			(clkgen_RST),
	.ADDR			(clkgen_ADDR),
	.WDATA			(clkgen_WDATA),
	.RDATA			(clkgen_RDATA),
	.READY			(clkgen_READY),
	.LOCKED			(clkgen_LOCKED)
);

endmodule
