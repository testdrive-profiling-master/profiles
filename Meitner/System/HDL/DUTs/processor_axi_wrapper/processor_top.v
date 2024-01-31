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
// Rev.  : 1/31/2024 Wed (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

// test example
module processor_top #(
		parameter	S_ADDR_WIDTH		= 4,
		parameter	M_ADDR_WIDTH		= 32,
		parameter	M_DATA_WIDTH		= 512
	) (
		//// system
		input							CLK, nRST,			// clock & reset (active low)
		output							BUSY,				// processor is busy
		output	reg						INTR,				// interrupt signal

		//// slave interface
		input							P_EN,				// slave enable
		input							P_WE,				// write enable
		input	[S_ADDR_WIDTH-1:0]		P_ADDR,				// address
		input	[`RANGE_DWORD]			P_WDATA,			// write data
		output	reg [`RANGE_DWORD]		P_RDATA,			// read data
		output							P_READY,			// data ready

		//// master interface
		// read
		output	reg						MR_REQ,				// read request
		output	[M_ADDR_WIDTH-1:0]		MR_ADDR,			// read address
		output	[`RANGE_BYTE]			MR_SIZE,			// read size
		input							MR_GRANT,			// read grant
		// read data
		input	[M_DATA_WIDTH-1:0]		MR_DATA,			// read data
		input							MR_VALID,			// read validation
		output							MR_READY,			// read ready
		input							MR_LAST,			// read last
		// write address
		output	reg						MW_REQ,				// write request
		output	[M_ADDR_WIDTH-1:0]		MW_ADDR,			// write address
		output	[`RANGE_BYTE]			MW_SIZE,			// write size
		input							MW_GRANT,			// write grant
		// write data
		output	reg [M_DATA_WIDTH-1:0]	MW_DATA,			// write data
		output							MW_VALID,			// write validation
		input							MW_READY,			// write ready
		input							MW_LAST,			// write last
		//// extra interface
		output	[M_ADDR_WIDTH-1:0]		FRAME_BASE			// framebuffer override address
	);

	// definition & assignment ---------------------------------------------------
	// synthesis translate_off
`ifdef USE_TESTDRIVE

	initial
		SetSystemDescription("testing example DUT processor");
`endif
	// synthesis translate_on

	assign	BUSY				= `FALSE;					// not use this signal at this time

	// master
	reg							bWrite;						// master write operation?
	reg		[63:0]				address;					// physical address
	reg		[`RANGE_BYTE]		trans_size;					// (transfer size - 1)
	reg		[`RANGE_BYTE]		count;						// same as 'trans_size' for count down
	reg							master_en;					// master bus operation enable

	assign	MR_ADDR				= address[M_ADDR_WIDTH-1:0];
	assign	MR_SIZE				= trans_size;
	assign	MR_READY			= master_en & ~bWrite;

	assign	MW_ADDR				= address[M_ADDR_WIDTH-1:0];
	assign	MW_SIZE				= trans_size;
	assign	MW_VALID			= master_en & bWrite;

	//// test design! for ProcessorTest app.
	assign	P_READY			= ~master_en;				// slave operation will be blocking when master bus is walking

	assign	FRAME_BASE			= 'd0;

	wire	data_trans		= bWrite ? (MW_VALID & MW_READY) : (MR_VALID & MR_READY);

	// implementation ------------------------------------------------------------
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			INTR		<= `FALSE;
			P_RDATA		<= 32'h0BEEF000;

			MR_REQ		<= `FALSE;
			MW_REQ		<= `FALSE;
			bWrite		<= `FALSE;
			trans_size	<= 'd0;
			count		<= 'd0;
			address		<= 64'd0;
			master_en	<= `FALSE;

			MW_DATA		<= {
				32'h000F000F, 32'h000E000E, 32'h000D000D, 32'h000C000C,
				32'h000B000B, 32'h000A000A, 32'h00090009, 32'h00080008,
				32'h00070007, 32'h00060006, 32'h00050005, 32'h00040004,
				32'h00030003, 32'h00020002, 32'h00010001, 32'h00000000
			};
		end
		else begin
			if(P_EN & P_WE & (P_ADDR == 'd4)) begin
				INTR		<= P_WDATA[0];	// Invoke interrupt
			end

			if(P_EN & ~P_WE)	begin // slave read test
				P_RDATA		<= {INTR, P_RDATA[30:0] + 1'b1};
			end

			if(P_EN && P_WE) begin	// slave write test
				if(P_ADDR == 'd0)	// set address
					address[31:0]	<= P_WDATA;
				if(P_ADDR == 'd1)	// set address
					address[63:32]	<= P_WDATA;
				if(P_ADDR == 'd2) begin	// set size
					trans_size	<= P_WDATA[`RANGE_BYTE] - 1'b1;
					count		<= P_WDATA[`RANGE_BYTE] - 1'b1;
				end
				if(P_ADDR == 'd3) begin	// start
					MR_REQ		<= ~P_WDATA[0];
					MW_REQ		<= P_WDATA[0];
					bWrite		<= P_WDATA[0];
					master_en	<= `TRUE;
				end
			end
			else begin
				if(MR_REQ && MR_GRANT)
					MR_REQ	<= `FALSE;
				if(MW_REQ && MW_GRANT)
					MW_REQ	<= `FALSE;

				if(master_en & data_trans) begin
					MW_DATA[(32* 1)-1:32* 0]	<= MW_DATA[(32* 1)-1:32* 0] + 32'h10;
					MW_DATA[(32* 2)-1:32* 1]	<= MW_DATA[(32* 2)-1:32* 1] + 32'h10;
					MW_DATA[(32* 3)-1:32* 2]	<= MW_DATA[(32* 3)-1:32* 2] + 32'h10;
					MW_DATA[(32* 4)-1:32* 3]	<= MW_DATA[(32* 4)-1:32* 3] + 32'h10;
					MW_DATA[(32* 5)-1:32* 4]	<= MW_DATA[(32* 5)-1:32* 4] + 32'h10;
					MW_DATA[(32* 6)-1:32* 5]	<= MW_DATA[(32* 6)-1:32* 5] + 32'h10;
					MW_DATA[(32* 7)-1:32* 6]	<= MW_DATA[(32* 7)-1:32* 6] + 32'h10;
					MW_DATA[(32* 8)-1:32* 7]	<= MW_DATA[(32* 8)-1:32* 7] + 32'h10;

					MW_DATA[(32* 9)-1:32* 8]	<= MW_DATA[(32* 9)-1:32* 8] + 32'h10;
					MW_DATA[(32*10)-1:32* 9]	<= MW_DATA[(32*10)-1:32* 9] + 32'h10;
					MW_DATA[(32*11)-1:32*10]	<= MW_DATA[(32*11)-1:32*10] + 32'h10;
					MW_DATA[(32*12)-1:32*11]	<= MW_DATA[(32*12)-1:32*11] + 32'h10;
					MW_DATA[(32*13)-1:32*12]	<= MW_DATA[(32*13)-1:32*12] + 32'h10;
					MW_DATA[(32*14)-1:32*13]	<= MW_DATA[(32*14)-1:32*13] + 32'h10;
					MW_DATA[(32*15)-1:32*14]	<= MW_DATA[(32*15)-1:32*14] + 32'h10;
					MW_DATA[(32*16)-1:32*15]	<= MW_DATA[(32*16)-1:32*15] + 32'h10;
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
