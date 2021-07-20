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

module MTSP_CacheLUT(
	// System
	input							CLK, nRST,					// main clock & reset

	// LUT clear interface
	input							LUT_CLEAR,					// start clear

	// Interface
	input							LUT_REQ,					// enable (active low)
	input	[`RANGE_CACHE_LUT]		LUT_ADDR,					// address
	input	[`RANGE_CACHE_PADDR]	LUT_DATA,					// data

	// Activate memory operation interface
	output							LUT_ACK,					// acknowledge
	output							LUT_HIT						// cache hit
);

// definition & assignment ---------------------------------------------------
// refresh operation
reg								clear_en;
reg		[`RANGE_CACHE_LUT]		clear_addr;

// pipelined output
reg		[1:0]					r_REQ;
reg								r_ACK;
reg		[`RANGE_CACHE_PADDR]	r_DATA;
wire							w_ACK;
wire	[`RANGE_CACHE_PADDR]	w_DOUT;


// memory wired
wire							m_nWE	= ((~LUT_ACK) & (~clear_en)) | LUT_REQ;
wire							m_nEN	= (~LUT_ACK) & (~clear_en) & (~LUT_REQ);
wire	[`RANGE_CACHE_LUT]		m_ADDR	= (LUT_REQ|LUT_ACK) ? LUT_ADDR : clear_addr;

assign	LUT_ACK					= r_REQ[1];
assign	LUT_HIT					= r_ACK & (r_DATA == LUT_DATA);

// implementation ------------------------------------------------------------
// cache tag memory
SRAM_Single #(
	.ADDR_WIDTH		(`SIZE_CACHE_LUT),
	.DATA_WIDTH		(1+`SIZE_CACHE_PADDR)
) cache_lut_0 (
	.CLK			(CLK),
	.nCE			(m_nEN),
	.nWE			(m_nWE),
	.ADDR			(m_ADDR),
	.DIN			({r_REQ[1], LUT_DATA}),		// {enable, physical address}
	.DOUT			({w_ACK, w_DOUT})
);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		// LUT clean-up
		clear_en		<= `TRUE;
		clear_addr		<= 0;
		// cache tag re-tagging
		r_REQ			<= {`FALSE, `FALSE};
		r_ACK			<= `FALSE;
		r_DATA			<= 0;
	end
	else begin
		if(LUT_CLEAR) begin
			clear_en		<= `TRUE;
			clear_addr		<= 'd0;
		end
		else if(!LUT_REQ) begin
			if(&clear_addr)
				clear_en		<= `FALSE;
			clear_addr		<= clear_addr+1;
		end

		r_REQ			<= {r_REQ[0], LUT_REQ};		// pipe out
		if(r_REQ[0]) begin
			r_ACK			<= w_ACK;
			r_DATA			<= w_DOUT;
		end
	end
end

endmodule
