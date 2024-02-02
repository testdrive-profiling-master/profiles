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
`include "testdrive_system.vh"

module processor_axi_wrapper #(
		parameter	C_CLKIN_PERIOD				= 5,	// default (200MHz input clock)
		// AXI-LITE master parameters
		parameter 	C_M_AXILITE_ADDR_WIDTH		= 20,
		// AXI master parameters
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

		//// AXI-LITE external master interface
		// write address
		output	[C_M_AXILITE_ADDR_WIDTH-1 : 0]	EX_AWADDR,		// Write address
		output									EX_AWVALID,		// Write address valid
		input									EX_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
		// write data
		output	[31 : 0]						EX_WDATA,		// Write data
		output	[3 : 0]							EX_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		output									EX_WVALID,		// Write valid
		input									EX_WREADY,		// Write ready
		// read address
		output	[C_M_AXILITE_ADDR_WIDTH-1 : 0]	EX_ARADDR,		// Read address
		output									EX_ARVALID,		// Read address valid
		input									EX_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		input	[31 : 0]						EX_RDATA,		// Read data.
		input	[1 : 0]							EX_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									EX_RVALID,		// Read valid (1 = read data available, 0 = read data not available)
		output									EX_RREADY,		// Read ready (1= master ready 0 = master not ready)
		// bus
		input	[1 : 0]							EX_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									EX_BVALID,		// Write response valid
		output									EX_BREADY,		// Response ready

		//// AXI master #0 interface
		// write address
		output	[C_M_AXI_ID_WIDTH-1:0]			M0_AWID,		// The ID tag for the write address group of signals
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M0_AWADDR,		// Write address
		output	[7:0]							M0_AWLEN,		// Burst_Length = AxLEN[7:0] + 1
		output	[2:0]							M0_AWSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
		output	[1:0]							M0_AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
		output									M0_AWLOCK,		// b0(Normal), b1(Exclusive)
		output	[3:0]							M0_AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
		output	[2:0]							M0_AWPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
		output	[3:0]							M0_AWREGION,	//
		output	[3:0]							M0_AWQOS,		//
		output									M0_AWVALID,		// Write address valid
		input									M0_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
		// write data
		output	[C_M_AXI_ID_WIDTH-1:0]			M0_WID,			//
		output	[C_M_AXI_DATA_WIDTH-1:0]		M0_WDATA,		// Write data
		output	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M0_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		output									M0_WLAST,		// Write last
		output									M0_WVALID,		// Write valid
		input									M0_WREADY,		// Write ready
		// bus interaction
		input	[C_M_AXI_ID_WIDTH-1:0]			M0_BID,			// The ID tag for the write response
		input	[1:0]							M0_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M0_BVALID,		// Write response valid
		output									M0_BREADY,		// Response ready
		// read address
		output	[C_M_AXI_ID_WIDTH-1:0]			M0_ARID,		// Read address ID tag
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M0_ARADDR,		// Read address
		output	[7:0]							M0_ARLEN,		// Burst_Length = AxLEN[7:0] + 1
		output	[2:0]							M0_ARSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
		output	[1:0]							M0_ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
		output									M0_ARLOCK,		// b0(Normal), b1(Exclusive)
		output	[3:0]							M0_ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
		output	[2:0]							M0_ARPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
		output	[3:0]							M0_ARREGION,	//
		output	[3:0]							M0_ARQOS,		//
		output									M0_ARVALID,		// Read address valid
		input									M0_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		input	[C_M_AXI_ID_WIDTH-1:0]			M0_RID,			// Read ID tag.
		input	[C_M_AXI_DATA_WIDTH-1:0]		M0_RDATA,		// Read data.
		input	[1:0]							M0_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M0_RLAST,		// Read last. This signal indicates the last transfer in a read burst.
		input									M0_RVALID,		// Read valid (1 = read data available, 0 = read data not available)
		output									M0_RREADY,		// Read ready (1= master ready 0 = master not ready)

		//// AXI master #1 interface
		// write address
		output	[C_M_AXI_ID_WIDTH-1:0]			M1_AWID,		// The ID tag for the write address group of signals
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M1_AWADDR,		// Write address
		output	[7:0]							M1_AWLEN,		// Burst_Length = AxLEN[7:0] + 1
		output	[2:0]							M1_AWSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
		output	[1:0]							M1_AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
		output									M1_AWLOCK,		// b0(Normal), b1(Exclusive)
		output	[3:0]							M1_AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
		output	[2:0]							M1_AWPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
		output	[3:0]							M1_AWREGION,	//
		output	[3:0]							M1_AWQOS,		//
		output									M1_AWVALID,		// Write address valid
		input									M1_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
		// write data
		output	[C_M_AXI_ID_WIDTH-1:0]			M1_WID,			//
		output	[C_M_AXI_DATA_WIDTH-1:0]		M1_WDATA,		// Write data
		output	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M1_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		output									M1_WLAST,		// Write last
		output									M1_WVALID,		// Write valid
		input									M1_WREADY,		// Write ready
		// bus interaction
		input	[C_M_AXI_ID_WIDTH-1:0]			M1_BID,			// The ID tag for the write response
		input	[1:0]							M1_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M1_BVALID,		// Write response valid
		output									M1_BREADY,		// Response ready
		// read address
		output	[C_M_AXI_ID_WIDTH-1:0]			M1_ARID,		// Read address ID tag
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M1_ARADDR,		// Read address
		output	[7:0]							M1_ARLEN,		// Burst_Length = AxLEN[7:0] + 1
		output	[2:0]							M1_ARSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
		output	[1:0]							M1_ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
		output									M1_ARLOCK,		// b0(Normal), b1(Exclusive)
		output	[3:0]							M1_ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
		output	[2:0]							M1_ARPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
		output	[3:0]							M1_ARREGION,	//
		output	[3:0]							M1_ARQOS,		//
		output									M1_ARVALID,		// Read address valid
		input									M1_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		input	[C_M_AXI_ID_WIDTH-1:0]			M1_RID,			// Read ID tag.
		input	[C_M_AXI_DATA_WIDTH-1:0]		M1_RDATA,		// Read data.
		input	[1:0]							M1_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M1_RLAST,		// Read last. This signal indicates the last transfer in a read burst.
		input									M1_RVALID,		// Read valid (1 = read data available, 0 = read data not available)
		output									M1_RREADY		// Read ready (1= master ready 0 = master not ready)
	);

	/*verilator tracing_off*/
	localparam	S_ADDR_WIDTH					= 4;			// 1 ~ 5(max)

	// definition & assignment ---------------------------------------------------
	// internal master for slave interface
	wire										ms_rreq,	ms_wreq;			// request
	wire										ms_rgrant,	ms_wgrant;			// grant
	wire	[C_M_AXILITE_ADDR_WIDTH-1:0]		ms_raddr,	ms_waddr;			// address
	wire	[31:0]								ms_rdata,	ms_wdata;			// data
	wire										ms_rvalid,	ms_wvalid;			// valid
	wire										ms_rready,	ms_wready;			// ready

	// internal master #0 interface
	wire										m0_rreq,	m0_wreq;			// request
	wire										m0_rgrant,	m0_wgrant;			// grant
	wire	[C_M_AXI_ADDR_WIDTH-1:0]			m0_raddr,	m0_waddr;			// address
	wire	[7:0]								m0_rsize,	m0_wsize;			// transfer size
	wire	[C_M_AXI_DATA_WIDTH-1:0]			m0_rdata,	m0_wdata;			// data
	wire										m0_rvalid,	m0_wvalid;			// valid
	wire										m0_rready,	m0_wready;			// ready
	wire										m0_rlast,	m0_wlast;			// last

	// internal master #1 interface
	wire										m1_rreq,	m1_wreq;			// request
	wire										m1_rgrant,	m1_wgrant;			// grant
	wire	[C_M_AXI_ADDR_WIDTH-1:0]			m1_raddr,	m1_waddr;			// address
	wire	[7:0]								m1_rsize,	m1_wsize;			// transfer size
	wire	[C_M_AXI_DATA_WIDTH-1:0]			m1_rdata,	m1_wdata;			// data
	wire										m1_rvalid,	m1_wvalid;			// valid
	wire										m1_rready,	m1_wready;			// ready
	wire										m1_rlast,	m1_wlast;			// last

	// async. DUT master lite for slave output interface
	wire										p_ms_rreq,		p_ms_wreq;		// request
	wire										p_ms_rgrant,	p_ms_wgrant;	// grant
	wire	[C_M_AXILITE_ADDR_WIDTH-1:0]		p_ms_raddr,		p_ms_waddr;		// address
	wire	[31:0]								p_ms_rdata,		p_ms_wdata;		// data
	wire										p_ms_rvalid,	p_ms_wvalid;	// valid
	wire										p_ms_rready,	p_ms_wready;	// ready

	// async. DUT master #0 interface
	wire										p_m0_rreq,		p_m0_wreq;		// request
	wire										p_m0_rgrant,	p_m0_wgrant;	// grant
	wire	[C_M_AXI_ADDR_WIDTH-1:0]			p_m0_raddr,		p_m0_waddr;		// address
	wire	[7:0]								p_m0_rsize,		p_m0_wsize;		// transfer size
	wire	[C_M_AXI_DATA_WIDTH-1:0]			p_m0_rdata,		p_m0_wdata;		// data
	wire										p_m0_rvalid,	p_m0_wvalid;	// valid
	wire										p_m0_rready,	p_m0_wready;	// ready
	wire										p_m0_rlast,		p_m0_wlast;		// last

	// async. DUT master #1 interface
	wire										p_m1_rreq,		p_m1_wreq;		// request
	wire										p_m1_rgrant,	p_m1_wgrant;	// grant
	wire	[C_M_AXI_ADDR_WIDTH-1:0]			p_m1_raddr,		p_m1_waddr;		// address
	wire	[7:0]								p_m1_rsize,		p_m1_wsize;		// transfer size
	wire	[C_M_AXI_DATA_WIDTH-1:0]			p_m1_rdata,		p_m1_wdata;		// data
	wire										p_m1_rvalid,	p_m1_wvalid;	// valid
	wire										p_m1_rready,	p_m1_wready;	// ready
	wire										p_m1_rlast,		p_m1_wlast;		// last

	// processor
	wire										p_CLK, p_nRST;				// processing clock & reset(active low)
	wire										busy;						// busy
	// processor slave bus
	wire										p_S_EN, p_S_WE;				// slave enable / write enable
	wire	[S_ADDR_WIDTH-1:0]					p_S_ADDR;					// slave address
	wire	[31:0]								p_S_RDATA, p_S_WDATA;		// slave r/w data
	wire										p_S_READY;					// slave ready

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
	axi_master_lite_wrapper #(
		.C_M_AXI_ADDR_WIDTH			(C_M_AXILITE_ADDR_WIDTH)
	) slave_master_wrapper (
		//// AXI-LITE master interface
		.M_CLK			(CLK),
		.M_nRST			(nRST),
		.M_AWADDR		(EX_AWADDR),
		.M_AWVALID		(EX_AWVALID),
		.M_AWREADY		(EX_AWREADY),
		.M_WDATA		(EX_WDATA),
		.M_WSTRB		(EX_WSTRB),
		.M_WVALID		(EX_WVALID),
		.M_WREADY		(EX_WREADY),
		.M_BRESP		(EX_BRESP),
		.M_BVALID		(EX_BVALID),
		.M_BREADY		(EX_BREADY),
		.M_ARADDR		(EX_ARADDR),
		.M_ARVALID		(EX_ARVALID),
		.M_ARREADY		(EX_ARREADY),
		.M_RDATA		(EX_RDATA),
		.M_RRESP		(EX_RRESP),
		.M_RVALID		(EX_RVALID),
		.M_RREADY		(EX_RREADY),
		//// internal master interface
		.MR_REQ			(ms_rreq),
		.MR_GRANT		(ms_rgrant),
		.MR_ADDR		(ms_raddr),
		.MR_DATA		(ms_rdata),
		.MR_VALID		(ms_rvalid),
		.MR_READY		(ms_rready),
		.MW_REQ			(ms_wreq),
		.MW_GRANT		(ms_wgrant),
		.MW_ADDR		(ms_waddr),
		.MW_DATA		(ms_wdata),
		.MW_VALID		(ms_wvalid),
		.MW_READY		(ms_wready)
	);

	// master bus wrapper #0
	axi_master_wrapper #(
		.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
		.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
		.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
	) master_wrapper_0 (
		// AXI master interface
		.M_CLK			(CLK),
		.M_nRST			(nRST),
		.M_AWID			(M0_AWID),
		.M_AWADDR		(M0_AWADDR),
		.M_AWLEN		(M0_AWLEN),
		.M_AWSIZE		(M0_AWSIZE),
		.M_AWBURST		(M0_AWBURST),
		.M_AWLOCK		(M0_AWLOCK),
		.M_AWCACHE		(M0_AWCACHE),
		.M_AWPROT		(M0_AWPROT),
		.M_AWREGION		(M0_AWREGION),
		.M_AWQOS		(M0_AWQOS),
		.M_AWVALID		(M0_AWVALID),
		.M_AWREADY		(M0_AWREADY),
		.M_WID			(M0_WID),
		.M_WDATA		(M0_WDATA),
		.M_WSTRB		(M0_WSTRB),
		.M_WLAST		(M0_WLAST),
		.M_WVALID		(M0_WVALID),
		.M_WREADY		(M0_WREADY),
		.M_BID			(M0_BID),
		.M_BRESP		(M0_BRESP),
		.M_BVALID		(M0_BVALID),
		.M_BREADY		(M0_BREADY),
		.M_ARID			(M0_ARID),
		.M_ARADDR		(M0_ARADDR),
		.M_ARLEN		(M0_ARLEN),
		.M_ARSIZE		(M0_ARSIZE),
		.M_ARBURST		(M0_ARBURST),
		.M_ARLOCK		(M0_ARLOCK),
		.M_ARCACHE		(M0_ARCACHE),
		.M_ARPROT		(M0_ARPROT),
		.M_ARREGION		(M0_ARREGION),
		.M_ARQOS		(M0_ARQOS),
		.M_ARVALID		(M0_ARVALID),
		.M_ARREADY		(M0_ARREADY),
		.M_RID			(M0_RID),
		.M_RDATA		(M0_RDATA),
		.M_RRESP		(M0_RRESP),
		.M_RLAST		(M0_RLAST),
		.M_RVALID		(M0_RVALID),
		.M_RREADY		(M0_RREADY),
		// internal master interface
		.MR_REQ			(m0_rreq),
		.MR_GRANT		(m0_rgrant),
		.MR_ADDR		(m0_raddr),
		.MR_SIZE		(m0_rsize),
		.MR_DATA		(m0_rdata),
		.MR_VALID		(m0_rvalid),
		.MR_LAST		(m0_rlast),
		.MR_READY		(m0_rready),
		.MW_REQ			(m0_wreq),
		.MW_GRANT		(m0_wgrant),
		.MW_ADDR		(m0_waddr),
		.MW_SIZE		(m0_wsize),
		.MW_DATA		(m0_wdata),
		.MW_VALID		(m0_wvalid),
		.MW_READY		(m0_wready),
		.MW_LAST		(m0_wlast)
	);

	// master bus wrapper #0
	axi_master_wrapper #(
		.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
		.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
		.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
	) master_wrapper_1 (
		// AXI master interface
		.M_CLK			(CLK),
		.M_nRST			(nRST),
		.M_AWID			(M1_AWID),
		.M_AWADDR		(M1_AWADDR),
		.M_AWLEN		(M1_AWLEN),
		.M_AWSIZE		(M1_AWSIZE),
		.M_AWBURST		(M1_AWBURST),
		.M_AWLOCK		(M1_AWLOCK),
		.M_AWCACHE		(M1_AWCACHE),
		.M_AWPROT		(M1_AWPROT),
		.M_AWREGION		(M1_AWREGION),
		.M_AWQOS		(M1_AWQOS),
		.M_AWVALID		(M1_AWVALID),
		.M_AWREADY		(M1_AWREADY),
		.M_WID			(M1_WID),
		.M_WDATA		(M1_WDATA),
		.M_WSTRB		(M1_WSTRB),
		.M_WLAST		(M1_WLAST),
		.M_WVALID		(M1_WVALID),
		.M_WREADY		(M1_WREADY),
		.M_BID			(M1_BID),
		.M_BRESP		(M1_BRESP),
		.M_BVALID		(M1_BVALID),
		.M_BREADY		(M1_BREADY),
		.M_ARID			(M1_ARID),
		.M_ARADDR		(M1_ARADDR),
		.M_ARLEN		(M1_ARLEN),
		.M_ARSIZE		(M1_ARSIZE),
		.M_ARBURST		(M1_ARBURST),
		.M_ARLOCK		(M1_ARLOCK),
		.M_ARCACHE		(M1_ARCACHE),
		.M_ARPROT		(M1_ARPROT),
		.M_ARREGION		(M1_ARREGION),
		.M_ARQOS		(M1_ARQOS),
		.M_ARVALID		(M1_ARVALID),
		.M_ARREADY		(M1_ARREADY),
		.M_RID			(M1_RID),
		.M_RDATA		(M1_RDATA),
		.M_RRESP		(M1_RRESP),
		.M_RLAST		(M1_RLAST),
		.M_RVALID		(M1_RVALID),
		.M_RREADY		(M1_RREADY),
		// internal master interface
		.MR_REQ			(m1_rreq),
		.MR_GRANT		(m1_rgrant),
		.MR_ADDR		(m1_raddr),
		.MR_SIZE		(m1_rsize),
		.MR_DATA		(m1_rdata),
		.MR_VALID		(m1_rvalid),
		.MR_LAST		(m1_rlast),
		.MR_READY		(m1_rready),
		.MW_REQ			(m1_wreq),
		.MW_GRANT		(m1_wgrant),
		.MW_ADDR		(m1_waddr),
		.MW_SIZE		(m1_wsize),
		.MW_DATA		(m1_wdata),
		.MW_VALID		(m1_wvalid),
		.MW_READY		(m1_wready),
		.MW_LAST		(m1_wlast)
	);

	processor_clkgen_slave #(
		.C_CLKIN_PERIOD	(C_CLKIN_PERIOD)
	) clk_gen_slave (
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
		.P_EN			(p_S_EN),
		.P_WE			(p_S_WE),
		.P_ADDR			(p_S_ADDR),
		.P_WDATA		(p_S_WDATA),
		.P_RDATA		(p_S_RDATA),
		.P_READY		(p_S_READY)
	);

	processor_async_master_lite #(
		.ADDR_WIDTH		(C_M_AXILITE_ADDR_WIDTH),
		.FIFO_DEPTH		(2)
	) async_master_lite (
		.CLK			(CLK),
		.nRST			(nRST),
		.MR_REQ			(ms_rreq),
		.MR_GRANT		(ms_rgrant),
		.MR_ADDR		(ms_raddr),
		.MR_DATA		(ms_rdata),
		.MR_VALID		(ms_rvalid),
		.MR_READY		(ms_rready),
		.MW_REQ			(ms_wreq),
		.MW_GRANT		(ms_wgrant),
		.MW_ADDR		(ms_waddr),
		.MW_DATA		(ms_wdata),
		.MW_VALID		(ms_wvalid),
		.MW_READY		(ms_wready),
		.P_CLK			(p_CLK),
		.P_nRST			(p_nRST),
		.P_MR_REQ		(p_ms_rreq),
		.P_MR_ADDR		(p_ms_raddr),
		.P_MR_GRANT		(p_ms_rgrant),
		.P_MR_DATA		(p_ms_rdata),
		.P_MR_VALID		(p_ms_rvalid),
		.P_MR_READY		(p_ms_rready),
		.P_MW_REQ		(p_ms_wreq),
		.P_MW_ADDR		(p_ms_waddr),
		.P_MW_GRANT		(p_ms_wgrant),
		.P_MW_DATA		(p_ms_wdata),
		.P_MW_VALID		(p_ms_wvalid),
		.P_MW_READY		(p_ms_wready)
	);

	processor_async_master #(
		.ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
		.DATA_WIDTH		(C_M_AXI_DATA_WIDTH),
		.USE_AXI4		(1)
	) async_master_0 (
		.CLK			(CLK),
		.nRST			(nRST),
		.MR_REQ			(m0_rreq),
		.MR_GRANT		(m0_rgrant),
		.MR_ADDR		(m0_raddr),
		.MR_SIZE		(m0_rsize),
		.MR_DATA		(m0_rdata),
		.MR_VALID		(m0_rvalid),
		.MR_READY		(m0_rready),
		.MR_LAST		(m0_rlast),
		.MW_REQ			(m0_wreq),
		.MW_GRANT		(m0_wgrant),
		.MW_ADDR		(m0_waddr),
		.MW_SIZE		(m0_wsize),
		.MW_DATA		(m0_wdata),
		.MW_VALID		(m0_wvalid),
		.MW_READY		(m0_wready),
		.MW_LAST		(m0_wlast),
		.P_CLK			(p_CLK),
		.P_nRST			(p_nRST),
		.P_MR_REQ		(p_m0_rreq),
		.P_MR_ADDR		(p_m0_raddr),
		.P_MR_SIZE		(p_m0_rsize),
		.P_MR_GRANT		(p_m0_rgrant),
		.P_MR_DATA		(p_m0_rdata),
		.P_MR_VALID		(p_m0_rvalid),
		.P_MR_READY		(p_m0_rready),
		.P_MR_LAST		(p_m0_rlast),
		.P_MW_REQ		(p_m0_wreq),
		.P_MW_ADDR		(p_m0_waddr),
		.P_MW_SIZE		(p_m0_wsize),
		.P_MW_GRANT		(p_m0_wgrant),
		.P_MW_DATA		(p_m0_wdata),
		.P_MW_VALID		(p_m0_wvalid),
		.P_MW_READY		(p_m0_wready),
		.P_MW_LAST		(p_m0_wlast)
	);

	processor_async_master #(
		.ADDR_WIDTH		(C_M_AXI_ADDR_WIDTH),
		.DATA_WIDTH		(C_M_AXI_DATA_WIDTH),
		.USE_AXI4		(1)
	) async_master_1 (
		.CLK			(CLK),
		.nRST			(nRST),
		.MR_REQ			(m1_rreq),
		.MR_GRANT		(m1_rgrant),
		.MR_ADDR		(m1_raddr),
		.MR_SIZE		(m1_rsize),
		.MR_DATA		(m1_rdata),
		.MR_VALID		(m1_rvalid),
		.MR_READY		(m1_rready),
		.MR_LAST		(m1_rlast),
		.MW_REQ			(m1_wreq),
		.MW_GRANT		(m1_wgrant),
		.MW_ADDR		(m1_waddr),
		.MW_SIZE		(m1_wsize),
		.MW_DATA		(m1_wdata),
		.MW_VALID		(m1_wvalid),
		.MW_READY		(m1_wready),
		.MW_LAST		(m1_wlast),
		.P_CLK			(p_CLK),
		.P_nRST			(p_nRST),
		.P_MR_REQ		(p_m1_rreq),
		.P_MR_ADDR		(p_m1_raddr),
		.P_MR_SIZE		(p_m1_rsize),
		.P_MR_GRANT		(p_m1_rgrant),
		.P_MR_DATA		(p_m1_rdata),
		.P_MR_VALID		(p_m1_rvalid),
		.P_MR_READY		(p_m1_rready),
		.P_MR_LAST		(p_m1_rlast),
		.P_MW_REQ		(p_m1_wreq),
		.P_MW_ADDR		(p_m1_waddr),
		.P_MW_SIZE		(p_m1_wsize),
		.P_MW_GRANT		(p_m1_wgrant),
		.P_MW_DATA		(p_m1_wdata),
		.P_MW_VALID		(p_m1_wvalid),
		.P_MW_READY		(p_m1_wready),
		.P_MW_LAST		(p_m1_wlast)
	);
	/*verilator tracing_on*/

	// processor top
	processor_top #(
		.S_ADDR_WIDTH	(S_ADDR_WIDTH),
		.M_ADDR_WIDTH	(C_M_AXI_ADDR_WIDTH),
		.M_DATA_WIDTH	(C_M_AXI_DATA_WIDTH),
		.MS_ADDR_WIDTH	(C_M_AXILITE_ADDR_WIDTH)
	) processor_inst (
		// system
		.CLK			(p_CLK),
		.nRST			(p_nRST),
		.BUSY			(busy),
		.INTR			(INTR),
		// slave bus
		.S_EN			(p_S_EN),
		.S_WE			(p_S_WE),
		.S_ADDR			(p_S_ADDR),
		.S_WDATA		(p_S_WDATA),
		.S_RDATA		(p_S_RDATA),
		.S_READY		(p_S_READY),
		//// master for slave output interface
		.MS_RREQ		(p_ms_rreq),
		.MS_RADDR		(p_ms_raddr),
		.MS_RGRANT		(p_ms_rgrant),
		.MS_RDATA		(p_ms_rdata),
		.MS_RVALID		(p_ms_rvalid),
		.MS_RREADY		(p_ms_rready),
		.MS_WREQ		(p_ms_wreq),
		.MS_WADDR		(p_ms_waddr),
		.MS_WGRANT		(p_ms_wgrant),
		.MS_WDATA		(p_ms_wdata),
		.MS_WVALID		(p_ms_wvalid),
		.MS_WREADY		(p_ms_wready),
		// internal master #0 interface
		.M0_RREQ		(p_m0_rreq),
		.M0_RGRANT		(p_m0_rgrant),
		.M0_RADDR		(p_m0_raddr),
		.M0_RSIZE		(p_m0_rsize),
		.M0_RDATA		(p_m0_rdata),
		.M0_RVALID		(p_m0_rvalid),
		.M0_RREADY		(p_m0_rready),
		.M0_RLAST		(p_m0_rlast),
		.M0_WREQ		(p_m0_wreq),
		.M0_WGRANT		(p_m0_wgrant),
		.M0_WADDR		(p_m0_waddr),
		.M0_WSIZE		(p_m0_wsize),
		.M0_WDATA		(p_m0_wdata),
		.M0_WVALID		(p_m0_wvalid),
		.M0_WREADY		(p_m0_wready),
		.M0_WLAST		(p_m0_wlast),
		// internal master #1 interface
		.M1_RREQ		(p_m1_rreq),
		.M1_RGRANT		(p_m1_rgrant),
		.M1_RADDR		(p_m1_raddr),
		.M1_RSIZE		(p_m1_rsize),
		.M1_RDATA		(p_m1_rdata),
		.M1_RVALID		(p_m1_rvalid),
		.M1_RREADY		(p_m1_rready),
		.M1_RLAST		(p_m1_rlast),
		.M1_WREQ		(p_m1_wreq),
		.M1_WGRANT		(p_m1_wgrant),
		.M1_WADDR		(p_m1_waddr),
		.M1_WSIZE		(p_m1_wsize),
		.M1_WDATA		(p_m1_wdata),
		.M1_WVALID		(p_m1_wvalid),
		.M1_WREADY		(p_m1_wready),
		.M1_WLAST		(p_m1_wlast)
	);

endmodule
