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
// Rev.  : 4/7/2023 Fri (clonextop@gmail.com)
//================================================================================
module FPU_F32_Adder(
	input	[31:0]		A,
	input	[31:0]		B,
	output				Exception,
	output	[31:0]		O	              	//Outputs in the format of IEEE-754 Representation.
);

wire	[31:0]			a, b;
// A must greater than B
assign {a, b} = (A[30:0] < B[30:0]) ? {B, A} : {A, B};

// dispatch floating point component
wire					sign_a, sign_b;
wire	[7:0]			exp_a, exp_b;
wire	[22:0]			mantissa_a, mantissa_b;

assign	{sign_a, exp_a, mantissa_a}		= a;
assign	{sign_b, exp_b, mantissa_b}		= b;

// significand
wire	[23:0]			significand_a	= {|exp_a, mantissa_a};
wire	[23:0]			significand_b	= {|exp_b, mantissa_b};

// sub operation?
wire	operation_sub	= A[31] ^ B[31];

// output sign
wire	sign_o			= sign_a;

// exponential difference
wire	[7:0]			exp_diff		= exp_a - exp_b;

// significand_b shifted
wire	[24:0]			significand_b_shifted = (
	(exp_diff == 8'd0 ) ? {significand_b, 1'b0} :
	(exp_diff == 8'd1 ) ? { 1'b0, significand_b} :
	(exp_diff == 8'd2 ) ? { 2'b0, significand_b[23:1]} :
	(exp_diff == 8'd3 ) ? { 3'b0, significand_b[23:2]} :
	(exp_diff == 8'd4 ) ? { 4'b0, significand_b[23:3]} :
	(exp_diff == 8'd5 ) ? { 5'b0, significand_b[23:4]} :
	(exp_diff == 8'd6 ) ? { 6'b0, significand_b[23:5]} :
	(exp_diff == 8'd7 ) ? { 7'b0, significand_b[23:6]} :
	(exp_diff == 8'd8 ) ? { 8'b0, significand_b[23:7]} :
	(exp_diff == 8'd9 ) ? { 9'b0, significand_b[23:8]} :
	(exp_diff == 8'd10) ? {10'b0, significand_b[23:9]} :
	(exp_diff == 8'd11) ? {11'b0, significand_b[23:10]} :
	(exp_diff == 8'd12) ? {12'b0, significand_b[23:11]} :
	(exp_diff == 8'd13) ? {13'b0, significand_b[23:12]} :
	(exp_diff == 8'd14) ? {14'b0, significand_b[23:13]} :
	(exp_diff == 8'd15) ? {15'b0, significand_b[23:14]} :
	(exp_diff == 8'd16) ? {16'b0, significand_b[23:15]} :
	(exp_diff == 8'd17) ? {17'b0, significand_b[23:16]} :
	(exp_diff == 8'd18) ? {18'b0, significand_b[23:17]} :
	(exp_diff == 8'd19) ? {19'b0, significand_b[23:18]} :
	(exp_diff == 8'd20) ? {20'b0, significand_b[23:19]} :
	(exp_diff == 8'd21) ? {21'b0, significand_b[23:20]} :
	(exp_diff == 8'd22) ? {22'b0, significand_b[23:21]} :
	(exp_diff == 8'd23) ? {23'b0, significand_b[23:22]} :
	(exp_diff == 8'd24) ? {24'b0, significand_b[23]} :
	/* default */         25'd0
);

//----------------------------------------------------------
// adder module
wire	[24:0]		significand_add 	= {1'b0, significand_a} + {1'b0, significand_b_shifted[24:1]};
wire	[24:0]		significand_add_o	= sign_o ? significand_add : (significand_add + {24'd0, significand_b_shifted[0]});

wire	[22:0]		mantissa_add		= significand_add_o[24] ? significand_add_o[23:1] : significand_add_o[22:0];
wire	[30:23]		exp_add				= significand_add_o[24] ? (1'b1 + exp_a) : exp_a;

//----------------------------------------------------------
// sub module
wire	[24:0]		significand_sub 	= {significand_a, 1'b0} - {significand_b_shifted};
wire	[23:0]		significand_sub_o	= sign_o ? significand_sub[24:1] : (significand_sub[24:1] + {23'd0, significand_sub[0]});
/*wire	[22:0]		mantissa_sub		= (

);*/


wire [23:0] significand_b_add_sub;
wire [7:0] exponent_b_add_sub;

wire [23:0] significand_sub_complement;
wire [24:0] significand_sub;
wire [30:0] sub_diff;
wire [24:0] subtraction_diff;
wire [7:0] exponent_sub;

//Exception flag sets 1 if either one of the exponent is 255.
assign Exception			= (&a[30:23]) | (&b[30:23]);

//Shifting significand_b according to exp_diff
assign significand_b_add_sub = significand_b >> exp_diff;

assign exponent_b_add_sub = b[30:23] + exp_diff;

//Checking exponents are same or not
wire perform = (a[30:23] == exponent_b_add_sub);

///////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------ADD BLOCK------------------------------------------//



///////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------SUB BLOCK------------------------------------------//

assign significand_sub_complement = (perform & operation_sub) ? ~(significand_b_add_sub) + 24'd1 : 24'd0 ;

assign significand_sub = perform ? (significand_a + significand_sub_complement) : 25'd0;

FPU_F32_PriorityEncoder pe(significand_sub, a[30:23], subtraction_diff, exponent_sub);

assign sub_diff[30:23] = exponent_sub;

assign sub_diff[22:0] = subtraction_diff[22:0];

///////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------OUTPUT--------------------------------------------//

//If there is no exception and operation will evaluate
assign O = Exception ? 32'b0 : (operation_sub ? {sign_o, sub_diff} : {sign_o, exp_add, mantissa_add});

endmodule
