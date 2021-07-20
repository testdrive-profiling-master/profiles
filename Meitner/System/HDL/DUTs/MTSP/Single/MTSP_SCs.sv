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

module MTSP_SCs #(
	parameter	CORE_ID				= 0
) (
	// system
	input							CLK, nRST,			// main clock & reset (active low)

	// input
	input							MEM_nEN,			// Enable (active low)
	input							MEM_SEL,			// Scratch counters selection
	input	[`RANGE_WORD]			MEM_OP,				// Operations
	input	[`RANGE_DWORDx4]		MEM_DATA,			// Data input

	// global scratch counters
	input	[`RANGE_WORDx4]			GSCs_DATA,			// data

	// output
	output	reg						EW1_nEN,			// enable (active low)
	output	reg [`RANGE_MASK4D]		EW1_MASK,			// Write mask
	output	reg	[`RANGE_DWORDx4]	EW1_DATA			// Write data
);

// register definition & assignment ------------------------------------------
wire	[`RANGE_DWORD]			IX, IY, IZ, IW;
wire	[`RANGE_WORD]			SC0_X, SC0_Y, SC0_Z, SC0_W,
SC1_X, SC1_Y, SC1_Z, SC1_W;
wire	[3:0]					OP_X, OP_Y, OP_Z, OP_W;
reg								r_nEN;
reg		[`RANGE_MASK4D]			r_MASK;
reg		[`RANGE_WORD]			r_DATA_X, r_DATA_Y, r_DATA_Z, r_DATA_W;
reg		[3:0]					r_OP_X, r_OP_Y, r_OP_Z, r_OP_W;

assign	{OP_X, OP_Y, OP_Z, OP_W} = MEM_OP;
assign	{IX,IY,IZ,IW}			= MEM_DATA;

// implementation ------------------------------------------------------------

MTSP_SCs_Primitive	Scratch_0(	CLK, nRST,
							  MEM_nEN|MEM_SEL, MEM_OP, IX[`RANGE_WORD], IY[`RANGE_WORD], IZ[`RANGE_WORD], IW[`RANGE_WORD],
							  SC0_X, SC0_Y, SC0_Z, SC0_W);

MTSP_SCs_Primitive	Scratch_1(	CLK, nRST,
							  MEM_nEN|(~MEM_SEL), MEM_OP, IX[`RANGE_WORD], IY[`RANGE_WORD], IZ[`RANGE_WORD], IW[`RANGE_WORD],
							  SC1_X, SC1_Y, SC1_Z, SC1_W);

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_nEN		<= `nFALSE;
		r_MASK		<= `MASK4D_ALLMASKED;
		r_DATA_X	<= 16'b0;
		r_DATA_Y	<= 16'b0;
		r_DATA_Z	<= 16'b0;
		r_DATA_W	<= 16'b0;
		{r_OP_X, r_OP_Y, r_OP_Z, r_OP_W}	<= 16'b0;

		EW1_nEN		<= `nFALSE;
		EW1_MASK	<= `MASK4D_ALLMASKED;
		EW1_DATA	<= 128'b0;
	end
	else begin
		r_nEN		<= MEM_nEN;
		r_MASK		<= MEM_nEN	? `MASK4D_ALLMASKED : {(~|OP_X[3:2])|OP_X[1], (~|OP_Y[3:2])|OP_Y[1], (~|OP_Z[3:2])|OP_Z[1], (~|OP_W[3:2])|OP_W[1]};
		r_DATA_X	<= MEM_nEN	? r_DATA_X : (MEM_SEL ? SC1_X : SC0_X);
		r_DATA_Y	<= MEM_nEN	? r_DATA_Y : (MEM_SEL ? SC1_Y : SC0_Y);
		r_DATA_Z	<= MEM_nEN	? r_DATA_Z : (MEM_SEL ? SC1_Z : SC0_Z);
		r_DATA_W	<= MEM_nEN	? r_DATA_W : (MEM_SEL ? SC1_W : SC0_W);
		{r_OP_X, r_OP_Y, r_OP_Z, r_OP_W}	<= MEM_nEN	? {r_OP_X, r_OP_Y, r_OP_Z, r_OP_W} : MEM_OP;

		EW1_nEN		<= r_nEN;
		EW1_MASK	<= r_MASK;
		EW1_DATA	<= r_nEN ? EW1_DATA : {16'b0, r_DATA_X, 16'b0, r_DATA_Y, 16'b0, r_DATA_Z, 16'b0, r_DATA_W};
		EW1_DATA[`RANGE_DWORDx4_X]	<= {16'b0, r_nEN ? EW1_DATA[`RANGE_DWORDx4_XL] : (r_OP_X[3] ? GSCs_DATA[`RANGE_WORDx4_X] : r_DATA_X)};
		EW1_DATA[`RANGE_DWORDx4_Y]	<= {16'b0, r_nEN ? EW1_DATA[`RANGE_DWORDx4_YL] : (r_OP_Y[3] ? GSCs_DATA[`RANGE_WORDx4_Y] : r_DATA_Y)};
		EW1_DATA[`RANGE_DWORDx4_Z]	<= {16'b0, r_nEN ? EW1_DATA[`RANGE_DWORDx4_ZL] : (r_OP_Z[3] ? GSCs_DATA[`RANGE_WORDx4_Z] : r_DATA_Z)};
		EW1_DATA[`RANGE_DWORDx4_W]	<= {16'b0, r_nEN ? EW1_DATA[`RANGE_DWORDx4_WL] : (r_OP_W[3] ? GSCs_DATA[`RANGE_WORDx4_W] : r_DATA_W)};
	end
end

endmodule
