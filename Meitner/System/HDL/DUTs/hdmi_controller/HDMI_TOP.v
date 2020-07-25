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

module HDMI_TOP(
	//// system
	input						ACLK,			// clock
	input						nRST,			// reset (active low)
	//// Simplified slave interface
	input						S_EN,			// enable
	input						S_WE,			// write enable
	input	[3:0]				S_ADDR,			// address
	input	[31:0]				S_WDATA,		// write data
	output	[31:0]				S_RDATA,		// read data
	//// Simplified master interface
	output						MR_REQ,			// read request
	input						MR_GRANT,		// read grant
	output	[31:0]				MR_ADDR,		// read address
	output	[7:0]				MR_SIZE,		// read size
	input	[512-1:0]			MR_DATA,		// read data
	input						MR_VALID,		// read validation
	output						MR_READY,		// read ready
	input						MR_LAST,		// last read

	//// Outputs
	// I2C interface
	output						I2C_nRST,		// i2c reset (active low)
	input						I2C_SCL_I,		// i2c SCL input
	output						I2C_SCL_T,		// i2c SCL tri-state enable
	input						I2C_SDA_I,		// i2c SDA input
	output						I2C_SDA_T,		// i2c SDA tri-state enable
	// HDMI SIGNALS
	output						HDMI_CLK,		// hdmi clock
	output						HDMI_DE,		// data enable
	output						HDMI_HSYNC,		// horizontal sync.
	output						HDMI_VSYNC,		// vertical sync.
	output	[35:0]				HDMI_DATA		// data output
);

// definition & assignment ---------------------------------------------------
wire					hdmi_en_n, hdmi_vclk;
wire					sync_re, sync_ft;
wire					cfg_we;
wire	[11:0]			cfg_wdata;

wire	[23:0]			linebuffer_rgb;

// video info.
wire	[31:0]			video_BASE;		// base address
wire	[15:0]			video_SIZE;		// video size
// pixel operation
wire					pixel_nREQ;
wire					pixel_nVALID;
wire					pixel_nREADY;
wire	[384-1:0]		pixel_DATA;

// implementation ------------------------------------------------------------
// Slave bus
HDMI_Slave	slave_inst(
	// AXI slave interface
	.CLK				(ACLK),
	.nRST				(nRST),
	//// simplified slave interface
	.S_EN				(S_EN),
	.S_WE				(S_WE),
	.S_ADDR				(S_ADDR),
	.S_WDATA			(S_WDATA),
	.S_RDATA			(S_RDATA),
	//// I2C interface
	.I2C_nRST			(I2C_nRST),
	.I2C_SCL_I			(I2C_SCL_I),
	.I2C_SCL_T			(I2C_SCL_T),
	.I2C_SDA_I			(I2C_SDA_I),
	.I2C_SDA_T			(I2C_SDA_T),
	//// HDMI interface
	// control
	.HDMI_nEN			(hdmi_en_n),
	.HDMI_CLK			(hdmi_vclk),
	// configuration
	.CFG_WE				(cfg_we),
	.CFG_DATA			(cfg_wdata),
	// video info.
	.VIDEO_BASE			(video_BASE),
	.VIDEO_SIZE			(video_SIZE)
);

//
HDMI_Master master_inst(
	.ACLK				(ACLK),
	.nRST				(nRST),
	//// Simplified master interface
	.MR_REQ				(MR_REQ),
	.MR_GRANT			(MR_GRANT),
	.MR_ADDR			(MR_ADDR),
	.MR_SIZE			(MR_SIZE),
	.MR_DATA			(MR_DATA),
	.MR_VALID			(MR_VALID),
	.MR_READY			(MR_READY),
	.MR_LAST			(MR_LAST),
	// video configuration
	.VIDEO_BASE			(video_BASE),
	.VIDEO_SIZE			(video_SIZE),
	// pixel operation
	.PIXEL_nREQ			(pixel_nREQ),
	.PIXEL_nVALID		(pixel_nVALID),
	.PIXEL_nREADY		(pixel_nREADY),
	.PIXEL_DATA			(pixel_DATA)
);

HDMI_LineBuffer #(
	.DEPTH				(8)
) line_buffer (
	.nRST				(~hdmi_en_n),
	// write
	.W_CLK				(ACLK),
	.W_nREQ				(pixel_nREQ),
	.W_nVALID			(pixel_nVALID),
	.W_nREADY			(pixel_nREADY),
	.W_DATA				(pixel_DATA),
	// read
	.R_CLK				(hdmi_vclk),
	.R_RE				(sync_re),
	.R_FT				(sync_ft),
	.R_DATA				(linebuffer_rgb)
);

// HDMI sync&output
HDMI_SyncGen	sync_gen(
	// clock & reset
	.ACLK				(ACLK),
	.nRST				(nRST),
	.VCLK				(hdmi_vclk),
	.RST				(hdmi_en_n),
	// configuration
	.CFG_WE				(cfg_we),
	.CFG_DATA			(cfg_wdata),
	// data sync.
	.SYNC_RE			(sync_re),
	.SYNC_FT			(sync_ft),
	.RGB				({linebuffer_rgb[7:0], 4'd0, linebuffer_rgb[15:8], 4'd0, linebuffer_rgb[23:16], 4'd0}),
	// output pins.
	.HDMI_CLK			(HDMI_CLK),
	.HDMI_DE			(HDMI_DE),
	.HDMI_HSYNC			(HDMI_HSYNC),
	.HDMI_VSYNC			(HDMI_VSYNC),
	.HDMI_DATA			(HDMI_DATA)
);

endmodule
