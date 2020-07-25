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

module MTSP_ID_1(
	// System
	input							CLK, nRST,				// main clock & reset (active low)
	// Input
	input	[`RANGE_PC]				PC,						// PC
	input	[`RANGE_UINSTx4]		UINSTx4,				// 4 unit instructions end & phase bit
	input	[`RANGE_DWORDx4]		SRC0A, SRC0B,
	// Output
	output	reg [`RANGE_PC]			PC_OUT,					// PC output
	output	reg [`RANGE_UINSTx4]	UINSTx4_OUT,			// phase instructions
	output	reg	[`RANGE_DWORDx4]	SRC0A_OUT, SRC0B_OUT
);

// definition & assignment ---------------------------------------------------
wire	[`RANGE_UINST]			p0_m, p0_s, p1_m, p1_s;			// input unit instruction
wire	[`RANGE_DWORDx4]		src0b_coord;

assign	{p0_m, p0_s, p1_m, p1_s}	= UINSTx4;

// implementation ------------------------------------------------------------
MTSP_SrcCoordinate			src_coord(1'b0, 1'b1, p0_s, SRC0B, src0b_coord);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		PC_OUT			<= 0;
		UINSTx4_OUT		<= 128'hFFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF;
		SRC0A_OUT		<= 0;
		SRC0B_OUT		<= 0;
	end
	else begin
		PC_OUT			<= PC;
		UINSTx4_OUT		<= UINSTx4;
		SRC0A_OUT		<= SRC0A;
		SRC0B_OUT		<= src0b_coord;
	end
end

endmodule
