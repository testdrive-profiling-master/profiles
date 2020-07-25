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

module MTSP_SF(	CLK, nRST,
				MO0_X, MO0_Y, MO0_Z, MO0_W, MO0_MASK,
				MO1_X, MO1_Y, MO1_Z, MO1_W, MO1_MASK,
				SRC0A, SRC0B,
				SRC1A, SRC1B,
				WE0, DOUT0, WE1, DOUT1);

// port definition -----------------------------------------------------------
// System
input							CLK, nRST;					// main clock & reset (active low)
// input
input	[`RANGE_MODESC]			MO0_X, MO0_Y, MO0_Z, MO0_W,	// phase #0 micro-operation description
MO1_X, MO1_Y, MO1_Z, MO1_W;	// phase #1 micro-operation description
input	[`RANGE_MASK4D]			MO0_MASK, MO1_MASK;			// phase #0/#1 micro-operation mask
input	[`RANGE_DWORDx4]		SRC0A, SRC0B,				// phase #0 source data
SRC1A, SRC1B;				// phase #1 source data
// output
output	reg [`RANGE_MASK4D]		WE0, WE1;					// write mask
output	[`RANGE_DWORD]			DOUT0, DOUT1;				// result out

// register definition & assignment ------------------------------------------
wire	[`RANGE_MASK4D]			EN_sp0, EN_sp1;
wire	[`RANGE_MODESC]			MO_sp0, MO_sp1;
wire	[`RANGE_DWORD]			datain_0, datain_1;

// Time analysis #0 --- begin
// active low special function enable
assign	EN_sp0		= {	((~MO0_X[`RANGE_MODESC_nEN]) & (MO0_X[3:1]==3'b010)),
				   ((~MO0_Y[`RANGE_MODESC_nEN]) & (MO0_Y[3:1]==3'b010)),
				   ((~MO0_Z[`RANGE_MODESC_nEN]) & (MO0_Z[3:1]==3'b010)),
				   ((~MO0_W[`RANGE_MODESC_nEN]) & (MO0_W[3:1]==3'b010))},
EN_sp1		= {	((~MO1_X[`RANGE_MODESC_nEN]) & (MO1_X[3:1]==3'b010)),
			((~MO1_Y[`RANGE_MODESC_nEN]) & (MO1_Y[3:1]==3'b010)),
			((~MO1_Z[`RANGE_MODESC_nEN]) & (MO1_Z[3:1]==3'b010)),
			((~MO1_W[`RANGE_MODESC_nEN]) & (MO1_W[3:1]==3'b010))};
// special function MO dispatch
assign	MO_sp0		= EN_sp0[`SELECT_X] ? MO0_X :
EN_sp0[`SELECT_Y] ? MO0_Y :
EN_sp0[`SELECT_Z] ? MO0_Z : MO0_W,
MO_sp1		= EN_sp1[`SELECT_X] ? MO1_X :
EN_sp1[`SELECT_Y] ? MO1_Y :
EN_sp1[`SELECT_Z] ? MO1_Z : MO1_W;
// special function data input dispatch
assign	datain_0	= EN_sp0[`SELECT_X] ? (MO0_MASK[`SELECT_X] ? SRC0B[`RANGE_DWORDx4_X] : SRC0A[`RANGE_DWORDx4_X]) :
EN_sp0[`SELECT_Y] ? (MO0_MASK[`SELECT_Y] ? SRC0B[`RANGE_DWORDx4_Y] : SRC0A[`RANGE_DWORDx4_Y]) :
EN_sp0[`SELECT_Z] ? (MO0_MASK[`SELECT_Z] ? SRC0B[`RANGE_DWORDx4_Z] : SRC0A[`RANGE_DWORDx4_Z]) :
(MO0_MASK[`SELECT_W] ? SRC0B[`RANGE_DWORDx4_W] : SRC0A[`RANGE_DWORDx4_W]),
datain_1	= EN_sp1[`SELECT_X] ? (MO1_MASK[`SELECT_X] ? SRC1B[`RANGE_DWORDx4_X] : SRC1A[`RANGE_DWORDx4_X]) :
EN_sp1[`SELECT_Y] ? (MO1_MASK[`SELECT_Y] ? SRC1B[`RANGE_DWORDx4_Y] : SRC1A[`RANGE_DWORDx4_Y]) :
EN_sp1[`SELECT_Z] ? (MO1_MASK[`SELECT_Z] ? SRC1B[`RANGE_DWORDx4_Z] : SRC1A[`RANGE_DWORDx4_Z]) :
(MO1_MASK[`SELECT_W] ? SRC1B[`RANGE_DWORDx4_W] : SRC1A[`RANGE_DWORDx4_W]);

// Time analysis #0 --- end : 3.949 ns / 0.370 ns
wire	[`RANGE_DWORD]			rcp_out, rsq_out, man_out, exp_out;
wire	[1:0]					rcp_en, rsq_en, man_en, exp_en;

// implementation ------------------------------------------------------------
MTSP_SF_RCP		sp_rcp(	CLK, nRST,			// MLCK, nRST,
					 MO_sp0, datain_0,	// MO_0, DIN_0
					 MO_sp1, datain_1,	// MO_1, DIN_1
					 rcp_en, rcp_out);	// PHASE_EN, DOUT
MTSP_SF_RSQ		sp_rsq(	CLK, nRST,			// MLCK, nRST,
					 MO_sp0, datain_0,	// MO_0, DIN_0
					 MO_sp1, datain_1,	// MO_1, DIN_1
					 rsq_en, rsq_out);	// PHASE_EN, DOUT
MTSP_SF_MAN		sp_man(	CLK, nRST,			// MLCK, nRST,
					 MO_sp0, datain_0,	// MO_0, DIN_0
					 MO_sp1, datain_1,	// MO_1, DIN_1
					 man_en, man_out);	// PHASE_EN, DOUT
MTSP_SF_EXP		sp_exp(	CLK, nRST,			// MLCK, nRST,
					 MO_sp0, datain_0,	// MO_0, DIN_0
					 MO_sp1, datain_1,	// MO_1, DIN_1
					 exp_en, exp_out);	// PHASE_EN, DOUT

assign	DOUT0	= rcp_en[0] ? rcp_out :
rsq_en[0] ? rsq_out :
man_en[0] ? man_out : exp_out,
DOUT1	= rcp_en[1] ? rcp_out :
rsq_en[1] ? rsq_out :
man_en[1] ? man_out : exp_out;

reg		[`RANGE_MASK4D]			we0_p[1:0], we1_p[1:0];	// write mask pipeline

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		WE0			<= 4'b0;
		WE1			<= 4'b0;
		we0_p[0]	<= 4'b0;
		we0_p[1]	<= 4'b0;
		we1_p[0]	<= 4'b0;
		we1_p[1]	<= 4'b0;
	end
	else begin	// 수정할 것.
		// SM            EX0             EX1
		{we0_p[0],	we1_p[0],	we0_p[1],	we1_p[1],	WE0,		WE1} <=
		{EN_sp0,	EN_sp1,		we0_p[0],	we1_p[0],	we0_p[1],	we1_p[1]};
	end
end

endmodule
