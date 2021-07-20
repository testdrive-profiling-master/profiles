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

module MTSP_SCs_Primitive(
	// system
	input						CLK, nRST,			// main clock & reset (active low)
	// input
	input						nEN,				// Enable (active low)
	input	[`RANGE_WORD]		OP,					// Operations
	input	[`RANGE_WORD]		IX, IY, IZ, IW,		// Data input
	// output
	output	[`RANGE_WORD]		OX, OY, OZ, OW		// Data output
);

// register definition & assignment ------------------------------------------
wire	[3:0]				OP_X, OP_Y, OP_Z, OP_W;
assign	{OP_X, OP_Y, OP_Z, OP_W} = OP;

wire						Carry_X, Carry_Y, Carry_Z;

// implementation ------------------------------------------------------------
// Element X
MTSP_SCs_Element	SCs_X(
	.CLK			(CLK),
	.nRST			(nRST),
	.nEN			(nEN|(~OP_X[2])),
	.OP				(OP_X[1:0]),
	.CARRY_IN		(1'b0),
	.DATA_IN		(IX),
	.CARRY_OUT		(Carry_X),
	.DATA_OUT		(OX)
);

// Element Y
MTSP_SCs_Element	SCs_Y(
	.CLK			(CLK),
	.nRST			(nRST),
	.nEN			(nEN|(~OP_Y[2])),
	.OP				(OP_Y[1:0]),
	.CARRY_IN		(Carry_X),
	.DATA_IN		(IY),
	.CARRY_OUT		(Carry_Y),
	.DATA_OUT		(OY)
);

// Element Z
MTSP_SCs_Element	SCs_Z(
	.CLK			(CLK),
	.nRST			(nRST),
	.nEN			(nEN|(~OP_Z[2])),
	.OP				(OP_Z[1:0]),
	.CARRY_IN		(Carry_Y),
	.DATA_IN		(IZ),
	.CARRY_OUT		(Carry_Z),
	.DATA_OUT		(OZ)
);

// Element W
MTSP_SCs_Element	SCs_W(
	.CLK			(CLK),
	.nRST			(nRST),
	.nEN			(nEN|(~OP_W[2])),
	.OP				(OP_W[1:0]),
	.CARRY_IN		(Carry_Z),
	.DATA_IN		(IW),
	.CARRY_OUT		(),
	.DATA_OUT		(OW)
);

endmodule
