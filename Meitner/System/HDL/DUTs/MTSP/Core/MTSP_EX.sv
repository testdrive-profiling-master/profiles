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

module MTSP_EX(
	// System
	input							CLK, nRST,					// main clock & reset (active low)

	// input
	input	[`RANGE_PC]				PC,							// next instruction program counter
	input							MEM_STALL,					// memory stall

	input	[`RANGE_MODESC]			MO0_X, MO0_Y, MO0_Z, MO0_W,	// phase #0 micro-operation description
	input	[`RANGE_MODESC]			MO1_X, MO1_Y, MO1_Z, MO1_W,	// phase #1 micro-operation description
	input	[`RANGE_MASK4D]			MO0_MASK, MO1_MASK,			// phase #0/#1 micro-operation mask
	input	[`RANGE_GPRs]			MO0_WADDR, MO1_WADDR,		// phase #0/#1 GPRs write address
	input	[`RANGE_DWORDx4]		SRC0A, SRC0B,				// phase #0 source data
	input	[`RANGE_DWORDx4]		SRC1A, SRC1B,				// phase #1 source data
	// Extended write
	input							EW0_nEN,					// write enable (active low)
	input	[`RANGE_MASK4D]			EW0_MASK,					// write mask
	input	[`RANGE_GPRs]			EW0_ADDR,					// write address
	input	[`RANGE_DWORDx4]		EW0_DATA,					// write data
	// output
	output	reg						WB0_nEN,					// phase #0 write enable (active low)
	output	reg [`RANGE_DWORDx4]	WB0_DATA, WB1_DATA,			// phase #0/#1 result output
	output	reg [`RANGE_MASK4D]		WB0_MASK, WB1_MASK,			// phase #0/#1 write mask
	output	reg [`RANGE_GPRs]		WB0_ADDR, WB1_ADDR			// phase #0/#1 GPRs write address
);

// definition & assignment ---------------------------------------------------
wire	[`RANGE_MASK4D]			SF_WE0, SF_WE1;
wire	[`RANGE_DWORD]			SF_DOUT0, SF_DOUT1;
wire							sm_nEN	= MO0_X[`RANGE_MODESC_nEN]&MO0_Y[`RANGE_MODESC_nEN]&MO0_Z[`RANGE_MODESC_nEN]&MO0_W[`RANGE_MODESC_nEN]&
MO1_X[`RANGE_MODESC_nEN]&MO1_Y[`RANGE_MODESC_nEN]&MO1_Z[`RANGE_MODESC_nEN]&MO1_W[`RANGE_MODESC_nEN];
reg		[`RANGE_GPRs]			ex_waddr0[2:0], ex_waddr1[2:0];
wire	[`RANGE_MASK4D]			mo1_int	= {MO1_X[`RANGE_MODESC_SEL], MO1_Y[`RANGE_MODESC_SEL], MO1_Z[`RANGE_MODESC_SEL], MO1_W[`RANGE_MODESC_SEL]};
wire	[`RANGE_MASK4D]			dest0_wmask, dest1_wmask;
wire	[`RANGE_MASK4D]			converse_type_1;
reg		[2:0]					pipe_nEN;
reg		[`RANGE_PC]				sm_PC;
reg		[1:0]					stall;

wire	[`RANGE_MASK4D]			alu_wmask_0, alu_wmask_1;
wire	[`RANGE_DWORDx4]		alu_data_0, alu_data_1;

// implementation ------------------------------------------------------------
//------- bypass entry : begin
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		pipe_nEN	<= 3'b111;			// pipe enable (active low)
		sm_PC		<= 0;				// program counter for "mvs" instruction
		stall		<= 0;
		ex_waddr0[0]<= 0;
		ex_waddr0[1]<=0;
		ex_waddr0[2]<=0;
		ex_waddr1[0]<= 0;
		ex_waddr1[1]<=0;
		ex_waddr1[2]<=0;
		WB0_ADDR	<= 0;
		WB1_ADDR	<= 0;
		WB0_nEN		<= `nFALSE;
		WB0_MASK	<= `MASK4D_ALLMASKED;
		WB1_MASK	<= `MASK4D_ALLMASKED;
		WB0_DATA	<= 0;
		WB1_DATA	<= 0;
	end
	else begin
		pipe_nEN	<= {pipe_nEN[1:0], sm_nEN};
		sm_PC		<= sm_nEN	? sm_PC : PC;
		stall		<= {stall[0], MEM_STALL};
		//sm pass
		{ex_waddr0[0], ex_waddr1[0]} <= sm_nEN	? {ex_waddr0[0], ex_waddr1[0]}
		: {MO0_WADDR, MO1_WADDR};
		// ex0 pass
		{ex_waddr0[1], ex_waddr1[1]} <= pipe_nEN[0]	? {ex_waddr0[1], ex_waddr1[1]}
		: {ex_waddr0[0], ex_waddr1[0]};
		// ex1 pass
		{ex_waddr0[2], ex_waddr1[2]} <= pipe_nEN[1]	? {ex_waddr0[2], ex_waddr1[2]}
		: {ex_waddr0[1], ex_waddr1[1]};

		WB0_nEN		<= (&{alu_wmask_0, EW0_nEN}) | stall[1];	// 메모리 스톨은 phase #0 연산기 결과에만 영향을 준다.
		WB0_MASK	<= EW0_nEN ? alu_wmask_0 : EW0_MASK;
		WB0_ADDR	<= EW0_nEN ? ex_waddr0[2] : EW0_ADDR;
		WB0_DATA	<= EW0_nEN ? alu_data_0 : EW0_DATA;

		WB1_MASK	<= alu_wmask_1;
		WB1_ADDR	<= pipe_nEN[2] ? WB1_ADDR : ex_waddr1[2];
		WB1_DATA	<= pipe_nEN[2] ? WB1_DATA : alu_data_1;
	end
end

//------- bypass entry : end

// Common special function units
MTSP_SF				special_function
(	CLK, nRST,
  MO0_X, MO0_Y, MO0_Z, MO0_W, MO0_MASK,
  MO1_X, MO1_Y, MO1_Z, MO1_W, MO1_MASK,
  SRC0A, SRC0B,
  SRC1A, SRC1B,
  SF_WE0, SF_DOUT0, SF_WE1, SF_DOUT1);
// each component ALUs
MTSP_EX_Primtive	ex_x
(	CLK, nRST,
  sm_PC,
  MO0_X, MO0_MASK[`SELECT_X], SRC0A[`RANGE_DWORDx4_X], SRC0B[`RANGE_DWORDx4_X],
  MO1_X, MO1_MASK[`SELECT_X], SRC1A[`RANGE_DWORDx4_X], SRC1B[`RANGE_DWORDx4_X],
  SF_WE0[`SELECT_X], SF_DOUT0, SF_WE1[`SELECT_X], SF_DOUT1,
  converse_type_1[`SELECT_X],
  alu_data_0[`RANGE_DWORDx4_X], alu_wmask_0[`SELECT_X],
  alu_data_1[`RANGE_DWORDx4_X], alu_wmask_1[`SELECT_X]);
MTSP_EX_Primtive	ex_y
(	CLK, nRST,
  sm_PC,
  MO0_Y, MO0_MASK[`SELECT_Y], SRC0A[`RANGE_DWORDx4_Y], SRC0B[`RANGE_DWORDx4_Y],
  MO1_Y, MO1_MASK[`SELECT_Y], SRC1A[`RANGE_DWORDx4_Y], SRC1B[`RANGE_DWORDx4_Y],
  SF_WE0[`SELECT_Y], SF_DOUT0, SF_WE1[`SELECT_Y], SF_DOUT1,
  converse_type_1[`SELECT_Y],
  alu_data_0[`RANGE_DWORDx4_Y], alu_wmask_0[`SELECT_Y],
  alu_data_1[`RANGE_DWORDx4_Y], alu_wmask_1[`SELECT_Y]);
MTSP_EX_Primtive	ex_z
(	CLK, nRST,
  sm_PC,
  MO0_Z, MO0_MASK[`SELECT_Z], SRC0A[`RANGE_DWORDx4_Z], SRC0B[`RANGE_DWORDx4_Z],
  MO1_Z, MO1_MASK[`SELECT_Z], SRC1A[`RANGE_DWORDx4_Z], SRC1B[`RANGE_DWORDx4_Z],
  SF_WE0[`SELECT_Z], SF_DOUT0, SF_WE1[`SELECT_Z], SF_DOUT1,
  converse_type_1[`SELECT_Z],
  alu_data_0[`RANGE_DWORDx4_Z], alu_wmask_0[`SELECT_Z],
  alu_data_1[`RANGE_DWORDx4_Z], alu_wmask_1[`SELECT_Z]);
MTSP_EX_Primtive	ex_w
(	CLK, nRST,
  sm_PC,
  MO0_W, MO0_MASK[`SELECT_W], SRC0A[`RANGE_DWORDx4_W], SRC0B[`RANGE_DWORDx4_W],
  MO1_W, MO1_MASK[`SELECT_W], SRC1A[`RANGE_DWORDx4_W], SRC1B[`RANGE_DWORDx4_W],
  SF_WE0[`SELECT_W], SF_DOUT0, SF_WE1[`SELECT_W], SF_DOUT1,
  converse_type_1[`SELECT_W],
  alu_data_0[`RANGE_DWORDx4_W], alu_wmask_0[`SELECT_W],
  alu_data_1[`RANGE_DWORDx4_W], alu_wmask_1[`SELECT_W]);

endmodule
