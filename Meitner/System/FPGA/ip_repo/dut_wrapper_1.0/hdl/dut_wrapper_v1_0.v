`include "DUTs/processor_axi_wrapper/includes.vh"
//`include "DUTs/meitner/processor_top.v"
//`include "DUTs/MTSP/processor_top.sv"
`include "DUTs/processor_axi_wrapper/processor_top.v"

module dut_wrapper_v1_0 #(
	// M_AXI
	parameter integer	C_M_AXI_ID_WIDTH		= 1,
	parameter integer	C_M_AXI_ADDR_WIDTH		= 32,
	parameter integer	C_M_AXI_DATA_WIDTH		= 512
) (
	//// system
	input									ACLK,
	input									nRST,
	output									INTR,

	//// slave APB -------------------------
	input									S_PSEL,
	input									S_PENABLE,
	input									S_PWRITE,
	input	[15:0]							S_PADDR,
	input	[31:0]							S_PWDATA,
	output	[31:0]							S_PRDATA,
	output									S_PREADY,
	output									S_PSLVERR,

	//// master AXI ------------------------
	// write address
	output	[C_M_AXI_ID_WIDTH-1 : 0]		M_AWID,
	output	[C_M_AXI_ADDR_WIDTH-1 : 0]		M_AWADDR,
	output	[7 : 0]							M_AWLEN,
	output	[2 : 0]							M_AWSIZE,
	output	[1 : 0]							M_AWBURST,
	output									M_AWLOCK,
	output	[3 : 0]							M_AWCACHE,
	output	[2 : 0]							M_AWPROT,
	output	[3 : 0]							M_AWQOS,
	output									M_AWVALID,
	input									M_AWREADY,
	// write data
	output	[C_M_AXI_ID_WIDTH-1 : 0]		M_WID,
	output	[C_M_AXI_DATA_WIDTH-1 : 0]		M_WDATA,
	output	[C_M_AXI_DATA_WIDTH/8-1 : 0]	M_WSTRB,
	output									M_WLAST,
	output									M_WVALID,
	input									M_WREADY,
	// read address
	output	[C_M_AXI_ID_WIDTH-1 : 0]		M_ARID,
	output	[C_M_AXI_ADDR_WIDTH-1 : 0]		M_ARADDR,
	output	[7 : 0]							M_ARLEN,
	output	[2 : 0]							M_ARSIZE,
	output	[1 : 0]							M_ARBURST,
	output									M_ARLOCK,
	output	[3 : 0]							M_ARCACHE,
	output	[2 : 0]							M_ARPROT,
	output	[3 : 0]							M_ARQOS,
	output									M_ARVALID,
	input									M_ARREADY,
	// read data
	input	[C_M_AXI_ID_WIDTH-1 : 0]		M_RID,
	input	[C_M_AXI_DATA_WIDTH-1 : 0]		M_RDATA,
	input	[1 : 0]							M_RRESP,
	input									M_RLAST,
	input									M_RVALID,
	output									M_RREADY,
	// bus
	input	[C_M_AXI_ID_WIDTH-1 : 0]		M_BID,
	input	[1 : 0]							M_BRESP,
	input									M_BVALID,
	output									M_BREADY
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
// DUT processor
processor_axi_wrapper #(
	.C_M_AXI_ID_WIDTH		(C_M_AXI_ID_WIDTH),
	.C_M_AXI_ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
	.C_M_AXI_DATA_WIDTH		(C_M_AXI_DATA_WIDTH)
) processor_inst (
	//// system
	.CLK			(ACLK),
	.nRST			(nRST),
	.INTR			(INTR),
	//// slave APB -------------------------
	.S_PSEL			(S_PSEL),
	.S_PENABLE		(S_PENABLE),
	.S_PWRITE		(S_PWRITE),
	.S_PADDR		(S_PADDR),
	.S_PWDATA		(S_PWDATA),
	.S_PRDATA		(S_PRDATA),
	.S_PREADY		(S_PREADY),
	.S_PSLVERR		(S_PSLVERR),
	//// AXI master interface --------------
	.M_AWID			(M_AWID),
	.M_AWADDR		(M_AWADDR),
	.M_AWLEN		(M_AWLEN),
	.M_AWSIZE		(M_AWSIZE),
	.M_AWBURST		(M_AWBURST),
	.M_AWLOCK		(M_AWLOCK),
	.M_AWCACHE		(M_AWCACHE),
	.M_AWPROT		(M_AWPROT),
	.M_AWREGION		(),
	.M_AWQOS		(M_AWQOS),
	.M_AWVALID		(M_AWVALID),
	.M_AWREADY		(M_AWREADY),
	.M_WID			(M_WID),
	.M_WDATA		(M_WDATA),
	.M_WSTRB		(M_WSTRB),
	.M_WLAST		(M_WLAST),
	.M_WVALID		(M_WVALID),
	.M_WREADY		(M_WREADY),
	.M_BID			(M_BID),
	.M_BRESP		(M_BRESP),
	.M_BVALID		(M_BVALID),
	.M_BREADY		(M_BREADY),
	.M_ARID			(M_ARID),
	.M_ARADDR		(M_ARADDR),
	.M_ARLEN		(M_ARLEN),
	.M_ARSIZE		(M_ARSIZE),
	.M_ARBURST		(M_ARBURST),
	.M_ARLOCK		(M_ARLOCK),
	.M_ARCACHE		(M_ARCACHE),
	.M_ARPROT		(M_ARPROT),
	.M_ARREGION		(),
	.M_ARQOS		(M_ARQOS),
	.M_ARVALID		(M_ARVALID),
	.M_ARREADY		(M_ARREADY),
	.M_RID			(M_RID),
	.M_RDATA		(M_RDATA),
	.M_RRESP		(M_RRESP),
	.M_RLAST		(M_RLAST),
	.M_RVALID		(M_RVALID),
	.M_RREADY		(M_RREADY)
);

endmodule
