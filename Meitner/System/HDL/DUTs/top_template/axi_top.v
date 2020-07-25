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
`timescale 1ns/1ns
`include "top/top_axi.sv"

module dut_top #(
	// slave
	parameter	C_S_AXI_ADDR_WIDTH			= 32,
	parameter	C_S_AXI_DATA_WIDTH			= 32,
	// master
	parameter	C_M_AXI_ID_WIDTH			= 1,
	parameter	C_M_AXI_ADDR_WIDTH			= 32,
	parameter	C_M_AXI_DATA_WIDTH			= 128
) (
	//// system ----------------------------
	input									CLK,				// clock
	input									nRST,				// reset (active low)
	output									BUSY,				// processor is busy
	output									INTR,				// interrupt signal

	//// AXI slave interface ---------------
	// write address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_AWADDR,
	input									S_AWVALID,
	output									S_AWREADY,
	// write data
	input	[C_S_AXI_DATA_WIDTH-1:0]		S_WDATA,
	input	[(C_S_AXI_DATA_WIDTH/8)-1:0]	S_WSTRB,
	input									S_WVALID,
	output									S_WREADY,
	// read address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_ARADDR,
	input									S_ARVALID,
	output									S_ARREADY,
	// read data
	output	[C_S_AXI_DATA_WIDTH-1:0]		S_RDATA,
	output	[1:0]							S_RRESP,
	output									S_RVALID,
	input									S_RREADY,
	// bus
	output	[1:0]							S_BRESP,
	output									S_BVALID,
	input									S_BREADY,

	//// AXI master interface --------------
	// write address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_AWID,
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR,
	output	[7:0]							M_AWLEN,
	output	[2:0]							M_AWSIZE,
	output	[1:0]							M_AWBURST,
	output	[1:0]							M_AWLOCK,
	output	[3:0]							M_AWCACHE,
	output	[2:0]							M_AWPROT,
	output	[3:0]							M_AWREGION,
	output	[3:0]							M_AWQOS,
	output									M_AWVALID,
	input									M_AWREADY,

	// write data
	output	[C_M_AXI_ID_WIDTH-1:0]			M_WID,
	output	[C_M_AXI_DATA_WIDTH-1:0]		M_WDATA,
	output	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M_WSTRB,
	output									M_WLAST,
	output									M_WVALID,
	input									M_WREADY,
	// bus interaction
	input	[C_M_AXI_ID_WIDTH-1:0]			M_BID,
	input	[1:0]							M_BRESP,
	input									M_BVALID,
	output									M_BREADY,
	// read address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_ARID,
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR,
	output	[7:0]							M_ARLEN,
	output	[2:0]							M_ARSIZE,
	output	[1:0]							M_ARBURST,
	output	[1:0]							M_ARLOCK,
	output	[3:0]							M_ARCACHE,
	output	[2:0]							M_ARPROT,
	input	[3:0]							M_ARREGION,
	input	[3:0]							M_ARQOS,
	output									M_ARVALID,
	input									M_ARREADY,
	// read data
	input	[C_M_AXI_ID_WIDTH-1:0]			M_RID,
	input	[C_M_AXI_DATA_WIDTH-1:0]		M_RDATA,
	input	[1:0]							M_RRESP,
	input									M_RLAST,
	input									M_RVALID,
	output									M_RREADY
);

// definition & assignment ---------------------------------------------------
// simulation will be run within 10000 clock
`define	END_OF_TIME		10000
reg	[31:0]			sim_counter;
assign	BUSY		= (sim_counter <  `END_OF_TIME);
assign	INTR		= (sim_counter == `END_OF_TIME);

//// discard slave bus.
// write address
assign	S_AWREADY	= `FALSE;
// write data
assign	S_WREADY	= `FALSE;
// read address
assign	S_ARREADY	= `FALSE;
// read data
assign	S_RDATA		= 'd0;
assign	S_RRESP		= 'd0;
assign	S_RVALID	= `FALSE;
// bus
assign	S_BRESP		= 'd0;
assign	S_BVALID	= `FALSE;

//// discard master bus.
// write address
assign	M_AWID		= 'd0;
assign	M_AWADDR	= 'd0;
assign	M_AWLEN		= 'd0;
assign	M_AWSIZE	= 'd0;
assign	M_AWBURST	= 'd0;
assign	M_AWLOCK	= 'd0;
assign	M_AWCACHE	= 'd0;
assign	M_AWPROT	= 'd0;
assign	M_AWREGION	= 'd0;
assign	M_AWQOS		= 'd0;
assign	M_AWVALID	= `FALSE;
// write data
assign	M_WID		= 'd0;
assign	M_WDATA		= 'd0;
assign	M_WSTRB		= 'd0;
assign	M_WLAST		= `FALSE;
assign	M_WVALID	= `FALSE;
// bus interaction
assign	M_BREADY	= `FALSE;
// read address
assign	M_ARID		= 'd0;
assign	M_ARADDR	= 'd0;
assign	M_ARLEN		= 'd0;
assign	M_ARSIZE	= 'd0;
assign	M_ARBURST	= 'd0;
assign	M_ARLOCK	= 'd0;
assign	M_ARCACHE	= 'd0;
assign	M_ARPROT	= 'd0;
assign	M_ARVALID	= `FALSE;
// read data
assign	M_RREADY	= `FALSE;

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		sim_counter	<= 'd0;
	end
	else begin
		sim_counter	<= sim_counter + 1;
	end
end

endmodule
