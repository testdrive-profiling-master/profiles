//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"
`include "top/testdrive_apb_slave_bfm.sv"

// testbench example...
module top (
	input							MCLK,			// clock
	input							nRST,			// reset (active low)
	output	reg						BUSY,			// processor is busy
	output							INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------

assign	BUSY	= 0;
assign	INTR	= 0;

wire							APB_PSEL;
wire							APB_PENABLE;
wire							APB_PWRITE;
wire	[10-1:0]				APB_PADDR;
wire	[`RANGE_DWORD]			APB_PWDATA;
wire	[3:0]					APB_PSTRB;
wire	[`RANGE_DWORD]			APB_PRDATA	= 32'h12345678;
wire							APB_PREADY	= `TRUE;
wire							APB_PSLVERR	= `FALSE;

// implementation ------------------------------------------------------------
testdrive_apb_slave_bfm #(
	.C_BUS_TITLE	("Test"),
	.C_BASE_ADDR	(32'h12300000),
	.C_ADDR_BITS	(10)
) apb (
	.CLK			(MCLK),
	.nRST			(nRST),
	.PSEL			(APB_PSEL),
	.PENABLE		(APB_PENABLE),
	.PWRITE			(APB_PWRITE),
	.PADDR			(APB_PADDR),
	.PWDATA			(APB_PWDATA),
	.PSTRB			(APB_PSTRB),
	.PRDATA			(APB_PRDATA),
	.PREADY			(APB_PREADY),
	.PSLVERR		(APB_PSLVERR)
);

endmodule
