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

module FPU_F32_FloatToInt(
	input		[31:0]		a_operand,
	output		[31:0]		Integer
);

reg [23:0] Integer_Value;

always @(*) begin
	if (a_operand[30:23] == 8'd127) begin
		Integer_Value = 23'd0;
	end

	else if (a_operand[30:23] == 8'd128) begin
		Integer_Value = {a_operand[22],22'd0};

	end

	else if (a_operand[30:23] == 8'd129) begin
		Integer_Value = {a_operand[22:21],21'd0};

	end

	else if (a_operand[30:23] == 8'd130) begin
		Integer_Value = {a_operand[22:20],20'd0};

	end

	else if (a_operand[30:23] == 8'd131) begin
		Integer_Value = {a_operand[22:19],19'd0};

	end

	else if (a_operand[30:23] == 8'd132) begin
		Integer_Value = {a_operand[22:18],18'd0};

	end

	else if (a_operand[30:23] == 8'd133) begin
		Integer_Value = {a_operand[22:17],17'd0};

	end

	else if (a_operand[30:23] == 8'd134) begin
		Integer_Value = {a_operand[22:16],16'd0};

	end

	else if (a_operand[30:23] == 8'd135) begin
		Integer_Value = {a_operand[22:15],15'd0};

	end

	else if (a_operand[30:23] == 8'd136) begin
		Integer_Value = {a_operand[22:14],14'd0};

	end

	else if (a_operand[30:23] == 8'd137) begin
		Integer_Value = {a_operand[22:13],13'd0};

	end

	else if (a_operand[30:23] == 8'd138) begin
		Integer_Value = {a_operand[22:12],12'd0};

	end

	else if (a_operand[30:23] == 8'd139) begin
		Integer_Value = {a_operand[22:11],11'd0};

	end

	else if (a_operand[30:23] == 8'd140) begin
		Integer_Value = {a_operand[22:10],10'd0};

	end

	else if (a_operand[30:23] == 8'd141) begin
		Integer_Value = {a_operand[22:9],9'd0};

	end

	else if (a_operand[30:23] == 8'd142) begin
		Integer_Value = {a_operand[22:8],8'd0};

	end

	else if (a_operand[30:23] == 8'd143) begin
		Integer_Value = {a_operand[22:7],7'd0};

	end

	else if (a_operand[30:23] == 8'd144) begin
		Integer_Value = {a_operand[22:6],6'd0};

	end

	else if (a_operand[30:23] == 8'd145) begin
		Integer_Value = {a_operand[22:5],5'd0};

	end

	else if (a_operand[30:23] == 8'd146) begin
		Integer_Value = {a_operand[22:4],4'd0};

	end

	else if (a_operand[30:23] == 8'd147) begin
		Integer_Value = {a_operand[22:3],3'd0};

	end

	else if (a_operand[30:23] == 8'd148) begin
		Integer_Value = {a_operand[22:2],2'd0};

	end

	else if (a_operand[30:23] == 8'd149) begin
		Integer_Value = {a_operand[22:1],1'd0};

	end

	else if (a_operand[30:23] >= 8'd150) begin
		Integer_Value = a_operand[22:0];

	end

	else if (a_operand[30:23] <= 8'd126) begin
		Integer_Value = 24'd0;

	end
end

assign Integer = {a_operand[31:23],Integer_Value[23:1]};

endmodule
