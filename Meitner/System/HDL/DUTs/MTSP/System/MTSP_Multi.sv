//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : MTSP(Multi-thread Stream Processor) 'Meitner' v1.1
// Rev.  : 7/20/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_Multi #(
	parameter	CORE_SIZE			= 4
) (
	// System
	input							CLK, nRST,					// main clock & reset
	output							BUSY,

	i_local_memory_control.s		local_memory,				// local memory control (LMB)

	// External thread control
	i_thread_control.s				thread_command,				// thread control
	i_memory_command.m				mem_command					// memory command
);
// synopsys template

// definition & assignment ---------------------------------------------------
genvar		index;				// generate index
integer		i;					// temporary index

// thread control
wire	[(CORE_SIZE-1):0]		core_busy;

// global scratch counters
wire	[`RANGE_MASK4D]			w_GSCs_RST[(CORE_SIZE-1):0];	// reset mask
wire	[`RANGE_MASK4D]			w_GSCs_INC[(CORE_SIZE-1):0];	// increase mask
wire	[`RANGE_WORDx4]			w_GSCs_DATA[(CORE_SIZE-1):0];	// data

// bus control
i_memory_command				mem_command_core[(CORE_SIZE-1):0];
i_local_memory_control			lmb_single[(CORE_SIZE-1):0];
i_sync							core_sync[(CORE_SIZE-1):0];		// sync. of multi-core

// implementation ------------------------------------------------------------
// Processor installation
generate
for(index=0;index<CORE_SIZE;index=index+1) begin : multi_core
	MTSP_Single #(
		.CORE_ID			(index)
	) single_inst (
		.CLK				(CLK),
		.nRST				(nRST),
		.BUSY				(core_busy[index]),
		.local_memory		(lmb_single[index]),
		.thread_command		(thread_command),
		.core_sync			(core_sync[index]),
		.GSCs_RST			(w_GSCs_RST[index]),
		.GSCs_INC			(w_GSCs_INC[index]),
		.GSCs_DATA			(w_GSCs_DATA[index]),
		.mem_command		(mem_command_core[index])
	);
end
endgenerate

// multi-core busy control
MTSP_Busy #(
	.CORE_SIZE		(CORE_SIZE)
) multicore_busy (
	.CLK			(CLK),
	.nRST			(nRST),
	.BUSY			(BUSY),
	.CORE_BUSY		(core_busy)
);

//----------------------------------------------------------------------------
// global scratch counter
//----------------------------------------------------------------------------
// data
reg		[`RANGE_WORDx4]			r_GSCs_DATA;
// reset
reg		[`RANGE_MASK4D]			r_GSCs_RST_0[(CORE_SIZE/4-1):0];
reg		[`RANGE_MASK4D]			r_GSCs_RST;
// increase
reg		[(CORE_SIZE-1):0]		r_GSCs_INC_Q1X;							// sum of quarter 1 bit
reg		[(CORE_SIZE-1):0]		r_GSCs_INC_Q1Y;
reg		[(CORE_SIZE-1):0]		r_GSCs_INC_Q1Z;
reg		[(CORE_SIZE-1):0]		r_GSCs_INC_Q1W;
reg		[1:0]					r_GSCs_INC_Q2X[(CORE_SIZE/4-1):0];	// sum of quarter 2 bits
reg		[1:0]					r_GSCs_INC_Q2Y[(CORE_SIZE/4-1):0];
reg		[1:0]					r_GSCs_INC_Q2Z[(CORE_SIZE/4-1):0];
reg		[1:0]					r_GSCs_INC_Q2W[(CORE_SIZE/4-1):0];
reg		[1:0]					r_GSCs_INC_Q3X[(CORE_SIZE/4-1):0];	// sum of quarter 3 bits
reg		[1:0]					r_GSCs_INC_Q3Y[(CORE_SIZE/4-1):0];
reg		[1:0]					r_GSCs_INC_Q3Z[(CORE_SIZE/4-1):0];
reg		[1:0]					r_GSCs_INC_Q3W[(CORE_SIZE/4-1):0];
reg		[2:0]					r_GSCs_INC_Q4X[(CORE_SIZE/4-1):0];	// sum of quarter 4 bits
reg		[2:0]					r_GSCs_INC_Q4Y[(CORE_SIZE/4-1):0];
reg		[2:0]					r_GSCs_INC_Q4Z[(CORE_SIZE/4-1):0];
reg		[2:0]					r_GSCs_INC_Q4W[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q2X[(CORE_SIZE/4-1):0];	// sum of quarter 2 bits wired
wire	[1:0]					w_GSCs_INC_Q2Y[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q2Z[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q2W[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q3X[(CORE_SIZE/4-1):0];	// sum of quarter 3 bits wired
wire	[1:0]					w_GSCs_INC_Q3Y[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q3Z[(CORE_SIZE/4-1):0];
wire	[1:0]					w_GSCs_INC_Q3W[(CORE_SIZE/4-1):0];
wire	[2:0]					w_GSCs_INC_Q4X[(CORE_SIZE/4-1):0];	// sum of quarter 4 bits wired
wire	[2:0]					w_GSCs_INC_Q4Y[(CORE_SIZE/4-1):0];
wire	[2:0]					w_GSCs_INC_Q4Z[(CORE_SIZE/4-1):0];
wire	[2:0]					w_GSCs_INC_Q4W[(CORE_SIZE/4-1):0];

reg		[4:0]					r_GSCs_INC_X[(CORE_SIZE-1):0];
reg		[4:0]					r_GSCs_INC_Y[(CORE_SIZE-1):0];
reg		[4:0]					r_GSCs_INC_Z[(CORE_SIZE-1):0];
reg		[4:0]					r_GSCs_INC_W[(CORE_SIZE-1):0];

generate
for(index=0;index<(CORE_SIZE/4);index=index+1) begin : gscs_sum
	assign	w_GSCs_INC_Q2X[index]	= w_GSCs_INC[(index*4)][`RANGE_MASK4D_X] + w_GSCs_INC[(index*4)+1][`RANGE_MASK4D_X];
	assign	w_GSCs_INC_Q2Y[index]	= w_GSCs_INC[(index*4)][`RANGE_MASK4D_Y] + w_GSCs_INC[(index*4)+1][`RANGE_MASK4D_Y];
	assign	w_GSCs_INC_Q2Z[index]	= w_GSCs_INC[(index*4)][`RANGE_MASK4D_Z] + w_GSCs_INC[(index*4)+1][`RANGE_MASK4D_Z];
	assign	w_GSCs_INC_Q2W[index]	= w_GSCs_INC[(index*4)][`RANGE_MASK4D_W] + w_GSCs_INC[(index*4)+1][`RANGE_MASK4D_W];

	assign	w_GSCs_INC_Q3X[index]	= w_GSCs_INC_Q2X[index] + w_GSCs_INC[(index*4)+2][`RANGE_MASK4D_X];
	assign	w_GSCs_INC_Q3Y[index]	= w_GSCs_INC_Q2Y[index] + w_GSCs_INC[(index*4)+2][`RANGE_MASK4D_Y];
	assign	w_GSCs_INC_Q3Z[index]	= w_GSCs_INC_Q2Z[index] + w_GSCs_INC[(index*4)+2][`RANGE_MASK4D_Z];
	assign	w_GSCs_INC_Q3W[index]	= w_GSCs_INC_Q2W[index] + w_GSCs_INC[(index*4)+2][`RANGE_MASK4D_W];

	assign	w_GSCs_INC_Q4X[index]	= w_GSCs_INC_Q3X[index] + w_GSCs_INC[(index*4)+3][`RANGE_MASK4D_X];
	assign	w_GSCs_INC_Q4Y[index]	= w_GSCs_INC_Q3Y[index] + w_GSCs_INC[(index*4)+3][`RANGE_MASK4D_Y];
	assign	w_GSCs_INC_Q4Z[index]	= w_GSCs_INC_Q3Z[index] + w_GSCs_INC[(index*4)+3][`RANGE_MASK4D_Z];
	assign	w_GSCs_INC_Q4W[index]	= w_GSCs_INC_Q3W[index] + w_GSCs_INC[(index*4)+3][`RANGE_MASK4D_W];
end
endgenerate

assign w_GSCs_DATA[0]	= r_GSCs_DATA;

generate
for(index=1;index<CORE_SIZE;index=index+1) begin : gscs_out
	assign	w_GSCs_DATA[index][`RANGE_WORDx4_X]	= r_GSCs_DATA[`RANGE_WORDx4_X] + r_GSCs_INC_X[(index-1)];
	assign	w_GSCs_DATA[index][`RANGE_WORDx4_Y]	= r_GSCs_DATA[`RANGE_WORDx4_Y] + r_GSCs_INC_Y[(index-1)];
	assign	w_GSCs_DATA[index][`RANGE_WORDx4_Z]	= r_GSCs_DATA[`RANGE_WORDx4_Z] + r_GSCs_INC_Z[(index-1)];
	assign	w_GSCs_DATA[index][`RANGE_WORDx4_W]	= r_GSCs_DATA[`RANGE_WORDx4_W] + r_GSCs_INC_W[(index-1)];
end
endgenerate

// reset
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		for(i=0;i<(CORE_SIZE/4);i=i+1)
			r_GSCs_RST_0[i]	<= `MASK4D_ALLUNMASKED;
		r_GSCs_RST		<= `MASK4D_ALLUNMASKED;
	end
	else begin
		for(i=0;i<(CORE_SIZE/4);i=i+1)
			r_GSCs_RST_0[i]	<= w_GSCs_RST[(i*4)]|w_GSCs_RST[(i*4)+1]|w_GSCs_RST[(i*4)+2]|w_GSCs_RST[(i*4)+3];

`ifdef MULTICORE_16

		r_GSCs_RST		<= r_GSCs_RST_0[0] | r_GSCs_RST_0[1] | r_GSCs_RST_0[2] | r_GSCs_RST_0[3];
`endif
`ifdef MULTICORE_12

		r_GSCs_RST		<= r_GSCs_RST_0[0] | r_GSCs_RST_0[1] | r_GSCs_RST_0[2];
`endif
`ifdef	MULTICORE_8

		r_GSCs_RST		<= r_GSCs_RST_0[0] | r_GSCs_RST_0[1];
`endif
`ifdef	MULTICORE_4

		r_GSCs_RST		<= r_GSCs_RST_0[0];
`endif

	end
end

// increase
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		for(i=0;i<(CORE_SIZE/4);i=i+1) begin
			{r_GSCs_INC_Q1X[i], r_GSCs_INC_Q1Y[i], r_GSCs_INC_Q1Z[i], r_GSCs_INC_Q1W[i]}	<= 4'b0;
			{r_GSCs_INC_Q2X[i], r_GSCs_INC_Q2Y[i], r_GSCs_INC_Q2Z[i], r_GSCs_INC_Q2W[i]}	<= 8'b0;
			{r_GSCs_INC_Q3X[i], r_GSCs_INC_Q3Y[i], r_GSCs_INC_Q3Z[i], r_GSCs_INC_Q3W[i]}	<= 8'b0;
			{r_GSCs_INC_Q4X[i], r_GSCs_INC_Q4Y[i], r_GSCs_INC_Q4Z[i], r_GSCs_INC_Q4W[i]}	<= 12'b0;
		end
		for(i=0;i<CORE_SIZE;i=i+1) begin
			r_GSCs_INC_X[i]		<= 5'b0;
			r_GSCs_INC_Y[i]		<= 5'b0;
			r_GSCs_INC_Z[i]		<= 5'b0;
			r_GSCs_INC_W[i]		<= 5'b0;
		end
	end
	else begin
		for(i=0;i<(CORE_SIZE/4);i=i+1) begin
			{r_GSCs_INC_Q1X[i], r_GSCs_INC_Q1Y[i], r_GSCs_INC_Q1Z[i], r_GSCs_INC_Q1W[i]}	<= w_GSCs_INC[(i*4)];
			{r_GSCs_INC_Q2X[i], r_GSCs_INC_Q2Y[i], r_GSCs_INC_Q2Z[i], r_GSCs_INC_Q2W[i]}	<= {w_GSCs_INC_Q2X[i], w_GSCs_INC_Q2Y[i], w_GSCs_INC_Q2Z[i], w_GSCs_INC_Q2W[i]};
			{r_GSCs_INC_Q3X[i], r_GSCs_INC_Q3Y[i], r_GSCs_INC_Q3Z[i], r_GSCs_INC_Q3W[i]}	<= {w_GSCs_INC_Q3X[i], w_GSCs_INC_Q3Y[i], w_GSCs_INC_Q3Z[i], w_GSCs_INC_Q3W[i]};
			{r_GSCs_INC_Q4X[i], r_GSCs_INC_Q4Y[i], r_GSCs_INC_Q4Z[i], r_GSCs_INC_Q4W[i]}	<= {w_GSCs_INC_Q4X[i], w_GSCs_INC_Q4Y[i], w_GSCs_INC_Q4Z[i], w_GSCs_INC_Q4W[i]};
		end
		// 코어 개수에 따라 아래의 주석을 풀어줘야 한다.
		r_GSCs_INC_X[0]		<= r_GSCs_INC_Q1X[0];
		r_GSCs_INC_X[1]		<= r_GSCs_INC_Q2X[0];
		r_GSCs_INC_X[2]		<= r_GSCs_INC_Q3X[0];
		r_GSCs_INC_X[3]		<= r_GSCs_INC_Q4X[0];
		/*
				r_GSCs_INC_X[4]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q1X[1];
				r_GSCs_INC_X[5]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q2X[1];
				r_GSCs_INC_X[6]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q3X[1];
				r_GSCs_INC_X[7]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1];
				/*
				r_GSCs_INC_X[8]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q1X[2];
				r_GSCs_INC_X[9]		<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q2X[2];
				r_GSCs_INC_X[10]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q3X[2];
				r_GSCs_INC_X[11]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q4X[2];
				r_GSCs_INC_X[12]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q4X[2] + r_GSCs_INC_Q1X[3];
				r_GSCs_INC_X[13]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q4X[2] + r_GSCs_INC_Q2X[3];
				r_GSCs_INC_X[14]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q4X[2] + r_GSCs_INC_Q3X[3];
				r_GSCs_INC_X[15]	<= r_GSCs_INC_Q4X[0] + r_GSCs_INC_Q4X[1] + r_GSCs_INC_Q4X[2] + r_GSCs_INC_Q4X[3];*/

		r_GSCs_INC_Y[0]		<= r_GSCs_INC_Q1Y[0];
		r_GSCs_INC_Y[1]		<= r_GSCs_INC_Q2Y[0];
		r_GSCs_INC_Y[2]		<= r_GSCs_INC_Q3Y[0];
		r_GSCs_INC_Y[3]		<= r_GSCs_INC_Q4Y[0];
		/*
				r_GSCs_INC_Y[4]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q1Y[1];
				r_GSCs_INC_Y[5]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q2Y[1];
				r_GSCs_INC_Y[6]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q3Y[1];
				r_GSCs_INC_Y[7]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1];
				/*
				r_GSCs_INC_Y[8]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q1Y[2];
				r_GSCs_INC_Y[9]		<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q2Y[2];
				r_GSCs_INC_Y[10]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q3Y[2];
				r_GSCs_INC_Y[11]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q4Y[2];
				r_GSCs_INC_Y[12]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q4Y[2] + r_GSCs_INC_Q1Y[3];
				r_GSCs_INC_Y[13]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q4Y[2] + r_GSCs_INC_Q2Y[3];
				r_GSCs_INC_Y[14]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q4Y[2] + r_GSCs_INC_Q3Y[3];
				r_GSCs_INC_Y[15]	<= r_GSCs_INC_Q4Y[0] + r_GSCs_INC_Q4Y[1] + r_GSCs_INC_Q4Y[2] + r_GSCs_INC_Q4Y[3];*/

		r_GSCs_INC_Z[0]		<= r_GSCs_INC_Q1Z[0];
		r_GSCs_INC_Z[1]		<= r_GSCs_INC_Q2Z[0];
		r_GSCs_INC_Z[2]		<= r_GSCs_INC_Q3Z[0];
		r_GSCs_INC_Z[3]		<= r_GSCs_INC_Q4Z[0];
		/*
				r_GSCs_INC_Z[4]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q1Z[1];
				r_GSCs_INC_Z[5]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q2Z[1];
				r_GSCs_INC_Z[6]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q3Z[1];
				r_GSCs_INC_Z[7]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1];
				/*
				r_GSCs_INC_Z[8]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q1Z[2];
				r_GSCs_INC_Z[9]		<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q2Z[2];
				r_GSCs_INC_Z[10]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q3Z[2];
				r_GSCs_INC_Z[11]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q4Z[2];
				r_GSCs_INC_Z[12]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q4Z[2] + r_GSCs_INC_Q1Z[3];
				r_GSCs_INC_Z[13]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q4Z[2] + r_GSCs_INC_Q2Z[3];
				r_GSCs_INC_Z[14]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q4Z[2] + r_GSCs_INC_Q3Z[3];
				r_GSCs_INC_Z[15]	<= r_GSCs_INC_Q4Z[0] + r_GSCs_INC_Q4Z[1] + r_GSCs_INC_Q4Z[2] + r_GSCs_INC_Q4Z[3];*/

		r_GSCs_INC_W[0]		<= r_GSCs_INC_Q1W[0];
		r_GSCs_INC_W[1]		<= r_GSCs_INC_Q2W[0];
		r_GSCs_INC_W[2]		<= r_GSCs_INC_Q3W[0];
		r_GSCs_INC_W[3]		<= r_GSCs_INC_Q4W[0];
		/*
				r_GSCs_INC_W[4]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q1W[1];
				r_GSCs_INC_W[5]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q2W[1];
				r_GSCs_INC_W[6]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q3W[1];
				r_GSCs_INC_W[7]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1];
				/*
				r_GSCs_INC_W[8]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q1W[2];
				r_GSCs_INC_W[9]		<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q2W[2];
				r_GSCs_INC_W[10]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q3W[2];
				r_GSCs_INC_W[11]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q4W[2];
				r_GSCs_INC_W[12]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q4W[2] + r_GSCs_INC_Q1W[3];
				r_GSCs_INC_W[13]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q4W[2] + r_GSCs_INC_Q2W[3];
				r_GSCs_INC_W[14]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q4W[2] + r_GSCs_INC_Q3W[3];
				r_GSCs_INC_W[15]	<= r_GSCs_INC_Q4W[0] + r_GSCs_INC_Q4W[1] + r_GSCs_INC_Q4W[2] + r_GSCs_INC_Q4W[3];*/
	end
end

// data
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_GSCs_DATA		<= {16'b0, 16'b0, 16'b0, 16'b0};
	end
	else begin
		r_GSCs_DATA[`RANGE_WORDx4_X]	<= r_GSCs_RST[`RANGE_MASK4D_X] ? 16'b0 : r_GSCs_DATA[`RANGE_WORDx4_X] + r_GSCs_INC_X[(CORE_SIZE-1)];
		r_GSCs_DATA[`RANGE_WORDx4_Y]	<= r_GSCs_RST[`RANGE_MASK4D_Y] ? 16'b0 : r_GSCs_DATA[`RANGE_WORDx4_Y] + r_GSCs_INC_Y[(CORE_SIZE-1)];
		r_GSCs_DATA[`RANGE_WORDx4_Z]	<= r_GSCs_RST[`RANGE_MASK4D_Z] ? 16'b0 : r_GSCs_DATA[`RANGE_WORDx4_Z] + r_GSCs_INC_Z[(CORE_SIZE-1)];
		r_GSCs_DATA[`RANGE_WORDx4_W]	<= r_GSCs_RST[`RANGE_MASK4D_W] ? 16'b0 : r_GSCs_DATA[`RANGE_WORDx4_W] + r_GSCs_INC_W[(CORE_SIZE-1)];
	end
end

//----------------------------------------------------------------------------
// Bus request container
//----------------------------------------------------------------------------
reg		[(CORE_SIZE-1):0]		r_BUS_en;
reg		[`RANGE_DWORDx4]		r_BUS_desc[(CORE_SIZE-1):0];
reg		[(CORE_SIZE-1):0]		r_BUS_valid;

wire							bus_process_en	= |r_BUS_en;

assign	mem_command.en			= r_BUS_en[0];
assign	mem_command.desc		= r_BUS_desc[0];

generate begin
	for(index=0;index<CORE_SIZE;index=index+1) begin : bus_req
		assign	mem_command_core[index].valid	= mem_command.valid ? (mem_command_core[index].en & mem_command_core[index].valid) : `FALSE;	// BUS validation

		`ALWAYS_CLOCK_RESET begin
			`ON_RESET begin
				r_BUS_en[index]		<= `FALSE;
				r_BUS_desc[index]	<= 128'b0;
				r_BUS_valid[index]	<= `FALSE;
			end
			else begin
				if(bus_process_en) begin			// 컨테이너에 모두 REQ가 없을 때... 코어들로부터 업데이트 한다.
					r_BUS_en[index]			<= mem_command_core[index].en;
					r_BUS_desc[index]		<= mem_command_core[index].en ? mem_command_core[index].desc : 128'b0;
					r_BUS_valid[index]		<= (index==0) ? `TRUE : `FALSE;
				end
				else if(mem_command.valid || !r_BUS_en[0]) begin		// ACK 신호가 오거나 최 앞단의 REQ가 존재하지 않을 때 컨테이너를 움직인다.
					r_BUS_en[index]			<= (index == (CORE_SIZE-1)) ? `FALSE : r_BUS_en[index+1];
					r_BUS_desc[index]		<= (index == (CORE_SIZE-1)) ? 128'b0 : r_BUS_desc[index+1];
					r_BUS_valid[index]		<= (index == 0) ? `FALSE : r_BUS_valid[index-1];
				end
			end
		end
	end
end
endgenerate

//----------------------------------------------------------------------------
// extended memory access
//----------------------------------------------------------------------------
wire	[(CORE_SIZE-1):0]		w_EX_READY_ARRAY;
wire	[(CORE_SIZE-1):0]		w_EX_VALID;
reg		[(CORE_SIZE-1):0]		r_EX_EN_MASK;					// operation mask
`ifndef	MULTICORE_4
reg		[(CORE_SIZE/4-1):0]		r_EX_VALID;
reg		[`RANGE_DWORDx8]		r_EX_RDATA[(CORE_SIZE/4-1):0];
`endif

assign	local_memory.ready		= |(w_EX_READY_ARRAY & r_EX_EN_MASK);

generate
for(index=0;index<CORE_SIZE;index=index+1) begin : ex_mem
	assign	lmb_single[index].en	= local_memory.en & r_EX_EN_MASK[index];
	assign	lmb_single[index].we	= local_memory.we;
	assign	lmb_single[index].addr	= local_memory.addr;
	assign	lmb_single[index].wdata	= local_memory.wdata;
	assign	w_EX_READY_ARRAY[index]	= lmb_single[index].ready;
	assign	w_EX_VALID[index]		= lmb_single[index].valid;
end
endgenerate

// memory control
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		for(i=0;i<CORE_SIZE;i=i+1) begin
			r_EX_EN_MASK[i]		<= `FALSE;
		end
		`ifndef	MULTICORE_4
		for(i=0;i<(CORE_SIZE/4);i=i+1) begin
			r_EX_VALID[i]		<= `FALSE;
			r_EX_RDATA[i]		<= 256'b0;
		end
`endif
		local_memory.valid	<= `FALSE;
		local_memory.rdata	<= 256'b0;
	end
	else begin
		for(i=0;i<CORE_SIZE;i=i+1) begin
			r_EX_EN_MASK[i]		<= ~BUSY | r_BUS_valid[i];
		end
`ifdef	MULTICORE_4
		local_memory.valid	<= |w_EX_VALID;
		local_memory.rdata	<= (|w_EX_VALID[1:0])	? (w_EX_VALID[0]	? lmb_single[0].rdata : lmb_single[1].rdata) :
		(w_EX_VALID[2]	? lmb_single[2].rdata : lmb_single[3].rdata);
`else
		for(i=0;i<(CORE_SIZE/4);i=i+1) begin
			r_EX_VALID[i]		<= w_EX_VALID[(i*4)]|w_EX_VALID[(i*4)+1]|w_EX_VALID[(i*4)+2]|w_EX_VALID[(i*4)+3];
			r_EX_RDATA[i]		<= (w_EX_VALID[(i*4)]|w_EX_VALID[(i*4)+1]) ?  (w_EX_VALID[(i*4)  ]	? lmb_single[(i*4)  ].rdata : lmb_single[(i*4)+1].rdata) :
			(w_EX_VALID[(i*4)+2]	? lmb_single[(i*4)+2].rdata : lmb_single[(i*4)+3].rdata);
		end
		local_memory.valid	<= |r_EX_VALID;
`endif

`ifdef	MULTICORE_8

		local_memory.rdata	<= r_EX_VALID[0] ? r_EX_RDATA[0] : r_EX_RDATA[1];
`endif

`ifdef	MULTICORE_12

		local_memory.rdata	<= r_EX_VALID[0] ? r_EX_RDATA[0] :
		(r_EX_VALID[1] ? r_EX_RDATA[1] : r_EX_RDATA[2]);
`endif

`ifdef	MULTICORE_16

		local_memory.rdata	<= (|r_EX_VALID[1:0]) ? (r_EX_VALID[0] ? r_EX_RDATA[0] : r_EX_RDATA[1]) :
		(r_EX_VALID[2] ? r_EX_RDATA[2] : r_EX_RDATA[3]);
`endif

	end
end

// Core's synchronization
MTSP_Synchronize #(
	.CORE_SIZE		(CORE_SIZE)
) multicore_synchronize (
	.CLK			(CLK),
	.nRST			(nRST),
	.core_sync		(core_sync)
);

endmodule
