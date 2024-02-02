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
// Title : processor AXI wrapper
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_lite_master_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"
`include "DUTs/processor_axi_wrapper/includes.vh"
`include "DUTs/processor_axi_wrapper/hdmi_controller.sv"

// AXI slave definitions
`ifndef SAXI_BASE_ADDR
`define SAXI_BASE_ADDR					64'h00020000
`endif
`ifndef SAXI_ADDR_BITS
`define SAXI_ADDR_BITS					16
`endif
// AXI master definitions
`ifndef MAXI_ID_WIDTH
`define MAXI_ID_WIDTH					2
`endif
`ifndef MAXI_DATA_WIDTH
`define	MAXI_DATA_WIDTH					512
`endif

/* HIDDEN */
module top (
		input								MCLK,				// clock
		input								nRST,				// reset (active low)
		output								BUSY,				// processor is busy
		output								INTR				// interrupt signal
	);

	localparam	C_M_AXILITE_ADDR_WIDTH		= 20;
	localparam	C_M_AXI_ID_WIDTH			= `MAXI_ID_WIDTH;
	localparam	C_M_AXI_ADDR_WIDTH			= 36;
	localparam	C_M_AXI_DATA_WIDTH			= `MAXI_DATA_WIDTH;

	// definition & assignment ---------------------------------------------------
	initial
		SetSystemDescription("Processor axi wrapper");

	assign	BUSY	= processor_axi_wrapper.busy;

	//// slave APB -------------------------
	wire									S_PSEL;
	wire									S_PENABLE;
	wire									S_PWRITE;
	wire	[(`SAXI_ADDR_BITS-1):0]			S_PADDR;
	wire	[31:0]							S_PWDATA;
	wire	[31:0]							S_PRDATA;
	wire									S_PREADY;
	wire									S_PSLVERR;

	//// AXI-LITE master  ------------------
	// write address
	wire	[C_M_AXILITE_ADDR_WIDTH-1:0]	EX_AWADDR;
	wire									EX_AWVALID;
	wire									EX_AWREADY;
	// write data
	wire	[31:0]							EX_WDATA;
	wire	[3:0]							EX_WSTRB;
	wire									EX_WVALID;
	wire									EX_WREADY;
	// bus
	wire	[1:0]							EX_BRESP;
	wire									EX_BVALID;
	wire									EX_BREADY;
	// read address
	wire	[C_M_AXILITE_ADDR_WIDTH-1:0]	EX_ARADDR;
	wire									EX_ARVALID;
	wire									EX_ARREADY;
	// read data
	wire	[31:0]							EX_RDATA;
	wire	[1:0]							EX_RRESP;
	wire									EX_RVALID;
	wire									EX_RREADY;

	//// AXI master #0 interface -----------
	// write address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M0_AWID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M0_AWADDR;
	wire	[7:0]							M0_AWLEN;
	wire	[2:0]							M0_AWSIZE;
	wire	[1:0]							M0_AWBURST;
	wire									M0_AWLOCK;
	wire	[3:0]							M0_AWCACHE;
	wire	[2:0]							M0_AWPROT;
	wire	[3:0]							M0_AWREGION;
	wire	[3:0]							M0_AWQOS;
	wire									M0_AWVALID;
	wire									M0_AWREADY;
	// write data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M0_WID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M0_WDATA;
	wire	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M0_WSTRB;
	wire									M0_WLAST;
	wire									M0_WVALID;
	wire									M0_WREADY;
	// bus interaction
	wire	[C_M_AXI_ID_WIDTH-1:0]			M0_BID;
	wire	[1:0]							M0_BRESP;
	wire									M0_BVALID;
	wire									M0_BREADY;
	// read address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M0_ARID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M0_ARADDR;
	wire	[7:0]							M0_ARLEN;
	wire	[2:0]							M0_ARSIZE;
	wire	[1:0]							M0_ARBURST;
	wire									M0_ARLOCK;
	wire	[3:0]							M0_ARCACHE;
	wire	[2:0]							M0_ARPROT;
	wire	[3:0]							M0_ARREGION;
	wire	[3:0]							M0_ARQOS;
	wire									M0_ARVALID;
	wire									M0_ARREADY;
	// read data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M0_RID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M0_RDATA;
	wire	[1:0]							M0_RRESP;
	wire									M0_RLAST;
	wire									M0_RVALID;
	wire									M0_RREADY;

	//// AXI master #1 interface -----------
	// write address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M1_AWID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M1_AWADDR;
	wire	[7:0]							M1_AWLEN;
	wire	[2:0]							M1_AWSIZE;
	wire	[1:0]							M1_AWBURST;
	wire									M1_AWLOCK;
	wire	[3:0]							M1_AWCACHE;
	wire	[2:0]							M1_AWPROT;
	wire	[3:0]							M1_AWREGION;
	wire	[3:0]							M1_AWQOS;
	wire									M1_AWVALID;
	wire									M1_AWREADY;
	// write data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M1_WID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M1_WDATA;
	wire	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M1_WSTRB;
	wire									M1_WLAST;
	wire									M1_WVALID;
	wire									M1_WREADY;
	// bus interaction
	wire	[C_M_AXI_ID_WIDTH-1:0]			M1_BID;
	wire	[1:0]							M1_BRESP;
	wire									M1_BVALID;
	wire									M1_BREADY;
	// read address
	wire	[C_M_AXI_ID_WIDTH-1:0]			M1_ARID;
	wire	[C_M_AXI_ADDR_WIDTH-1:0]		M1_ARADDR;
	wire	[7:0]							M1_ARLEN;
	wire	[2:0]							M1_ARSIZE;
	wire	[1:0]							M1_ARBURST;
	wire									M1_ARLOCK;
	wire	[3:0]							M1_ARCACHE;
	wire	[2:0]							M1_ARPROT;
	wire	[3:0]							M1_ARREGION;
	wire	[3:0]							M1_ARQOS;
	wire									M1_ARVALID;
	wire									M1_ARREADY;
	// read data
	wire	[C_M_AXI_ID_WIDTH-1:0]			M1_RID;
	wire	[C_M_AXI_DATA_WIDTH-1:0]		M1_RDATA;
	wire	[1:0]							M1_RRESP;
	wire									M1_RLAST;
	wire									M1_RVALID;
	wire									M1_RREADY;

	// implementation ------------------------------------------------------------
	// APB bus implementation
	testdrive_apb_slave_bfm #(
		.C_BUS_TITLE		("APB"),
		.C_BASE_ADDR		(`SAXI_BASE_ADDR),
		.C_ADDR_BITS		(`SAXI_ADDR_BITS)
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

	testdrive_axi4_lite_master_bfm #(
		.C_BUS_TITLE		("AXI-LITE"),
		.C_ADDR_WIDTH		(C_M_AXILITE_ADDR_WIDTH)
	) axi4_lite_master_bfm (
		.CLK				(MCLK),
		.nRST				(nRST),
		.AWADDR				(EX_AWADDR),
		.AWVALID			(EX_AWVALID),
		.AWREADY			(EX_AWREADY),
		.WDATA				(EX_WDATA),
		.WSTRB				(EX_WSTRB),
		.WVALID				(EX_WVALID),
		.WREADY				(EX_WREADY),
		.BRESP				(EX_BRESP),
		.BVALID				(EX_BVALID),
		.BREADY				(EX_BREADY),
		.ARADDR				(EX_ARADDR),
		.ARVALID			(EX_ARVALID),
		.ARREADY			(EX_ARREADY),
		.RDATA				(EX_RDATA),
		.RRESP				(EX_RRESP),
		.RVALID				(EX_RVALID),
		.RREADY				(EX_RREADY)
	);

	// AXI4 #0 bus implementation
	testdrive_axi4_master_bfm #(
		.C_BUS_TITLE		("AXIM#0"),
		.C_THREAD_ID_WIDTH	(C_M_AXI_ID_WIDTH),
		.C_ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
		.C_DATA_WIDTH		(C_M_AXI_DATA_WIDTH)
	) axi4_master_0_bfm (
		// system
		.CLK				(MCLK),
		.nRST				(nRST),
		// mAXI
		.AWID				(M0_AWID),
		.AWADDR				(M0_AWADDR),
		.AWLEN				(M0_AWLEN),
		.AWSIZE				(M0_AWSIZE),
		.AWBURST			(M0_AWBURST),
		.AWLOCK				(M0_AWLOCK),
		.AWCACHE			(M0_AWCACHE),
		.AWPROT				(M0_AWPROT),
		.AWREGION			(M0_AWREGION),
		.AWQOS				(M0_AWQOS),
		.AWVALID			(M0_AWVALID),
		.AWREADY			(M0_AWREADY),
		.WID				(M0_WID),
		.WDATA				(M0_WDATA),
		.WSTRB				(M0_WSTRB),
		.WLAST				(M0_WLAST),
		.WVALID				(M0_WVALID),
		.WREADY				(M0_WREADY),
		.BID				(M0_BID),
		.BRESP				(M0_BRESP),
		.BVALID				(M0_BVALID),
		.BREADY				(M0_BREADY),
		.ARID				(M0_ARID),
		.ARADDR				(M0_ARADDR),
		.ARLEN				(M0_ARLEN),
		.ARSIZE				(M0_ARSIZE),
		.ARBURST			(M0_ARBURST),
		.ARLOCK				(M0_ARLOCK),
		.ARCACHE			(M0_ARCACHE),
		.ARPROT				(M0_ARPROT),
		.ARREGION			(M0_ARREGION),
		.ARQOS				(M0_ARQOS),
		.ARVALID			(M0_ARVALID),
		.ARREADY			(M0_ARREADY),
		.RID				(M0_RID),
		.RDATA				(M0_RDATA),
		.RRESP				(M0_RRESP),
		.RLAST				(M0_RLAST),
		.RVALID				(M0_RVALID),
		.RREADY				(M0_RREADY)
	);

	// AXI4 #1 bus implementation
	testdrive_axi4_master_bfm #(
		.C_BUS_TITLE		("AXIM#1"),
		.C_THREAD_ID_WIDTH	(C_M_AXI_ID_WIDTH),
		.C_ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
		.C_DATA_WIDTH		(C_M_AXI_DATA_WIDTH)
	) axi4_master_1_bfm (
		// system
		.CLK				(MCLK),
		.nRST				(nRST),
		// mAXI
		.AWID				(M1_AWID),
		.AWADDR				(M1_AWADDR),
		.AWLEN				(M1_AWLEN),
		.AWSIZE				(M1_AWSIZE),
		.AWBURST			(M1_AWBURST),
		.AWLOCK				(M1_AWLOCK),
		.AWCACHE			(M1_AWCACHE),
		.AWPROT				(M1_AWPROT),
		.AWREGION			(M1_AWREGION),
		.AWQOS				(M1_AWQOS),
		.AWVALID			(M1_AWVALID),
		.AWREADY			(M1_AWREADY),
		.WID				(M1_WID),
		.WDATA				(M1_WDATA),
		.WSTRB				(M1_WSTRB),
		.WLAST				(M1_WLAST),
		.WVALID				(M1_WVALID),
		.WREADY				(M1_WREADY),
		.BID				(M1_BID),
		.BRESP				(M1_BRESP),
		.BVALID				(M1_BVALID),
		.BREADY				(M1_BREADY),
		.ARID				(M1_ARID),
		.ARADDR				(M1_ARADDR),
		.ARLEN				(M1_ARLEN),
		.ARSIZE				(M1_ARSIZE),
		.ARBURST			(M1_ARBURST),
		.ARLOCK				(M1_ARLOCK),
		.ARCACHE			(M1_ARCACHE),
		.ARPROT				(M1_ARPROT),
		.ARREGION			(M1_ARREGION),
		.ARQOS				(M1_ARQOS),
		.ARVALID			(M1_ARVALID),
		.ARREADY			(M1_ARREADY),
		.RID				(M1_RID),
		.RDATA				(M1_RDATA),
		.RRESP				(M1_RRESP),
		.RLAST				(M1_RLAST),
		.RVALID				(M1_RVALID),
		.RREADY				(M1_RREADY)
	);

	processor_axi_wrapper #(
		// master parameters
		.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
		.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
		.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
	) processor_axi_wrapper (
		//// system
		.CLK				(MCLK),
		.nRST				(nRST),
		.INTR				(INTR),
		//// slave APB
		.S_PSEL				(S_PSEL),
		.S_PENABLE			(S_PENABLE),
		.S_PWRITE			(S_PWRITE),
		.S_PADDR			(S_PADDR),
		.S_PWDATA			(S_PWDATA),
		.S_PRDATA			(S_PRDATA),
		.S_PREADY			(S_PREADY),
		.S_PSLVERR			(S_PSLVERR),
		//// AXI-LITE external master interface
		.EX_AWADDR			(EX_AWADDR),
		.EX_AWVALID			(EX_AWVALID),
		.EX_AWREADY			(EX_AWREADY),
		.EX_WDATA			(EX_WDATA),
		.EX_WSTRB			(EX_WSTRB),
		.EX_WVALID			(EX_WVALID),
		.EX_WREADY			(EX_WREADY),
		.EX_ARADDR			(EX_ARADDR),
		.EX_ARVALID			(EX_ARVALID),
		.EX_ARREADY			(EX_ARREADY),
		.EX_RDATA			(EX_RDATA),
		.EX_RRESP			(EX_RRESP),
		.EX_RVALID			(EX_RVALID),
		.EX_RREADY			(EX_RREADY),
		.EX_BRESP			(EX_BRESP),
		.EX_BVALID			(EX_BVALID),
		.EX_BREADY			(EX_BREADY),
		//// AXI master #0 interface
		.M0_AWID			(M0_AWID),
		.M0_AWADDR			(M0_AWADDR),
		.M0_AWLEN			(M0_AWLEN),
		.M0_AWSIZE			(M0_AWSIZE),
		.M0_AWBURST			(M0_AWBURST),
		.M0_AWLOCK			(M0_AWLOCK),
		.M0_AWCACHE			(M0_AWCACHE),
		.M0_AWPROT			(M0_AWPROT),
		.M0_AWREGION		(M0_AWREGION),
		.M0_AWQOS			(M0_AWQOS),
		.M0_AWVALID			(M0_AWVALID),
		.M0_AWREADY			(M0_AWREADY),
		.M0_WID				(M0_WID),
		.M0_WDATA			(M0_WDATA),
		.M0_WSTRB			(M0_WSTRB),
		.M0_WLAST			(M0_WLAST),
		.M0_WVALID			(M0_WVALID),
		.M0_WREADY			(M0_WREADY),
		.M0_BID				(M0_BID),
		.M0_BRESP			(M0_BRESP),
		.M0_BVALID			(M0_BVALID),
		.M0_BREADY			(M0_BREADY),
		.M0_ARID			(M0_ARID),
		.M0_ARADDR			(M0_ARADDR),
		.M0_ARLEN			(M0_ARLEN),
		.M0_ARSIZE			(M0_ARSIZE),
		.M0_ARBURST			(M0_ARBURST),
		.M0_ARLOCK			(M0_ARLOCK),
		.M0_ARCACHE			(M0_ARCACHE),
		.M0_ARPROT			(M0_ARPROT),
		.M0_ARREGION		(M0_ARREGION),
		.M0_ARQOS			(M0_ARQOS),
		.M0_ARVALID			(M0_ARVALID),
		.M0_ARREADY			(M0_ARREADY),
		.M0_RID				(M0_RID),
		.M0_RDATA			(M0_RDATA),
		.M0_RRESP			(M0_RRESP),
		.M0_RLAST			(M0_RLAST),
		.M0_RVALID			(M0_RVALID),
		.M0_RREADY			(M0_RREADY),
		//// AXI master #1 interface
		.M1_AWID			(M1_AWID),
		.M1_AWADDR			(M1_AWADDR),
		.M1_AWLEN			(M1_AWLEN),
		.M1_AWSIZE			(M1_AWSIZE),
		.M1_AWBURST			(M1_AWBURST),
		.M1_AWLOCK			(M1_AWLOCK),
		.M1_AWCACHE			(M1_AWCACHE),
		.M1_AWPROT			(M1_AWPROT),
		.M1_AWREGION		(M1_AWREGION),
		.M1_AWQOS			(M1_AWQOS),
		.M1_AWVALID			(M1_AWVALID),
		.M1_AWREADY			(M1_AWREADY),
		.M1_WID				(M1_WID),
		.M1_WDATA			(M1_WDATA),
		.M1_WSTRB			(M1_WSTRB),
		.M1_WLAST			(M1_WLAST),
		.M1_WVALID			(M1_WVALID),
		.M1_WREADY			(M1_WREADY),
		.M1_BID				(M1_BID),
		.M1_BRESP			(M1_BRESP),
		.M1_BVALID			(M1_BVALID),
		.M1_BREADY			(M1_BREADY),
		.M1_ARID			(M1_ARID),
		.M1_ARADDR			(M1_ARADDR),
		.M1_ARLEN			(M1_ARLEN),
		.M1_ARSIZE			(M1_ARSIZE),
		.M1_ARBURST			(M1_ARBURST),
		.M1_ARLOCK			(M1_ARLOCK),
		.M1_ARCACHE			(M1_ARCACHE),
		.M1_ARPROT			(M1_ARPROT),
		.M1_ARREGION		(M1_ARREGION),
		.M1_ARQOS			(M1_ARQOS),
		.M1_ARVALID			(M1_ARVALID),
		.M1_ARREADY			(M1_ARREADY),
		.M1_RID				(M1_RID),
		.M1_RDATA			(M1_RDATA),
		.M1_RRESP			(M1_RRESP),
		.M1_RLAST			(M1_RLAST),
		.M1_RVALID			(M1_RVALID),
		.M1_RREADY			(M1_RREADY)
	);

	hdmi_controller	#(
		.M_AXI_ADDR_WIDTH	(C_M_AXI_ADDR_WIDTH)
	) hdmi (
		.CLK				(MCLK),
		.nRST				(nRST)
	);

endmodule
