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
// Title : Template design
// Rev.  : 6/30/2021 Wed (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"
/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// The AHB Master BFM
//-----------------------------------------------------------------------------

module testdrive_ahb_master_bfm #(
	parameter string	C_BUS_TITLE			= "",
	parameter integer	C_THREAD_ID_WIDTH	= 1,
	parameter integer	C_ADDR_WIDTH		= 32,
	parameter integer	C_DATA_WIDTH		= 32,
	parameter integer	C_DEBUG_LEVEL		= 0
) (
	input									CLK,			// clock
	input									nRST,			// reset (active low)
	// write address
	input	[C_THREAD_ID_WIDTH-1:0]			AWID,			// The ID tag for the write address group of signals
	input	[C_ADDR_WIDTH-1:0]				AWADDR,			// Write address
	input	[(C_USE_AXI4==0 ? 4:8)-1:0]		AWLEN,			// Burst_Length = AxLEN + 1
	input	[2:0]							AWSIZE,			// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	input	[1:0]							AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	input	[1:0]							AWLOCK,			// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved)
	input	[3:0]							AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	input	[2:0]							AWPROT,			// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	input	[3:0]							AWREGION,		//
	input	[3:0]							AWQOS,			//
	input									AWVALID,		// Write address valid
	output reg								AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
	// write data
	input [C_THREAD_ID_WIDTH-1:0]			WID,			//
	input [C_DATA_WIDTH-1:0]				WDATA,			// Write data
	input [(C_DATA_WIDTH/8)-1:0]			WSTRB,			// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
	input									WLAST,			// Write last
	input									WVALID,			// Write valid
	output reg								WREADY,			// Write ready
	// bus
	output reg	[C_THREAD_ID_WIDTH-1:0]		BID,			// The ID tag for the write response
	output reg	[1:0]						BRESP,			// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	output reg								BVALID,			// Write response valid
	input									BREADY,			// Response ready
	// read address
	input	[C_THREAD_ID_WIDTH-1:0]			ARID,			// Read address ID tag
	input	[C_ADDR_WIDTH-1:0]				ARADDR,			// Read address
	input	[(C_USE_AXI4==0 ? 4:8)-1:0]		ARLEN,			// Burst_Length = AxLEN + 1
	input	[2:0]							ARSIZE,			// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	input	[1:0]							ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	input	[1:0]							ARLOCK,			// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved)
	input	[3:0]							ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	input	[2:0]							ARPROT,			// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output reg	[3:0]						ARREGION,		//
	output reg	[3:0]						ARQOS,			//
	input									ARVALID,		// Read address valid
	output reg								ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
	// read data
	output reg	[C_THREAD_ID_WIDTH-1:0]		RID,			// Read ID tag.
	output reg	[C_DATA_WIDTH-1:0]			RDATA,			// Read data.
	output reg	[1:0]						RRESP,			// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	output reg								RLAST,			// Read last. This signal indicates the last transfer in a read burst.
	output reg								RVALID,			// Read valid (1 = read data available, 0 = read data not available)
	input									RREADY			// Read ready (1= master ready 0 = master not ready)
);

// definition & assignment ---------------------------------------------------
// object
int maxi;
// initialize
`DPI_FUNCTION int CreateMAXI (
	input	string							sTitle,
	input	int								iDataWidth,
	input	int								bUseAXI4,
	input	int								iDebugLevel
);

initial begin
	maxi	= CreateMAXI(C_BUS_TITLE, C_DATA_WIDTH, C_USE_AXI4, C_DEBUG_LEVEL);
end

// write interface
`DPI_FUNCTION void MAXIW_Interface (
	input	int								MAXI,
	input	bit								nRST,
	// write address
	input	int								AWID,			// The ID tag for the write address group of signals
	input	bit [C_ADDR_WIDTH-1:0]			AWADDR,			// Write address
	input	bit [(C_USE_AXI4==0 ? 4:8)-1:0]	AWLEN,			// Burst_Length = AxLEN + 1
	input	bit [2:0]						AWSIZE,			// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	input	bit [1:0]						AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	input	bit [1:0]						AWLOCK,			// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved) : Locked/Reserved is removed in AXI4
	input	bit [3:0]						AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	input	bit [2:0]						AWPROT,			// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	input	bit [3:0]						AWREGION,		//
	input	bit [3:0]						AWQOS,			//
	input	bit								AWVALID,		// Write address valid
	output	bit								AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
	// write data
	input	int								WID,			// : this is removed in AXI4
	input	bit [C_DATA_WIDTH-1:0]			WDATA,			// Write data
	input	bit [(C_DATA_WIDTH/8)-1:0]		WSTRB,			// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
	input	bit								WLAST,			// Write last
	input	bit								WVALID,			// Write valid
	output	bit								WREADY,			// Write ready
	// bus
	output	int								BID,			// The ID tag for the write response
	output	bit [1:0]						BRESP,			// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	output	bit								BVALID,			// Write response valid
	input	bit								BREADY			// Response ready
);

// register pipes
reg								r_AWREADY;
reg								r_WREADY;
reg	[1:0]						r_BRESP;
reg								r_BVALID;

// read interface
`DPI_FUNCTION void MAXIR_Interface (
	input	int								MAXI,
	input	bit								nRST,
	// read address
	input	int								ARID,			// Read address ID tag
	input	bit [C_ADDR_WIDTH-1:0]			ARADDR,			// Read address
	input	bit [(C_USE_AXI4==0 ? 4:8)-1:0]	ARLEN,			// Burst_Length = AxLEN + 1
	input	bit [2:0]						ARSIZE,			// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	input	bit [1:0]						ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	input	bit [1:0]						ARLOCK,			// b00(Normal), b01(Exclusive), b10(Locked), b11(Reserved)
	input	bit [3:0]						ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	input	bit [2:0]						ARPROT,			// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	bit	[3:0]						ARREGION,		//
	output	bit	[3:0]						ARQOS,			//
	input	bit								ARVALID,		// Read address valid
	output	bit								ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
	// read data
	output	int								RID,			// Read ID tag.
	output	bit [C_DATA_WIDTH-1:0]			RDATA,			// Read data.
	output	bit [1:0]						RRESP,			// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	output	bit								RLAST,			// Read last. This signal indicates the last transfer in a read burst.
	output	bit								RVALID,			// Read valid (1 = read data available, 0 = read data not available)
	input	bit								RREADY			// Read ready (1= master ready 0 = master not ready)
);

// register pipes
reg	[3:0]						r_ARREGION;
reg	[3:0]						r_ARQOS;
reg								r_ARREADY;
reg [C_DATA_WIDTH-1:0]			r_RDATA;
reg [1:0]						r_RRESP;
reg								r_RLAST;
reg								r_RVALID;

integer		iAWID, iARID, iWID, iBID, iRID;
// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	// write interaction
	iAWID	/* verilator lint_off WIDTH */ = AWID;
	iWID	/* verilator lint_off WIDTH */ = WID;
	MAXIW_Interface(
		maxi, nRST,
		iAWID, AWADDR, AWLEN, AWSIZE, AWBURST, AWLOCK, AWCACHE, AWPROT,
		AWREGION, AWQOS, AWVALID, r_AWREADY, iWID,
		WDATA, WSTRB, WLAST, WVALID, r_WREADY,
		iBID, r_BRESP, r_BVALID, BREADY
	);
	AWREADY		<= r_AWREADY;
	WREADY		<= r_WREADY;
	BID			<= iBID;
	BRESP		<= r_BRESP;
	BVALID		<= r_BVALID;

	iARID	/* verilator lint_off WIDTH */ = ARID;
	// read interaction
	MAXIR_Interface(
		maxi, nRST,
		iARID, ARADDR, ARLEN, ARSIZE, ARBURST, ARLOCK, ARCACHE,
		ARPROT, r_ARREGION, r_ARQOS, ARVALID, r_ARREADY,
		iRID, r_RDATA, r_RRESP, r_RLAST, r_RVALID, RREADY
	);
	ARREGION	<= r_ARREGION;
	ARQOS		<= r_ARQOS;
	ARREADY		<= r_ARREADY;
	RID			<= iRID;
	RDATA		<= r_RDATA;
	RRESP		<= r_RRESP;
	RLAST		<= r_RLAST;
	RVALID		<= r_RVALID;
end

endmodule
/*verilator tracing_on*/
