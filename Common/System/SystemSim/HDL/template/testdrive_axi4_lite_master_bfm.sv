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
// Title : TestDrive template design
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"
/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// The AXI 4 LITE Master BFM
//-----------------------------------------------------------------------------

module testdrive_axi4_lite_master_bfm #(
		parameter string	C_BUS_TITLE			= "",
		parameter integer	C_ADDR_WIDTH		= 20,
		parameter integer	C_DEBUG_LEVEL		= 0
	) (
		input									CLK,			// clock
		input									nRST,			// reset (active low)
		// write address
		input	[C_ADDR_WIDTH-1:0]				AWADDR,			// Write address
		input									AWVALID,		// Write address valid
		output reg								AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
		// write data
		input	[31:0]							WDATA,			// Write data
		input	[3:0]							WSTRB,			// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		input									WVALID,			// Write valid
		output reg								WREADY,			// Write ready
		// bus
		output reg	[1:0]						BRESP,			// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		output reg								BVALID,			// Write response valid
		input									BREADY,			// Response ready
		// read address
		input	[C_ADDR_WIDTH-1:0]				ARADDR,			// Read address
		input									ARVALID,		// Read address valid
		output reg								ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		output reg	[31:0]						RDATA,			// Read data
		output reg	[1:0]						RRESP,			// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		output reg								RVALID,			// Read valid (1 = read data available, 0 = read data not available)
		input									RREADY			// Read ready (1= master ready 0 = master not ready)
	);

	// definition & assignment ---------------------------------------------------
	// object
	chandle maxi;
	// initialize
	`DPI_FUNCTION chandle CreateMAXI (
		input	string							sTitle,
		input	int								iDataWidth,
		input	int								bUseAXI4,
		input	int								bLite,
		input	int								iDebugLevel
	);

	initial begin
		maxi	= CreateMAXI(C_BUS_TITLE, 32, 1, 1, C_DEBUG_LEVEL);
	end

	// write interface
	`DPI_FUNCTION void MAXIW_LITE_Interface (
		input	chandle							hMAXI,			// handle
		input	bit								nRST,			// reset (active low)
		// write address
		input	longint unsigned				AWADDR,			// Write address
		input	bit								AWVALID,		// Write address valid
		output	bit								AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)
		// write data
		input	bit [31:0]						WDATA,			// Write data
		input	bit [3:0]						WSTRB,			// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		input	bit								WVALID,			// Write valid
		output	bit								WREADY,			// Write ready
		// bus
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
	`DPI_FUNCTION void MAXIR_LITE_Interface (
		input	chandle							hMAXI,			// handle
		input	bit								nRST,			// reset (active low)
		// read address
		input	longint unsigned				ARADDR,			// Read address
		input	bit								ARVALID,		// Read address valid
		output	bit								ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		output	bit [31:0]						RDATA,			// Read data.
		output	bit [1:0]						RRESP,			// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		output	bit								RVALID,			// Read valid (1 = read data available, 0 = read data not available)
		input	bit								RREADY			// Read ready (1= master ready 0 = master not ready)
	);

	// register pipes
	reg								r_ARREADY;
	reg [31:0]						r_RDATA;
	reg [1:0]						r_RRESP;
	reg								r_RVALID;

	// implementation ------------------------------------------------------------
	always@(posedge CLK) begin
		// write interaction
		MAXIW_LITE_Interface(
			maxi, nRST,
			{{(64-C_ADDR_WIDTH){1'b0}}, AWADDR}, AWVALID, r_AWREADY,
			WDATA, WSTRB, WVALID, r_WREADY,
			r_BRESP, r_BVALID, BREADY
		);
		AWREADY		<= r_AWREADY;
		WREADY		<= r_WREADY;
		BRESP		<= r_BRESP;
		BVALID		<= r_BVALID;

		// read interaction
		MAXIR_LITE_Interface(
			maxi, nRST,
			{{(64-C_ADDR_WIDTH){1'b0}}, ARADDR}, ARVALID, r_ARREADY,
			r_RDATA, r_RRESP, r_RVALID, RREADY
		);
		ARREADY		<= r_ARREADY;
		RDATA		<= r_RDATA;
		RRESP		<= r_RRESP;
		RVALID		<= r_RVALID;
	end

endmodule
/*verilator tracing_on*/
