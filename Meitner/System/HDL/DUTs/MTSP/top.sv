//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
`include "includes.vh"

/* HIDDEN */
module top (
	input								MCLK,				// clock
	input								nRST,				// reset (active low)
	output								BUSY,				// processor is busy
	output								INTR				// interrupt signal
);

localparam	C_M_AXI_ID_WIDTH			= 1;
localparam	C_M_AXI_ADDR_WIDTH			= 36;
localparam	C_M_AXI_DATA_WIDTH			= 512;
localparam	C_S_APB_ADDR_WIDTH			= 12;

// definition & assignment ---------------------------------------------------
initial
	SetSystemDescription("MTSP");

//// slave APB -------------------------
wire									S_PSEL;
wire									S_PENABLE;
wire									S_PWRITE;
wire	[C_S_APB_ADDR_WIDTH-1:0]		S_PADDR;
wire	[31:0]							S_PWDATA;
wire	[31:0]							S_PRDATA;
wire	[3:0]							S_PSTRB;
wire									S_PREADY;
wire									S_PSLVERR;

//// AXI master interface for instruction --------------
// read address
wire	[C_M_AXI_ID_WIDTH-1:0]			M0_ARID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M0_ARADDR;
wire	[7:0]							M0_ARLEN;
wire	[2:0]							M0_ARSIZE;
wire	[1:0]							M0_ARBURST;
wire									M0_ARLOCK;
wire	[3:0]							M0_ARCACHE;
wire	[2:0]							M0_ARPROT;
wire									M0_ARVALID;
wire									M0_ARREADY;
wire	[3:0]							M0_ARQOS;
wire	[3:0]							M0_ARREGION;
// read data
wire	[C_M_AXI_ID_WIDTH-1:0]			M0_RID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M0_RDATA;
wire	[1:0]							M0_RRESP;
wire									M0_RLAST;
wire									M0_RVALID;
wire									M0_RREADY;
// write address
wire	[C_M_AXI_ID_WIDTH-1:0]			M0_AWID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M0_AWADDR;
wire	[7:0]							M0_AWLEN;
wire	[2:0]							M0_AWSIZE;
wire	[1:0]							M0_AWBURST;
wire									M0_AWLOCK;
wire	[3:0]							M0_AWCACHE;
wire	[2:0]							M0_AWPROT;
wire									M0_AWVALID;
wire									M0_AWREADY;
wire	[3:0]							M0_AWREGION;
wire	[3:0]							M0_AWQOS;
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

//// AXI master interface for data --------------
// read address
wire	[C_M_AXI_ID_WIDTH-1:0]			M1_ARID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M1_ARADDR;
wire	[7:0]							M1_ARLEN;
wire	[2:0]							M1_ARSIZE;
wire	[1:0]							M1_ARBURST;
wire									M1_ARLOCK;
wire	[3:0]							M1_ARCACHE;
wire	[2:0]							M1_ARPROT;
wire									M1_ARVALID;
wire									M1_ARREADY;
wire	[3:0]							M1_ARQOS;
wire	[3:0]							M1_ARREGION;
// read data
wire	[C_M_AXI_ID_WIDTH-1:0]			M1_RID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M1_RDATA;
wire	[1:0]							M1_RRESP;
wire									M1_RLAST;
wire									M1_RVALID;
wire									M1_RREADY;
// write address
wire	[C_M_AXI_ID_WIDTH-1:0]			M1_AWID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M1_AWADDR;
wire	[7:0]							M1_AWLEN;
wire	[2:0]							M1_AWSIZE;
wire	[1:0]							M1_AWBURST;
wire									M1_AWLOCK;
wire	[3:0]							M1_AWCACHE;
wire	[2:0]							M1_AWPROT;
wire									M1_AWVALID;
wire									M1_AWREADY;
wire	[3:0]							M1_AWREGION;
wire	[3:0]							M1_AWQOS;
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

//// AXI master interface for stack --------------
// read address
wire	[C_M_AXI_ID_WIDTH-1:0]			M2_ARID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M2_ARADDR;
wire	[7:0]							M2_ARLEN;
wire	[2:0]							M2_ARSIZE;
wire	[1:0]							M2_ARBURST;
wire									M2_ARLOCK;
wire	[3:0]							M2_ARCACHE;
wire	[2:0]							M2_ARPROT;
wire									M2_ARVALID;
wire									M2_ARREADY;
wire	[3:0]							M2_ARQOS;
wire	[3:0]							M2_ARREGION;
// read data
wire	[C_M_AXI_ID_WIDTH-1:0]			M2_RID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M2_RDATA;
wire	[1:0]							M2_RRESP;
wire									M2_RLAST;
wire									M2_RVALID;
wire									M2_RREADY;
// write address
wire	[C_M_AXI_ID_WIDTH-1:0]			M2_AWID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M2_AWADDR;
wire	[7:0]							M2_AWLEN;
wire	[2:0]							M2_AWSIZE;
wire	[1:0]							M2_AWBURST;
wire									M2_AWLOCK;
wire	[3:0]							M2_AWCACHE;
wire	[2:0]							M2_AWPROT;
wire									M2_AWVALID;
wire									M2_AWREADY;
wire	[3:0]							M2_AWREGION;
wire	[3:0]							M2_AWQOS;
// write data
wire	[C_M_AXI_ID_WIDTH-1:0]			M2_WID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M2_WDATA;
wire	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M2_WSTRB;
wire									M2_WLAST;
wire									M2_WVALID;
wire									M2_WREADY;
// bus interaction
wire	[C_M_AXI_ID_WIDTH-1:0]			M2_BID;
wire	[1:0]							M2_BRESP;
wire									M2_BVALID;
wire									M2_BREADY;

// implementation ------------------------------------------------------------

// APB bus implementation
testdrive_apb_slave_bfm #(
	.C_BUS_TITLE		("MTSP control APB slave"),
	.C_BASE_ADDR		(64'h10000000),
	.C_ADDR_BITS		(C_S_APB_ADDR_WIDTH)
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
	.PSTRB				(S_PSTRB),		// not use this
	.PRDATA				(S_PRDATA),
	.PREADY				(S_PREADY),
	.PSLVERR			(S_PSLVERR)
);

// AXI4 bus implementation
testdrive_axi4_master_bfm #(
	.C_BUS_TITLE				("AXI master (Instruction)"),
	.C_THREAD_ID_WIDTH			(C_M_AXI_ID_WIDTH),
	.C_ADDR_WIDTH				(C_M_AXI_ADDR_WIDTH),
	.C_DATA_WIDTH				(C_M_AXI_DATA_WIDTH)
) axi4m_inst (
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

// AXI4 bus implementation
testdrive_axi4_master_bfm #(
	.C_BUS_TITLE				("AXI master (Data)"),
	.C_THREAD_ID_WIDTH			(C_M_AXI_ID_WIDTH),
	.C_ADDR_WIDTH				(C_M_AXI_ADDR_WIDTH),
	.C_DATA_WIDTH				(C_M_AXI_DATA_WIDTH)
) axi4m_data (
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

MTSP_wrapper mtspu (
	// clock & reset
	.MCLK				(MCLK),
	.nRST				(nRST),
	.INTR				(INTR),

	// apb
	.S_PSEL				(S_PSEL),
	.S_PENABLE			(S_PENABLE),
	.S_PWRITE			(S_PWRITE),
	.S_PADDR			(S_PADDR),
	.S_PWDATA			(S_PWDATA),
	.S_PREADY			(S_PREADY),
	.S_PRDATA			(S_PRDATA),
	.S_PSLVERR			(S_PSLVERR),

	// axi_0 : instruction bus
	.M0_AWREADY			(M0_AWREADY),
	.M0_WREADY			(M0_WREADY),
	.M0_BVALID			(M0_BVALID),
	.M0_BRESP			(M0_BRESP),
	.M0_BID				(M0_BID),
	.M0_ARREADY			(M0_ARREADY),
	.M0_RVALID			(M0_RVALID),
	.M0_RDATA			(M0_RDATA),
	.M0_RRESP			(M0_RRESP),
	.M0_RID				(M0_RID),
	.M0_ARQOS			(M0_ARQOS),
	.M0_ARREGION		(M0_ARREGION),
	.M0_AWVALID			(M0_AWVALID),
	.M0_AWADDR			(M0_AWADDR),
	.M0_AWSIZE			(M0_AWSIZE),
	.M0_AWBURST			(M0_AWBURST),
	.M0_AWCACHE			(M0_AWCACHE),
	.M0_AWPROT			(M0_AWPROT),
	.M0_AWID			(M0_AWID),
	.M0_AWLEN			(M0_AWLEN),
	.M0_AWLOCK			(M0_AWLOCK),
	.M0_WVALID			(M0_WVALID),
	.M0_WLAST			(M0_WLAST),
	.M0_WDATA			(M0_WDATA),
	.M0_WSTRB			(M0_WSTRB),
	.M0_WID				(M0_WID),
	.M0_BREADY			(M0_BREADY),
	.M0_ARVALID			(M0_ARVALID),
	.M0_ARADDR			(M0_ARADDR),
	.M0_ARSIZE			(M0_ARSIZE),
	.M0_ARBURST			(M0_ARBURST),
	.M0_ARCACHE			(M0_ARCACHE),
	.M0_ARPROT			(M0_ARPROT),
	.M0_ARID			(M0_ARID),
	.M0_ARLEN			(M0_ARLEN),
	.M0_ARLOCK			(M0_ARLOCK),
	.M0_RREADY			(M0_RREADY),
	.M0_RLAST			(M0_RLAST),
	.M0_AWQOS			(M0_AWQOS),
	.M0_AWREGION		(M0_AWREGION),

	// axi_1 : data bus
	.M1_AWREADY			(M1_AWREADY),
	.M1_WREADY			(M1_WREADY),
	.M1_BVALID			(M1_BVALID),
	.M1_BRESP			(M1_BRESP),
	.M1_BID				(M1_BID),
	.M1_ARREADY			(M1_ARREADY),
	.M1_RVALID			(M1_RVALID),
	.M1_RDATA			(M1_RDATA),
	.M1_RRESP			(M1_RRESP),
	.M1_RID				(M1_RID),
	.M1_ARQOS			(M1_ARQOS),
	.M1_ARREGION		(M1_ARREGION),
	.M1_AWVALID			(M1_AWVALID),
	.M1_AWADDR			(M1_AWADDR),
	.M1_AWSIZE			(M1_AWSIZE),
	.M1_AWBURST			(M1_AWBURST),
	.M1_AWCACHE			(M1_AWCACHE),
	.M1_AWPROT			(M1_AWPROT),
	.M1_AWID			(M1_AWID),
	.M1_AWLEN			(M1_AWLEN),
	.M1_AWLOCK			(M1_AWLOCK),
	.M1_WVALID			(M1_WVALID),
	.M1_WLAST			(M1_WLAST),
	.M1_WDATA			(M1_WDATA),
	.M1_WSTRB			(M1_WSTRB),
	.M1_WID				(M1_WID),
	.M1_BREADY			(M1_BREADY),
	.M1_ARVALID			(M1_ARVALID),
	.M1_ARADDR			(M1_ARADDR),
	.M1_ARSIZE			(M1_ARSIZE),
	.M1_ARBURST			(M1_ARBURST),
	.M1_ARCACHE			(M1_ARCACHE),
	.M1_ARPROT			(M1_ARPROT),
	.M1_ARID			(M1_ARID),
	.M1_ARLEN			(M1_ARLEN),
	.M1_ARLOCK			(M1_ARLOCK),
	.M1_RREADY			(M1_RREADY),
	.M1_RLAST			(M1_RLAST),
	.M1_AWQOS			(M1_AWQOS),
	.M1_AWREGION		(M1_AWREGION),

	// axi_2 : stack bus
	.M2_AWREADY			(M2_AWREADY),
	.M2_WREADY			(M2_WREADY),
	.M2_BVALID			(M2_BVALID),
	.M2_BRESP			(M2_BRESP),
	.M2_BID				(M2_BID),
	.M2_ARREADY			(M2_ARREADY),
	.M2_RVALID			(M2_RVALID),
	.M2_RDATA			(M2_RDATA),
	.M2_RRESP			(M2_RRESP),
	.M2_RID				(M2_RID),
	.M2_ARQOS			(M2_ARQOS),
	.M2_ARREGION		(M2_ARREGION),
	.M2_AWVALID			(M2_AWVALID),
	.M2_AWADDR			(M2_AWADDR),
	.M2_AWSIZE			(M2_AWSIZE),
	.M2_AWBURST			(M2_AWBURST),
	.M2_AWCACHE			(M2_AWCACHE),
	.M2_AWPROT			(M2_AWPROT),
	.M2_AWID			(M2_AWID),
	.M2_AWLEN			(M2_AWLEN),
	.M2_AWLOCK			(M2_AWLOCK),
	.M2_WVALID			(M2_WVALID),
	.M2_WLAST			(M2_WLAST),
	.M2_WDATA			(M2_WDATA),
	.M2_WSTRB			(M2_WSTRB),
	.M2_WID				(M2_WID),
	.M2_BREADY			(M2_BREADY),
	.M2_ARVALID			(M2_ARVALID),
	.M2_ARADDR			(M2_ARADDR),
	.M2_ARSIZE			(M2_ARSIZE),
	.M2_ARBURST			(M2_ARBURST),
	.M2_ARCACHE			(M2_ARCACHE),
	.M2_ARPROT			(M2_ARPROT),
	.M2_ARID			(M2_ARID),
	.M2_ARLEN			(M2_ARLEN),
	.M2_ARLOCK			(M2_ARLOCK),
	.M2_RREADY			(M2_RREADY),
	.M2_RLAST			(M2_RLAST),
	.M2_AWQOS			(M2_AWQOS),
	.M2_AWREGION		(M2_AWREGION),

	// extra command from MTSP (not used yet here.)
	.CMD_EN				(),
	.CMD_OP				(),
	.CMD_DATA			(),
	.CMD_READY			(1'b1)
);

assign	BUSY	= mtspu.MTSP.MTSP_Command.BUSY;		// processor is busy

endmodule
