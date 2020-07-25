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

module MTSP_PackUnpack(
	// system
	input							CLK, nRST,			// main clock & reset (active low)

	// input
	input							MEM_nEN,			// Enable (active low)
	input							MEM_WRITE,			// Write enable (active low)
	input	[`RANGE_WORD]			MEM_ADDR,			// address
	input	[`RANGE_DWORDx4]		MEM_DATA,			// Data input

	output	reg						EW1_nEN,
	output	reg [`RANGE_MASK4D]		EW1_MASK,
	output	reg [`RANGE_DWORDx4]	EW1_DATA
);

// register definition & assignment ------------------------------------------
wire	[`RANGE_DWORD]			x,y,z,w;
wire	[`RANGE_BYTE]			ux, uy, uz, uw;
assign							{x,y,z,w}	= MEM_DATA;

// implementation ------------------------------------------------------------

MTSP_UnpackElement		unpack_x(x, ux);
MTSP_UnpackElement		unpack_y(y, uy);
MTSP_UnpackElement		unpack_z(z, uz);
MTSP_UnpackElement		unpack_w(w, uw);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		EW1_nEN		<= `nFALSE;
		EW1_MASK	<= `MASK4D_ALLMASKED;
		EW1_DATA	<= 0;
	end
	else begin
		EW1_nEN		<= MEM_nEN;
		EW1_MASK	<= MEM_nEN ? `MASK4D_ALLMASKED : (MEM_WRITE ? 4'b0111 : 4'b0000);
		EW1_DATA	<= MEM_nEN ? EW1_DATA : (MEM_WRITE ? {ux, uy, uz, uw, 32'b0, 32'b0, 32'b0} :
										  {24'b0, x[31:24], 24'b0, x[23:16], 24'b0, x[15:8], 24'b0, x[7:0]});
	end
end

endmodule
