
`timescale 1 ns / 1 ps

module TMDS_controller #
(
	// S_AXI
	parameter integer	C_S_AXI_DATA_WIDTH	= 32,
	parameter integer	C_S_AXI_ADDR_WIDTH	= 16,
	// M_AXI
	parameter integer	C_M_AXI_ID_WIDTH	= 1,
	parameter integer	C_M_AXI_ADDR_WIDTH	= 32,
	parameter integer	C_M_AXI_DATA_WIDTH	= 512
)
(
	// system
	input									ACLK,			// clock
	input									nRST,			// reset (active low)
	output									INTR,			// interrupt output
	//// slave APB -------------------------
	input									S_PSEL,
	input									S_PENABLE,
	input									S_PWRITE,
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_PADDR,
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
	input									HDMI_REF_CLK,		// reference clock
	inout									HDMI_CTL_I2C_SCL,	// control i2c SCL
	inout									HDMI_CTL_I2C_SDA,	// control i2c SDA
	output [4:0]							HDMI_DATA_P,		// hdmi output clock & data (positive)
	output [4:0]							HDMI_DATA_N,		// hdmi output clock & data (negative)
	inout									HDMI_I2C_SCL,		// hdmi output source i2c SCL
	inout									HDMI_I2C_SDA,		// hdmi output source i2c SDA
	inout									HDMI_CEC,			// hdmi output CEC
	input									HDMI_HPD,			// hdmi hot plug-in detect
	output									HDMI_OE				// hdmi output enable
);

// TMDS output data & clock
wire	tmds_clk;
wire	tmds_data[3];

OBUFDS tmds_data_0	(.I(tmds_data[0]),	.O(HDMI_DATA_P[0]), .OB(HDMI_DATA_N[0]));
OBUFDS tmds_data_1	(.I(tmds_data[1]),	.O(HDMI_DATA_P[1]), .OB(HDMI_DATA_N[1]));
OBUFDS tmds_data_2	(.I(tmds_data[2]),	.O(HDMI_DATA_P[2]), .OB(HDMI_DATA_N[2]));
OBUFDS tmds_clk		(.I(tmds_clk),		.O(HDMI_DATA_P[3]), .OB(HDMI_DATA_N[3]));
/*OBUFDS_GTE3 #(
	.REFCLK_EN_TX_PATH(1'b1),
	.REFCLK_ICNTL_TX(5'b00111)
) tmds_clk (
	.I(tmds_clk),
	.CEB(1'b0),
	.O(HDMI_DATA_P[3]),
	.OB(HDMI_DATA_N[3])
);*/


// controller's I2C interface (actually not used)
IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) ctrl_scl (
	.T		(1'b1),
	.I		(1'b0),
	.O		(),
	.IO		(HDMI_CTL_I2C_SCL)
);

IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) ctrl_sda (
	.T		(1'b1),
	.I		(1'b0),
	.O		(),
	.IO		(HDMI_CTL_I2C_SDA)
);

// CEC (not used yet)
IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) tmds_cec (
	.T		(1'b1),
	.I		(1'b0),
	.O		(),
	.IO		(HDMI_CEC)
);

// External display's I2C interface
wire	tmds_scl, tmds_sda;
IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) ext_scl (
	.T		(tmds_scl),
	.I		(1'b0),
	.O		(),
	.IO		(HDMI_I2C_SCL)
);

IOBUF #(
	.DRIVE			(8),
	.SLEW			("SLOW"),
	.IOSTANDARD		("LVCMOS18")
) ext_sda (
	.T		(tmds_sda),
	.I		(1'b0),
	.O		(),
	.IO		(HDMI_I2C_SDA)
);

assigned HDMI_OE = 1'b0;

endmodule
