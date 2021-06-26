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
// Title : Processor
// Rev.  : 6/26/2021 Sat (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`include "template/top_axi.sv"
`include "template/axi_wrapper.v"

module dut_top_ex(
	//// system ----------------------------
	input									CLK,				// clock
	input									nRST,				// reset (active low)
	output									BUSY,				// processor is busy
	output									INTR,				// interrupt signal

	//// slave interface -------------------
	// read
	input									SR_REQ,
	output									SR_VALID,
	input	[`RANGE_BYTE]					SR_ADDR,
	output	reg [`RANGE_DWORD]				SR_DATA,			// 1 clock delayed output
	// write
	input									SW_REQ,
	output									SW_VALID,
	input	[`RANGE_BYTE]					SW_ADDR,
	input	[`RANGE_DWORD]					SW_DATA,			// enable synchronized output

	//// master interface ------------------
	// read
	output	reg								MR_REQ,
	input									MR_GRANT,
	output	[`RANGE_DWORD]					MR_ADDR,
	output	[`RANGE_BYTE]					MR_SIZE,
	input	[`RANGE_DWORDx4]				MR_DATA,
	input									MR_VALID,
	output									MR_READY,
	input									MR_LAST,
	// write
	output	reg								MW_REQ,
	input									MW_GRANT,
	output	[`RANGE_DWORD]					MW_ADDR,
	output	[`RANGE_BYTE]					MW_SIZE,
	output	[`RANGE_DWORDx4]				MW_DATA,
	output									MW_VALID,
	input									MW_READY,
	input									MW_LAST
);

// definition & assignment ---------------------------------------------------
reg					cmd_en;
reg					cmd_write;
reg	[`RANGE_DWORD]	cmd_address;
reg	[`RANGE_BYTE]	cmd_burst_size;

reg	[`RANGE_BYTE]	mw_data;

wire	cmd_wen		= cmd_en & cmd_write;
wire	cmd_ren		= cmd_en & (~cmd_write);


assign	BUSY		= 1'b0;
assign	INTR		= MR_LAST | MW_LAST;

// master
assign	MR_ADDR		= cmd_address;
assign	MR_SIZE		= cmd_burst_size;
assign	MR_READY	= 1'b1;

assign	MW_ADDR		= cmd_address;
assign	MW_SIZE		= cmd_burst_size;
assign	MW_DATA		= {
	24'h300000, mw_data,
	24'h200000, mw_data,
	24'h100000, mw_data,
	24'h000000, mw_data
};
assign	MW_VALID	= 1'b1;

assign	SR_VALID	= SR_REQ;
assign	SW_VALID	= SW_REQ;

`ifdef USE_TESTDRIVE	// for infomation filter
wire	[41:0]	sr_info		= SR_REQ ? {1'b0, SR_REQ, SR_ADDR, SR_DATA} : 0;//42'dz;
wire	[41:0]	sw_info		= SW_REQ ? {1'b1, SW_REQ, SW_ADDR, SW_DATA} : 0;//42'dz;
`endif

// implementation ------------------------------------------------------------
// slave test for master operation
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		SR_DATA			<= 32'b0;

		cmd_en			<= 1'b0;
		cmd_write		<= 1'b0;
		cmd_address		<= 32'h0;
		cmd_burst_size	<= 8'b0;
	end
	else begin
		if(SR_REQ) begin
			$display("READ(%d) = 0x%X", SR_ADDR, SR_DATA);
			SR_DATA		<= SR_DATA + 1;
		end

		if(SW_REQ) begin
			$display("WRITE(%d) = 0x%X", SW_ADDR, SW_DATA);

			if(SW_ADDR==8'h00) begin	// set address
				cmd_address	<= SW_DATA;
				$display("cmd : target address = 0x%X", SW_DATA);
			end

			if(SW_ADDR==8'h01) begin	// set burst size
				cmd_burst_size	<= SW_DATA[`RANGE_BYTE];
				$display("cmd : burst_size = 0x%X", SW_DATA[`RANGE_BYTE]);
			end

			if(SW_ADDR==8'h02) begin	// set write or read
				cmd_write	<= SW_DATA[0];
				cmd_en		<= 1'b1;
				$display("cmd : write = %d", SW_DATA[0]);
			end
		end
		else begin
			cmd_en	<= 1'b0;
		end
	end
end

// master test
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		MR_REQ		<= 1'b0;

		MW_REQ		<= 1'b0;
		mw_data		<= 8'b0;
	end
	else begin
		MR_REQ		<= cmd_ren | (MR_REQ & (~MR_GRANT));

		MW_REQ		<= cmd_wen | (MW_REQ & (~MW_GRANT));
		mw_data		<= cmd_wen	? 0 :
		MW_READY	? mw_data + 1'b1 :
		mw_data;
	end
end

endmodule
