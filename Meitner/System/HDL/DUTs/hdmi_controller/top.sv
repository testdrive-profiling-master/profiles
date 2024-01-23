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
`timescale 1ns/1ns
`define	M_AXI_USE_AXI4			1

`include "testdrive_system.vh"
`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"
`include "DUTs/hdmi_controller/includes.vh"

/* HIDDEN */
module top (
		input									MCLK,				// clock
		input									nRST,				// reset (active low)
		output	reg								BUSY,				// processor is busy
		output									INTR				// interrupt signal
	);

	localparam	C_S_AXI_DATA_WIDTH			= 32;
	localparam	C_S_AXI_ADDR_WIDTH			= 16;
	localparam	C_M_AXI_ID_WIDTH			= 2;
	localparam	C_M_AXI_ADDR_WIDTH			= 32;
	localparam	C_M_AXI_DATA_WIDTH			= 512;


	// definition & assignment ---------------------------------------------------
	wire	CLK	= MCLK;
	//// slave APB -------------------------
	wire									S_PSEL;
	wire									S_PENABLE;
	wire									S_PWRITE;
	wire	[15:0]							S_PADDR;
	wire	[31:0]							S_PWDATA;
	wire	[31:0]							S_PRDATA;
	wire									S_PREADY;
	wire									S_PSLVERR;

	//// AXI master interface --------------
	// write address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M_AWID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR;
	wire	[7:0]							M_AWLEN;
	wire	[2:0]							M_AWSIZE;
	wire	[1:0]							M_AWBURST;
	wire									M_AWLOCK;
	wire	[3:0]							M_AWCACHE;
	wire	[2:0]							M_AWPROT;
	wire	[3:0]							M_AWREGION;
	wire	[3:0]							M_AWQOS;
	wire									M_AWVALID;
	wire									M_AWREADY;

	// write data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M_WID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M_WDATA;
	wire	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M_WSTRB;
	wire									M_WLAST;
	wire									M_WVALID;
	wire									M_WREADY;
	// bus interaction
	wire	[C_M_AXI_ID_WIDTH-1:0]			M_BID;
	wire	[1:0]							M_BRESP;
	wire									M_BVALID;
	wire									M_BREADY;
	// read address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M_ARID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR;
	wire	[7:0]							M_ARLEN;
	wire	[2:0]							M_ARSIZE;
	wire	[1:0]							M_ARBURST;
	wire									M_ARLOCK;
	wire	[3:0]							M_ARCACHE;
	wire	[2:0]							M_ARPROT;
	wire	[3:0]							M_ARREGION;
	wire	[3:0]							M_ARQOS;
	wire									M_ARVALID;
	wire									M_ARREADY;
	// read data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M_RID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M_RDATA;
	wire	[1:0]							M_RRESP;
	wire									M_RLAST;
	wire									M_RVALID;
	wire									M_RREADY;


	// HDMI wiring
	wire					hdmi_clk, hdmi_de, hdmi_hsync, hdmi_vsync;
	wire	[23:0]			hdmi_data;

	// simulation life cycle
	reg						prev_vsync;
	reg		[7:0]			frame_count;
	wire	end_of_frame	= {prev_vsync, hdmi_vsync} == 2'b10;

	assign	INTR			= end_of_frame & (frame_count == 2);

	// implementation ------------------------------------------------------------
`ifdef USE_TESTDRIVE

	initial
		SetSystemDescription("HDMI controller");
`endif

	`ALWAYS_CLOCK_RESET begin
		`ON_RESET begin
			prev_vsync	<= `FALSE;
			BUSY		<= `TRUE;
			frame_count	<= 0;
		end
		else begin
			prev_vsync	<= hdmi_vsync;
			if(end_of_frame)
				frame_count <= frame_count + 1'b1;
			if(INTR)
				BUSY		<= `FALSE;
		end
	end

	// APB bus implementation
	testdrive_apb_slave_bfm #(
		.C_BUS_TITLE		("HDMI"),
		.C_BASE_ADDR		(64'h00010000),
		.C_ADDR_BITS		(C_S_AXI_ADDR_WIDTH)
	) apb_slave_bfm (
		// system
		.CLK				(MCLK),
		.nRST				(nRST),
		// Write
		.PSEL				(S_PSEL),
		.PENABLE			(S_PENABLE),
		.PWRITE				(S_PWRITE),
		.PADDR				(S_PADDR),
		.PWDATA				(S_PWDATA),
		.PSTRB				(),				// not using! all 32bit data operation
		.PRDATA				(S_PRDATA),
		.PREADY				(S_PREADY),
		.PSLVERR			(S_PSLVERR)
	);
	// AXI4 bus implementation
	testdrive_axi4_master_bfm #(
		.C_THREAD_ID_WIDTH			(C_M_AXI_ID_WIDTH),
		.C_ADDR_WIDTH				(C_M_AXI_ADDR_WIDTH),
		.C_DATA_WIDTH				(C_M_AXI_DATA_WIDTH)
	) axi4_master_bfm (
		// system
		.CLK				(MCLK),
		.nRST				(nRST),
		// mAXI
		.AWID				(M_AWID),
		.AWADDR				(M_AWADDR),
		.AWLEN				(M_AWLEN),
		.AWSIZE				(M_AWSIZE),
		.AWBURST			(M_AWBURST),
		.AWLOCK				(M_AWLOCK),
		.AWCACHE			(M_AWCACHE),
		.AWPROT				(M_AWPROT),
		.AWREGION			(M_AWREGION),
		.AWQOS				(M_AWQOS),
		.AWVALID			(M_AWVALID),
		.AWREADY			(M_AWREADY),
		.WID				(M_WID),
		.WDATA				(M_WDATA),
		.WSTRB				(M_WSTRB),
		.WLAST				(M_WLAST),
		.WVALID				(M_WVALID),
		.WREADY				(M_WREADY),
		.BID				(M_BID),
		.BRESP				(M_BRESP),
		.BVALID				(M_BVALID),
		.BREADY				(M_BREADY),
		.ARID				(M_ARID),
		.ARADDR				(M_ARADDR),
		.ARLEN				(M_ARLEN),
		.ARSIZE				(M_ARSIZE),
		.ARBURST			(M_ARBURST),
		.ARLOCK				(M_ARLOCK),
		.ARCACHE			(M_ARCACHE),
		.ARPROT				(M_ARPROT),
		.ARREGION			(M_ARREGION),
		.ARQOS				(M_ARQOS),
		.ARVALID			(M_ARVALID),
		.ARREADY			(M_ARREADY),
		.RID				(M_RID),
		.RDATA				(M_RDATA),
		.RRESP				(M_RRESP),
		.RLAST				(M_RLAST),
		.RVALID				(M_RVALID),
		.RREADY				(M_RREADY)
	);

	// HDMI controller implementation
	hdmi_controller #(
		.C_S_AXI_DATA_WIDTH			(C_S_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH			(C_S_AXI_ADDR_WIDTH),
		.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
		.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
		.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
	) hdmi_ctrl (
		//// AXI slave interface
		// system
		.ACLK				(MCLK),
		.nRST				(nRST),
		.INTR				(),
		//// slave APB -------------------------
		.S_PSEL				(S_PSEL),
		.S_PENABLE			(S_PENABLE),
		.S_PWRITE			(S_PWRITE),
		.S_PADDR			(S_PADDR),
		.S_PWDATA			(S_PWDATA),
		.S_PRDATA			(S_PRDATA),
		.S_PREADY			(S_PREADY),
		.S_PSLVERR			(S_PSLVERR),
		//// master AXI ------------------------
		.M_AWID				(M_AWID),
		.M_AWADDR			(M_AWADDR),
		.M_AWLEN			(M_AWLEN),
		.M_AWSIZE			(M_AWSIZE),
		.M_AWBURST			(M_AWBURST),
		.M_AWLOCK			(M_AWLOCK),
		.M_AWCACHE			(M_AWCACHE),
		.M_AWPROT			(M_AWPROT),
		.M_AWREGION			(M_AWREGION),
		.M_AWQOS			(M_AWQOS),
		.M_AWVALID			(M_AWVALID),
		.M_AWREADY			(M_AWREADY),
		.M_WID				(M_WID),
		.M_WDATA			(M_WDATA),
		.M_WSTRB			(M_WSTRB),
		.M_WLAST			(M_WLAST),
		.M_WVALID			(M_WVALID),
		.M_WREADY			(M_WREADY),
		.M_ARID				(M_ARID),
		.M_ARADDR			(M_ARADDR),
		.M_ARLEN			(M_ARLEN),
		.M_ARSIZE			(M_ARSIZE),
		.M_ARBURST			(M_ARBURST),
		.M_ARLOCK			(M_ARLOCK),
		.M_ARCACHE			(M_ARCACHE),
		.M_ARPROT			(M_ARPROT),
		.M_ARREGION			(M_ARREGION),
		.M_ARQOS			(M_ARQOS),
		.M_ARVALID			(M_ARVALID),
		.M_ARREADY			(M_ARREADY),
		.M_RID				(M_RID),
		.M_RDATA			(M_RDATA),
		.M_RRESP			(M_RRESP),
		.M_RLAST			(M_RLAST),
		.M_RVALID			(M_RVALID),
		.M_RREADY			(M_RREADY),
		.M_BID				(M_BID),
		.M_BRESP			(M_BRESP),
		.M_BVALID			(M_BVALID),
		.M_BREADY			(M_BREADY),
		//// Outputs
		// I2C interface
		.I2C_nRST			(),
		.I2C_SCL_I			(`TRUE),
		.I2C_SCL_O			(),
		.I2C_SCL_T			(),
		.I2C_SDA_I			(`TRUE),
		.I2C_SDA_O			(),
		.I2C_SDA_T			(),
		// HDMI SIGNALS
		.HDMI_INTR			(),
		.HDMI_CLK			(hdmi_clk),
		.HDMI_DE			(hdmi_de),
		.HDMI_HSYNC			(hdmi_hsync),
		.HDMI_VSYNC			(hdmi_vsync),
		.HDMI_DATA			(hdmi_data),
		.HDMI_SPDIF			()
	);

	// dummy connection
	import "DPI-C" function void hdmi_out (input bit de, input bit hsync, input bit vsync, input bit [31:0] data);
		`ALWAYS_CLOCK_RESET begin
			`ON_RESET begin
			end
			else begin
				hdmi_out(hdmi_de, hdmi_hsync, hdmi_vsync,
						 {
							 8'd0,
							 hdmi_data[7:0],
							 hdmi_data[15:8],
							 hdmi_data[23:16]
						 }
						);
			end
		end

	endmodule
