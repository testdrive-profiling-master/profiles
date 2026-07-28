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

module FPU_F32_PriorityEncoder(
	input		[24:0]		IN,
	input		[7:0]		EXP_A,
	output reg	[24:0]		OUT,
	output		[7:0]		EXP_SUB
);

reg [4:0] shift;

always @(IN) begin
	if(!IN[24]) begin
		OUT		= (~IN) + 1'b1;
		shift	= 5'd0;
	end
	else if (IN[23]) begin
		OUT = IN;
		shift = 5'd0;
	end
	else if (IN[22]) begin
		OUT = IN << 1;
		shift = 5'd1;
	end
	else if (IN[21]) begin
		OUT = IN << 2;
		shift = 5'd2;
	end
	else if (IN[20]) begin
		OUT = IN << 3;
		shift = 5'd3;
	end
	else if (IN[19]) begin
		OUT = IN << 4;
		shift = 5'd4;
	end
	else if (IN[18]) begin
		OUT = IN << 5;
		shift = 5'd5;
	end
	else if (IN[17]) begin
		OUT = IN << 6;
		shift = 5'd6;
	end
	else if (IN[16]) begin
		OUT = IN << 7;
		shift = 5'd7;
	end
	else if (IN[15]) begin
		OUT = IN << 8;
		shift = 5'd8;
	end
	else if (IN[14]) begin
		OUT = IN << 9;
		shift = 5'd9;
	end
	else if (IN[13]) begin
		OUT = IN << 10;
		shift = 5'd10;
	end
	else if (IN[12]) begin
		OUT = IN << 11;
		shift = 5'd11;
	end
	else if (IN[11]) begin
		OUT = IN << 12;
		shift = 5'd12;
	end
	else if (IN[10]) begin
		OUT = IN << 13;
		shift = 5'd13;
	end
	else if (IN[9]) begin
		OUT = IN << 14;
		shift = 5'd14;
	end
	else if (IN[8]) begin
		OUT = IN << 15;
		shift = 5'd15;
	end
	else if (IN[7]) begin
		OUT = IN << 16;
		shift = 5'd16;
	end
	else if (IN[6]) begin
		OUT = IN << 17;
		shift = 5'd17;
	end
	else if (IN[5]) begin
		OUT = IN << 18;
		shift = 5'd18;
	end
	else if (IN[4]) begin
		OUT = IN << 19;
		shift = 5'd19;
	end
	else if (IN[3]) begin
		OUT = IN << 20;
		shift = 5'd20;
	end
	else if (IN[2]) begin
		OUT = IN << 21;
		shift = 5'd21;
	end
	else if (IN[1]) begin
		OUT = IN << 22;
		shift = 5'd22;
	end
	else if (IN[0]) begin
		OUT = IN << 23;
		shift = 5'd23;
	end
	else begin
		OUT = IN << 24;
		shift = 5'd24;
	end
end
assign EXP_SUB	= EXP_A - {3'd0, shift};

endmodule
