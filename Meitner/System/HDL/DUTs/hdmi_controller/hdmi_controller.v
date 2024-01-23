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
module hdmi_controller #(
		// S_AXI
		parameter integer	C_S_AXI_DATA_WIDTH	= 32,
		parameter integer	C_S_AXI_ADDR_WIDTH	= 16,
		// M_AXI
		parameter integer	C_M_AXI_ID_WIDTH	= 1,
		parameter integer	C_M_AXI_ADDR_WIDTH	= 32,
		parameter integer	C_M_AXI_DATA_WIDTH	= 512,
		// HDMI parameters
		parameter			C_CLKIN_PERIOD		= 5,
		parameter			C_DEFAULT_BASE_ADDR	= 64'h00000000_80000000
	) (
		//// AXI slave interface
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

		// I2C interface
		output									I2C_nRST,		// i2c reset (active low)
		input									I2C_SCL_I,		// i2c SCL input
		output									I2C_SCL_O,		// i2c SCL output
		output									I2C_SCL_T,		// i2c SCL tri-state enable
		input									I2C_SDA_I,		// i2c SDA input
		output									I2C_SDA_O,		// i2c SDA output
		output									I2C_SDA_T,		// i2c SDA tri-state enable
		// HDMI SIGNALS
		input									HDMI_INTR,		// hdmi interrupt input
		output									HDMI_CLK,		// hdmi clock
		output									HDMI_DE,		// data enable
		output									HDMI_HSYNC,		// horizontal sync.
		output									HDMI_VSYNC,		// vertical sync.
		output	[23:0]							HDMI_DATA,		// data output
		output									HDMI_SPDIF		// sony & philips digital interconnect format, sound output
	);

	// definition & assignment ---------------------------------------------------
	// interrupt bypass
	assign	INTR			= HDMI_INTR;
	assign	HDMI_SPDIF		= 1'b0;				// sound is not used.

	// i2c output tied to zero when tri-state is enable for valid input handling.
	assign	I2C_SCL_O		= 1'b0;
	assign	I2C_SDA_O		= 1'b0;

	//// Simplified slave bus interface
	assign	S_PSLVERR		= 1'b0;
	assign	S_PREADY		= 1'b1;

	//// Simplified master bus interface
	wire										mr_req;		// request
	wire										mr_grant;	// grant
	wire	[C_M_AXI_ADDR_WIDTH-1:0]			mr_addr;	// address
	wire	[7:0]								mr_size;	// transfer size
	wire	[C_M_AXI_DATA_WIDTH-1:0]			mr_data;	// data
	wire										mr_valid;	// valid
	wire										mr_ready;	// ready
	wire										mr_last;	// last

	// implementation ------------------------------------------------------------
	// master bus
	// unused write
	assign	M_AWID			= {C_M_AXI_ID_WIDTH{1'b0}};
	assign	M_AWADDR		= {C_M_AXI_ADDR_WIDTH{1'b0}};
	assign	M_AWLEN			= 8'd0;
	assign	M_AWSIZE		= 3'd0;
	assign	M_AWBURST		= 2'd0;
	assign	M_AWLOCK		= 1'b0;
	assign	M_AWCACHE		= 4'd0;
	assign	M_AWPROT		= 3'd0;
	assign	M_AWREGION		= 4'd0;
	assign	M_AWQOS			= 4'd0;
	assign	M_AWVALID		= 1'b0;
	assign	M_WID			= {C_M_AXI_ID_WIDTH{1'b0}};
	assign	M_WDATA			= {C_M_AXI_DATA_WIDTH{1'b0}};
	assign	M_WSTRB			= {(C_M_AXI_DATA_WIDTH/8){1'b0}};
	assign	M_WLAST			= 1'b0;
	assign	M_WVALID		= 1'b0;
	assign	M_BREADY		= 1'b1;

	axi_master_wrapper#(
		.C_M_AXI_ID_WIDTH		(C_M_AXI_ID_WIDTH),
		.C_M_AXI_ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
		.C_M_AXI_DATA_WIDTH		(C_M_AXI_DATA_WIDTH)
	) master_inst(
		//// AXI master interface
		// system
		.M_CLK					(ACLK),
		.M_nRST					(nRST),
		// write address (not used.)
		.M_AWID					(),
		.M_AWADDR				(),
		.M_AWLEN				(),
		.M_AWSIZE				(),
		.M_AWBURST				(),
		.M_AWLOCK				(),
		.M_AWCACHE				(),
		.M_AWPROT				(),
		.M_AWREGION				(),
		.M_AWQOS				(),
		.M_AWVALID				(),
		.M_AWREADY				(1'b1),
		// write data
		.M_WID					(),
		.M_WDATA				(),
		.M_WSTRB				(),
		.M_WLAST				(),
		.M_WVALID				(),
		.M_WREADY				(1'b1),
		.M_BID					({C_M_AXI_ID_WIDTH{1'b0}}),
		.M_BRESP				(2'd0),
		.M_BVALID				(1'b0),
		.M_BREADY				(),
		// read address
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
		// read data
		.M_RID					(M_RID),
		.M_RDATA				(M_RDATA),
		.M_RRESP				(M_RRESP),
		.M_RLAST				(M_RLAST),
		.M_RVALID				(M_RVALID),
		.M_RREADY				(M_RREADY),
		//// Simplified master bus interface
		.MR_REQ					(mr_req),
		.MR_GRANT				(mr_grant),
		.MR_ADDR				(mr_addr),
		.MR_SIZE				(mr_size),
		.MR_DATA				(mr_data),
		.MR_VALID				(mr_valid),
		.MR_LAST				(mr_last),
		.MR_READY				(mr_ready),

		.MW_REQ					(1'b0),
		.MW_GRANT				(),
		.MW_ADDR				('d0),
		.MW_SIZE				('d0),
		.MW_DATA				('d0),
		.MW_VALID				(1'b0),
		.MW_READY				(),
		.MW_LAST				()
	);

	HDMI_TOP #(
		.C_DEFAULT_BASE_ADDR	(C_DEFAULT_BASE_ADDR),
		.C_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
		.C_CLKIN_PERIOD			(C_CLKIN_PERIOD)
	) hdmi_top (
		//// system
		.ACLK					(ACLK),
		.nRST					(nRST),
		//// Simplified Slave interface
		.S_EN					(S_PSEL & S_PENABLE),
		.S_WE					(S_PWRITE),
		.S_ADDR					(S_PADDR[5:2]),
		.S_WDATA				(S_PWDATA),
		.S_RDATA				(S_PRDATA),
		//// Simplified master interface
		.MR_REQ					(mr_req),
		.MR_GRANT				(mr_grant),
		.MR_ADDR				(mr_addr),
		.MR_SIZE				(mr_size),
		.MR_DATA				(mr_data),
		.MR_VALID				(mr_valid),
		.MR_LAST				(mr_last),
		.MR_READY				(mr_ready),

		// I2C interface
		.I2C_nRST				(I2C_nRST),
		.I2C_SCL_I				(I2C_SCL_I),
		.I2C_SCL_T				(I2C_SCL_T),
		.I2C_SDA_I				(I2C_SDA_I),
		.I2C_SDA_T				(I2C_SDA_T),
		// HDMI SIGNALS
		.HDMI_CLK				(HDMI_CLK),
		.HDMI_DE				(HDMI_DE),
		.HDMI_HSYNC				(HDMI_HSYNC),
		.HDMI_VSYNC				(HDMI_VSYNC),
		.HDMI_DATA				(HDMI_DATA)
	);

endmodule
