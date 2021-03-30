//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : FPU
// Rev.  : 3/30/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "FPU_F32_ADD.sv"
`include "FPU_F32_MUL.sv"
`include "FPU_F32_DIV.sv"
`include "FPU_F32_to_INT.sv"
`include "FPU_INT_to_F32.sv"

/* HIDDEN */
module top (
	input						nRST,
	input						MCLK,
	output						INTR,
	output	reg					BUSY
);

// definition & assignment ---------------------------------------------------
assign	INTR			= `FALSE;

reg		[31:0]			A, B, golden_data;
wire	[31:0]			O;
wire					PASS	= (O == golden_data);

`DPI_FUNCTION bit FPU_32f_testbench_get(output bit [31:0] A, output bit [31:0] B);

// implementation ------------------------------------------------------------
always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		A				= 'd0;
		B				= 'd0;
		BUSY			= `TRUE;
	end
	else begin
		if(!FPU_32f_testbench_get(A, B)) begin
			BUSY	= `FALSE;
		end
	end
end

`ifdef FPU_ADDER
// golden data
`DPI_FUNCTION void FPU_32f_Add(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A, B) FPU_32f_Add(A, B, golden_data);

// FPU unit output
FPU_F32_ADD fpu_inst(
	.A(A),
	.B(B),
	.O(O)
);
`endif

`ifdef FPU_MULTIPLIER
// golden data
`DPI_FUNCTION void FPU_32f_Multiply(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A, B) FPU_32f_Multiply(A, B, golden_data);

// FPU unit output
FPU_F32_MUL fpu_inst(
	.A(A),
	.B(B),
	.O(O)
);
`endif

`ifdef FPU_DIVIDER
// golden data
`DPI_FUNCTION void FPU_32f_Divide(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A, B) FPU_32f_Divide(A, B, golden_data);

// FPU unit output
FPU_F32_DIV fpu_inst(
	.A(A),
	.B(B),
	.O(O)
);
`endif

`ifdef FPU_FLOAT2INT
// golden data
`DPI_FUNCTION void FPU_32f_to_int(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A) FPU_32f_to_int(A, golden_data);

// FPU unit output
FPU_F32_to_INT fpu_inst(
	.A(A),
	.O(O)
);
`endif

`ifdef FPU_INT2FLOAT
// golden data
`DPI_FUNCTION void FPU_32f_from_int(input bit [31:0] A, input bit [31:0] B, output bit [31:0] O);
always@(A) FPU_32f_from_int(A, golden_data);

// FPU unit output
FPU_INT_to_F32 fpu_inst(
	.A(A),
	.O(O)
);
`endif

endmodule
