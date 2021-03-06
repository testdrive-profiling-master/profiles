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
// Title : Processor
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"

module MAD_multicycled_Slice #(
	parameter				CYCLE	= 3,
	parameter				COUNT	= 2
) (
	input					CLK,	// clock
	input					nRST,	// reset (active low)
	input					IE,		// input enable
	output					IREADY,	// input ready
	input	[31:0]			A,		// A
	input	[31:0]			B,		// B
	input	[31:0]			C,		// C
	output					OE,		// output enable
	output	[31:0]			O		// output
);

// definition & assignment ---------------------------------------------------
wire	[32-1:0]	result;

// implementation ------------------------------------------------------------
MAD mad_inst(
	.A		(A),
	.B		(B),
	.C		(C),
	.O		(result)
);

MultiCycleSlice #(
	.WIDTH			(32),
	.CYCLE			(3),
	.CHAINED		(0)
) muti_slice (
	.CLK			(CLK),
	.nRST			(nRST),
	.IE				(IE),
	.IDATA			(result),
	.IREADY			(IREADY),
	.OE				(OE),
	.ODATA			(O),
	.OREADY			(`TRUE)
);

endmodule
