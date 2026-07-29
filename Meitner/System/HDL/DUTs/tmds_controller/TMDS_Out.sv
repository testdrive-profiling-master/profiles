//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TMDS controller
// Rev.  : 7/29/2026 Wed (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

module TMDS_Out (
	// port list -----------------------------------------------------------------
	input					CLK, RST,
	input					HSYNC, VSYNC, DE,
	input	[23:0]			RGB,
	output					TMDS_CLK,
	output	[9:0]			TMDS_DATA_0,
	output	[9:0]			TMDS_DATA_1,
	output	[9:0]			TMDS_DATA_2
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
TMDS_Encoder tmds_encoder_0 (
	.CLK		(CLK),
	.nRST		(~RST),
	.DE			(DE),
	.CTRL		({VSYNC, HSYNC}),
	.DIN		(RGB[7:0]),		// red
	.DOUT		(TMDS_DATA_0)
);

TMDS_Encoder tmds_encoder_1 (
	.CLK		(CLK),
	.nRST		(~RST),
	.DE			(DE),
	.CTRL		(2'b0),
	.DIN		(RGB[15:8]),	// green
	.DOUT		(TMDS_DATA_1)
);

TMDS_Encoder tmds_encoder_2 (
	.CLK		(CLK),
	.nRST		(~RST),
	.DE			(DE),
	.CTRL		(2'b0),
	.DIN		(RGB[23:16]),	// blue
	.DOUT		(TMDS_DATA_2)
);

assign	TMDS_CLK	= CLK;

endmodule
