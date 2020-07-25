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
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_MemoryCommand (
	// System
	input								CLK, nRST,			// main clock & reset

	// memory control
	i_memory_command.s					mem_command_ex,		// memory command (external)
	i_memory_command.s					mem_command_pu,		// memory command (processor unit), high priority

	// LUT
	input								CACHE_LUT_CLEAR,	// cache look-up table clear
	output	reg							CACHE_HIT,			// cache LUT hit

	// Bus bridge interface
	i_mem_command_desc.m				mem_desc,			// memory description
	input								MEM_BUSY
);

// definition & assignment ---------------------------------------------------
reg								r_nREQ, r_nBUS;
i_memory_command				mem_command;

// bus interface
wire							mem_req_stream	= mem_command_pu.en & (~mem_command_pu.valid);
wire							mem_req_command	= mem_command_ex.en & (~mem_command_ex.valid);
wire							w_nREQ			= (mem_req_stream & mem_req_command) | ((~r_nREQ) & (~mem_req_valid));
wire							mem_req_valid	= mem_command_pu.valid | mem_command_ex.valid;

// requested memory description's contents
reg		[`RANGE_DWORDx4]		desc;
reg								desc_is_stream;	// 1(from stream), 0(from command)
wire							desc_dummy, desc_we, desc_is_inst, desc_cache_only, desc_cache_en;
wire	[`RANGE_INDEX]			desc_index;
wire	[`RANGE_WORD]			desc_id, desc_mask, desc_gbase;
wire	[`RANGE_BYTE]			desc_stride, desc_size;
wire	[`RANGE_DWORD]			desc_pbase;
wire	[14:0]					desc_lbase;

assign	{desc_dummy, desc_we, desc_is_inst, desc_cache_only, desc_index, desc_id}	= desc[`RANGE_DWORDx4_X];
assign	{desc_stride, desc_size, desc_mask}											= desc[`RANGE_DWORDx4_Y];
assign	desc_pbase																	= desc[`RANGE_DWORDx4_Z];
assign	{desc_cache_en, desc_lbase, desc_gbase}										= desc[`RANGE_DWORDx4_W];

// pipe #0
reg								r0_nEN;
reg		[23:0]					r0_POFFSET;		// physical offset
reg		[`RANGE_GMB]			r0_GOFFSET;		// global memory offset
reg		[`RANGE_CACHE_LUT]		r0_LUT;
wire	w0_nACK					= (mem_desc.size != 0);
wire	w0_nEN					= (~r0_nEN) | (r_nREQ) | (~w0_nACK);

// pipe #1
reg								r1_nEN;
reg		[`RANGE_WORD]			r1_CADDR;
wire	w1_nEN					= (~r1_nEN) | r0_nEN;

// cache LUT
reg								lut_req;
reg 	[`RANGE_CACHE_LUT]		lut_addr;
wire	[`RANGE_CACHE_PADDR]	lut_data		= desc_pbase[(`SIZE_CACHE_PADDR+3):4];
wire	[`RANGE_WORD]			lut_masked_id	= desc_id & desc_mask;
wire							lut_ack, lut_hit;

// BUS
reg		r_BUSY;
wire							w_nACK_RES	= r_nREQ | (w0_nACK&((~r_BUSY)|MEM_BUSY))|mem_req_valid;

localparam	FSM_IDLE			= 2'd0;
localparam	FSM_1				= 2'd1;
localparam	FSM_2				= 2'd2;
localparam	FSM_3				= 2'd3;

reg	[2:0]	fsm					= FSM_IDLE;			// state machine

assign	mem_command_ex.valid	= mem_command_pu.valid & (~mem_command_pu.en);	// higher priority for PU.
// implementation ------------------------------------------------------------
// command input
ControlSlice #(
	.WIDTH				(`SIZE_DWORDx8),
	.CHAINED			(0)
) command_demux (
	.CLK				(CLK),
	.nRST				(nRST),
	.IE					((mem_command_ex.en | mem_command_pu.en)),
	.IDATA				((mem_command_pu.en ? mem_command_pu.desc : mem_command_ex.desc)),
	.IREADY				(mem_command_pu.valid),
	.OE					(mem_command.en),
	.ODATA				(mem_command.desc),
	.OREADY				(mem_command.valid)
);

// cache look-up table (read check only tag)
MTSP_CacheLUT	cache_lut(
	.CLK				(CLK),
	.nRST				(nRST),
	.LUT_CLEAR			(CACHE_LUT_CLEAR),
	.LUT_REQ			(lut_req),
	.LUT_ADDR			(lut_addr),
	.LUT_DATA			(lut_data),
	.LUT_ACK			(lut_ack),
	.LUT_HIT			(lut_hit)
);

/*
// pipe #0 : compute real address & check violation
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r0_nEN				<= `nFALSE;
		r0_POFFSET			<= 0;
		r0_GOFFSET			<= 0;
		lut_addr			<= 0;
		mem_desc.req_ext	<= `FALSE;
	end
	else begin
		if(!w0_nEN) begin
			r0_nEN				<= `nTRUE;
			r0_POFFSET			<= i_STRIDE * i_ID;
			r0_GOFFSET			<= mem_desc.size * i_MASKED_ID;
			lut_req				<= mem_desc.cache_en & (~mem_desc.cache_only);			// Discard LUT check when using the cache only mode
			mem_desc.req_ext	<= `TRUE;
		end
		else begin
			r0_nEN				<= r0_nEN | mem_req_valid;
			mem_desc.req_ext	<= `FALSE;
		end
	end
end
 
// pipe #1 : compute real address & check violation
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r1_nEN			<= `nFALSE;
		mem_desc.paddr	<= 0;
		mem_desc.gaddr	<= 0;
	end
	else begin
		if(!w1_nEN) begin
			r1_nEN			<= `nTRUE;
			mem_desc.paddr	<= lut_data + r0_POFFSET;
			mem_desc.gaddr	<= i_GBASE[`RANGE_GMB] + r0_GOFFSET;
		end
		else begin
			r1_nEN			<= r1_nEN | mem_req_valid;
		end
	end
end
 
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_nBUS				<= `nFALSE;
		r_nREQ				<= `nFALSE;
		r_BUSY				<= `FALSE;
		mem_command_pu.valid	<= `FALSE;
		mem_command_ex.valid	<= `FALSE;
		mem_desc.req_master	<= `FALSE;
		mem_desc.req_cache	<= `FALSE;
		CACHE_HIT			<= `FALSE;
	end
	else begin
		if(!w_nREQ) begin
			r_nBUS				<= mem_req_stream;
			r_nREQ				<= (r_nREQ|mem_req_valid) ? w_nREQ : r_nREQ;
		end
		else begin
			r_nREQ				<= (r_nREQ|mem_req_valid) ? w_nREQ : r_nREQ;
		end
		r_nBUS				<= w_nREQ ? r_nBUS : mem_req_stream;
		r_nREQ				<= (r_nREQ|mem_req_valid) ? w_nREQ : r_nREQ;
		r_BUSY				<= MEM_BUSY;
		mem_command_pu.valid	<= (~w_nACK_RES) & (~r_nBUS);
		mem_command_ex.valid	<= (~w_nACK_RES) & r_nBUS;
		mem_desc.req_master	<= ((~w1_nEN)&(~mem_desc.cache_only)&(~mem_desc.cache_en)) | (lut_ack & (mem_desc.we | (~lut_hit)));
		mem_desc.req_cache	<= ((~w1_nEN)&(mem_desc.cache_only)) | lut_ack;
		CACHE_HIT			<= (~mem_req_valid)&(CACHE_HIT|(lut_ack & lut_hit));
	end
end
*/
endmodule
