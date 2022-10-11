//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : FPU 32bit(IEEE-754) unit
// Rev.  : 9/2/2022 Fri (clonextop@gmail.com)
//================================================================================
`include "../system_defines.vh"

module FPU_F32_fully_stage_Divide_3 #(
	parameter				CYCLE	= 5
) (
	input					CLK,		// clock
	input					nRST,		// reset (active low)
	input					IE,			// input enable
	input	[31:0]			A,			// A
	input	[31:0]			B,			// B
	output					OE,			// output enable
	output					EXCEPTION,	// EXCEPTION
	output	[31:0]			O			// output
);

// definition & assignment ---------------------------------------------------
genvar i;

wire	[(CYCLE*(32*2))-1:0]	pipe_i;
wire	[(CYCLE*(32+1))-1:0]	pipe_o;

// implementation ------------------------------------------------------------
(* dont_touch = "yes" *) MultiCyclePath #(
	.IWIDTH		(32*2),
	.OWIDTH		(32 + 1),
	.CYCLE		(CYCLE)
) multi_pipe (
	.CLK		(CLK),
	.nRST		(nRST),
	.IE			(IE),
	.IDATA		({A, B}),
	.PIPE_I		(pipe_i),
	.PIPE_O		(pipe_o),
	.OE			(OE),
	.ODATA		({EXCEPTION, O})
);

generate
for(i=0;i<CYCLE;i=i+1) begin : divide_gen
	wire	[32-1:0]	a,b,o;
	wire				e;	// exception
	assign	{a,b}		= pipe_i[`BUS_RANGE((32*2), i)];
	assign	{e, o}		= pipe_o[`BUS_RANGE((32+1), i)];
	//assign	pipe_o[`BUS_RANGE((32+1), i)]	= {e, o};
	FPU_F32_Divide	divide_inst(
		.a_operand	(a),
		.b_operand	(b),
		.Exception	(e),
		.result		(o)
	);
end
endgenerate

endmodule
