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
`ifndef __AXI_SLAVE_WRAPPER_V__
`define __AXI_SLAVE_WRAPPER_V__
/*verilator tracing_off*/

module axi_slave_wrapper #(
	parameter	C_S_AXI_ADDR_WIDTH			= 32,
	parameter	C_S_AXI_DATA_WIDTH			= 32,
	parameter	ADDR_WIDTH					= 8
) (
	//// AXI slave interface
	// system
	input									S_CLK,			// clock
	input									S_nRST,			// reset (active low)
	// write address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_AWADDR,		// write address
	input									S_AWVALID,		// write address validation
	output	reg								S_AWREADY,		// write address ready
	// write data
	input	[C_S_AXI_DATA_WIDTH-1:0]		S_WDATA,		// write data
	input	[(C_S_AXI_DATA_WIDTH/8)-1:0]	S_WSTRB,		// write data byte strobe
	input									S_WVALID,		// write data validation
	output									S_WREADY,		// write data ready for slave
	// read address
	input	[C_S_AXI_ADDR_WIDTH-1:0]		S_ARADDR,		// read address
	input									S_ARVALID,		// read address validation
	output	reg								S_ARREADY,		// read address ready
	// read data
	output	[C_S_AXI_DATA_WIDTH-1:0]		S_RDATA,		// read data
	output	[1:0]							S_RRESP,		// read data response
	output									S_RVALID,		// read data validation
	input									S_RREADY,		// read data ready for master
	// bus
	output	[1:0]							S_BRESP,		// bus respense
	output	reg								S_BVALID,		// bus validation
	input									S_BREADY,		// bus ready

	//// Simplified slave decoded bus interface
	// read
	output									SR_REQ,			// read request
	input									SR_VALID,		// read validation
	output	reg [(ADDR_WIDTH-1):0]			SR_ADDR,		// read address
	input	[31:0]							SR_DATA,		// read data 1 clock delayed input
	// write
	output									SW_REQ,			// write request
	input									SW_VALID,		// write valid
	output	reg [(ADDR_WIDTH-1):0]			SW_ADDR,		// write address
	output	[31:0]							SW_DATA			// write data, enable synchronized output
);

// definition & assignment ---------------------------------------------------
// constants
assign	S_RRESP				= 2'b00;	// always OKAY
assign	S_BRESP				= 2'b00;	// always OKAY

// action
wire	rd_valid_data		= S_RVALID & S_RREADY;
wire	wr_valid_data		= S_WVALID & S_WREADY;

// bypass interconnection
assign	SR_REQ				= S_RREADY;
assign	S_RVALID			= SR_VALID;
assign	S_RDATA				= SR_DATA;

assign	SW_REQ				= S_WVALID;
assign	S_WREADY			= SW_VALID;
assign	SW_DATA				= S_WDATA;

// implementation ------------------------------------------------------------
// bypass read transaction
always@(posedge S_CLK, negedge S_nRST) begin
	if(!S_nRST) begin
		S_ARREADY			<= 1'b1;
		SR_ADDR				<= 'd0;
	end
	else begin
		if(S_ARREADY) begin
			if(S_ARVALID) begin
				S_ARREADY	<= 1'b0;
				SR_ADDR		<= S_ARADDR[(ADDR_WIDTH-1)+2:2];
			end
		end
		else begin
			if(rd_valid_data) begin
				S_ARREADY	<= 1'b1;
			end
		end
	end
end

// bypass write transaction
always@(posedge S_CLK, negedge S_nRST) begin
	if(!S_nRST) begin
		S_AWREADY			<= 1'b1;
		S_BVALID			<= 1'b0;
		SW_ADDR				<= 'd0;
	end
	else begin
		if(S_AWREADY) begin
			if(S_AWVALID) begin
				S_AWREADY	<= 1'b0;
				SW_ADDR		<= S_AWADDR[(ADDR_WIDTH-1)+2:2];
			end
		end
		else begin
			if(wr_valid_data) begin
				S_AWREADY	<= 1'b1;
			end
		end

		if(S_BVALID) begin
			if(S_BREADY)
				S_BVALID	<= 1'b0;
		end
		else begin
			S_BVALID		<= wr_valid_data;
		end
	end
end

endmodule
/*verilator tracing_on*/
`endif//__AXI_SLAVE_WRAPPER_V__
