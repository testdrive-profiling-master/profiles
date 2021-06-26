//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 6/26/2021 Sat (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`ifdef USE_TESTDRIVE
`include "template/axi_master_wrapper.v"
`include "template/axi_slave_wrapper.v"
`endif

module dut_top #(
	// slave parameters
	parameter	C_S_AXI_ADDR_WIDTH			= 32,
	parameter	C_S_AXI_DATA_WIDTH			= 32,
	// master parameters
	parameter	C_M_AXI_ID_WIDTH			= 1,
	parameter	C_M_AXI_ADDR_WIDTH			= 32,
	parameter	C_M_AXI_DATA_WIDTH			= 128
) (
	//// system
	input									CLK,			// clock
	input									nRST,			// reset (active low)
	output									BUSY,			// process is busy
	output									INTR,			// interrupt

	//// AXI slave interface
	// write address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_AWADDR,		// write address
	input									S_AWVALID,		// write address validation
	output									S_AWREADY,		// write address ready
	// write data
	input	[C_S_AXI_DATA_WIDTH-1:0]		S_WDATA,		// write data
	input	[(C_S_AXI_DATA_WIDTH/8)-1:0]	S_WSTRB,		// write data byte strobe
	input									S_WVALID,		// write data validation
	output									S_WREADY,		// write data ready for slave
	// read address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_ARADDR,		// read address
	input									S_ARVALID,		// read address validation
	output									S_ARREADY,		// read address ready
	// read data
	output	[C_S_AXI_DATA_WIDTH-1:0]		S_RDATA,		// read data
	output	[1:0]							S_RRESP,		// read data response
	output									S_RVALID,		// read data validation
	input									S_RREADY,		// read data ready for master
	// bus
	output	[1:0]							S_BRESP,		// bus respense
	output									S_BVALID,		// bus validation
	input									S_BREADY,		// bus ready

	//// AXI master interface
	// write address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_AWID,			// The ID tag for the write address group of signals
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR,		// Write address
	output	[7:0]							M_AWLEN,		// Burst_Length = AxLEN[7:0] + 1
	output	[2:0]							M_AWSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]							M_AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output	[1:0]							M_AWLOCK,		// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved)
	output	[3:0]							M_AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]							M_AWPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	[3:0]							M_AWREGION,		//
	output	[3:0]							M_AWQOS,		//
	output									M_AWVALID,		// Write address valid
	input									M_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)

	// write data
	output	[C_M_AXI_ID_WIDTH-1:0]			M_WID,			//
	output	[C_M_AXI_DATA_WIDTH-1:0]		M_WDATA,		// Write data
	output	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
	output									M_WLAST,		// Write last
	output									M_WVALID,		// Write valid
	input									M_WREADY,		// Write ready
	// bus interaction
	input	[C_M_AXI_ID_WIDTH-1:0]			M_BID,			// The ID tag for the write response
	input	[1:0]							M_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input									M_BVALID,		// Write response valid
	output									M_BREADY,		// Response ready
	// read address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_ARID,			// Read address ID tag
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR,		// Read address
	output	[7:0]							M_ARLEN,		// Burst_Length = AxLEN[7:0] + 1
	output	[2:0]							M_ARSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]							M_ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output	[1:0]							M_ARLOCK,		// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved)
	output	[3:0]							M_ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]							M_ARPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	[3:0]							M_ARREGION,		//
	output	[3:0]							M_ARQOS,		//
	output									M_ARVALID,		// Read address valid
	input									M_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
	// read data
	input	[C_M_AXI_ID_WIDTH-1:0]			M_RID,			// Read ID tag.
	input	[C_M_AXI_DATA_WIDTH-1:0]		M_RDATA,		// Read data.
	input	[1:0]							M_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input									M_RLAST,		// Read last. This signal indicates the last transfer in a read burst.
	input									M_RVALID,		// Read valid (1 = read data available, 0 = read data not available)
	output									M_RREADY		// Read ready (1= master ready 0 = master not ready)
);

// definition & assignment ---------------------------------------------------
// internal slave interface
wire					sr_req,		sw_req;		// request
wire					sr_valid,	sw_valid;	// valid
wire	[7:0]			sr_addr,	sw_addr;	// address
wire	[31:0]			sr_data,	sw_data;	// data
// internal master interface
wire					mr_req,		mw_req;		// request
wire					mr_grant,	mw_grant;	// grant
wire	[31:0]			mr_addr,	mw_addr;	// address
wire	[7:0]			mr_size,	mw_size;	// transfer size
wire	[127:0]			mr_data,	mw_data;	// data
wire					mr_valid,	mw_valid;	// valid
wire					mr_ready,	mw_ready;	// ready
wire					mr_last,	mw_last;	// last

// implementation ------------------------------------------------------------
// Slave bus
axi_slave_wrapper#(
	.C_S_AXI_ADDR_WIDTH	( C_S_AXI_ADDR_WIDTH	),
	.C_S_AXI_DATA_WIDTH	( C_S_AXI_DATA_WIDTH	)
)
slave_inst(
	// system
	.S_CLK			( CLK			),
	.S_nRST			( nRST			),
	// write address
	.S_AWADDR		( S_AWADDR		),
	.S_AWVALID		( S_AWVALID		),
	.S_AWREADY		( S_AWREADY		),
	// write data
	.S_WDATA		( S_WDATA		),
	.S_WSTRB		( S_WSTRB		),
	.S_WVALID		( S_WVALID		),
	.S_WREADY		( S_WREADY		),
	// read address
	.S_ARADDR		( S_ARADDR		),
	.S_ARVALID		( S_ARVALID		),
	.S_ARREADY		( S_ARREADY		),
	// read data
	.S_RDATA		( S_RDATA		),
	.S_RRESP		( S_RRESP		),
	.S_RVALID		( S_RVALID		),
	.S_RREADY		( S_RREADY		),
	// bus interaction
	.S_BRESP		( S_BRESP		),
	.S_BVALID		( S_BVALID		),
	.S_BREADY		( S_BREADY		),
	// slave read interface
	.SR_REQ			( sr_req		),
	.SR_VALID		( sr_valid		),
	.SR_ADDR		( sr_addr		),
	.SR_DATA		( sr_data		),
	// slave write interface
	.SW_REQ			( sw_req		),
	.SW_VALID		( sw_valid		),
	.SW_ADDR		( sw_addr		),
	.SW_DATA		( sw_data		)
);

// master bus
axi_master_wrapper#(
	.C_M_AXI_ID_WIDTH			( C_M_AXI_ID_WIDTH			),
	.C_M_AXI_ADDR_WIDTH			( C_M_AXI_ADDR_WIDTH		),
	.C_M_AXI_DATA_WIDTH			( C_M_AXI_DATA_WIDTH		)
)
master_inst(
	// AXI master interface
	.M_CLK			( CLK			),
	.M_nRST			( nRST			),
	.M_AWID			( M_AWID		),
	.M_AWADDR		( M_AWADDR		),
	.M_AWLEN		( M_AWLEN		),
	.M_AWSIZE		( M_AWSIZE		),
	.M_AWBURST		( M_AWBURST		),
	.M_AWLOCK		( M_AWLOCK		),
	.M_AWCACHE		( M_AWCACHE		),
	.M_AWPROT		( M_AWPROT		),
	.M_AWREGION		( M_AWREGION	),
	.M_AWQOS		( M_AWQOS		),
	.M_AWVALID		( M_AWVALID		),
	.M_AWREADY		( M_AWREADY		),
	.M_WID			( M_WID			),
	.M_WDATA		( M_WDATA		),
	.M_WSTRB		( M_WSTRB		),
	.M_WLAST		( M_WLAST		),
	.M_WVALID		( M_WVALID		),
	.M_WREADY		( M_WREADY		),
	.M_BID			( M_BID			),
	.M_BRESP		( M_BRESP		),
	.M_BVALID		( M_BVALID		),
	.M_BREADY		( M_BREADY		),
	.M_ARID			( M_ARID		),
	.M_ARADDR		( M_ARADDR		),
	.M_ARLEN		( M_ARLEN		),
	.M_ARSIZE		( M_ARSIZE		),
	.M_ARBURST		( M_ARBURST		),
	.M_ARLOCK		( M_ARLOCK		),
	.M_ARCACHE		( M_ARCACHE		),
	.M_ARPROT		( M_ARPROT		),
	.M_ARREGION		( M_ARREGION	),
	.M_ARQOS		( M_ARQOS		),
	.M_ARVALID		( M_ARVALID		),
	.M_ARREADY		( M_ARREADY		),
	.M_RID			( M_RID			),
	.M_RDATA		( M_RDATA		),
	.M_RRESP		( M_RRESP		),
	.M_RLAST		( M_RLAST		),
	.M_RVALID		( M_RVALID		),
	.M_RREADY		( M_RREADY		),
	// internal master interface
	.MR_REQ			( mr_req		),
	.MR_GRANT		( mr_grant		),
	.MR_ADDR		( mr_addr		),
	.MR_SIZE		( mr_size		),
	.MR_DATA		( mr_data		),
	.MR_VALID		( mr_valid		),
	.MR_LAST		( mr_last		),
	.MR_READY		( mr_ready		),
	.MW_REQ			( mw_req		),
	.MW_GRANT		( mw_grant		),
	.MW_ADDR		( mw_addr		),
	.MW_SIZE		( mw_size		),
	.MW_DATA		( mw_data		),
	.MW_VALID		( mw_valid		),
	.MW_READY		( mw_ready		),
	.MW_LAST		( mw_last		)
);

// processor
dut_top_ex			dut_top_ex(
	// system
	.CLK			( CLK			),
	.nRST			( nRST			),
	.BUSY			( BUSY			),
	.INTR			( INTR			),
	// slave bus
	.SR_REQ			( sr_req		),
	.SR_VALID		( sr_valid		),
	.SR_ADDR		( sr_addr		),
	.SR_DATA		( sr_data		),
	.SW_REQ			( sw_req		),
	.SW_VALID		( sw_valid		),
	.SW_ADDR		( sw_addr		),
	.SW_DATA		( sw_data		),
	// master bus
	.MR_REQ			( mr_req		),
	.MR_GRANT		( mr_grant		),
	.MR_ADDR		( mr_addr		),
	.MR_SIZE		( mr_size		),
	.MR_DATA		( mr_data		),
	.MR_VALID		( mr_valid		),
	.MR_READY		( mr_ready		),
	.MR_LAST		( mr_last		),
	.MW_REQ			( mw_req		),
	.MW_GRANT		( mw_grant		),
	.MW_ADDR		( mw_addr		),
	.MW_SIZE		( mw_size		),
	.MW_DATA		( mw_data		),
	.MW_VALID		( mw_valid		),
	.MW_READY		( mw_ready		),
	.MW_LAST		( mw_last		)
);

endmodule
