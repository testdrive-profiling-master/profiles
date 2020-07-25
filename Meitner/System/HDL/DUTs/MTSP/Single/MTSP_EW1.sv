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

module MTSP_EW1(
	// system
	input							CLK, nRST,			// main clock & reset

	// core common input
	input	[`RANGE_GPRs]			MEM_SRC,			// source index

	// Local memory block extended write
	input							LMB_nEN,			// write enable (active low)
	input	[`RANGE_GPRs]			LMB_ADDR,			// address
	input	[`RANGE_DWORDx4]		LMB_DATA,			// data

	// Scratch counters input
	input							SCs_nEN,			// write enable (active low)
	input	[`RANGE_MASK4D]			SCs_MASK,			// write mask
	input	[`RANGE_DWORDx4]		SCs_DATA,			// write data

	// pack & unpack
	input							PU_nEN,				// write enabe (active low)
	input	[`RANGE_MASK4D]			PU_MASK,			// write mask
	input	[`RANGE_DWORDx4]		PU_DATA,			// write data

	// External write to GPRs
	output	reg						EW1_nEN,			// write enable (active low)
	output	reg [`RANGE_MASK4D]		EW1_MASK,			// write mask
	output	reg [`RANGE_GPRs]		EW1_ADDR,			// write address
	output	reg [`RANGE_DWORDx4]	EW1_DATA			// write data
);

// register definition & assignment ------------------------------------------
reg								mem_nEN;
reg		[`RANGE_MASK4D]			mem_WMASK;
reg		[`RANGE_DWORDx4]		mem_DATA;
reg		[`RANGE_GPRs]			ext_addr_0, ext_addr_1;

// implementation ------------------------------------------------------------
// Memory operation data write path
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		ext_addr_0		<= 0;
		ext_addr_1		<= 0;

		mem_nEN			<= `nFALSE;
		mem_WMASK		<= `MASK4D_ALLMASKED;
		mem_DATA		<= 128'b0;

		EW1_nEN			<= `nFALSE;
		EW1_MASK		<= `MASK4D_ALLMASKED;
		EW1_ADDR		<= 0;
		EW1_DATA		<= 128'b0;

	end
	else begin
		{ext_addr_0, ext_addr_1}	<= {MEM_SRC, ext_addr_0};

		mem_nEN			<= PU_nEN;
		mem_WMASK		<= PU_MASK;
		mem_DATA		<= PU_DATA;

		EW1_nEN			<= SCs_nEN & LMB_nEN & mem_nEN;
		EW1_MASK		<= {LMB_nEN, LMB_nEN, LMB_nEN, LMB_nEN} & SCs_MASK & mem_WMASK;
		EW1_ADDR		<= LMB_nEN ? ext_addr_1 : LMB_ADDR;
		EW1_DATA		<= (
			!SCs_nEN	? SCs_DATA :
			!LMB_nEN	? LMB_DATA : mem_DATA
		);
	end
end

endmodule
