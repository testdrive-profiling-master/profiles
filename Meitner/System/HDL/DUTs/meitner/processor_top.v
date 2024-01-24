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
// Title : Processor
// Rev.  : 1/24/2024 Wed (clonextop@gmail.com)
//================================================================================
`include "DUTs/meitner/includes.vh"
`define NO_TEST_DESIGN		// overriding test top design

`ifdef USE_TESTDRIVE
`include "DUTs/processor_axi_wrapper/top.sv"
`else
`include "DUTs/processor_axi_wrapper/includes.vh"
`endif
/* TOP_MODULE = processor_axi_wrapper */
// *** Do not modify to here.

module processor_top #(
		parameter	S_ADDR_WIDTH		= 4,
		parameter	M_ADDR_WIDTH		= 32,
		parameter	M_DATA_WIDTH		= 512
	) (
		//// system
		input							CLK, nRST,			// clock & reset (active low)
		output							BUSY,				// processor is busy
		output							INTR,				// interrupt signal

		//// slave interface
		input							P_EN,				// slave enable
		input							P_WE,				// write enable
		input	[S_ADDR_WIDTH-1:0]		P_ADDR,				// address
		input	[`RANGE_DWORD]			P_WDATA,			// write data
		output	reg [`RANGE_DWORD]		P_RDATA,			// read data
		output							P_READY,			// data ready

		//// master interface
		// read
		output							MR_REQ,				// read request
		output	[M_ADDR_WIDTH-1:0]		MR_ADDR,			// read address
		output	[`RANGE_BYTE]			MR_SIZE,			// read size
		input							MR_GRANT,			// read grant
		// read data
		input	[M_DATA_WIDTH-1:0]		MR_DATA,			// read data
		input							MR_VALID,			// read validation
		output							MR_READY,			// read ready
		input							MR_LAST,			// read last
		// write address
		output							MW_REQ,				// write request
		output	[M_ADDR_WIDTH-1:0]		MW_ADDR,			// write address
		output	[`RANGE_BYTE]			MW_SIZE,			// write size
		input							MW_GRANT,			// write grant
		// write data
		output	[M_DATA_WIDTH-1:0]		MW_DATA,			// write data
		output							MW_VALID,			// write validation
		input							MW_READY,			// write ready
		input							MW_LAST				// write last
	);

	// definition & assignment ---------------------------------------------------
`ifdef USE_TESTDRIVE

	initial
		SetSystemDescription("Meitner");
`endif

	// life-cycle control (not using here...)
	assign	BUSY			= `FALSE;
	assign	INTR			= `FALSE;

	// slave non-blocking Slave2Processor write
	assign	P_READY			= `TRUE;					// non-blocking for Slave2Processor write

	// master (not using here... just bypass)
	assign	MR_REQ			= `FALSE;
	assign	MR_READY		= `FALSE;
	assign	MW_REQ			= `FALSE;
	assign	MW_VALID		= `FALSE;

	// implementation ------------------------------------------------------------
	// slave counter up design
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			P_RDATA		<= 'd0;
		end
		else begin
			if(P_EN) begin
				if(P_WE) begin
					P_RDATA		<= P_WDATA;
				end
				else begin
					P_RDATA		<= P_RDATA + 'd1;
				end
			end
		end
	end

endmodule
