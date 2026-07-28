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
// Rev.  : 5/25/2023 Thu (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`ifndef __AXI_MASTER_WRAPPER_V__
`define __AXI_MASTER_WRAPPER_V__
/*verilator tracing_off*/

module axi_master_wrapper #(
	parameter integer	C_M_AXI_ID_WIDTH	= 1,
	parameter integer	C_M_AXI_ADDR_WIDTH	= 32,
	parameter integer	C_M_AXI_DATA_WIDTH	= 128,
	parameter integer	C_USE_AXI4			= 1
) (
	//// AXI master interface
	// system
	input									M_CLK,			// clock
	input									M_nRST,			// reset (active low)
	// write address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_AWID,			// The ID tag for the write address group of signals
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_AWADDR,		// Write address
	output	[((C_USE_AXI4==1) ? 8:4)-1:0]	M_AWLEN,		// Burst_Length = AxLEN + 1
	output	[2:0]							M_AWSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]							M_AWBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output									M_AWLOCK,		// b0(Normal), b1(Exclusive)
	output	[3:0]							M_AWCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]							M_AWPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	[3:0]							M_AWREGION,		//
	output	[3:0]							M_AWQOS,		//
	output									M_AWVALID,		// Write address valid
	input									M_AWREADY,		// Write address ready (1 = slave ready, 0 = slave not ready)

	// write data
	output	[C_M_AXI_ID_WIDTH-1:0]			M_WID,			//
	output	[C_M_AXI_DATA_WIDTH-1:0]		M_WDATA,		// Write data
	output	[(C_M_AXI_DATA_WIDTH/8)-1:0]	M_WSTRB,		// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
	output	reg								M_WLAST,		// Write last
	output									M_WVALID,		// Write valid
	input									M_WREADY,		// Write ready
	// bus
	input	[C_M_AXI_ID_WIDTH-1:0]			M_BID,			// The ID tag for the write response
	input	[1:0]							M_BRESP,		// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input									M_BVALID,		// Write response valid
	output	reg								M_BREADY,		// Response ready
	// read address
	output	[C_M_AXI_ID_WIDTH-1:0]			M_ARID,			// Read address ID tag
	output	[C_M_AXI_ADDR_WIDTH-1:0]		M_ARADDR,		// Read address
	output	[((C_USE_AXI4==1) ? 8:4)-1:0]	M_ARLEN,		// Burst_Length = AxLEN + 1
	output	[2:0]							M_ARSIZE,		// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]							M_ARBURST,		// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output									M_ARLOCK,		// b0(Normal), b1(Exclusive)
	output	[3:0]							M_ARCACHE,		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]							M_ARPROT,		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	[3:0]							M_ARREGION,		//
	output	[3:0]							M_ARQOS,		//
	output									M_ARVALID,		// Read address valid
	input									M_ARREADY,		// Read address ready (1 = slave ready, 0 = slave not ready)
	// read data
	input	[C_M_AXI_ID_WIDTH-1:0]			M_RID,			// Read ID tag.
	input	[C_M_AXI_DATA_WIDTH-1:0]		M_RDATA,		// Read data.
	input	[1:0]							M_RRESP,		// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input									M_RLAST,		// Read last. This signal indicates the last transfer in a read burst.
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
	input	[((C_USE_AXI4==1) ? 8:4)-1:0]	MR_SIZE,		// read size
	output	[C_M_AXI_DATA_WIDTH-1:0]		MR_DATA,		// read data
	output									MR_VALID,		// read validation
	input									MR_READY,		// read ready
	output									MR_LAST,		// last read
	// write
	input									MW_REQ,			// write request
	output									MW_GRANT,		// write grant
	input	[C_M_AXI_ADDR_WIDTH-1:0]		MW_ADDR,		// write address
	input	[((C_USE_AXI4==1) ? 8:4)-1:0]	MW_SIZE,		// write size
	input	[C_M_AXI_DATA_WIDTH-1:0]		MW_DATA,		// write data
	input									MW_VALID,		// write validation
	output									MW_READY,		// write ready
	output									MW_LAST			// last write
);

// definition & assignment ---------------------------------------------------
// write control for overlapping
reg			wq_en;			// queued write enable
reg	[7:0]	wq_count;		// queued write counter

// write data control
reg	[7:0]	w_count;		// write counter
reg			w_de;			// write data enable

// write address
assign	M_AWID				= 'd0;
assign	M_AWADDR			= MW_ADDR;
assign	M_AWLEN				= MW_SIZE;
assign	M_AWSIZE			= (
	C_M_AXI_DATA_WIDTH == 8		? 3'b000 :	// 8 bit
	C_M_AXI_DATA_WIDTH == 16	? 3'b001 :	// 16 bit
	C_M_AXI_DATA_WIDTH == 32	? 3'b010 :	// 32 bit
	C_M_AXI_DATA_WIDTH == 64	? 3'b011 :	// 64 bit
	C_M_AXI_DATA_WIDTH == 128	? 3'b100 :	// 128 bit
	C_M_AXI_DATA_WIDTH == 256	? 3'b101 :	// 256 bit
	C_M_AXI_DATA_WIDTH == 512	? 3'b110 :	// 512 bit
	/* 1024 */
	3'b111	// 1024 bit
);
assign	M_AWBURST			= 2'b01;		// INCR
assign	M_AWLOCK			= 1'b0;			// normal
assign	M_AWCACHE			= 4'b0000;		// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
assign	M_AWPROT			= 3'b000;		// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
assign	M_AWREGION			= 4'b0000;
assign	M_AWQOS				= 4'b0000;
assign	M_AWVALID			= MW_REQ & (~wq_en);
assign	MW_GRANT			= M_AWREADY & (~wq_en);
// write data
assign	M_WID				= 'd0;
assign	M_WDATA				= MW_DATA;
assign	M_WSTRB				= {(C_M_AXI_DATA_WIDTH/8){1'b1}};
assign	M_WVALID			= MW_VALID & w_de;
assign	MW_READY			= M_WREADY & w_de;
assign	MW_LAST				= M_WLAST;

// read address
assign	M_ARID				= 'd0;			// always zero! : don't care 'M_RID'
assign	M_ARADDR			= MR_ADDR;
assign	M_ARLEN				= MR_SIZE;
assign	M_ARSIZE			= (
	C_M_AXI_DATA_WIDTH == 8		? 3'b000 :	// 8 bit
	C_M_AXI_DATA_WIDTH == 16	? 3'b001 :	// 16 bit
	C_M_AXI_DATA_WIDTH == 32	? 3'b010 :	// 32 bit
	C_M_AXI_DATA_WIDTH == 64	? 3'b011 :	// 64 bit
	C_M_AXI_DATA_WIDTH == 128	? 3'b100 :	// 128 bit
	C_M_AXI_DATA_WIDTH == 256	? 3'b101 :	// 256 bit
	C_M_AXI_DATA_WIDTH == 512	? 3'b110 :	// 512 bit
	/* 1024 */                    3'b111	// 1024 bit
);
assign	M_ARBURST			= 2'b01;		// incrementaing-address burst
assign	M_ARLOCK			= 1'b0;			// normal
assign	M_ARCACHE			= 4'b0;			// no cache use : [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
assign	M_ARPROT			= 3'b010;		// none secure
assign	M_ARREGION			= 4'b0000;
assign	M_ARQOS				= 4'b0000;
assign	M_ARVALID			= MR_REQ;
assign	MR_GRANT			= M_ARREADY;
// read data
assign	MR_DATA				= M_RDATA;
assign	M_RREADY			= MR_READY;
assign	MR_VALID			= M_RVALID;
assign	MR_LAST				= M_RLAST;

// implementation ------------------------------------------------------------
// write address queue control (for overlapped transaction)
always@(posedge M_CLK, negedge M_nRST) begin
	if(!M_nRST) begin
		wq_en			<= 1'b0;
		wq_count		<= 'd0;
	end
	else begin
		if(!wq_en) begin
			if(M_AWVALID & M_AWREADY) begin
				wq_en		<= 1'b1;
				wq_count	<= M_AWLEN;
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
		M_WLAST			<= 1'b0;
		w_count			<= 'd0;
		w_de			<= 1'b0;
		M_BREADY		<= 1'b0;
	end
	else begin
		if(!M_BREADY) begin
			if(wq_en) begin
				w_count		<= wq_count;
				w_de		<= 1'b1;
				M_WLAST		<= wq_count==8'd0 ? 1'b1 : 1'b0;
				M_BREADY	<= 1'b1;
			end
		end
		else begin
			if(w_de && M_WVALID && M_WREADY) begin
				w_de		<= |w_count;
				w_count 	<= w_count -1;
				M_WLAST		<= (w_count==8'd1) ? 1'b1 : 1'b0;
			end

			if(!w_de && M_BVALID) begin
				M_BREADY	<= 1'b0;
			end
		end
	end
end

endmodule
/*verilator tracing_on*/
`endif//__AXI_MASTER_WRAPPER_V__
