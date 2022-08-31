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

module FPU_F32_PriorityEncoder(
	input		[24:0]		significand,
	input		[7:0]		Exponent_a,
	output reg	[24:0]		Significand,
	output		[7:0]		Exponent_sub
);

reg [4:0] shift;

always @(significand) begin
	casex (significand)
		25'b1_1xxx_xxxx_xxxx_xxxx_xxxx_xxxx :	begin
			Significand = significand;
			shift = 5'd0;
		end
		25'b1_01xx_xxxx_xxxx_xxxx_xxxx_xxxx : 	begin
			Significand = significand << 1;
			shift = 5'd1;
		end

		25'b1_001x_xxxx_xxxx_xxxx_xxxx_xxxx : 	begin
			Significand = significand << 2;
			shift = 5'd2;
		end

		25'b1_0001_xxxx_xxxx_xxxx_xxxx_xxxx : 	begin
			Significand = significand << 3;
			shift = 5'd3;
		end

		25'b1_0000_1xxx_xxxx_xxxx_xxxx_xxxx : 	begin
			Significand = significand << 4;
			shift = 5'd4;
		end

		25'b1_0000_01xx_xxxx_xxxx_xxxx_xxxx : 	begin
			Significand = significand << 5;
			shift = 5'd5;
		end

		25'b1_0000_001x_xxxx_xxxx_xxxx_xxxx : 	begin						// 24'h020000
			Significand = significand << 6;
			shift = 5'd6;
		end

		25'b1_0000_0001_xxxx_xxxx_xxxx_xxxx : 	begin						// 24'h010000
			Significand = significand << 7;
			shift = 5'd7;
		end

		25'b1_0000_0000_1xxx_xxxx_xxxx_xxxx : 	begin						// 24'h008000
			Significand = significand << 8;
			shift = 5'd8;
		end

		25'b1_0000_0000_01xx_xxxx_xxxx_xxxx : 	begin						// 24'h004000
			Significand = significand << 9;
			shift = 5'd9;
		end

		25'b1_0000_0000_001x_xxxx_xxxx_xxxx : 	begin						// 24'h002000
			Significand = significand << 10;
			shift = 5'd10;
		end

		25'b1_0000_0000_0001_xxxx_xxxx_xxxx : 	begin						// 24'h001000
			Significand = significand << 11;
			shift = 5'd11;
		end

		25'b1_0000_0000_0000_1xxx_xxxx_xxxx : 	begin						// 24'h000800
			Significand = significand << 12;
			shift = 5'd12;
		end

		25'b1_0000_0000_0000_01xx_xxxx_xxxx : 	begin						// 24'h000400
			Significand = significand << 13;
			shift = 5'd13;
		end

		25'b1_0000_0000_0000_001x_xxxx_xxxx : 	begin						// 24'h000200
			Significand = significand << 14;
			shift = 5'd14;
		end

		25'b1_0000_0000_0000_0001_xxxx_xxxx  : 	begin						// 24'h000100
			Significand = significand << 15;
			shift = 5'd15;
		end

		25'b1_0000_0000_0000_0000_1xxx_xxxx : 	begin						// 24'h000080
			Significand = significand << 16;
			shift = 5'd16;
		end

		25'b1_0000_0000_0000_0000_01xx_xxxx : 	begin						// 24'h000040
			Significand = significand << 17;
			shift = 5'd17;
		end

		25'b1_0000_0000_0000_0000_001x_xxxx : 	begin						// 24'h000020
			Significand = significand << 18;
			shift = 5'd18;
		end

		25'b1_0000_0000_0000_0000_0001_xxxx : 	begin						// 24'h000010
			Significand = significand << 19;
			shift = 5'd19;
		end

		25'b1_0000_0000_0000_0000_0000_1xxx :	begin						// 24'h000008
			Significand = significand << 20;
			shift = 5'd20;
		end

		25'b1_0000_0000_0000_0000_0000_01xx : 	begin						// 24'h000004
			Significand = significand << 21;
			shift = 5'd21;
		end

		25'b1_0000_0000_0000_0000_0000_001x : 	begin						// 24'h000002
			Significand = significand << 22;
			shift = 5'd22;
		end

		25'b1_0000_0000_0000_0000_0000_0001 : 	begin						// 24'h000001
			Significand = significand << 23;
			shift = 5'd23;
		end

		25'b1_0000_0000_0000_0000_0000_0000 : 	begin						// 24'h000000
			Significand = significand << 24;
			shift = 5'd24;
		end
		default : 	begin
			Significand = (~significand) + 1'b1;
			shift = 8'd0;
		end
	endcase
end
assign Exponent_sub = Exponent_a - shift;

endmodule
