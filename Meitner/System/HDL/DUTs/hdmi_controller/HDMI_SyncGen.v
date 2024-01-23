//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : HDMI controller
// Rev.  : 1/23/2024 Tue (clonextop@gmail.com)
//================================================================================
`timescale 1ns / 1ps
//----------------------------------------------------------------------------
// Product   : DVI Sync Generator
// File Name : DVI_SyncGen.v
// Author    : HyungKi Jeong (ks2ii@daum.net)
// Org./Date : SeoKyeong Univ. ESL / 2013.8.12
// Rev.Count : 0001
// Etc.Desc. : 1920x1080
//----------------------------------------------------------------------------
// 1920x1080 sync timing @ 200MHz
/*
`define	SCREEN_WIDTH	1920
`define	HSYNC_GAP		768
`define	HSYNC_FRONT		96
`define	HSYNC_BACK		96
`define	HSYNC_WIDTH		(`HSYNC_GAP+`HSYNC_BACK+`SCREEN_WIDTH+`HSYNC_FRONT)
`define	SCREEN_HEIGHT	1080
`define	VSYNC_GAP		17
`define	VSYNC_FRONT		30
`define	VSYNC_BACK		30
`define	VSYNC_WIDTH		(`VSYNC_GAP+`VSYNC_BACK+`SCREEN_HEIGHT+`VSYNC_FRONT)
`define BUFFER_DELAY	1
*/

// 1920x1080 sync timing @ 150MHz
`define	SCREEN_WIDTH	1920
`define	HSYNC_GAP		48
`define	HSYNC_FRONT		120
`define	HSYNC_BACK		128
`define	HSYNC_WIDTH		(`HSYNC_GAP+`HSYNC_BACK+`SCREEN_WIDTH+`HSYNC_FRONT)
`define	SCREEN_HEIGHT	1080
`define	VSYNC_GAP		16
`define	VSYNC_FRONT		8
`define	VSYNC_BACK		23
`define	VSYNC_WIDTH		(`VSYNC_GAP+`VSYNC_BACK+`SCREEN_HEIGHT+`VSYNC_FRONT)
`define BUFFER_DELAY	1

module HDMI_SyncGen(
		// port list -----------------------------------------------------------------
		// input
		input				ACLK, nRST,						// system clock & reset (active low)
		input				VCLK, RST,						// VGA Clock

		// configuration
		input				CFG_WE,
		input	[11:0]		CFG_DATA,

		// Sync. output
		output	reg			SYNC_RE, SYNC_FT,				// read enable, Frame toggle
		input	[23:0]		RGB,

		// HDMI output
		output				HDMI_CLK,
		output				HDMI_DE, HDMI_HSYNC, HDMI_VSYNC,
		output	[23:0]		HDMI_DATA
	);

	// definition & assignment ---------------------------------------------------
	reg		[11:0]		count_h;						// horizontal count
	reg		[11:0]		count_v;						// vertical count
	reg					hsync, vsync, de;				// output signal
	reg		[3:0]		de_probe;

	reg		[11:0]		cfg_v_sync, cfg_v_gap, cfg_v_start, cfg_v_end;
	reg		[11:0]		cfg_h_sync, cfg_h_gap, cfg_h_start, cfg_h_end;

	// Sync. Counter
	wire				hsync_end		= (count_h == cfg_h_sync);
	wire				vsync_end		= (count_v == cfg_v_sync);

	wire				line_end		= (~SYNC_RE) & de;

	// implementation ------------------------------------------------------------
	// HDMI output
	HDMI_Out hdmi_output(
		// control
		.CLK		(VCLK),
		.RST		(RST),
		.HSYNC		(hsync),
		.VSYNC		(vsync),
		.DE			(de),
		.RGB		(RGB),
		// output
		.HDMI_CLK	(HDMI_CLK),
		.HDMI_DE	(HDMI_DE),
		.HDMI_HSYNC	(HDMI_HSYNC),
		.HDMI_VSYNC	(HDMI_VSYNC),
		.HDMI_DATA	(HDMI_DATA)
	);	/* verilator lint_off WIDTH */

	// HDMI configuration
	always @(posedge ACLK) begin
		if(!nRST) begin
			cfg_h_sync	<= (`HSYNC_WIDTH-1'd1);
			cfg_v_sync	<= (`VSYNC_WIDTH-1'd1);
			cfg_h_gap	<= `HSYNC_GAP;
			cfg_v_gap	<= `VSYNC_GAP;
			cfg_h_start	<= (`HSYNC_GAP+`HSYNC_BACK-2'd2);
			cfg_v_start	<= (`VSYNC_GAP+`VSYNC_BACK);
			cfg_h_end	<= (`HSYNC_GAP+`HSYNC_BACK+`SCREEN_WIDTH-2'd2);
			cfg_v_end	<= (`VSYNC_GAP+`VSYNC_BACK+`SCREEN_HEIGHT);
		end
		else begin
			if(CFG_WE) begin
				cfg_h_sync	<= CFG_DATA;
				cfg_v_sync	<= cfg_h_sync;
				cfg_h_gap	<= cfg_v_sync;
				cfg_v_gap	<= cfg_h_gap;
				cfg_h_start	<= cfg_v_gap;
				cfg_v_start	<= cfg_h_start;
				cfg_h_end	<= cfg_v_start;
				cfg_v_end	<= cfg_h_end;
			end
		end
	end

	// Sync. Counter
	always @(posedge VCLK)
		if (RST) begin
			count_h		<= 'd0;
			count_v		<= 'd0;
		end
		else begin
			count_h		<= hsync_end ? 'd0 : (count_h + 1'b1);
			count_v		<= hsync_end ? count_v + 1'b1 : vsync_end ? 'd0 : count_v;
		end

	// Sync. Generator
	always @(posedge VCLK)
		if (RST) begin
			hsync		<= 1'b0;
			vsync		<= 1'b0;
			de			<= 1'b0;

			de_probe	<= 4'b0;
			SYNC_RE		<= 1'b0;
			SYNC_FT		<= 1'b0;
		end
		else begin
			hsync		<= (count_h >= cfg_h_gap);
			vsync		<= (count_v >= cfg_v_gap);
			de			<= SYNC_RE;

			de_probe[0]	<= count_h >= cfg_h_start;
			de_probe[1]	<= count_h <  cfg_h_end;
			de_probe[2]	<= count_v >= cfg_v_start;
			de_probe[3]	<= count_v <  cfg_v_end;
			SYNC_RE		<= &de_probe;
			SYNC_FT		<= ((count_v+1'b1) == cfg_v_end);
		end

endmodule
