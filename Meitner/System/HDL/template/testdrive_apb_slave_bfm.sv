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
// Title : Template design
// Rev.  : 3/15/2023 Wed (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"
/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// The APB Slave BFM
//-----------------------------------------------------------------------------

module testdrive_apb_slave_bfm #(
	parameter string	C_BUS_TITLE			= "",
	parameter			C_BASE_ADDR			= 32'h0,
	parameter integer	C_ADDR_BITS			= 10,
	parameter integer	C_TIME_OUT			= 3000		// if '0', not use timeout
) (
	// system
	input									CLK,
	input									nRST,
	// Write
	output	reg								PSEL,
	output	reg								PENABLE,
	output	reg								PWRITE,
	output	reg [C_ADDR_BITS-1:0]			PADDR,
	output	reg [`RANGE_DWORD]				PWDATA,
	output	reg [3:0]						PSTRB,
	input	[`RANGE_DWORD]					PRDATA,
	input									PREADY,
	input									PSLVERR
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void APB_Slave_Interface (
	input	chandle							hSAPB,
	input	bit								nRST,
	// write
	output	bit 							PSEL,
	output	bit 							PENABLE,
	output	bit 							PWRITE,
	output	bit	[`RANGE_DWORD] 				PADDR,
	output	bit	[`RANGE_DWORD]				PWDATA,
	output	bit	[3:0]						PSTRB,
	input	bit	[`RANGE_DWORD]				PRDATA,
	input	bit								PREADY,
	input	bit								PSLVERR
);

// register pipes
reg											r_sel, r_enable, r_write;
reg	[`RANGE_DWORD]							r_addr, r_wdata;
reg	[3:0]									r_strb;

// implementation ------------------------------------------------------------
// object
chandle sapb;
// initialize
`DPI_FUNCTION chandle CreateAPBSlave(
	input	string							sTitle,
	input	int unsigned					dwAddrBase,
	input	int unsigned					dwAddrBits,
	input	int unsigned					dwTimeOut
);
initial begin
	sapb	= CreateAPBSlave(C_BUS_TITLE, C_BASE_ADDR, C_ADDR_BITS, C_TIME_OUT);
end

always@(posedge CLK) begin
	APB_Slave_Interface(
		sapb, nRST,
		r_sel, r_enable, r_write, r_addr, r_wdata, r_strb,
		PRDATA, PREADY, PSLVERR
	);
	PSEL		<= r_sel;
	PENABLE		<= r_enable;
	PWRITE		<= r_write;
	PADDR		<= r_addr[C_ADDR_BITS-1:0];
	PWDATA		<= r_wdata;
	PSTRB		<= r_strb;
end

endmodule
/*verilator tracing_on*/
