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
// Title : processor AXI wrapper
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

module processor_clkgen_slave (
	// port list -----------------------------------------------------------------
	// clock in/out
	input				CLK, nRST,		// system clock & reset (active low)
	output				PCLK, nPRST,	// processing clock & reset (active low)
	input				PINTR,			// processor interrupt (one hot - positive)

	// slave interface
	input				EN,				// enable
	input				WE,				// write enable
	input	[31:0]		WDATA,			// write data
	output	[31:0]		RDATA,			// read data

	// processor states
	input				BUSY,			// processor is busy
	output	reg			INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------
// clkgen
reg						clkgen_EN, clkgen_WE, clkgen_RST;
reg		[6:0]			clkgen_ADDR;
reg		[15:0]			clkgen_WDATA;
wire	[15:0]			clkgen_RDATA;
wire					clkgen_READY, clkgen_LOCKED;

// interrupt
reg						intr_s_toggle, intr_p_toggle;
reg		[9:0]			intr_counter;	// interrupt counter

assign	RDATA			= {
	BUSY,
	INTR,
	2'd0,
	intr_counter,
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
		intr_s_toggle		<= 1'b0;
		intr_counter		<= 'd0;
		INTR				<= 1'b0;
	end
	else begin
		if(EN & WE) begin
			clkgen_RST		<= WDATA[25];
			clkgen_EN		<= WDATA[24];
			clkgen_WE		<= WDATA[23];
			clkgen_ADDR		<= WDATA[22:16];
			clkgen_WDATA	<= WDATA[15:0];
		end

		if(EN & (~WE)) begin
			intr_counter	<= 'd0;
			INTR			<= 1'b0;
		end
		else begin
			if(intr_s_toggle ^ intr_p_toggle) begin
				intr_s_toggle		<= ~intr_s_toggle;
				intr_counter		<= intr_counter + 1'b1;
				INTR				<= 1'b1;
			end
		end
	end
end

always@(posedge PCLK, negedge nRST) begin
	if(!nRST) begin
		reset_pipe			<= 4'b0;
		intr_p_toggle		<= 1'b0;
	end
	else begin
		reset_pipe			<= {reset_pipe[2:0], clkgen_LOCKED};
		if(PINTR)
			intr_p_toggle		<= ~intr_p_toggle;
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
