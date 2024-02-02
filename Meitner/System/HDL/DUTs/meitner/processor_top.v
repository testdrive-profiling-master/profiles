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
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
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
		parameter	S_ADDR_WIDTH		= 4,				// slave address width
		parameter	M_ADDR_WIDTH		= 32,				// master address width
		parameter	M_DATA_WIDTH		= 512,				// master data width
		parameter	MS_ADDR_WIDTH		= 20				// AXI-LITE address width
	) (
		//// system
		input							CLK, nRST,			// clock & reset (active low)
		output							BUSY,				// processor is busy
		output							INTR,				// interrupt signal

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
		// read
		output							M0_RREQ,			// read request
		output	[M_ADDR_WIDTH-1:0]		M0_RADDR,			// read address
		output	[`RANGE_BYTE]			M0_RSIZE,			// read size
		input							M0_RGRANT,			// read grant
		// read data
		input	[M_DATA_WIDTH-1:0]		M0_RDATA,			// read data
		input							M0_RVALID,			// read validation
		output							M0_RREADY,			// read ready
		input							M0_RLAST,			// read last
		// write address
		output							M0_WREQ,			// write request
		output	[M_ADDR_WIDTH-1:0]		M0_WADDR,			// write address
		output	[`RANGE_BYTE]			M0_WSIZE,			// write size
		input							M0_WGRANT,			// write grant
		// write data
		output	[M_DATA_WIDTH-1:0]		M0_WDATA,			// write data
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
`ifdef USE_TESTDRIVE

	initial
		SetSystemDescription("Meitner");
`endif

	// life-cycle control (not using here...)
	assign	BUSY			= `FALSE;
	assign	INTR			= `FALSE;

	// slave non-blocking Slave2Processor write
	assign	S_READY			= `TRUE;					// non-blocking for Slave2Processor write

	// master (not used in here...)
	assign	M0_RREQ			= `FALSE;
	assign	M0_RREADY		= `FALSE;
	assign	M0_WREQ			= `FALSE;
	assign	M0_WVALID		= `FALSE;
	assign	M1_RREQ			= `FALSE;
	assign	M1_RREADY		= `FALSE;
	assign	M1_WREQ			= `FALSE;
	assign	M1_WVALID		= `FALSE;

	// master for slave (not used in here...)
	assign	MS_RREQ			= `FALSE;
	assign	MS_RREADY		= `FALSE;
	assign	MS_WREQ			= `FALSE;
	assign	MS_WVALID		= `FALSE;

	// implementation ------------------------------------------------------------
	// slave counter up design
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			S_RDATA		<= 'd0;
		end
		else begin
			if(S_EN) begin
				if(S_WE) begin
					S_RDATA		<= S_WDATA;
				end
				else begin
					S_RDATA		<= S_RDATA + 'd1;
				end
			end
		end
	end

endmodule
