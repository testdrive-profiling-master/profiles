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
`include "FPU_F32_AddSub.v"
`include "FPU_F32_Multiply.v"
`include "FPU_F32_Divide.v"
`include "FPU_F32_FloatToInt.v"

module FPU_F32_ALU(
	input [31:0] a_operand,
	input [31:0] b_operand,
	input [3:0] Operation,
	output [31:0] ALU_Output,
	output Exception,Overflow,Underflow
);


wire [31:0] Add_Sub_A,Add_Sub_B,Mul_A,Mul_B,Div_A,Div_B,OR_Output,AND_Output,XOR_Output,L_S_Output,R_S_Output,Floating_Point;
wire Add_Sub_Exception,Mul_Exception,Mul_Overflow,Mul_Underflow,Div_Exception;
wire [31:0] Add_Sub_Output,Mul_Output,Div_Output,Integer_Value;
wire AddBar_Sub;
wire [31:0] Complement_output;



assign {Add_Sub_A,Add_Sub_B,AddBar_Sub} = (Operation == 4'd10) ? {a_operand,b_operand,1'b0} : 64'dz;
assign {Mul_A,Mul_B} = (Operation == 4'd1) ? {a_operand,b_operand} : 64'dz;
assign {Div_A,Div_B} = (Operation == 4'd2) ? {a_operand,b_operand}	: 64'dz;
assign {Add_Sub_A,Add_Sub_B,AddBar_Sub} = (Operation == 4'd3) ? {a_operand,b_operand,1'b1} : 64'dz;
assign OR_Output = (Operation == 4'd4) ? a_operand | b_operand	: 32'dz;
assign AND_Output = (Operation == 4'd5) ? a_operand & b_operand	: 32'dz;
assign XOR_Output = (Operation == 4'd6) ? a_operand ^ b_operand: 32'dz;
assign L_S_Output = (Operation == 4'd7) ? a_operand << 1'b1 : 32'dz;
assign R_S_Output = (Operation == 4'd8) ? a_operand >> 1'b1	: 32'dz;
assign Floating_Point = (Operation == 4'd9) ? a_operand	: 32'dz;
assign Complement_output = (Operation == 4'd11) ? !a_operand : 32'dz;


FPU_F32_AddSub		AuI(Add_Sub_A,Add_Sub_B,AddBar_Sub,Add_Sub_Exception,Add_Sub_Output);
FPU_F32_Multiply	MuI(Mul_A,Mul_B,Mul_Exception,Mul_Overflow,Mul_Underflow,Mul_Output);
FPU_F32_Divide		DuI(Div_A,Div_B,Div_Exception,Div_Output);
FPU_F32_FloatToInt	FuI(Floating_Point,Integer_Value);


assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd10) ? {Add_Sub_Exception,1'b0,1'b0,Add_Sub_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd1) ? {Mul_Exception,Mul_Overflow,Mul_Underflow,Mul_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd2) ? {Div_Exception,1'b0,1'b0,Div_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd3) ? {Add_Sub_Exception,1'b0,1'b0,Add_Sub_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd4) ? {1'b0,1'b0,1'b0,OR_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd5) ? {1'b0,1'b0,1'b0,AND_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd6) ? {1'b0,1'b0,1'b0,XOR_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd7) ? {1'b0,1'b0,1'b0,L_S_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd8) ? {1'b0,1'b0,1'b0,R_S_Output}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd9) ? {1'b0,1'b0,1'b0,Integer_Value}	: 35'dz;
assign {Exception,Overflow,Underflow,ALU_Output} = (Operation == 4'd11) ? {1'b0,1'b0,1'b0,Complement_output} : 35'dz;

endmodule
