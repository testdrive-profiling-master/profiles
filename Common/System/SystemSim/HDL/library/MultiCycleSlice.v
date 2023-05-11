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
// Title : Common verilog library
// Rev.  : 5/11/2023 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_MULTICYCLE_SLICE_V__
`define __TESTDRIVE_MULTICYCLE_SLICE_V__
`include "testdrive_system.vh"

module MultiCycleSlice #(
	parameter		WIDTH			= 4,				// control width
	parameter		CYCLE			= 2					// must >= 2
) (
	input							CLK,				// clock
	input							nRST,				// reset (active low)
	// control input
	input							IE,					// input enable
	input	[WIDTH-1:0]				IDATA,				// input data
	// control output
	output							OE,					// output enable
	output	[WIDTH-1:0]				ODATA				// output data
);
// synopsys template

// register definition & assignment ------------------------------------------
wire	[(WIDTH*CYCLE)-1:0]	pipe_i;

// implementation ------------------------------------------------------------
MultiCyclePath #(
	.IWIDTH		(WIDTH),
	.OWIDTH		(WIDTH),
	.CYCLE		(CYCLE)
) multi_pipe (
	.CLK		(CLK),
	.nRST		(nRST),
	.IE			(IE),
	.IDATA		(IDATA),
	.PIPE_I		(pipe_i),
	.PIPE_O		(pipe_i),
	.OE			(OE),
	.ODATA		(ODATA)
);

`endif//__TESTDRIVE_MULTICYCLE_SLICE_V__
