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

module MTSP_SrcOperate4D(	OP, SRC_IN,
						  SRC_OUT);

// port list -----------------------------------------------------------------
// input
input	[`RANGE_SRCOP]		OP;						// unit instructions input
input	[`RANGE_DWORDx4]	SRC_IN;					// source input

// output
output	[`RANGE_DWORDx4]	SRC_OUT;				// source output

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
MTSP_SrcOperate1D	operate_x	(OP, SRC_IN[`RANGE_DWORDx4_X], SRC_OUT[`RANGE_DWORDx4_X]),
operate_y	(OP, SRC_IN[`RANGE_DWORDx4_Y], SRC_OUT[`RANGE_DWORDx4_Y]),
operate_z	(OP, SRC_IN[`RANGE_DWORDx4_Z], SRC_OUT[`RANGE_DWORDx4_Z]),
operate_w	(OP, SRC_IN[`RANGE_DWORDx4_W], SRC_OUT[`RANGE_DWORDx4_W]);
endmodule
