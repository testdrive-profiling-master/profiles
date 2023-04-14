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
// Rev.  : 4/14/2023 Fri (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"
/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// The Virtual slave BFM
//-----------------------------------------------------------------------------

module testdrive_virtual_slave_bfm #(
	parameter string	C_BUS_TITLE		= "",
	parameter			C_BASE_ADDR		= 32'h0,
	parameter integer	C_ADDR_BITS		= 10
) (
	// system
	input									CLK,
	input									nRST,
	// Write
	output	reg								WE,
	output	reg [C_ADDR_BITS-1:0]			WADDR,
	output	reg [`RANGE_DWORD]				WDATA,
	// Read
	output	reg								RE,
	output	reg [C_ADDR_BITS-1:0]			RADDR,
	input	[`RANGE_DWORD]					RDATA
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void VirtualSlave_Write (
	input	chandle							hSVirtual,
	input	bit								nRST,
	// write
	output	bit 							EN,
	output	longint unsigned				ADDR,
	output	bit	[`RANGE_DWORD]				DATA
);

`DPI_FUNCTION void VirtualSlave_Read (
	input	chandle							hSVirtual,
	input	bit								nRST,
	// read
	output	bit 							EN,
	output	longint unsigned				ADDR,
	input	bit	[`RANGE_DWORD]				DATA
);

// register pipes
reg											r_en, w_en;
reg	[63:0]									r_addr, w_addr;
reg	[`RANGE_DWORD]							w_data;

// implementation ------------------------------------------------------------
// object
chandle svirtual;
// initialize
`DPI_FUNCTION chandle CreateVirtualSlave(
	input	string							sTitle,
	input	longint unsigned				dwAddrBase,
	input	int unsigned					dwAddrBits
);
initial begin
	svirtual	= CreateVirtualSlave(C_BUS_TITLE, C_BASE_ADDR, C_ADDR_BITS);
end

always@(posedge CLK) begin
	VirtualSlave_Write(
		svirtual, nRST,
		w_en, w_addr, w_data
	);
	WE		<= w_en;
	WADDR	<= w_addr[C_ADDR_BITS-1:0];
	WDATA	<= w_data;

	VirtualSlave_Read(
		svirtual, nRST,
		r_en, r_addr, RDATA
	);
	RE		<= r_en;
	RADDR	<= r_addr[C_ADDR_BITS-1:0];
end

endmodule
/*verilator tracing_on*/
