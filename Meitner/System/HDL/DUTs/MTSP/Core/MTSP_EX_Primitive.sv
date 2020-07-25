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

module MTSP_EX_Primtive(	CLK, nRST,
						 PC,
						 MO0, MO0_MASK, SRC0A, SRC0B,
						 MO1, MO1_MASK, SRC1A, SRC1B,
						 SF_WE0, SF_DOUT0, SF_WE1, SF_DOUT1,
						 CONVERSE_TYPE,
						 DEST0, WMASK0,
						 DEST1, WMASK1);

// port list -----------------------------------------------------------------
// System
input							CLK, nRST;				// main clock & reset (active low)

// input
input	[`RANGE_PC]				PC;						// next instruction program counter @ SM output
input	[`RANGE_MODESC]			MO0, MO1;				// phase #0/#1 micro-operation description
input							MO0_MASK, MO1_MASK;		// phase #0/#1 micro-operation mask
input	[`RANGE_DWORD]			SRC0A, SRC0B,			// phase #0 source data
SRC1A, SRC1B;			// phase #1 source data
input							SF_WE0, SF_WE1;			// special function write mask
input	[`RANGE_DWORD]			SF_DOUT0, SF_DOUT1;		// special function result input
// output
output							CONVERSE_TYPE;			// converse with fp. and int.
output	[`RANGE_DWORD]			DEST0, DEST1;			// phase #0/#1 output data
output							WMASK0, WMASK1;			// phase #0/#1 write mask
// definition & assignment ---------------------------------------------------
wire	[1:0]					en_flfr, en_conv, en_bw, en_cmp, en_mul, en_add, en_badd, en_fmt;
wire	[`RANGE_DWORD]			out_flfr, out_conv, out_bw, out_cmp, out_mul, out_add, out_badd_0, out_badd_1, out_fmt_0, out_fmt_1;	// ALU's output
assign	CONVERSE_TYPE			= en_conv[1];

// implementation ------------------------------------------------------------
// Floor
MTSP_ALU_FLFR	alu_flfr
(	CLK, nRST,
  MO0, MO0_MASK, SRC0A, SRC0B,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_flfr, out_flfr);

// FP/INT conversion
MTSP_ALU_CONV	alu_conv
(	CLK, nRST,
  MO0, MO0_MASK, SRC0A, SRC0B,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_conv, out_conv);
// Bitwise
MTSP_ALU_BW		alu_bw
(	CLK, nRST,
  MO0, MO0_MASK, SRC0A, SRC0B,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_bw, out_bw);
// Compare
MTSP_ALU_CMP	alu_cmp
(	CLK, nRST,
  MO0, MO0_MASK, SRC0A, SRC0B,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_cmp, out_cmp);
// Multiplier
MTSP_ALU_MUL	alu_mul
(	CLK, nRST,
  MO0, SRC0A, SRC0B,
  MO1, SRC1A, SRC1B,
  en_mul, out_mul);
// Adder
MTSP_ALU_ADD	alu_add
(	CLK, nRST,
  MO0, MO0_MASK, SRC0A, SRC0B,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_add, out_add);
// Byte generator
MTSP_ALU_BA		alu_ba_0
(	CLK, nRST,
  MO0, SRC0A, SRC0B,
  MO1, SRC1A, SRC1B,
  en_badd, out_badd_0, out_badd_1);

MTSP_ALU_FMT	alu_fmt_0
(	CLK, nRST,
  MO0, SRC0A, SRC0B,
  en_fmt[0], out_fmt_0);

MTSP_ALU_FMT	alu_fmt_1
(	CLK, nRST,
  MO1, SRC1A, SRC1B,
  en_fmt[1], out_fmt_1);

// Move : phase #0
MTSP_ALU_MOV	alu_mov_phase_0
(	CLK, nRST,
  PC,
  MO0, MO0_MASK, SRC0A, SRC0B,
  en_flfr[0], en_conv[0], en_bw[0], en_cmp[0], en_mul[0], en_add[0], SF_WE0, en_badd[0], en_fmt[0],
  out_flfr, out_conv, out_bw, out_cmp, out_mul, out_add, SF_DOUT0, out_badd_0, out_fmt_0,
  WMASK0, DEST0);
// Move : phase #1
MTSP_ALU_MOV	alu_mov_phase_1
(	CLK, nRST,
  PC,
  MO1, MO1_MASK, SRC1A, SRC1B,
  en_flfr[1], en_conv[1], en_bw[1], en_cmp[1], en_mul[1], en_add[1], SF_WE1, en_badd[1], en_fmt[1],
  out_flfr, out_conv, out_bw, out_cmp, out_mul, out_add, SF_DOUT1, out_badd_1, out_fmt_1,
  WMASK1, DEST1);
endmodule
