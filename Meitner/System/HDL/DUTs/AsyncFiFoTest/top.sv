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
// Title : Processor
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "library/SyncPipe.v"
`include "library/SRAM_Dual_Distributed.v"
`include "library/FiFo_Async.v"
`include "template/testdrive_clock_bfm.sv"

/* HIDDEN */
module top (
	//// system ----------------------------
	input							MCLK,			// clock
	input							nRST,			// reset
	output	reg						BUSY,			// processor is busy
	output							INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------
`define	END_OF_TIME		10000
reg	[31:0]			sim_cnt_w;
reg	[31:0]			sim_cnt_r;
assign	INTR		= `FALSE;

reg					we, re;
reg		[31:0]		wdata;
wire	[31:0]		rdata;
wire				empty, full;

`define ON_TIME_W(t)	if(sim_cnt_w==t)
`define ON_TIME_R(t)	if(sim_cnt_r==t)

wire				r_clk, w_clk;

// implementation ------------------------------------------------------------
// write clock
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(30000)
) w_clk_gen (
	.CLK	(w_clk),
	.RST	()
);

// read clock
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(20000)
) r_clk_gen (
	.CLK	(r_clk),
	.RST	()
);

FiFo_Async #(
	.FIFO_DEPTH		(2),
	.DATA_WIDTH		(32)
)FiFo_inst(
	// push interface
	.W_CLK			(w_clk),
	.W_nRST			(nRST),
	.W_nEN			(~we),
	.W_DATA			(wdata),
	.W_FULL			(full),
	// pop interface
	.R_CLK			(r_clk),
	.R_nRST			(nRST),
	.R_nEN			(~re),
	.R_DATA			(rdata),
	.R_EMPTY		(empty)
);

always@(posedge w_clk, negedge nRST) begin
	if(!nRST) begin
		sim_cnt_w		<= 'd0;
		we				<= `FALSE;
		wdata			<= 0;
		BUSY			<= `TRUE;
	end
	else begin
		sim_cnt_w		<= sim_cnt_w + 1;

		wdata			<= wdata + 1;

		`ON_TIME_W(2) begin
			we			<= `TRUE;
		end

		`ON_TIME_W(7) begin
			we			<= `FALSE;
		end

		`ON_TIME_W(17) begin
			we			<= `TRUE;
		end

		`ON_TIME_W(18) begin
			we			<= `FALSE;
		end

		`ON_TIME_W(100) begin
			BUSY		<= `FALSE;
		end
	end
end

always@(posedge r_clk, negedge nRST) begin
	if(!nRST) begin
		sim_cnt_r		<= 'd0;
		re				<= `FALSE;
	end
	else begin
		sim_cnt_r		<= sim_cnt_r + 1;

		`ON_TIME_R(9) begin
			re			<= `TRUE;
		end

		`ON_TIME_R(15) begin
			re			<= `FALSE;
		end


		`ON_TIME_R(27) begin
			re			<= `TRUE;
		end

		`ON_TIME_R(29) begin
			re			<= `FALSE;
		end
	end
end

endmodule
