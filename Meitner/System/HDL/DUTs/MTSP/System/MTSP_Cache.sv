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

module MTSP_Cache(
	// System
	input							CLK, nRST,					// main clock & reset
	// Q Interface
	input							QR_RE,						// read enable
	input	[`RANGE_DWORDx8]		QR_DOUT,					// read data
	output							QR_WE,						// write enable
	output	[`RANGE_DWORDx8]		QR_DIN,						// write data
	// Processor interface
	input							EX_VALID,					// output data valid
	input	[`RANGE_DWORDx8]		EX_RDATA,					// read data
	// Cache interface
	input							CACHE_HIT,
	// memory interface to processor
	i_mem_command_desc.s			mem_desc,					// memory description
	output	reg						MEM_BUSY					// memory operation is busy
);

// definition & assignment ---------------------------------------------------
reg		[`RANGE_BYTE]			r_SIZE;
reg								r_WE;
reg		[`RANGE_GMB]			r_GADDR;
reg								r_QRR_nEN, r_QRW_nEN, r_EX_nEN;

wire							w_CE	= ~((r_QRR_nEN|(~QR_RE)) & ((~EX_VALID)|r_EX_nEN) & r_QRW_nEN);
wire	[`RANGE_DWORDx8]		w_DIN	= (r_QRR_nEN|(~QR_RE)) ? EX_RDATA : QR_DOUT;

wire	[`RANGE_BYTE]			w_SIZE	= (
	mem_desc.req_cache	? mem_desc.size :
	w_CE				? (r_SIZE - 1'b1) : r_SIZE
);
wire							w_BUSY	= (|w_SIZE);

// implementation ------------------------------------------------------------
// global memory block
MTSP_GMB gmb (
	.CLK		(CLK),
	.nRST		(nRST),
	.CE			(w_CE),
	.WE			(r_WE),
	.ADDR		(r_GADDR),
	.DIN		(w_DIN),
	.OE			(QR_WE),
	.DOUT		(QR_DIN)
);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_QRR_nEN	<= `nFALSE;
		r_QRW_nEN	<= `nFALSE;
		r_EX_nEN	<= `nFALSE;
		r_WE		<= `FALSE;
		r_GADDR		<= 0;
		r_SIZE		<= 0;
		MEM_BUSY	<= 0;
	end
	else begin
		if(mem_desc.req_cache) begin
			r_QRR_nEN	<= mem_desc.cache_only | ((~mem_desc.cache_en) | mem_desc.we | CACHE_HIT);
			r_QRW_nEN	<= (~(CACHE_HIT | mem_desc.cache_only)) | mem_desc.we;
			r_EX_nEN	<= ~((mem_desc.cache_en | mem_desc.cache_only) & mem_desc.we);
			r_WE		<= mem_desc.cache_only ? mem_desc.we : (mem_desc.cache_en & (mem_desc.we | ~CACHE_HIT));
			r_GADDR		<= mem_desc.gaddr;
		end
		else begin
			if(!w_BUSY) begin
				r_QRR_nEN	<= `nFALSE;
				r_QRW_nEN	<= `nFALSE;
				r_EX_nEN	<= `nFALSE;
			end
			if(w_CE) begin
				r_GADDR		<= r_GADDR + 1'b1;
			end
		end

		r_SIZE		<= w_SIZE;
		MEM_BUSY	<= w_BUSY;
	end
end

endmodule
