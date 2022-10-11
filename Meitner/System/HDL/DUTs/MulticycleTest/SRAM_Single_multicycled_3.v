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
// Title : Processor
// Rev.  : 10/11/2022 Tue (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"

module SRAM_Single_multicycled_3 (
	input					CLK,	// clock
	input					nRST,	// reset (active low)
	input					nCE,	// chip enable (active low)
	input					nWE,	// write enable (active low)
	output					READY,	// input ready
	input	[6:0]			ADDR,	// address
	input	[31:0]			DIN,	// data input
	output					OE,		// output enable
	output	[31:0]			DOUT	// data output
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
SRAM_Single_Multicycle #(
	.ADDR_WIDTH		(7),
	.DATA_WIDTH		(32),
	.CYCLE			(3)
) sram (
	.CLK			(CLK),
	.nRST			(nRST),
	.nCE			(nCE),
	.nWE			(nWE),
	.READY			(READY),
	.ADDR			(ADDR),
	.DIN			(DIN),
	.OE				(OE),
	.DOUT			(DOUT)
);

endmodule
