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
`timescale 1ns/1ns
`include "testdrive_system.vh"
`include "template/testdrive_virtual_slave_bfm.sv"

`ifndef VIRTUAL_SLAVE_BASE_ADDR
`define	VIRTUAL_SLAVE_BASE_ADDR		32'h0
`endif
`ifndef VIRTUAL_SLAVE_ADDR_BITS
`define	VIRTUAL_SLAVE_ADDR_BITS		16
`endif

module top(
	// system
	input		MCLK,		// clock
	input		nRST,		// reset (active low)
	// control
	output		BUSY,		// processor is busy
	output		INTR		// interrupt signal
);

// definition & assignment ---------------------------------------------------
// write
wire									S_WE;
wire	[`VIRTUAL_SLAVE_ADDR_BITS-1:0]	S_WADDR;
wire	[`RANGE_DWORD]					S_WDATA;

// read
wire									S_RE;
wire	[`VIRTUAL_SLAVE_ADDR_BITS-1:0]	S_RADDR;
wire	[`RANGE_DWORD]					S_RDATA;

// implementation ------------------------------------------------------------
// virtual slave bus
testdrive_virtual_slave_bfm #(
	.C_BASE_ADDR		(`VIRTUAL_SLAVE_BASE_ADDR),
	.C_ADDR_BITS		(`VIRTUAL_SLAVE_ADDR_BITS)
)  virtual_slave (
	MCLK, nRST,									// system
	S_WE, S_WADDR, S_WDATA,						// write
	S_RE, S_RADDR, S_RDATA						// read
);

//----------------------------------------------------------------------------
// processor wrapper implementation
dut_top  dut_top (
	//// system
	.CLK				(MCLK),
	.nRST				(nRST),
	.BUSY				(BUSY),
	.INTR				(INTR),
	//// slave bus
	// write
	.S_WE				(S_WE),
	.S_WADDR			(S_WADDR),
	.S_WDATA			(S_WDATA),
	// read
	.S_RE				(S_RE),
	.S_RADDR			(S_RADDR),
	.S_RDATA			(S_RDATA)
);

endmodule
