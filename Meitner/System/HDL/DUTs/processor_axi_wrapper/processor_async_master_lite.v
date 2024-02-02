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

module processor_async_master_lite #(
		parameter		ADDR_WIDTH				= 32,
		parameter		FIFO_DEPTH				= 2
	) (
		//// system
		input									CLK,				// clock
		input									nRST,				// reset (active low)

		//// Simplified master bus interface
		// read
		output									MR_REQ,				// request
		input									MR_GRANT,			// request grant
		output	[ADDR_WIDTH-1:0]				MR_ADDR,			// address
		input	[31:0]							MR_DATA,			// read data
		input									MR_VALID,			// read data validation
		output									MR_READY,			// read operation is ready
		// write
		output									MW_REQ,				// request
		input									MW_GRANT,			// request grant
		output	[ADDR_WIDTH-1:0]				MW_ADDR,			// address
		output	[31:0]							MW_DATA,			// write data
		output									MW_VALID,			// write data validation
		input									MW_READY,			// write operation is ready

		//// async. master for DUT interface
		input									P_CLK,				// clock
		input									P_nRST,				// reset (active low)
		// read address
		input									P_MR_REQ,			// request
		input	[ADDR_WIDTH-1:0]				P_MR_ADDR,			// address
		output									P_MR_GRANT,			// request grant
		// read data
		output	[31:0]							P_MR_DATA,			// read data
		output									P_MR_VALID,			// read data validation
		input									P_MR_READY,			// read operation is ready
		// write address
		input									P_MW_REQ,			// request
		input	[ADDR_WIDTH-1:0]				P_MW_ADDR,			// address
		output									P_MW_GRANT,			// request grant
		// write data
		input	[31:0]							P_MW_DATA,			// write data
		input									P_MW_VALID,			// write data validation
		output									P_MW_READY			// write operation is ready
	);

	// definition & assignment ---------------------------------------------------
	// read/write address
	wire							fifo_ra_empty, fifo_ra_full;	// read address
	wire							fifo_wa_empty, fifo_wa_full;	// write address
	wire							fifo_w_push;					// write fifo push

	assign	MR_REQ					= ~fifo_ra_empty;
	assign	P_MR_GRANT				= ~fifo_ra_full;
	assign	MW_REQ					= ~fifo_wa_empty;
	assign	P_MW_GRANT				= ~fifo_wa_full;
	assign	fifo_w_push				= P_MW_REQ & P_MW_GRANT;

	// read/write data
	wire							fifo_rd_empty, fifo_rd_full;	// read data
	wire							fifo_wd_empty, fifo_wd_full;	// write data

	assign	MR_READY				= ~fifo_rd_full;
	assign	P_MR_VALID				= ~fifo_rd_empty;

	assign	MW_VALID				= ~fifo_wd_empty;
	assign	P_MW_READY				= ~fifo_wd_full;

	// implementation ------------------------------------------------------------
	// read address fifo
	FiFo_Async #(
		.FIFO_DEPTH		(FIFO_DEPTH),
		.DATA_WIDTH		(ADDR_WIDTH)
	) read_req_fifo (
		// push interface
		.W_CLK			(P_CLK),
		.W_nRST			(P_nRST),
		.W_nEN			(~P_MR_REQ),
		.W_DATA			(P_MR_ADDR),
		.W_FULL			(fifo_ra_full),
		// pop interface
		.R_CLK			(CLK),
		.R_nRST			(nRST & P_nRST),
		.R_nEN			(~MR_GRANT),
		.R_DATA			(MR_ADDR),
		.R_EMPTY		(fifo_ra_empty)
	);

	// read data fifo
	FiFo_Async #(
		.FIFO_DEPTH		(FIFO_DEPTH),
		.DATA_WIDTH		(32)
	) read_data_fifo (
		// push interface
		.W_CLK			(CLK),
		.W_nRST			(nRST & P_nRST),
		.W_nEN			(~MR_VALID),
		.W_DATA			(MR_DATA),
		.W_FULL			(fifo_rd_full),
		// pop interface
		.R_CLK			(P_CLK),
		.R_nRST			(P_nRST),
		.R_nEN			(~P_MR_READY),
		.R_DATA			(P_MR_DATA),
		.R_EMPTY		(fifo_rd_empty)
	);

	// write address fifo
	FiFo_Async #(
		.FIFO_DEPTH		(FIFO_DEPTH),
		.DATA_WIDTH		(ADDR_WIDTH)
	) write_req_fifo (
		// push interface
		.W_CLK			(P_CLK),
		.W_nRST			(P_nRST),
		.W_nEN			(~fifo_w_push),
		.W_DATA			(P_MW_ADDR),
		.W_FULL			(fifo_wa_full),
		// pop interface
		.R_CLK			(CLK),
		.R_nRST			(nRST & P_nRST),
		.R_nEN			(~MW_GRANT),
		.R_DATA			(MW_ADDR),
		.R_EMPTY		(fifo_wa_empty)
	);

	// write data fifo
	FiFo_Async #(
		.FIFO_DEPTH		(FIFO_DEPTH),
		.DATA_WIDTH		(32)
	) write_data_fifo (
		// push interface
		.W_CLK			(P_CLK),
		.W_nRST			(P_nRST),
		.W_nEN			(~P_MW_VALID),
		.W_DATA			(P_MW_DATA),
		.W_FULL			(fifo_wd_full),
		// pop interface
		.R_CLK			(CLK),
		.R_nRST			(nRST & P_nRST),
		.R_nEN			(~MW_READY),
		.R_DATA			(MW_DATA),
		.R_EMPTY		(fifo_wd_empty)
	);

endmodule
