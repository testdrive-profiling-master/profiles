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
// Title : processor AXI wrapper
// Rev.  : 6/26/2021 Sat (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"
`include "DUTs/processor_axi_wrapper/includes.vh"
`include "DUTs/processor_axi_wrapper/hdmi_controller.sv"

// AXI slave definitions
`ifndef SAXI_BASE_ADDR
`define SAXI_BASE_ADDR					32'h00020000
`endif
`ifndef SAXI_ADDR_BITS
`define SAXI_ADDR_BITS					16
`endif
// AXI master definitions
`ifndef MAXI_ID_WIDTH
`define MAXI_ID_WIDTH					2
`endif
`ifndef MAXI_DATA_WIDTH
`define	MAXI_DATA_WIDTH					512
`endif

/* HIDDEN */
module top (
	input								MCLK,				// clock
	input								nRST,				// reset (active low)
	output								BUSY,				// processor is busy
	output								INTR				// interrupt signal
);

localparam	C_M_AXI_ID_WIDTH			= `MAXI_ID_WIDTH;
localparam	C_M_AXI_ADDR_WIDTH			= 32;
localparam	C_M_AXI_DATA_WIDTH			= `MAXI_DATA_WIDTH;

// definition & assignment ---------------------------------------------------
initial
	SetSystemDescription("Processor axi wrapper");

assign	BUSY	= processor_axi_wrapper.busy;

//// slave APB -------------------------
wire									S_PSEL;
wire									S_PENABLE;
wire									S_PWRITE;
wire	[(`SAXI_ADDR_BITS-1):0]			S_PADDR;
wire	[31:0]							S_PWDATA;
wire	[31:0]							S_PRDATA;
wire									S_PREADY;
wire									S_PSLVERR;

//// AXI master interface --------------
// write address
wire	[C_M_AXI_ID_WIDTH-1:0]			M_AWID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR;
wire	[7:0]							M_AWLEN;
wire	[2:0]							M_AWSIZE;
wire	[1:0]							M_AWBURST;
wire	[1:0]							M_AWLOCK;
wire	[3:0]							M_AWCACHE;
wire	[2:0]							M_AWPROT;
wire	[3:0]							M_AWREGION;
wire	[3:0]							M_AWQOS;
wire									M_AWVALID;
wire									M_AWREADY;

// write data
wire	[C_M_AXI_ID_WIDTH-1:0]			M_WID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M_WDATA;
wire	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M_WSTRB;
wire									M_WLAST;
wire									M_WVALID;
wire									M_WREADY;
// bus interaction
wire	[C_M_AXI_ID_WIDTH-1:0]			M_BID;
wire	[1:0]							M_BRESP;
wire									M_BVALID;
wire									M_BREADY;
// read address
wire	[C_M_AXI_ID_WIDTH-1:0]			M_ARID;
wire	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR;
wire	[7:0]							M_ARLEN;
wire	[2:0]							M_ARSIZE;
wire	[1:0]							M_ARBURST;
wire	[1:0]							M_ARLOCK;
wire	[3:0]							M_ARCACHE;
wire	[2:0]							M_ARPROT;
wire	[3:0]							M_ARREGION;
wire	[3:0]							M_ARQOS;
wire									M_ARVALID;
wire									M_ARREADY;
// read data
wire	[C_M_AXI_ID_WIDTH-1:0]			M_RID;
wire	[C_M_AXI_DATA_WIDTH-1:0]		M_RDATA;
wire	[1:0]							M_RRESP;
wire									M_RLAST;
wire									M_RVALID;
wire									M_RREADY;

// implementation ------------------------------------------------------------
// APB bus implementation
testdrive_apb_slave_bfm #(
	.C_BUS_TITLE		("APB slave"),
	.C_BASE_ADDR		(`SAXI_BASE_ADDR),
	.C_ADDR_BITS		(`SAXI_ADDR_BITS)
) apb_slave_bfm (
	// system
	.CLK				(MCLK),
	.nRST				(nRST),
	// Write
	.PSEL				(S_PSEL),
	.PENABLE			(S_PENABLE),
	.PWRITE				(S_PWRITE),
	.PADDR				(S_PADDR),
	.PWDATA				(S_PWDATA),
	.PSTRB				(),				// not using! all 32bit data operation
	.PRDATA				(S_PRDATA),
	.PREADY				(S_PREADY),
	.PSLVERR			(S_PSLVERR)
);

// AXI4 bus implementation
testdrive_axi4_master_bfm #(
	.C_THREAD_ID_WIDTH			(C_M_AXI_ID_WIDTH),
	.C_ADDR_WIDTH				(C_M_AXI_ADDR_WIDTH),
	.C_DATA_WIDTH				(C_M_AXI_DATA_WIDTH)
) axi4_master_bfm (
	// system
	.CLK				(MCLK),
	.nRST				(nRST),
	// mAXI
	.AWID				(M_AWID),
	.AWADDR				(M_AWADDR),
	.AWLEN				(M_AWLEN),
	.AWSIZE				(M_AWSIZE),
	.AWBURST			(M_AWBURST),
	.AWLOCK				(M_AWLOCK),
	.AWCACHE			(M_AWCACHE),
	.AWPROT				(M_AWPROT),
	.AWREGION			(M_AWREGION),
	.AWQOS				(M_AWQOS),
	.AWVALID			(M_AWVALID),
	.AWREADY			(M_AWREADY),
	.WID				(M_WID),
	.WDATA				(M_WDATA),
	.WSTRB				(M_WSTRB),
	.WLAST				(M_WLAST),
	.WVALID				(M_WVALID),
	.WREADY				(M_WREADY),
	.BID				(M_BID),
	.BRESP				(M_BRESP),
	.BVALID				(M_BVALID),
	.BREADY				(M_BREADY),
	.ARID				(M_ARID),
	.ARADDR				(M_ARADDR),
	.ARLEN				(M_ARLEN),
	.ARSIZE				(M_ARSIZE),
	.ARBURST			(M_ARBURST),
	.ARLOCK				(M_ARLOCK),
	.ARCACHE			(M_ARCACHE),
	.ARPROT				(M_ARPROT),
	.ARREGION			(M_ARREGION),
	.ARQOS				(M_ARQOS),
	.ARVALID			(M_ARVALID),
	.ARREADY			(M_ARREADY),
	.RID				(M_RID),
	.RDATA				(M_RDATA),
	.RRESP				(M_RRESP),
	.RLAST				(M_RLAST),
	.RVALID				(M_RVALID),
	.RREADY				(M_RREADY)
);

processor_axi_wrapper #(
	// master parameters
	.C_M_AXI_ID_WIDTH			(C_M_AXI_ID_WIDTH),
	.C_M_AXI_ADDR_WIDTH			(C_M_AXI_ADDR_WIDTH),
	.C_M_AXI_DATA_WIDTH			(C_M_AXI_DATA_WIDTH)
) processor_axi_wrapper (
	//// system
	.CLK				(MCLK),
	.nRST				(nRST),
	.INTR				(INTR),
	//// slave APB -------------------------
	.S_PSEL				(S_PSEL),
	.S_PENABLE			(S_PENABLE),
	.S_PWRITE			(S_PWRITE),
	.S_PADDR			(S_PADDR),
	.S_PWDATA			(S_PWDATA),
	.S_PRDATA			(S_PRDATA),
	.S_PREADY			(S_PREADY),
	.S_PSLVERR			(S_PSLVERR),
	//// AXI master interface --------------
	// write address
	.M_AWID				(M_AWID),
	.M_AWADDR			(M_AWADDR),
	.M_AWLEN			(M_AWLEN),
	.M_AWSIZE			(M_AWSIZE),
	.M_AWBURST			(M_AWBURST),
	.M_AWLOCK			(M_AWLOCK),
	.M_AWCACHE			(M_AWCACHE),
	.M_AWPROT			(M_AWPROT),
	.M_AWREGION			(M_AWREGION),
	.M_AWQOS			(M_AWQOS),
	.M_AWVALID			(M_AWVALID),
	.M_AWREADY			(M_AWREADY),

	// write data
	.M_WID				(M_WID),
	.M_WDATA			(M_WDATA),
	.M_WSTRB			(M_WSTRB),
	.M_WLAST			(M_WLAST),
	.M_WVALID			(M_WVALID),
	.M_WREADY			(M_WREADY),
	// bus interaction
	.M_BID				(M_BID),
	.M_BRESP			(M_BRESP),
	.M_BVALID			(M_BVALID),
	.M_BREADY			(M_BREADY),
	// read address
	.M_ARID				(M_ARID),
	.M_ARADDR			(M_ARADDR),
	.M_ARLEN			(M_ARLEN),
	.M_ARSIZE			(M_ARSIZE),
	.M_ARBURST			(M_ARBURST),
	.M_ARLOCK			(M_ARLOCK),
	.M_ARCACHE			(M_ARCACHE),
	.M_ARPROT			(M_ARPROT),
	.M_ARREGION			(M_ARREGION),
	.M_ARQOS			(M_ARQOS),
	.M_ARVALID			(M_ARVALID),
	.M_ARREADY			(M_ARREADY),
	// read data
	.M_RID				(M_RID),
	.M_RDATA			(M_RDATA),
	.M_RRESP			(M_RRESP),
	.M_RLAST			(M_RLAST),
	.M_RVALID			(M_RVALID),
	.M_RREADY			(M_RREADY)
);

hdmi_controller	hdmi (
	.CLK				(MCLK),
	.nRST				(nRST)
);

endmodule
