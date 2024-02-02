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
`timescale 1ns/1ns
`ifndef __AXI_MASTER_LITE_WRAPPER_V__
`define __AXI_MASTER_LITE_WRAPPER_V__
/*verilator tracing_off*/

module axi_master_lite_wrapper #(
		parameter integer	C_M_AXI_ADDR_WIDTH	= 20
	) (
		//// AXI-LITE master interface
		// system
		input									M_CLK,			// clock
		input									M_nRST,			// reset (active low)
		// write address
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR,		// Write address
		output									M_AWVALID,		// Write address valid
		input									M_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)

		// write data
		output	[31:0]							M_WDATA,		// Write data
		output	[3:0]							M_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
		output									M_WVALID,		// Write valid
		input									M_WREADY,		// Write ready
		// bus
		input	[1:0]							M_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M_BVALID,		// Write response valid
		output	reg								M_BREADY,		// Response ready
		// read address
		output	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR,		// Read address
		output									M_ARVALID,		// Read address valid
		input									M_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
		// read data
		input	[31:0]							M_RDATA,		// Read data.
		input	[1:0]							M_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
		input									M_RVALID,		// Read valid (1 = read data available, 0 = read data not available)
		output									M_RREADY,		// Read ready (1= master ready 0 = master not ready)
		/*
		   Read transaction handshake dependencies
		   ARVALID ----------------------->> RVALID
		      |                           ^     |
		      |                           ^     |
		      |                           |     |
		      +--------> ARREADY ---------+     +-----> RREADY

		   Write transaction handshake dependencies
		   AWVALID              +---- WVALID --------->> BVALID
		      |                 |        |                ^  |
		      +---------------------+    |                |  |
		      |                 |   |    |                |  |
		      -> AWREADY <----+   +--> +-> WREADY ------+  +--> BREADY
		*/

		//// internal master interface
		// read
		input									MR_REQ,			// read request
		output									MR_GRANT,		// read grant
		input	[C_M_AXI_ADDR_WIDTH-1:0]		MR_ADDR,		// read address
		output	[31:0]							MR_DATA,		// read data
		output									MR_VALID,		// read validation
		input									MR_READY,		// read ready
		// write
		input									MW_REQ,			// write request
		output									MW_GRANT,		// write grant
		input	[C_M_AXI_ADDR_WIDTH-1:0]		MW_ADDR,		// write address
		input	[31:0]							MW_DATA,		// write data
		input									MW_VALID,		// write validation
		output									MW_READY		// write ready
	);

	// definition & assignment ---------------------------------------------------
	// write control for overlapping
	reg			wq_en;			// queued write enable

	// write data control
	reg			w_de;			// write data enable

	// write address
	assign	M_AWADDR			= MW_ADDR;
	assign	M_AWVALID			= MW_REQ & (~wq_en);
	assign	MW_GRANT			= M_AWREADY & (~wq_en);
	// write data
	assign	M_WDATA				= MW_DATA;
	assign	M_WSTRB				= 4'b1111;
	assign	M_WVALID			= MW_VALID & w_de;
	assign	MW_READY			= M_WREADY & w_de;
	// read address
	assign	M_ARADDR			= MR_ADDR;
	assign	M_ARVALID			= MR_REQ;
	assign	MR_GRANT			= M_ARREADY;
	// read data
	assign	MR_DATA				= M_RDATA;
	assign	M_RREADY			= MR_READY;
	assign	MR_VALID			= M_RVALID;

	// implementation ------------------------------------------------------------
	// write address queue control (for overlapped transaction)
	always@(posedge M_CLK, negedge M_nRST) begin
		if(!M_nRST) begin
			wq_en			<= 1'b0;
		end
		else begin
			if(!wq_en) begin
				if(M_AWVALID & M_AWREADY) begin
					wq_en		<= 1'b1;
				end
			end
			else begin
				if(!M_BREADY) begin
					wq_en		<= 1'b0;
				end
			end
		end
	end

	// write data control
	always@(posedge M_CLK, negedge M_nRST) begin
		if(!M_nRST) begin
			w_de			<= 1'b0;
			M_BREADY		<= 1'b0;
		end
		else begin
			if(!M_BREADY) begin
				if(wq_en) begin
					w_de		<= 1'b1;
					M_BREADY	<= 1'b1;
				end
			end
			else begin
				if(w_de && M_WVALID && M_WREADY) begin
					w_de		<= 1'b0;
				end

				if(!w_de && M_BVALID) begin
					M_BREADY	<= 1'b0;
				end
			end
		end
	end

endmodule
/*verilator tracing_on*/
`endif//__AXI_MASTER_LITE_WRAPPER_V__
