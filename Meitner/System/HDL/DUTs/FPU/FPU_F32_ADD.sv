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
// Title : FPU 32bit(IEEE-754) unit
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "FPU.vh"

/* HIDDEN */
module FPU_F32_ADD (
	input						SUBTRACT,
	input	[31:0]				A,
	input	[31:0]				B,
	output	[31:0]				O
);

// definition & assignment ---------------------------------------------------
`ifdef SW_FPU_MODE
// software implimentation
`DPI_FUNCTION void FPU_32f_Add(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A, B, SUBTRACT) FPU_32f_Add(A, {SUBTRACT^B[31], B[30:0]}, O);
`else
// hardware implimentation
wire	[22:0]		a_m, b_m, o_m;
wire	[7:0]		a_e, b_e, o_e;
wire				a_s, b_s, o_s;

// Unpack input
assign				{a_s, a_e, a_m}	= A;
assign				{b_s, b_e, b_m}	= {SUBTRACT^B[31], B[30:0]};

// implementation ------------------------------------------------------------
// special case (NaN, INF)
wire				a_nz	= |a_m;			// A's mantissa is not zero
wire				b_nz	= |b_m;			// B's mantissa is not zero
wire				a_ef	= &a_e;			// A's exponent is full
wire				b_ef	= &a_e;			// B's exponent is full

wire	NaN			= (a_ef & ~a_nz) | (b_ef & ~b_nz);		// If A is NaN or B is NaN, result is NaN
wire	INF			= (a_ef & a_nz) | (b_ef & b_nz);		// Infinity number

// result output
assign	O			= NaN ? 32'b0_11111111_00000000000000000000000 : {o_s, o_e, o_m};

`endif

endmodule
