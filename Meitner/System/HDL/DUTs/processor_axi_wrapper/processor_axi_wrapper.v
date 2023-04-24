//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 4/24/2023 Mon (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`include "testdrive_system.vh"

module processor_axi_wrapper #(
	// master parameters
	parameter	C_M_AXI_ID_WIDTH			= 2,
	parameter	C_M_AXI_ADDR_WIDTH			= 32,
	parameter	C_M_AXI_DATA_WIDTH			= 512
) (
	//// system
	input									CLK,
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

	//// AXI master interface --------------
	// write address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_AWID,			// The ID tag for the write address group of signals
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR,		// Write address
	output	[7:0]							M_AWLEN,		// Burst_Length = AxLEN[7:0] + 1
	output	[2:0]							M_AWSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]							M_AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output									M_AWLOCK,		// b0(Normal), b1(Exclusive)
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
	output									M_ARLOCK,		// b0(Normal), b1(Exclusive)
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

/*verilator tracing_off*/
localparam	S_ADDR_WIDTH					= 4;			// 1 ~ 5(max)

// definition & assignment ---------------------------------------------------
// internal master interface
wire										mr_req,		mw_req;		// request
wire										mr_grant,	mw_grant;	// grant
wire	[31:0]								mr_addr,	mw_addr;	// address
wire	[7:0]								mr_size,	mw_size;	// transfer size
wire	[C_M_AXI_DATA_WIDTH-1:0]			mr_data,	mw_data;	// data
wire										mr_valid,	mw_valid;	// valid
wire										mr_ready,	mw_ready;	// ready
wire										mr_last,	mw_last;	// last

// async. DUT master interface
wire										p_mr_req,	p_mw_req;	// request
wire										p_mr_grant,	p_mw_grant;	// grant
wire	[31:0]								p_mr_addr,	p_mw_addr;	// address
wire	[7:0]								p_mr_size,	p_mw_size;	// transfer size
wire	[C_M_AXI_DATA_WIDTH-1:0]			p_mr_data,	p_mw_data;	// data
wire										p_mr_valid,	p_mw_valid;	// valid
wire										p_mr_ready,	p_mw_ready;	// ready
wire										p_mr_last,	p_mw_last;	// last

// processor
wire										p_CLK, p_nRST;			// processing clock & reset(active low)
wire										busy;					// busy
wire										p_EN, p_WE;				// slave enable / write enable
wire	[S_ADDR_WIDTH-1:0]					p_ADDR;					// slave address
wire	[31:0]								p_RDATA, p_WDATA;		// slave r/w data
wire										p_READY;				// slave ready

// clock generator
wire	clkgen_en	= S_PADDR[8];

// async slave
wire										S_PREADY_async;
wire	[31:0]								S_PRDATA_clkgen, S_PRDATA_async;

// APB slave interface
assign	S_PRDATA	= clkgen_en ? S_PRDATA_clkgen : S_PRDATA_async;
assign	S_PREADY	= clkgen_en | S_PREADY_async;		// clock_gen is none blocking
assign	S_PSLVERR	= 1'b0;

// implementation ------------------------------------------------------------
// master bus
axi_master_wrapper#(
	.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
	.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
	.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
) master_inst(
	// AXI master interface
	.M_CLK			(CLK),
	.M_nRST			(nRST),
	.M_AWID			(M_AWID),
	.M_AWADDR		(M_AWADDR),
	.M_AWLEN		(M_AWLEN),
	.M_AWSIZE		(M_AWSIZE),
	.M_AWBURST		(M_AWBURST),
	.M_AWLOCK		(M_AWLOCK),
	.M_AWCACHE		(M_AWCACHE),
	.M_AWPROT		(M_AWPROT),
	.M_AWREGION		(M_AWREGION),
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
	.M_ARREGION		(M_ARREGION),
	.M_ARQOS		(M_ARQOS),
	.M_ARVALID		(M_ARVALID),
	.M_ARREADY		(M_ARREADY),
	.M_RID			(M_RID),
	.M_RDATA		(M_RDATA),
	.M_RRESP		(M_RRESP),
	.M_RLAST		(M_RLAST),
	.M_RVALID		(M_RVALID),
	.M_RREADY		(M_RREADY),
	// internal master interface
	.MR_REQ			(mr_req),
	.MR_GRANT		(mr_grant),
	.MR_ADDR		(mr_addr),
	.MR_SIZE		(mr_size),
	.MR_DATA		(mr_data),
	.MR_VALID		(mr_valid),
	.MR_LAST		(mr_last),
	.MR_READY		(mr_ready),
	.MW_REQ			(mw_req),
	.MW_GRANT		(mw_grant),
	.MW_ADDR		(mw_addr),
	.MW_SIZE		(mw_size),
	.MW_DATA		(mw_data),
	.MW_VALID		(mw_valid),
	.MW_READY		(mw_ready),
	.MW_LAST		(mw_last)
);

processor_clkgen_slave clk_gen_slave (
	.CLK			(CLK),
	.nRST			(nRST),
	.PCLK			(p_CLK),
	.nPRST			(p_nRST),
	.EN				(S_PSEL & S_PENABLE & clkgen_en),
	.WE				(S_PWRITE),
	.WDATA			(S_PWDATA),
	.RDATA			(S_PRDATA_clkgen),
	.BUSY			(busy)
);

processor_async_slave #(
	.ADDR_WIDTH		(S_ADDR_WIDTH)
) async_slave (
	.S_CLK			(CLK),
	.S_nRST			(nRST),
	.S_EN			(S_PSEL & S_PENABLE & (~clkgen_en)),
	.S_WE			(S_PWRITE),
	.S_ADDR			(S_PADDR[2+(S_ADDR_WIDTH-1):2]),
	.S_WDATA		(S_PWDATA),
	.S_RDATA		(S_PRDATA_async),
	.S_READY		(S_PREADY_async),
	.P_CLK			(p_CLK),
	.P_nRST			(p_nRST),
	.P_EN			(p_EN),
	.P_WE			(p_WE),
	.P_ADDR			(p_ADDR),
	.P_WDATA		(p_WDATA),
	.P_RDATA		(p_RDATA),
	.P_READY		(p_READY)
);

processor_async_master #(
	.DATA_WIDTH		(C_M_AXI_DATA_WIDTH),
	.USE_AXI4		(1)
) async_master (
	.CLK			(CLK),
	.nRST			(nRST),
	.MR_REQ			(mr_req),
	.MR_GRANT		(mr_grant),
	.MR_ADDR		(mr_addr),
	.MR_SIZE		(mr_size),
	.MR_DATA		(mr_data),
	.MR_VALID		(mr_valid),
	.MR_READY		(mr_ready),
	.MR_LAST		(mr_last),
	.MW_REQ			(mw_req),
	.MW_GRANT		(mw_grant),
	.MW_ADDR		(mw_addr),
	.MW_SIZE		(mw_size),
	.MW_DATA		(mw_data),
	.MW_VALID		(mw_valid),
	.MW_READY		(mw_ready),
	.MW_LAST		(mw_last),
	.P_CLK			(p_CLK),
	.P_nRST			(p_nRST),
	.P_MR_REQ		(p_mr_req),
	.P_MR_ADDR		(p_mr_addr),
	.P_MR_SIZE		(p_mr_size),
	.P_MR_GRANT		(p_mr_grant),
	.P_MR_DATA		(p_mr_data),
	.P_MR_VALID		(p_mr_valid),
	.P_MR_READY		(p_mr_ready),
	.P_MR_LAST		(p_mr_last),
	.P_MW_REQ		(p_mw_req),
	.P_MW_ADDR		(p_mw_addr),
	.P_MW_SIZE		(p_mw_size),
	.P_MW_GRANT		(p_mw_grant),
	.P_MW_DATA		(p_mw_data),
	.P_MW_VALID		(p_mw_valid),
	.P_MW_READY		(p_mw_ready),
	.P_MW_LAST		(p_mw_last)
);
/*verilator tracing_on*/

// processor top
processor_top #(
	.ADDR_WIDTH		(S_ADDR_WIDTH),
	.DATA_WIDTH		(C_M_AXI_DATA_WIDTH)
) processor_inst (
	// system
	.CLK			(p_CLK),
	.nRST			(p_nRST),
	.BUSY			(busy),
	.INTR			(INTR),
	// slave bus
	.P_EN			(p_EN),
	.P_WE			(p_WE),
	.P_ADDR			(p_ADDR),
	.P_WDATA		(p_WDATA),
	.P_RDATA		(p_RDATA),
	.P_READY		(p_READY),
	// internal master interface
	.MR_REQ			(p_mr_req),
	.MR_GRANT		(p_mr_grant),
	.MR_ADDR		(p_mr_addr),
	.MR_SIZE		(p_mr_size),
	.MR_DATA		(p_mr_data),
	.MR_VALID		(p_mr_valid),
	.MR_READY		(p_mr_ready),
	.MR_LAST		(p_mr_last),
	.MW_REQ			(p_mw_req),
	.MW_GRANT		(p_mw_grant),
	.MW_ADDR		(p_mw_addr),
	.MW_SIZE		(p_mw_size),
	.MW_DATA		(p_mw_data),
	.MW_VALID		(p_mw_valid),
	.MW_READY		(p_mw_ready),
	.MW_LAST		(p_mw_last)
);

endmodule
