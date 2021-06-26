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
// Title : Processor
// Rev.  : 6/26/2021 Sat (clonextop@gmail.com)
//================================================================================
`include "system_defines.vh"
`include "template/testdrive_clock_bfm.sv"
`include "template/testdrive_interrupt_bfm.sv"

module top ();

// definition & assignment ---------------------------------------------------
wire	CLK_A, CLK_B, CLK_C;
wire	RST_A, RST_B, RST_C;
wire	INTR;

assign	INTR	= 0;

// implementation ------------------------------------------------------------
// Clock instance #0
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(5000),		// 5ns clock  period, 200MHz
	.C_RESET_POLARITY		(1)			// active high reset
) clk_0 (
	.CLK		(CLK_A),
	.RST		(RST_A)
);

// Clock instance #1
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(3000),		// 3ns clock period, 333.3...MHz
	.C_CLOCK_PHASE_PS		(300),		// phase shift 300ps
	.C_RESET_POLARITY		(0)			// active low reset
) clk_1 (
	.CLK		(CLK_B),
	.RST		(RST_B)
);

// Clock instance #1
testdrive_clock_bfm #(
	.C_CLOCK_PERIOD_PS		(1000),		// 3ns clock period, 333.3...MHz
	.C_CLOCK_PHASE_PS		(100),		// phase shift 300ps
	.C_RESET_POLARITY		(0)			// active low reset
) clk_2 (
	.CLK		(CLK_C),
	.RST		(RST_C)
);

// interrupt sample
testdrive_interrupt_bfm #(
	.C_EDGE_DETECT			(1),		// edge detection
	.C_ACTIVE				(1)			// active high
) intr (
	.CLK		(CLK_C),
	.nRST		(RST_C),
	.EN			(1'b1),
	.INTR		(INTR)
);

endmodule
