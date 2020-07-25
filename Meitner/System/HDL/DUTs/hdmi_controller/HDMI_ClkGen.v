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
`timescale 1 ns / 100 ps
module HDMI_ClkGen #(
	parameter MMCM_TYPE   = 'd0
) (
	// port list -----------------------------------------------------------------
	// clock in/out
	input				REF_CLK,
	output				CLK_OUT,

	// control signals
	input				EN, WE, RST,
	input	[6:0]		ADDR,
	input	[15:0]		WDATA,
	output	[15:0]		RDATA,
	output				READY, LOCKED
);

// definition & assignment ---------------------------------------------------
wire	fb_clk;			// feedback clock
wire	fb_clk_buf;		// feedback clock global buffer
wire	clk_out_0;		// clock out #0

// implementation ------------------------------------------------------------
`ifdef USE_TESTDRIVE
assign	CLK_OUT	= REF_CLK;
assign	READY	= 1'b1;
assign	LOCKED	= 1'b1;
`else
generate
if (MMCM_TYPE == 1) begin
	// Xilinx MMCM clock instance "MMCM_ADV"
	MMCM_ADV #(
	    .BANDWIDTH				("OPTIMIZED"),
	    .CLKOUT4_CASCADE		("FALSE"),
	    .CLOCK_HOLD				("FALSE"),
	    .COMPENSATION			("ZHOLD"),
	    .STARTUP_WAIT			("FALSE"),
	    .DIVCLK_DIVIDE			(1),
	    .CLKFBOUT_MULT_F		(7),
	    .CLKFBOUT_PHASE			(0),
	    .CLKFBOUT_USE_FINE_PS	("FALSE"),
	    .CLKOUT0_DIVIDE_F		(7),
	    .CLKOUT0_PHASE			(0),
	    .CLKOUT0_DUTY_CYCLE		(0.5),
	    .CLKOUT0_USE_FINE_PS	("FALSE"),
	    .CLKIN1_PERIOD			(5),
	    .REF_JITTER1			(0.01))
	mmcm_inst (
	    .CLKFBOUT				(fb_clk),
	    .CLKFBIN				(fb_clk_buf),
	    .CLKFBOUTB				(),
	    .CLKOUT0				(clk_out_0),
	    .CLKOUT0B				(),
	    .CLKOUT1				(),
	    .CLKOUT1B				(),
	    .CLKOUT2				(),
	    .CLKOUT2B				(),
	    .CLKOUT3				(),
	    .CLKOUT3B				(),
	    .CLKOUT4				(),
	    .CLKOUT5				(),
	    .CLKOUT6				(),
	    .CLKIN1					(REF_CLK),
	    .CLKIN2					(1'b0),
	    .CLKINSEL				(1'b1),
	    .DCLK					(REF_CLK),
	    .DADDR					(ADDR),
	    .DEN					(EN),
	    .DI						(WDATA),
	    .DO						(RDATA),
	    .DRDY					(READY),
	    .DWE					(WE),
	    .PSCLK					(1'b0),
	    .PSEN					(1'b0),
	    .PSINCDEC				(1'b0),
	    .PSDONE					(),
	    .LOCKED					(LOCKED),
	    .CLKINSTOPPED			(),
	    .CLKFBSTOPPED			(),
	    .PWRDWN					(1'b0),
	    .RST					(RST));
end
else begin
	// Xilinx MMCM clock instance "MMCME2_ADV"
	MMCME2_ADV #(
	    .BANDWIDTH				("OPTIMIZED"),
	    .CLKOUT4_CASCADE		("FALSE"),
	    .COMPENSATION			("ZHOLD"),
	    .STARTUP_WAIT			("FALSE"),
	    .DIVCLK_DIVIDE			(1),
	    .CLKFBOUT_MULT_F		(7),
	    .CLKFBOUT_PHASE			(0),
	    .CLKFBOUT_USE_FINE_PS	("FALSE"),
	    .CLKOUT0_DIVIDE_F		(7),
	    .CLKOUT0_PHASE			(0),
	    .CLKOUT0_DUTY_CYCLE		(0.5),
	    .CLKOUT0_USE_FINE_PS	("FALSE"),
	    .CLKIN1_PERIOD			(5),
	    .REF_JITTER1			(0.01))
	mmcm_inst (
	    .CLKFBOUT				(fb_clk),
	    .CLKFBIN				(fb_clk_buf),
	    .CLKFBOUTB				(),
	    .CLKOUT0				(clk_out_0),
	    .CLKOUT0B				(),
	    .CLKOUT1				(),
	    .CLKOUT1B				(),
	    .CLKOUT2				(),
	    .CLKOUT2B				(),
	    .CLKOUT3				(),
	    .CLKOUT3B				(),
	    .CLKOUT4				(),
	    .CLKOUT5				(),
	    .CLKOUT6				(),
	    .CLKIN1					(REF_CLK),
	    .CLKIN2					(1'b0),
	    .CLKINSEL				(1'b1),
	    .DCLK					(REF_CLK),
	    .DADDR					(ADDR),
	    .DEN					(EN),
	    .DI						(WDATA),
	    .DO						(RDATA),
	    .DRDY					(READY),
	    .DWE					(WE),
	    .PSCLK					(1'b0),
	    .PSEN					(1'b0),
	    .PSINCDEC				(1'b0),
	    .PSDONE					(),
	    .LOCKED					(LOCKED),
	    .CLKINSTOPPED			(),
	    .CLKFBSTOPPED			(),
	    .PWRDWN					(1'b0),
	    .RST					(RST));
end
endgenerate

BUFG fb_bufg_inst (
    .I (fb_clk),
    .O (fb_clk_buf));

BUFG bufg_inst (
    .I (clk_out_0),
    .O (CLK_OUT));
`endif

endmodule
