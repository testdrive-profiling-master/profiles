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

module MTSP_SrcSwizzle4D(	OP, SWZ, SRC_IN, SRC_CONST,
						  SRC_OUT);

// port list -----------------------------------------------------------------
// input
input	[`RANGE_SRCOP]		OP;						// unit instructions input
input	[`RANGE_SWZ4D]		SWZ;					// swizzle operation
input	[`RANGE_DWORDx4]	SRC_IN;					// sources input
input	[12:0]				SRC_CONST;				// constant input
// output
output	[`RANGE_DWORDx4]	SRC_OUT;				// source output

// definition & assignment ---------------------------------------------------
wire						bINT	= OP[`RANGE_SRCOP_SELECT],		// integer operation
bMOD	= OP[`RANGE_SRCOP_MODIFIER];	// source modifier

wire	[`RANGE_DWORD]		src_mod;
wire	[`RANGE_DWORDx4]	src_swz;
// implementation ------------------------------------------------------------
assign	src_swz[`RANGE_DWORDx4_X]		= (SWZ[`RANGE_SWZ4D_X] == `DATA_X) ? SRC_IN[`RANGE_DWORDx4_X] :
(SWZ[`RANGE_SWZ4D_X] == `DATA_Y) ? SRC_IN[`RANGE_DWORDx4_Y] :
(SWZ[`RANGE_SWZ4D_X] == `DATA_Z) ? SRC_IN[`RANGE_DWORDx4_Z] : SRC_IN[`RANGE_DWORDx4_W],
src_swz[`RANGE_DWORDx4_Y]		= (SWZ[`RANGE_SWZ4D_Y] == `DATA_X) ? SRC_IN[`RANGE_DWORDx4_X] :
(SWZ[`RANGE_SWZ4D_Y] == `DATA_Y) ? SRC_IN[`RANGE_DWORDx4_Y] :
(SWZ[`RANGE_SWZ4D_Y] == `DATA_Z) ? SRC_IN[`RANGE_DWORDx4_Z] : SRC_IN[`RANGE_DWORDx4_W],
src_swz[`RANGE_DWORDx4_Z]		= (SWZ[`RANGE_SWZ4D_Z] == `DATA_X) ? SRC_IN[`RANGE_DWORDx4_X] :
(SWZ[`RANGE_SWZ4D_Z] == `DATA_Y) ? SRC_IN[`RANGE_DWORDx4_Y] :
(SWZ[`RANGE_SWZ4D_Z] == `DATA_Z) ? SRC_IN[`RANGE_DWORDx4_Z] : SRC_IN[`RANGE_DWORDx4_W],
src_swz[`RANGE_DWORDx4_W]		= (SWZ[`RANGE_SWZ4D_W] == `DATA_X) ? SRC_IN[`RANGE_DWORDx4_X] :
(SWZ[`RANGE_SWZ4D_W] == `DATA_Y) ? SRC_IN[`RANGE_DWORDx4_Y] :
(SWZ[`RANGE_SWZ4D_W] == `DATA_Z) ? SRC_IN[`RANGE_DWORDx4_Z] : SRC_IN[`RANGE_DWORDx4_W];

assign	src_mod	= bINT	? {19'b0, SRC_CONST} : {1'b0, SRC_CONST[6:0], 16'b0};

assign	SRC_OUT	= bMOD	? {src_mod, src_mod, src_mod, src_mod} : src_swz;

endmodule
