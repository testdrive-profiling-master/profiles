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
// Title : processor AXI wrapper
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

module processor_async_master #(
	parameter		DATA_WIDTH				= 256,
	parameter		ADDR_FIFO_DEPTH			= 2,
	parameter		DATA_FIFO_DEPTH			= 4,
	parameter		USE_AXI4				= 1
) (
	//// system
	input									CLK,				// clock
	input									nRST,				// reset (active low)

	//// Simplified master bus interface
	// read
	output									MR_REQ,				// request
	input									MR_GRANT,			// request grant
	output	[`RANGE_DWORD]					MR_ADDR,			// address
	output	[((USE_AXI4+1)*4)-1:0]			MR_SIZE,			// transfer size(AXI3(4bit), AXI4(8bit))
	input	[DATA_WIDTH-1:0]				MR_DATA,			// read data
	input									MR_VALID,			// read data validation
	output									MR_READY,			// read operation is ready
	input									MR_LAST,			// last read data
	// write
	output									MW_REQ,				// request
	input									MW_GRANT,			// request grant
	output	[`RANGE_DWORD]					MW_ADDR,			// address
	output	[((USE_AXI4+1)*4)-1:0]			MW_SIZE,			// transfer size(AXI3(4bit), AXI4(8bit))
	output	[DATA_WIDTH-1:0]				MW_DATA,			// write data
	output									MW_VALID,			// write data validation
	input									MW_READY,			// write operation is ready
	input									MW_LAST,			// last write data

	//// async. master for DUT interface
	input									P_CLK,				// clock
	input									P_nRST,				// reset (active low)
	// read address
	input									P_MR_REQ,			// request
	input	[`RANGE_DWORD]					P_MR_ADDR,			// address
	input	[((USE_AXI4+1)*4)-1:0]			P_MR_SIZE,			// transfer size(AXI3(4bit), AXI4(8bit))
	output									P_MR_GRANT,			// request grant
	// read data
	output	[DATA_WIDTH-1:0]				P_MR_DATA,			// read data
	output									P_MR_VALID,			// read data validation
	input									P_MR_READY,			// read operation is ready
	output									P_MR_LAST,			// read last
	// write address
	input									P_MW_REQ,			// request
	input	[`RANGE_DWORD]					P_MW_ADDR,			// address
	input	[((USE_AXI4+1)*4)-1:0]			P_MW_SIZE,			// transfer size(AXI3(4bit), AXI4(8bit))
	output									P_MW_GRANT,			// request grant
	// write data
	input	[DATA_WIDTH-1:0]				P_MW_DATA,			// write data
	input									P_MW_VALID,			// write data validation
	output									P_MW_READY,			// write operation is ready
	output	reg								P_MW_LAST			// write last
);

// definition & assignment ---------------------------------------------------
// read/write address
wire							fifo_ra_empty, fifo_ra_full;	// read address
wire							fifo_wa_empty, fifo_wa_full;	// write address
wire							fifo_wl_empty, fifo_wl_full;	// write last
wire							fifo_w_push;					// write fifo push
wire	[((USE_AXI4+1)*4)-1:0]	fifo_wl_data;					// write last data
reg								write_en;						// write data operation enable
reg		[((USE_AXI4+1)*4)-1:0]	write_count;					// write data count
wire	[((USE_AXI4+1)*4)-1:0]	write_count_next;				// next write data count

assign	MR_REQ					= ~fifo_ra_empty;
assign	P_MR_GRANT				= ~fifo_ra_full;
assign	MW_REQ					= ~fifo_wa_empty;
assign	P_MW_GRANT				= (~fifo_wa_full) & (~fifo_wl_full);
assign	fifo_w_push				= P_MW_REQ & P_MW_GRANT;
assign	write_count_next		= write_count - 1'b1;

// read/write data
wire							fifo_rd_empty, fifo_rd_full;	// read data
wire							fifo_wd_empty, fifo_wd_full;	// write data

assign	MR_READY				= ~fifo_rd_full;
assign	P_MR_VALID				= ~fifo_rd_empty;

assign	MW_VALID				= ~fifo_wd_empty;
assign	P_MW_READY				= (~fifo_wd_full) & write_en;

// implementation ------------------------------------------------------------
// read address fifo
FiFo_Async #(
	.FIFO_DEPTH		(ADDR_FIFO_DEPTH),
	.DATA_WIDTH		(((USE_AXI4+1)*4) + `SIZE_DWORD)	// size + address
) read_req_fifo (
	// push interface
	.W_CLK			(P_CLK),
	.W_nRST			(P_nRST),
	.W_nEN			(~P_MR_REQ),
	.W_DATA			({P_MR_SIZE, P_MR_ADDR}),
	.W_FULL			(fifo_ra_full),
	// pop interface
	.R_CLK			(CLK),
	.R_nRST			(nRST & P_nRST),
	.R_nEN			(~MR_GRANT),
	.R_DATA			({MR_SIZE, MR_ADDR}),
	.R_EMPTY		(fifo_ra_empty)
);

// read data fifo
FiFo_Async #(
	.FIFO_DEPTH		(DATA_FIFO_DEPTH),
	.DATA_WIDTH		(DATA_WIDTH + 1)
) read_data_fifo (
	// push interface
	.W_CLK			(CLK),
	.W_nRST			(nRST & P_nRST),
	.W_nEN			(~MR_VALID),
	.W_DATA			({MR_DATA, MR_LAST}),
	.W_FULL			(fifo_rd_full),
	// pop interface
	.R_CLK			(P_CLK),
	.R_nRST			(P_nRST),
	.R_nEN			(~P_MR_READY),
	.R_DATA			({P_MR_DATA, P_MR_LAST}),
	.R_EMPTY		(fifo_rd_empty)
);

// write address fifo
FiFo_Async #(
	.FIFO_DEPTH		(ADDR_FIFO_DEPTH),
	.DATA_WIDTH		(((USE_AXI4+1)*4) + `SIZE_DWORD)	// size + address
) write_req_fifo (
	// push interface
	.W_CLK			(P_CLK),
	.W_nRST			(P_nRST),
	.W_nEN			(~fifo_w_push),
	.W_DATA			({P_MW_SIZE, P_MW_ADDR}),
	.W_FULL			(fifo_wa_full),
	// pop interface
	.R_CLK			(CLK),
	.R_nRST			(nRST & P_nRST),
	.R_nEN			(~MW_GRANT),
	.R_DATA			({MW_SIZE, MW_ADDR}),
	.R_EMPTY		(fifo_wa_empty)
);

// write data fifo
FiFo_Async #(
	.FIFO_DEPTH		(DATA_FIFO_DEPTH),
	.DATA_WIDTH		(DATA_WIDTH)
) write_data_fifo (
	// push interface
	.W_CLK			(P_CLK),
	.W_nRST			(P_nRST),
	.W_nEN			((~P_MW_VALID) | (~write_en)),
	.W_DATA			(P_MW_DATA),
	.W_FULL			(fifo_wd_full),
	// pop interface
	.R_CLK			(CLK),
	.R_nRST			(nRST & P_nRST),
	.R_nEN			(~MW_READY),
	.R_DATA			(MW_DATA),
	.R_EMPTY		(fifo_wd_empty)
);

// write last control
FiFo #(
	.FIFO_DEPTH		(ADDR_FIFO_DEPTH),
	.DATA_WIDTH		(((USE_AXI4+1)*4))
) write_size_fifo (
	.CLK			(P_CLK),
	.nCLR			(P_nRST),
	.nWE			(~fifo_w_push),
	.DIN			(P_MW_SIZE),
	.FULL			(fifo_wl_full),
	.nRE			(write_en),
	.DOUT			(fifo_wl_data),
	.EMPTY			(fifo_wl_empty)
);

always@(posedge P_CLK, negedge P_nRST) begin
	if(!P_nRST) begin
		write_en		<= `FALSE;
		write_count		<= 'd0;
		P_MW_LAST		<= `FALSE;
	end
	else begin
		if(write_en) begin
			if(P_MW_VALID & (~fifo_wd_full)) begin
				write_en		<= |write_count;
				write_count		<= write_count_next;
				P_MW_LAST		<= ~|write_count_next;
			end
		end
		else begin
			if(!fifo_wl_empty) begin
				write_en		<= `TRUE;
				write_count		<= fifo_wl_data;
				P_MW_LAST		<= ~|fifo_wl_data;
			end
		end
	end
end
endmodule
