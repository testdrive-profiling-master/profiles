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

module MTSP_BusMaster (
	// System
	input							CLK, nRST,					// main clock & reset

	i_bus_master.m					bus_master,					// master bus

	// memory interface to processor
	i_mem_command_desc.s			mem_desc,					// memory description
	// input data
	input							MEM_nIE,					// input enable
	input	[`RANGE_DWORDx8]		MEM_IDATA,					// input data
	output							MEM_nIVALID,				// input valid
	// output data
	output							MEM_OE,						// output enable
	output	[`RANGE_DWORDx8]		MEM_ODATA,					// output data
	output	reg						MEM_BUSY					// master bus is busy
);

// definition & assignment ---------------------------------------------------
//assign	bus_master.wdata	= MEM_IDATA;
//assign	bus_master.wvalid	= MEM_nIE;

/*
wire	w_MSTR_valid_n		= MSTR_src_rdy_n | MSTR_dst_rdy_n;
wire	w_MSTW_valid_n		= MSTW_src_rdy_n | MSTW_dst_rdy_n;
reg		r_MSTW_sof_n;
reg		r_R_nEN;
reg		[8:0]				r_MSTW_count, r_MSTR_count;
 
wire	req_READ			= mem_desc.req_master & (~mem_desc.we);
wire	req_WRITE			= mem_desc.req_master & mem_desc.we;
 
assign	MSTW_data			= MEM_IDATA;
assign	MSTW_src_rdy_n		= r_R_nEN | MEM_nIE;
assign	MEM_nIVALID			= w_MSTW_valid_n | (r_MSTW_count[1:0]!=2'd1);
*/
// dummy
assign	bus_master.rready		= `FALSE;
assign	bus_master.wvalid		= `FALSE;

/*
logic						rvalid;			// read validation
logic						rready;			// read ready
 
assign	MEM_ODATA				= bus_master.rdata;*/

// implementation ------------------------------------------------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		bus_master.rreq		<= `FALSE;
		bus_master.wreq		<= `FALSE;
		bus_master.raddr	<= 'd0;
		bus_master.waddr	<= 'd0;
		bus_master.rsize	<= 'd0;
		bus_master.wsize	<= 'd0;
		/*
		// read control
		r_MSTR_count		<= 0;
		MSTR_dst_rdy_n		<= `nFALSE;
		// read data
		MEM_OE				<= `FALSE;
		MEM_ODATA			<= 256'b0;

		// write control
		r_MSTW_count		<= 0;
		// write data
		r_R_nEN				<= `nFALSE;*/
		// busy
		MEM_BUSY			<= `FALSE;
	end
	else begin
		// read request
		if(bus_master.rreq) begin
			if(mem_desc.req_master & (~mem_desc.we)) begin
				bus_master.rreq		<= `TRUE;
				bus_master.raddr	<= {4'h8, mem_desc.paddr, 4'b0};
				bus_master.rsize	<= mem_desc.size;
			end
		end
		else begin
			bus_master.rreq		<= ~bus_master.rgrant;
		end

		// write request
		if(bus_master.wreq) begin
			if(mem_desc.req_master & mem_desc.we) begin
				bus_master.wreq		<= `TRUE;
				bus_master.waddr	<= {4'h8, mem_desc.paddr, 4'b0};
				bus_master.wsize	<= mem_desc.size;
			end
		end
		else begin
			bus_master.wreq		<= ~bus_master.wgrant;
		end
		/*
		// read control
		MSTR_Req			<= (MSTR_Req | req_READ) & (~MST_CmdAck);
		r_MSTR_count		<= req_READ			? MST_Length[11:3] :
		w_MSTR_valid_n	? r_MSTR_count : (r_MSTR_count - 1);
		MSTR_dst_rdy_n		<= ((~req_READ) & MSTR_dst_rdy_n);
		// read data
		MEM_OE				<= (~w_MSTR_valid_n) & (r_MSTR_count[1:0]==2'b1);
		MEM_ODATA			<= w_MSTR_valid_n ? MEM_ODATA : bus_master.rdata;

		// write control
		MSTW_Req			<= (MSTW_Req | req_WRITE) & (~MST_CmdAck);
		r_MSTW_count		<= req_WRITE		? MST_Length[11:3] :
		w_MSTW_valid_n	? r_MSTW_count : (r_MSTW_count - 1);

		// write data
		r_R_nEN				<= r_R_nEN ? (~req_WRITE) : ~(w_MSTW_valid_n);
		// busy
		MEM_BUSY			<= mem_desc.req_master | (MEM_BUSY);	// 메모리 전송이 끝날 때까지 busy 이어야 한다.
		*/
	end
end

endmodule
