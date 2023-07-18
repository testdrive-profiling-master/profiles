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
wire	[7:0]			exp_org_a, exp_org_b, exp_a, exp_b;
wire	[22:0]			mantissa_a, mantissa_b;

assign	{sign_a, exp_org_a, mantissa_a}	= a;
assign	{sign_b, exp_org_b, mantissa_b}	= b;
// denormalized number check
assign	exp_a			= (exp_org_a == 0) ? 1 : exp_org_a;
assign	exp_b			= (exp_org_b == 0) ? 1 : exp_org_b;

// significand
wire	[31:0]			significand_a	= {1'b0, |exp_a, mantissa_a, 7'd0};
wire	[31:0]			significand_b	= {1'b0, |exp_b, mantissa_b, 7'd0};

// sub operation?
wire	operation_sub	= A[31] ^ B[31];

// exponential difference
wire	[7:0]			exp_diff	= exp_a - exp_b;

// significand_b shifted
wire	[31:0]			significand_b_shifted = (
	(exp_diff ==  'd0) ? {       significand_b} :
	(exp_diff ==  'd1) ? { 1'b0, significand_b[31:1]} :
	(exp_diff ==  'd2) ? { 2'b0, significand_b[31:2]} :
	(exp_diff ==  'd3) ? { 3'b0, significand_b[31:3]} :
	(exp_diff ==  'd4) ? { 4'b0, significand_b[31:4]} :
	(exp_diff ==  'd5) ? { 5'b0, significand_b[31:5]} :
	(exp_diff ==  'd6) ? { 6'b0, significand_b[31:6]} :
	(exp_diff ==  'd7) ? { 7'b0, significand_b[31:7]} :
	(exp_diff ==  'd8) ? { 8'b0, significand_b[31:8]} :
	(exp_diff ==  'd9) ? { 9'b0, significand_b[31:9]} :
	(exp_diff == 'd10) ? {10'b0, significand_b[31:10]} :
	(exp_diff == 'd11) ? {11'b0, significand_b[31:11]} :
	(exp_diff == 'd12) ? {12'b0, significand_b[31:12]} :
	(exp_diff == 'd13) ? {13'b0, significand_b[31:13]} :
	(exp_diff == 'd14) ? {14'b0, significand_b[31:14]} :
	(exp_diff == 'd15) ? {15'b0, significand_b[31:15]} :
	(exp_diff == 'd16) ? {16'b0, significand_b[31:16]} :
	(exp_diff == 'd17) ? {17'b0, significand_b[31:17]} :
	(exp_diff == 'd18) ? {18'b0, significand_b[31:18]} :
	(exp_diff == 'd19) ? {19'b0, significand_b[31:19]} :
	(exp_diff == 'd20) ? {20'b0, significand_b[31:20]} :
	(exp_diff == 'd21) ? {21'b0, significand_b[31:21]} :
	(exp_diff == 'd22) ? {22'b0, significand_b[31:22]} :
	(exp_diff == 'd23) ? {23'b0, significand_b[31:23]} :
	(exp_diff == 'd24) ? {24'b0, significand_b[31:24]} :
	(exp_diff == 'd25) ? {25'b0, significand_b[31:25]} :
	(exp_diff == 'd26) ? {26'b0, significand_b[31:26]} :
	(exp_diff == 'd27) ? {27'b0, significand_b[31:27]} :
	(exp_diff == 'd28) ? {28'b0, significand_b[31:28]} :
	(exp_diff == 'd29) ? {29'b0, significand_b[31:29]} :
	(exp_diff == 'd30) ? {30'b0, significand_b[31:30]} :
	(exp_diff == 'd31) ? {31'b0, significand_b[31]   } : 32'd0
);

// output sign
wire	sign_o			= ~(operation_sub & (significand_a == significand_b_shifted)) & sign_a;

// add/sub significand
wire	[31:0]			significand_ans	= operation_sub ? (significand_a - significand_b_shifted) : (significand_a + significand_b_shifted);

// normalize
wire	[22:0]			significand_normalized	= (
	significand_ans[31] ? significand_ans[30:8] :
	significand_ans[30] ? significand_ans[29:7] :
	significand_ans[29] ? significand_ans[28:6] :
	significand_ans[28] ? significand_ans[27:5] :
	significand_ans[27] ? significand_ans[26:4] :
	significand_ans[26] ? significand_ans[25:3] :
	significand_ans[25] ? significand_ans[24:2] :
	significand_ans[24] ? significand_ans[23:1] :
	significand_ans[23] ? significand_ans[22:0] :
	significand_ans[22] ? { 1'd0, significand_ans[21:0]} :
	significand_ans[21] ? { 2'd0, significand_ans[20:0]} :
	significand_ans[20] ? { 3'd0, significand_ans[19:0]} :
	significand_ans[19] ? { 4'd0, significand_ans[18:0]} :
	significand_ans[18] ? { 5'd0, significand_ans[17:0]} :
	significand_ans[17] ? { 6'd0, significand_ans[16:0]} :
	significand_ans[16] ? { 7'd0, significand_ans[15:0]} :
	significand_ans[15] ? { 8'd0, significand_ans[14:0]} :
	significand_ans[14] ? { 9'd0, significand_ans[13:0]} :
	significand_ans[13] ? {10'd0, significand_ans[12:0]} :
	significand_ans[12] ? {11'd0, significand_ans[11:0]} :
	significand_ans[11] ? {12'd0, significand_ans[10:0]} :
	significand_ans[10] ? {13'd0, significand_ans[ 9:0]} :
	significand_ans[ 9] ? {14'd0, significand_ans[ 8:0]} :
	significand_ans[ 8] ? {15'd0, significand_ans[ 7:0]} :
	significand_ans[ 7] ? {16'd0, significand_ans[ 6:0]} :
	significand_ans[ 6] ? {17'd0, significand_ans[ 5:0]} :
	significand_ans[ 5] ? {18'd0, significand_ans[ 4:0]} :
	significand_ans[ 4] ? {19'd0, significand_ans[ 3:0]} :
	significand_ans[ 3] ? {20'd0, significand_ans[ 2:0]} :
	significand_ans[ 2] ? {21'd0, significand_ans[ 1:0]} :
	significand_ans[ 1] ? {22'd0, significand_ans[   0]} :
	/* none */            23'd0
);

// rounding
wire	residual		= (
	significand_ans[31] ? significand_ans[7] :
	significand_ans[30] ? significand_ans[6] :
	significand_ans[29] ? significand_ans[5] :
	significand_ans[28] ? significand_ans[4] :
	significand_ans[27] ? significand_ans[3] :
	significand_ans[26] ? significand_ans[2] :
	significand_ans[25] ? significand_ans[1] :
	significand_ans[24] ? significand_ans[0] : 1'b0
);

wire	sticky			= (
	significand_ans[31] ? |significand_ans[6:0] :
	significand_ans[30] ? |significand_ans[5:0] :
	significand_ans[29] ? |significand_ans[4:0] :
	significand_ans[28] ? |significand_ans[3:0] :
	significand_ans[27] ? |significand_ans[2:0] :
	significand_ans[26] ? |significand_ans[1:0] :
	significand_ans[25] ? significand_ans[0] : 1'b0
);

wire	[24:0]	significand_round = (
	((residual & sticky) | (residual & significand_normalized[0])) ? ({2'b01, significand_normalized} + 25'd1) :
	/* no round up */ {2'b01, significand_normalized}
);

wire	[22:0]	significand_o = (
	significand_round[24] ? significand_round[23:1] : significand_round[22:0]
);

//----------------------------------------------------------
// exceptions
wire	inf_a		= (&exp_a) & (mantissa_a == 0);	// a is infinity
wire	inf_b		= (&exp_b) & (mantissa_b == 0);	// b is infinity
wire	nan_a		= (&exp_a) & (mantissa_a != 0);	// a is NaN
wire	nan_b		= (&exp_b) & (mantissa_b != 0);	// b is NaN
wire	nan_o		= (
	nan_a |	// a is NaN
	nan_b | // b is NaN
	(inf_a & inf_b & (sign_a ^ sign_b))	// Infinity - Infinity
);

assign Exception	= (&exp_a) | (&exp_b);

assign O = (
	nan_o	? 32'h7FFFFFFF :
	inf_a	? a :
	inf_b	? b :
	/* OUT */ (operation_sub ? {sign_o, 31'd0} : {sign_o, /*exp_add*/ 8'd0, significand_o})
);

endmodule
