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
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "DUTs/MTSP/includes.vh"
`define NO_TEST_DESIGN		// overriding test top design

`ifdef USE_TESTDRIVE
`define MAXI_DATA_WIDTH			256
`include "DUTs/processor_axi_wrapper/top.sv"
`else
`include "DUTs/processor_axi_wrapper/includes.vh"
`endif
/* TOP_MODULE = processor_axi_wrapper */
// *** Do not modify to here.

module processor_top #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 256
) (
	//// system
	input							CLK, nRST,			// clock & reset (active low)
	output							BUSY,				// processor is busy
	output							INTR,				// interrupt signal

	//// slave interface
	input							P_EN,				// slave enable
	input							P_WE,				// write enable
	input	[ADDR_WIDTH-1:0]		P_ADDR,				// address
	input	[`RANGE_DWORD]			P_WDATA,			// write data
	output	reg [`RANGE_DWORD]		P_RDATA,			// read data
	output							P_READY,			// data ready

	//// master interface
	// read
	output							MR_REQ,				// read request
	output	[`RANGE_DWORD]			MR_ADDR,			// read address
	output	[`RANGE_BYTE]			MR_SIZE,			// read size
	input							MR_GRANT,			// read grant
	// read data
	input	[DATA_WIDTH-1:0]		MR_DATA,			// read data
	input							MR_VALID,			// read validation
	output							MR_READY,			// read ready
	input							MR_LAST,			// read last
	// write address
	output							MW_REQ,				// write request
	output	[`RANGE_DWORD]			MW_ADDR,			// write address
	output	[`RANGE_BYTE]			MW_SIZE,			// write size
	input							MW_GRANT,			// write grant
	// write data
	output	[DATA_WIDTH-1:0]		MW_DATA,			// write data
	output							MW_VALID,			// write validation
	input							MW_READY,			// write ready
	input							MW_LAST				// write last
);

// definition & assignment ---------------------------------------------------
// slave bus interface wrapper
i_bus_slave					bus_slave;

assign	bus_slave.en		= P_EN;
assign	bus_slave.we		= P_WE;
assign	bus_slave.addr		= P_ADDR;
assign	bus_slave.wdata		= P_WDATA;
assign	P_RDATA				= bus_slave.rdata;
assign	P_READY				= bus_slave.ready;

// master bus interface wrapper
i_bus_master				bus_master;

assign	MR_REQ				= bus_master.rreq;
assign	MR_ADDR				= bus_master.raddr;
assign	MR_SIZE				= bus_master.rsize;
assign	bus_master.rgrant	= MR_GRANT;
assign	bus_master.rdata	= MR_DATA;
assign	bus_master.rvalid	= MR_VALID;
assign	MR_READY			= bus_master.rready;
assign	bus_master.rlast	= MR_LAST;
assign	MW_REQ				= bus_master.wreq;
assign	MW_ADDR				= bus_master.waddr;
assign	MW_SIZE				= bus_master.wsize;
assign	bus_master.wgrant	= MW_GRANT;
assign	MW_DATA				= bus_master.wdata;
assign	MW_VALID			= bus_master.wvalid;
assign	bus_master.wready	= MW_READY;
assign	bus_master.wlast	= MW_LAST;

// implementation ------------------------------------------------------------
// MTSP system
MTSP_System #(
	.CORE_SIZE				(`MEITNER_CORE_SIZE),
	.COMMAND_QUEUE_DEPTH	(6)
) mtsp (
	// System
	.CLK					(CLK),
	.nRST					(nRST),
	.BUSY					(BUSY),
	.INTR					(INTR),
	.bus_slave				(bus_slave),
	.bus_master				(bus_master),
	// external tft lcd driver interface
	.VIDEO_BASE				()
);

endmodule
