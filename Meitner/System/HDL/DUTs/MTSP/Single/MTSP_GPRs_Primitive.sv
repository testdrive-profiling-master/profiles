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

module MTSP_GPRs_Primitive #(
	parameter	CORE_ID				= 0,
	parameter	PRIMITIVE_ID		= 0
) (
	// System
	input							CLK, nRST,			// main clock

	input	[5:0]					nEN,
	// source -----------------------------
	// Phase #0 source input
	input	[`SIZE_GPRs:0]			SRC0A_ADDR,			// Source A index
	input	[`SIZE_GPRs:0]			SRC0B_ADDR,			// Source B index
	// Phase #1 source input
	input	[`SIZE_GPRs:0]			SRC1A_ADDR,			// Source A index
	input	[`SIZE_GPRs:0]			SRC1B_ADDR,			// Source B index

	// write back -------------------------
	// Phase #0 output
	input	[`RANGE_MASK4D]			WB0_WMASK,			// Write mask
	input	[`SIZE_GPRs:0]			WB0_ADDR,			// Write index
	input	[`RANGE_DWORDx4]		WB0_DATA,			// Write data
	// Phase #1 output
	input	[`RANGE_MASK4D]			WB1_WMASK,			// Write mask
	input	[`SIZE_GPRs:0]			WB1_ADDR,			// Write index
	input	[`RANGE_DWORDx4]		WB1_DATA,			// Write data

	// output -----------------------------
	output	[`RANGE_DWORDx4]		RDATA
);

// definition & assignment ---------------------------------------------------
reg			[`SIZE_GPRs:0]			addr;
reg			[`RANGE_MASK4D]			wmask;
reg			[`RANGE_DWORDx4]		wdata;

// implementation ------------------------------------------------------------

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		addr	<= 'd0;
		wmask	<= `MASK4D_ALLMASKED;
		wdata	<= 'd0;
	end
	else begin
		addr	<= !nEN[0]	? SRC0A_ADDR :
		!nEN[1]	? SRC0B_ADDR :
		!nEN[2]	? SRC1A_ADDR :
		!nEN[3]	? SRC1B_ADDR :
		!nEN[4]	? WB0_ADDR : WB1_ADDR;
		wmask	<= !nEN[4]	? WB0_WMASK :
		!nEN[5]	? WB1_WMASK : `MASK4D_ALLMASKED;
		wdata	<= !nEN[4]	? WB0_DATA : WB1_DATA;
	end
end

MTSP_GPRs_Element #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(PRIMITIVE_ID)
) gprs_inst (
	.CLK			(CLK),
	.nRST			(nRST),
	.WMASK			(wmask),
	.ADDR			(addr),
	.WDATA			(wdata),
	.RDATA			(RDATA)
);

endmodule

