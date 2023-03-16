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
// Title : TestDrive template design
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"
`timescale 1ns/1ns
/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// The AXI 4 Lite slave BFM
//-----------------------------------------------------------------------------

module testdrive_axi4_lite_bfm #(
	parameter string	C_BUS_TITLE		= "",
	parameter			C_DATA_WIDTH	= 32,
	parameter			C_BASE_ADDR		= 32'h0,
	parameter			C_ADDR_BITS		= 10
) (
	// system
	input									CLK,
	input									nRST,
	// write address
	output reg	[C_ADDR_BITS-1:0]			AWADDR,
	output reg								AWVALID,
	input									AWREADY,
	// write data
	output reg	[C_DATA_WIDTH-1:0]			WDATA,
	output reg	[(C_DATA_WIDTH/8)-1:0]		WSTRB,
	output reg								WVALID,
	input									WREADY,
	// bus interaction
	input	[1:0]							BRESP,
	input									BVALID,
	output reg								BREADY,
	// read address
	output reg	[C_ADDR_BITS-1:0]			ARADDR,
	output reg								ARVALID,
	input									ARREADY,
	// read data
	input	[C_DATA_WIDTH-1:0]				RDATA,
	input	[1:0]							RRESP,
	input									RVALID,
	output reg								RREADY
);

// Parameters ----------------------------------------------------------------

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void SAXIW_Interface (
	input	chandle							hSAXIL,
	input	bit								nRST,
	// write address
	output	bit [`RANGE_DWORD]				AWADDR,
	output	bit								AWVALID,
	input	bit								AWREADY,
	// write data
	output	bit [C_DATA_WIDTH-1:0]			WDATA,
	output	bit [(C_DATA_WIDTH/8)-1:0]		WSTRB,
	output	bit								WVALID,
	input	bit								WREADY,
	// bus interaction
	input	bit [1:0]						BRESP,
	input	bit								BVALID,
	output	bit								BREADY
);

// register pipes
reg	[`RANGE_DWORD]							r_AWADDR;
reg											r_AWVALID;
reg	[C_DATA_WIDTH-1:0]						r_WDATA;
reg	[(C_DATA_WIDTH/8)-1:0]					r_WSTRB;
reg											r_WVALID;

`DPI_FUNCTION void SAXIR_Interface (
	input	chandle							hSAXIL,
	input	bit								nRST,
	// read address
	output	bit [`RANGE_DWORD]				ARADDR,
	output	bit								ARVALID,
	input	bit								ARREADY,
	// read data
	input	bit [C_DATA_WIDTH-1:0]			RDATA,
	input	bit [1:0]						RRESP,
	input	bit								RVALID,
	output	bit								RREADY,
);

// register pipes
reg	[`RANGE_DWORD]							r_ARADDR;
reg											r_ARVALID;
reg											r_RREADY;
reg											r_BREADY;

// implementation ------------------------------------------------------------
// object
chandle saxil;
// initialize
`DPI_FUNCTION chandle CreateSAXILite(
	input	string							sTitle,
	input	int unsigned					dwAddrBase,
	input	int unsigned					dwAddrBits
);
initial begin
	saxil	= CreateSAXILite(C_BUS_TITLE, C_BASE_ADDR, C_ADDR_BITS);
end

always@(posedge CLK) begin
	SAXIW_Interface(
		saxil, nRST,
		r_AWADDR, r_AWVALID, AWREADY,			// write address
		r_WDATA, r_WSTRB, r_WVALID, WREADY,		// write data
		BRESP, BVALID, r_BREADY					// bus interaction
	);
	AWADDR	<= r_AWADDR[C_ADDR_BITS-1:0];
	AWVALID	<= r_AWVALID;
	WDATA	<= r_WDATA;
	WSTRB	<= r_WSTRB;
	WVALID	<= r_WVALID;
	BREADY	<= r_BREADY;

	SAXIR_Interface(
		saxil, nRST,
		r_ARADDR, r_ARVALID, ARREADY,			// read address
		RDATA, RRESP, RVALID, r_RREADY			// read data
	);
	ARADDR	<= r_ARADDR[C_ADDR_BITS-1:0];
	ARVALID	<= r_ARVALID;
	RREADY	<= r_RREADY;
end

endmodule
/*verilator tracing_on*/
