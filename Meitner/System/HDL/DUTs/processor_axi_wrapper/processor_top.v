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
// Title : processor AXI wrapper
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

// test example
module processor_top #(
		parameter	S_ADDR_WIDTH		= 4,				// slave address width
		parameter	M_ADDR_WIDTH		= 32,				// master address width
		parameter	M_DATA_WIDTH		= 512,				// master data width
		parameter	MS_ADDR_WIDTH		= 20				// AXI-LITE address width
	) (
		//// system
		input							CLK, nRST,			// clock & reset (active low)
		output							BUSY,				// processor is busy
		output	reg						INTR,				// interrupt signal

		//// slave interface
		input							S_EN,				// slave enable
		input							S_WE,				// write enable
		input	[S_ADDR_WIDTH-1:0]		S_ADDR,				// address
		input	[`RANGE_DWORD]			S_WDATA,			// write data
		output	reg [`RANGE_DWORD]		S_RDATA,			// read data
		output							S_READY,			// data ready

		//// master for slave output interface
		// read address
		output							MS_RREQ,			// request
		output	[MS_ADDR_WIDTH-1:0]		MS_RADDR,			// address
		input							MS_RGRANT,			// request grant
		// read data
		input	[31:0]					MS_RDATA,			// read data
		input							MS_RVALID,			// read data validation
		output							MS_RREADY,			// read operation is ready
		// write address
		output							MS_WREQ,			// request
		output	[MS_ADDR_WIDTH-1:0]		MS_WADDR,			// address
		input							MS_WGRANT,			// request grant
		// write data
		output	[31:0]					MS_WDATA,			// write data
		output							MS_WVALID,			// write data validation
		input							MS_WREADY,			// write operation is ready

		//// master #0 interface
		// read address
		output	reg						M0_RREQ,			// read request
		output	[M_ADDR_WIDTH-1:0]		M0_RADDR,			// read address
		output	[`RANGE_BYTE]			M0_RSIZE,			// read size
		input							M0_RGRANT,			// read grant
		// read data
		input	[M_DATA_WIDTH-1:0]		M0_RDATA,			// read data
		input							M0_RVALID,			// read validation
		output							M0_RREADY,			// read ready
		input							M0_RLAST,			// read last
		// write address
		output	reg						M0_WREQ,			// write request
		output	[M_ADDR_WIDTH-1:0]		M0_WADDR,			// write address
		output	[`RANGE_BYTE]			M0_WSIZE,			// write size
		input							M0_WGRANT,			// write grant
		// write data
		output	reg [M_DATA_WIDTH-1:0]	M0_WDATA,			// write data
		output							M0_WVALID,			// write validation
		input							M0_WREADY,			// write ready
		input							M0_WLAST,			// write last

		//// master #1 interface
		// read
		output							M1_RREQ,			// read request
		output	[M_ADDR_WIDTH-1:0]		M1_RADDR,			// read address
		output	[`RANGE_BYTE]			M1_RSIZE,			// read size
		input							M1_RGRANT,			// read grant
		// read data
		input	[M_DATA_WIDTH-1:0]		M1_RDATA,			// read data
		input							M1_RVALID,			// read validation
		output							M1_RREADY,			// read ready
		input							M1_RLAST,			// read last
		// write address
		output							M1_WREQ,			// write request
		output	[M_ADDR_WIDTH-1:0]		M1_WADDR,			// write address
		output	[`RANGE_BYTE]			M1_WSIZE,			// write size
		input							M1_WGRANT,			// write grant
		// write data
		output	[M_DATA_WIDTH-1:0]		M1_WDATA,			// write data
		output							M1_WVALID,			// write validation
		input							M1_WREADY,			// write ready
		input							M1_WLAST			// write last
	);

	// definition & assignment ---------------------------------------------------
	// synthesis translate_off
`ifdef USE_TESTDRIVE

	initial
		SetSystemDescription("testing example DUT processor");
`endif
	// synthesis translate_on

	assign	BUSY				= `FALSE;					// not use this signal at this time

	// master #0
	reg							bWrite;						// master write operation?
	reg		[63:0]				address;					// physical address
	reg		[`RANGE_BYTE]		trans_size;					// (transfer size - 1)
	reg		[`RANGE_BYTE]		count;						// same as 'trans_size' for count down
	reg							master_en;					// master bus operation enable

	assign	M0_RADDR			= address[M_ADDR_WIDTH-1:0];
	assign	M0_RSIZE			= trans_size;
	assign	M0_RREADY			= master_en & ~bWrite;

	assign	M0_WADDR			= address[M_ADDR_WIDTH-1:0];
	assign	M0_WSIZE			= trans_size;
	assign	M0_WVALID			= master_en & bWrite;

	//// test design! for ProcessorTest app.
	assign	S_READY			= ~master_en;				// slave operation will be blocking when master bus is walking

	wire	data_trans		= bWrite ? (M0_WVALID & M0_WREADY) : (M0_RVALID & M0_RREADY);


	// master #1 (not used in here...)
	assign	M1_RREQ			= 'd0;
	assign	M1_RADDR		= 'd0;
	assign	M1_RSIZE		= 'd0;
	assign	M1_RREADY		= 'd0;
	assign	M1_WREQ			= 'd0;
	assign	M1_WADDR		= 'd0;
	assign	M1_WSIZE		= 'd0;
	assign	M1_WDATA		= 'd0;
	assign	M1_WVALID		= 'd0;

	// master for slave (not used in here...)
	assign	MS_RREQ			= 'd0;
	assign	MS_RADDR		= 'd0;
	assign	MS_RREADY		= 'd0;
	assign	MS_WREQ			= 'd0;
	assign	MS_WADDR		= 'd0;
	assign	MS_WDATA		= 'd0;
	assign	MS_WVALID		= 'd0;

	// implementation ------------------------------------------------------------
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			INTR		<= `FALSE;
			S_RDATA		<= 32'h0BEEF000;

			M0_RREQ		<= `FALSE;
			M0_WREQ		<= `FALSE;
			bWrite		<= `FALSE;
			trans_size	<= 'd0;
			count		<= 'd0;
			address		<= 64'd0;
			master_en	<= `FALSE;

			M0_WDATA	<= {
				32'h000F000F, 32'h000E000E, 32'h000D000D, 32'h000C000C,
				32'h000B000B, 32'h000A000A, 32'h00090009, 32'h00080008,
				32'h00070007, 32'h00060006, 32'h00050005, 32'h00040004,
				32'h00030003, 32'h00020002, 32'h00010001, 32'h00000000
			};
		end
		else begin
			if(S_EN & S_WE & (S_ADDR == 'd4)) begin
				INTR		<= S_WDATA[0];	// Invoke interrupt
			end

			if(S_EN & ~S_WE)	begin // slave read test
				S_RDATA		<= {INTR, S_RDATA[30:0] + 1'b1};
			end

			if(S_EN && S_WE) begin	// slave write test
				if(S_ADDR == 'd0)	// set address
					address[31:0]	<= S_WDATA;
				if(S_ADDR == 'd1)	// set address
					address[63:32]	<= S_WDATA;
				if(S_ADDR == 'd2) begin	// set size
					trans_size	<= S_WDATA[`RANGE_BYTE] - 1'b1;
					count		<= S_WDATA[`RANGE_BYTE] - 1'b1;
				end
				if(S_ADDR == 'd3) begin	// start
					M0_RREQ		<= ~S_WDATA[0];
					M0_WREQ		<= S_WDATA[0];
					bWrite		<= S_WDATA[0];
					master_en	<= `TRUE;
				end
			end
			else begin
				if(M0_RREQ && M0_RGRANT)
					M0_RREQ	<= `FALSE;
				if(M0_WREQ && M0_WGRANT)
					M0_WREQ	<= `FALSE;

				if(master_en & data_trans) begin
					M0_WDATA[(32* 1)-1:32* 0]	<= M0_WDATA[(32* 1)-1:32* 0] + 32'h10;
					M0_WDATA[(32* 2)-1:32* 1]	<= M0_WDATA[(32* 2)-1:32* 1] + 32'h10;
					M0_WDATA[(32* 3)-1:32* 2]	<= M0_WDATA[(32* 3)-1:32* 2] + 32'h10;
					M0_WDATA[(32* 4)-1:32* 3]	<= M0_WDATA[(32* 4)-1:32* 3] + 32'h10;
					M0_WDATA[(32* 5)-1:32* 4]	<= M0_WDATA[(32* 5)-1:32* 4] + 32'h10;
					M0_WDATA[(32* 6)-1:32* 5]	<= M0_WDATA[(32* 6)-1:32* 5] + 32'h10;
					M0_WDATA[(32* 7)-1:32* 6]	<= M0_WDATA[(32* 7)-1:32* 6] + 32'h10;
					M0_WDATA[(32* 8)-1:32* 7]	<= M0_WDATA[(32* 8)-1:32* 7] + 32'h10;

					M0_WDATA[(32* 9)-1:32* 8]	<= M0_WDATA[(32* 9)-1:32* 8] + 32'h10;
					M0_WDATA[(32*10)-1:32* 9]	<= M0_WDATA[(32*10)-1:32* 9] + 32'h10;
					M0_WDATA[(32*11)-1:32*10]	<= M0_WDATA[(32*11)-1:32*10] + 32'h10;
					M0_WDATA[(32*12)-1:32*11]	<= M0_WDATA[(32*12)-1:32*11] + 32'h10;
					M0_WDATA[(32*13)-1:32*12]	<= M0_WDATA[(32*13)-1:32*12] + 32'h10;
					M0_WDATA[(32*14)-1:32*13]	<= M0_WDATA[(32*14)-1:32*13] + 32'h10;
					M0_WDATA[(32*15)-1:32*14]	<= M0_WDATA[(32*15)-1:32*14] + 32'h10;
					M0_WDATA[(32*16)-1:32*15]	<= M0_WDATA[(32*16)-1:32*15] + 32'h10;
				end

				if(master_en & data_trans) begin
					if(|count) begin
						count	<= count - 'd1;
					end
					else begin
						master_en	<= `FALSE;
					end
				end
			end
		end
	end

endmodule
