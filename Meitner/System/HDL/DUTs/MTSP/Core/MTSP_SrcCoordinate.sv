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

module MTSP_SrcCoordinate(	PHASE, SUB, UINST, SRC,
						   SRC_OUT);

// port list -----------------------------------------------------------------
// input
input						PHASE;					// phase number
input						SUB;					// sub instruction
input	[`RANGE_UINST]		UINST;					// unit instructions input
input	[`RANGE_DWORDx4]	SRC;

// output
output	[`RANGE_DWORDx4]	SRC_OUT;

// definition & assignment ---------------------------------------------------
wire	[`RANGE_MO]			MO		= UINST[`RANGE_UINST_MO];
wire						nALU	= UINST[`RANGE_UINST_nALU];
wire	[`RANGE_SRCOP]		OP		= nALU ? 3'b000 : {UINST[`RANGE_UINST_SELECT],
		(UINST[`RANGE_UINST_SWITCH]&SUB) ? 2'b0 : {UINST[`RANGE_UINST_MODIFIER], UINST[`RANGE_UINST_NEGATE]}};
wire	[`RANGE_SWZ4D]		SWZ		= nALU ? {`DATA_X, `DATA_Y, `DATA_Z, `DATA_W} : UINST[`RANGE_UINST_SWZ4D];

wire	[`RANGE_DWORDx4]	src_data_swz;

// implementation ------------------------------------------------------------
MTSP_SrcSwizzle4D	src_swz	(OP, SWZ, SRC, {UINST[`RANGE_UINST_SRC], UINST[`RANGE_BYTE]}, src_data_swz);
MTSP_SrcOperate4D	src_op	(OP, src_data_swz, SRC_OUT);

endmodule
