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

module MTSP_WB(
	// System
	input							CLK, nRST,					// main clock & reset (active low)
	// input
	input	[`RANGE_DWORDx4]		DEST1,						// phase #0/#1 result input
	input	[`RANGE_MASK4D]			WMASK1,						// phase #0/#1 write mask
	input	[`RANGE_GPRs]			WADDR1,						// phase #0/#1 GPRs write address

	// External write to GPRs
	input							EW1_nEN,					// external input enable
	input	[`RANGE_MASK4D]			EW1_MASK,					// external write mask
	input	[`RANGE_GPRs]			EW1_ADDR,					// write address
	input	[`RANGE_DWORDx4]		EW1_DATA,					// external data input
	// output
	output	reg						WB1_nEN,					// write enable (active low)
	output	reg [`RANGE_MASK4D]		WB1_MASK,					// GPRs phase #1 write mask
	output	reg [`RANGE_GPRs]		WB1_ADDR,					// GPRs phase #1 write address
	output	reg [`RANGE_DWORDx4]	WB1_DATA					// GPRs phase #1 result output
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		WB1_nEN			<= `nFALSE;					// enable (active low)
		WB1_MASK		<= `MASK4D_ALLMASKED;		// phase #1 write mask
		WB1_ADDR		<= 0;						// phase #1 write address
		WB1_DATA		<= 0;						// phase #1 write data
	end
	else begin
		WB1_nEN			<= &{EW1_nEN, WMASK1};
		WB1_MASK		<= EW1_nEN	? WMASK1	: EW1_MASK;
		WB1_ADDR		<= EW1_nEN	? WADDR1	: EW1_ADDR;
		WB1_DATA		<= EW1_nEN	? DEST1		: EW1_DATA;
	end
end

endmodule
