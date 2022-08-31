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
// Rev.  : 8/31/2022 Wed (clonextop@gmail.com)
//================================================================================

module FPU_F32_Divide(
	input [31:0]		a_operand,
	input [31:0]		b_operand,
	output				Exception,
	output [31:0]		result
);

wire sign;
wire [7:0] shift;
wire [7:0] exponent_a;
wire [31:0] divisor;
wire [31:0] operand_a;
wire [31:0] Intermediate_X0;
wire [31:0] Iteration_X0;
wire [31:0] Iteration_X1;
wire [31:0] Iteration_X2;
wire [31:0] Iteration_X3;
wire [31:0] solution;

wire [31:0] denominator;
wire [31:0] operand_a_change;

assign Exception = (&a_operand[30:23]) | (&b_operand[30:23]);

assign sign = a_operand[31] ^ b_operand[31];

assign shift = 8'd126 - b_operand[30:23];

assign divisor = {1'b0,8'd126,b_operand[22:0]};

assign denominator = divisor;

assign exponent_a = a_operand[30:23] + shift;

assign operand_a = {a_operand[31],exponent_a,a_operand[22:0]};

assign operand_a_change = operand_a;

//32'hC00B_4B4B = (-37)/17
FPU_F32_Multiply x0(32'hC00B_4B4B,divisor,,,,Intermediate_X0);

//32'h4034_B4B5 = 48/17
Addition_Subtraction X0(Intermediate_X0,32'h4034_B4B5,1'b0,,Iteration_X0);

FPU_F32_Iteration X1(Iteration_X0,divisor,Iteration_X1);

FPU_F32_Iteration X2(Iteration_X1,divisor,Iteration_X2);

FPU_F32_Iteration X3(Iteration_X2,divisor,Iteration_X3);

FPU_F32_Multiply END(Iteration_X3,operand_a,,,,solution);

assign result = {sign,solution[30:0]};
endmodule
