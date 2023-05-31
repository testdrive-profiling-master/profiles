//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 5/31/2023 Wed (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_SrcOperate1D(	OP, SRC_IN,
						  SRC_OUT);

// port list -----------------------------------------------------------------
// input
input	[`RANGE_SRCOP]		OP;				// unit instructions input
input	[`RANGE_DWORD]		SRC_IN;			// source input

// output
output	[`RANGE_DWORD]		SRC_OUT;		// source output

// definition & assignment ---------------------------------------------------
wire						bINT	= OP[`RANGE_SRCOP_SELECT],		// integer operation
bNEG	= OP[`RANGE_SRCOP_NEGATE];		// negate

wire	[`RANGE_DWORD]		src_fp, src_int;

// implementation ------------------------------------------------------------
// 24bit floating point일 경우
assign	src_fp		= {SRC_IN[31:24], (bNEG^SRC_IN[`RANGE_DATA_SIGN]), SRC_IN[`MSB_DATA_EXP:0]};
// integer일 경우
assign	src_int		= bNEG ? -SRC_IN : SRC_IN;
// Negate/Absolute 결과 출력
assign	SRC_OUT		= {(bINT ? src_int : src_fp)};

endmodule
