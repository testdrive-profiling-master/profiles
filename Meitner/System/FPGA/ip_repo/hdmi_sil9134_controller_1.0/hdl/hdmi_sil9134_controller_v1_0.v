`include "DUTs/hdmi_controller/includes.vh"

module hdmi_sil9134_controller_v1_0 #(
	// S_AXI
	parameter integer	C_S_AXI_DATA_WIDTH	= 32,
	parameter integer	C_S_AXI_ADDR_WIDTH	= 16,
	// M_AXI
	parameter integer	C_M_AXI_ID_WIDTH	= 1,
	parameter integer	C_M_AXI_ADDR_WIDTH	= 32,
	parameter integer	C_M_AXI_DATA_WIDTH	= 512
)(
	// system
	input									ACLK,			// clock
	input									nRST,			// reset (active low)
	output									INTR,			// interrupt output
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
	output	[3 : 0]							M_AWREGION,
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
	output	[3 : 0]							M_ARREGION,
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
	output									M_BREADY,
	
	//--------------------------------------------------------
	// HDMI interface
	output									HDMI_nRST,
	// I2C interface
	inout									HDMI_I2C_SCL,	// i2c SCL
	inout									HDMI_I2C_SDA,	// i2c SDA
	// HDMI SIGNALS
	input									HDMI_INTR,		// hdmi interrupt input
	input									HDMI_HPD,		// hdmi hot plug-in detect
	output									HDMI_CLK,		// hdmi clock
	output									HDMI_DE,		// data enable
	output									HDMI_HSYNC,		// horizontal sync.
	output									HDMI_VSYNC,		// vertical sync.
	output	[23:0]							HDMI_DATA		// data output
);

// definition & assignment ---------------------------------------------------
wire			I2C_SCL_I, I2C_SCL_O, I2C_SCL_T;
wire			I2C_SDA_I, I2C_SDA_O, I2C_SDA_T;
wire	[35:0]	hdmi_out;
// implementation ------------------------------------------------------------
// for i2c io buffer
IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) scl_buf (
	.T		(I2C_SCL_T),
	.I		(I2C_SCL_O),
	.O		(I2C_SCL_I),
	.IO		(HDMI_I2C_SCL)
);

IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) sda_buf (
	.T		(I2C_SDA_T),
	.I		(I2C_SDA_O),
	.O		(I2C_SDA_I),
	.IO		(HDMI_I2C_SDA)
);

// HDMI controller
hdmi_controller #(
	.C_S_AXI_DATA_WIDTH		(C_S_AXI_DATA_WIDTH),
	.C_S_AXI_ADDR_WIDTH		(C_S_AXI_ADDR_WIDTH),
	.C_M_AXI_ID_WIDTH		(C_M_AXI_ID_WIDTH),
	.C_M_AXI_ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
	.C_M_AXI_DATA_WIDTH		(C_M_AXI_DATA_WIDTH),
	.C_CLKIN_PERIOD			(3.3333333333333333),
	.C_DEFAULT_BASE_ADDR	(64'h00000001_00000000)
) hdmi_ctrl (
	.ACLK					(ACLK),
	.nRST					(nRST),
	.INTR					(INTR),
	
	.S_PSEL					(S_PSEL),
	.S_PENABLE				(S_PENABLE),
	.S_PWRITE				(S_PWRITE),
	.S_PADDR				(S_PADDR),
	.S_PWDATA				(S_PWDATA),
	.S_PRDATA				(S_PRDATA),
	.S_PREADY				(S_PREADY),
	.S_PSLVERR				(S_PSLVERR),

	.M_AWID					(M_AWID),
	.M_AWADDR				(M_AWADDR),
	.M_AWLEN				(M_AWLEN),
	.M_AWSIZE				(M_AWSIZE),
	.M_AWBURST				(M_AWBURST),
	.M_AWLOCK				(M_AWLOCK),
	.M_AWCACHE				(M_AWCACHE),
	.M_AWPROT				(M_AWPROT),
	.M_AWREGION				(M_AWREGION),
	.M_AWQOS				(M_AWQOS),
	.M_AWVALID				(M_AWVALID),
	.M_AWREADY				(M_AWREADY),
	.M_WID					(M_WID),
	.M_WDATA				(M_WDATA),
	.M_WSTRB				(M_WSTRB),
	.M_WLAST				(M_WLAST),
	.M_WVALID				(M_WVALID),
	.M_WREADY				(M_WREADY),
	.M_ARID					(M_ARID),
	.M_ARADDR				(M_ARADDR),
	.M_ARLEN				(M_ARLEN),
	.M_ARSIZE				(M_ARSIZE),
	.M_ARBURST				(M_ARBURST),
	.M_ARLOCK				(M_ARLOCK),
	.M_ARCACHE				(M_ARCACHE),
	.M_ARPROT				(M_ARPROT),
	.M_ARREGION				(M_ARREGION),
	.M_ARQOS				(M_ARQOS),
	.M_ARVALID				(M_ARVALID),
	.M_ARREADY				(M_ARREADY),
	.M_RID					(M_RID),
	.M_RDATA				(M_RDATA),
	.M_RRESP				(M_RRESP),
	.M_RLAST				(M_RLAST),
	.M_RVALID				(M_RVALID),
	.M_RREADY				(M_RREADY),
	.M_BID					(M_BID),
	.M_BRESP				(M_BRESP),
	.M_BVALID				(M_BVALID),
	.M_BREADY				(M_BREADY),

	.I2C_nRST				(HMDI_nRST),
	.I2C_SCL_I				(I2C_SCL_I),
	.I2C_SCL_O				(I2C_SCL_O),
	.I2C_SCL_T				(I2C_SCL_T),
	.I2C_SDA_I				(I2C_SDA_I),
	.I2C_SDA_O				(I2C_SDA_O),
	.I2C_SDA_T				(I2C_SDA_T),

	.HDMI_INTR				(HDMI_INTR),
	.HDMI_CLK				(HDMI_CLK),
	.HDMI_DE				(HDMI_DE),
	.HDMI_HSYNC				(HDMI_HSYNC),
	.HDMI_VSYNC				(HDMI_VSYNC),
	.HDMI_DATA				(HDMI_DATA),
	.HDMI_SPDIF				()
);

endmodule
