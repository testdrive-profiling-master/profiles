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
// Title : processor AXI wrapper
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"

// test example
module processor_top #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 512
) (
	//// system
	input							CLK, nRST,			// clock & reset (active low)
	output							BUSY,				// processor is busy
	output	reg						INTR,				// interrupt signal

	//// slave interface
	input							P_EN,				// slave enable
	input							P_WE,				// write enable
	input	[ADDR_WIDTH-1:0]		P_ADDR,				// address
	input	[`RANGE_DWORD]			P_WDATA,			// write data
	output	reg [`RANGE_DWORD]		P_RDATA,			// read data
	output							P_READY,			// data ready

	//// master interface
	// read
	output	reg						MR_REQ,				// read request
	output	[`RANGE_DWORD]			MR_ADDR,			// read address
	output	[`RANGE_BYTE]			MR_SIZE,			// read size
	input							MR_GRANT,			// read grant
	// read data
	input	[DATA_WIDTH-1:0]		MR_DATA,			// read data
	input							MR_VALID,			// read validation
	output							MR_READY,			// read ready
	input							MR_LAST,			// read last
	// write address
	output	reg						MW_REQ,				// write request
	output	[`RANGE_DWORD]			MW_ADDR,			// write address
	output	[`RANGE_BYTE]			MW_SIZE,			// write size
	input							MW_GRANT,			// write grant
	// write data
	output	reg [DATA_WIDTH-1:0]	MW_DATA,			// write data
	output							MW_VALID,			// write validation
	input							MW_READY,			// write ready
	input							MW_LAST				// write last
);

// definition & assignment ---------------------------------------------------
// synthesis translate_off
`ifdef USE_TESTDRIVE
initial
	SetSystemDescription("testing example DUT processor");
`endif
// synthesis translate_on

assign	BUSY			= `FALSE;					// not use this signal at this time

// master
reg						bWrite;						// master write operation?
reg		[31:0]			address;					// physical address
reg		[`RANGE_BYTE]	trans_size;					// (transfer size - 1)
reg		[`RANGE_BYTE]	count;						// same as 'trans_size' for count down
reg						master_en;					// master bus operation enable

assign	MR_ADDR			= address;
assign	MR_SIZE			= trans_size;
assign	MR_READY		= master_en & ~bWrite;

assign	MW_ADDR			= address;
assign	MW_SIZE			= trans_size;
assign	MW_VALID		= master_en & bWrite;

//// test design! for ProcessorTest app.
assign	P_READY			= ~master_en;				// slave operation will be blocking when master bus is walking

wire	data_trans		= bWrite ? (MW_VALID & MW_READY) : (MR_VALID & MR_READY);

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		INTR		<= `FALSE;
		P_RDATA		<= 32'hBEEF0000;

		MR_REQ		<= `FALSE;
		MW_REQ		<= `FALSE;
		bWrite		<= `FALSE;
		count		<= 'd0;
		address		<= 'd0;
		master_en	<= `FALSE;

		MW_DATA		<= {
			32'h000F000F, 32'h000E000E, 32'h000D000D, 32'h000C000C,
			32'h000B000B, 32'h000A000A, 32'h00090009, 32'h00080008,
			32'h00070007, 32'h00060006, 32'h00050005, 32'h00040004,
			32'h00030003, 32'h00020002, 32'h00010001, 32'h00000000
		};
	end
	else begin
		INTR		<= P_EN & P_WE & (P_ADDR == 'd4) & P_WDATA[0];	// Invoke interrupt

		if(P_EN & ~P_WE & MR_READY)	begin // slave read test
			P_RDATA		<= P_RDATA + 1'b1;
		end

		if(P_EN && P_WE) begin	// slave write test
			if(P_ADDR == 'd0)	// set address
				address		<= P_WDATA;
			if(P_ADDR == 'd1) begin	// set size
				trans_size	<= P_WDATA[`RANGE_BYTE] - 1'b1;
				count		<= P_WDATA[`RANGE_BYTE] - 1'b1;
			end
			if(P_ADDR == 'd2) begin	// start
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
				MW_DATA[(32* 1)-1:32* 0]	<= MW_DATA[(32* 1)-1:32* 0] + 5'h10;
				MW_DATA[(32* 2)-1:32* 1]	<= MW_DATA[(32* 2)-1:32* 1] + 5'h10;
				MW_DATA[(32* 3)-1:32* 2]	<= MW_DATA[(32* 3)-1:32* 2] + 5'h10;
				MW_DATA[(32* 4)-1:32* 3]	<= MW_DATA[(32* 4)-1:32* 3] + 5'h10;
				MW_DATA[(32* 5)-1:32* 4]	<= MW_DATA[(32* 5)-1:32* 4] + 5'h10;
				MW_DATA[(32* 6)-1:32* 5]	<= MW_DATA[(32* 6)-1:32* 5] + 5'h10;
				MW_DATA[(32* 7)-1:32* 6]	<= MW_DATA[(32* 7)-1:32* 6] + 5'h10;
				MW_DATA[(32* 8)-1:32* 7]	<= MW_DATA[(32* 8)-1:32* 7] + 5'h10;

				MW_DATA[(32* 9)-1:32* 8]	<= MW_DATA[(32* 9)-1:32* 8] + 5'h10;
				MW_DATA[(32*10)-1:32* 9]	<= MW_DATA[(32*10)-1:32* 9] + 5'h10;
				MW_DATA[(32*11)-1:32*10]	<= MW_DATA[(32*11)-1:32*10] + 5'h10;
				MW_DATA[(32*12)-1:32*11]	<= MW_DATA[(32*12)-1:32*11] + 5'h10;
				MW_DATA[(32*13)-1:32*12]	<= MW_DATA[(32*13)-1:32*12] + 5'h10;
				MW_DATA[(32*14)-1:32*13]	<= MW_DATA[(32*14)-1:32*13] + 5'h10;
				MW_DATA[(32*15)-1:32*14]	<= MW_DATA[(32*15)-1:32*14] + 5'h10;
				MW_DATA[(32*16)-1:32*15]	<= MW_DATA[(32*16)-1:32*15] + 5'h10;
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
