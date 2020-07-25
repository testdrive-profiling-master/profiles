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
// Title : Processor
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"
`include "library/SRAM_Dual_Distributed.v"
`include "library/FiFo.v"

/* HIDDEN */
module top (
	//// system ----------------------------
	input							MCLK,			// clock
	input							nRST,			// reset
	output	reg						BUSY,			// processor is busy
	output							INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------
reg	[31:0]			sim_counter;
assign	INTR		= `FALSE;

reg					we, re;
reg		[31:0]		wdata;
wire	[31:0]		rdata;
wire				empty, full;

`define ON_TIME(t)	if(sim_counter==t)

// implementation ------------------------------------------------------------
always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		sim_counter		<= 'd0;
		we				<= `FALSE;
		re				<= `FALSE;
		wdata			<= 0;
		BUSY			<= `TRUE;
	end
	else begin
		sim_counter		<= sim_counter + 1;
		wdata			<= wdata + 1;

		`ON_TIME(2) begin
			we			<= `TRUE;
		end

		`ON_TIME(7) begin
			we			<= `FALSE;
		end

		`ON_TIME(9) begin
			re			<= `TRUE;
		end

		`ON_TIME(15) begin
			re			<= `FALSE;
		end

		`ON_TIME(17) begin
			we			<= `TRUE;
		end

		`ON_TIME(18) begin
			we			<= `FALSE;
			re			<= `TRUE;
		end

		`ON_TIME(20) begin
			re			<= `FALSE;

			BUSY		<= `FALSE;
		end
	end
end

FiFo #(
	.FIFO_DEPTH		(2),
	.DATA_WIDTH		(32)
)FiFo_inst(
	// system signals
	.CLK			(MCLK),
	.nCLR			(nRST),
	// push interface
	.nWE			(~we),
	.DIN			(wdata),
	.FULL			(full),
	// pop interface
	.nRE			(~re),
	.DOUT			(rdata),
	.EMPTY			(empty)
);

endmodule
