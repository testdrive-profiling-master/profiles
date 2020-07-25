//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common verilog library
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_COMMAND_QUEUE_V__
`define __TESTDRIVE_COMMAND_QUEUE_V__
`timescale 1ns/1ns

module CommandQueue #(
	parameter	FIFO_DEPTH			= 4,
	parameter	DATA_WIDTH			= 32,
	parameter	ID_DEPTH			= 1,
	parameter	NONBLOCK_SLICE		= 0
) (
	// system signals
	input							CLK,			// clock
	input							nRST,			// reset (active low)
	// command input
	input							I_EN,			// enable
	input	[ID_DEPTH-1:0]			I_ID,			// ID
	input	[DATA_WIDTH-1:0]		I_DATA,			// input data
	output							I_VALID,		// output validation
	// command output
	output	[(2**ID_DEPTH)-1:0]		O_EN,			// enable
	output	[DATA_WIDTH-1:0]		O_DATA,			// output data
	input	[(2**ID_DEPTH)-1:0]		O_VALID			// output validation
);
// synopsys template

localparam	ID_SIZE	= (2**ID_DEPTH);
// register definition & assignment ------------------------------------------
// queue
wire								q_full, q_empty;
wire	[ID_DEPTH+DATA_WIDTH-1:0]	q_data;
// slice
wire								s_iready, s_oe, s_oready;
wire	[ID_DEPTH-1:0]				s_id;

assign	s_oready					= |(O_EN & O_VALID);

assign	I_VALID						= ~q_full;

// implementation ------------------------------------------------------------
// command queue memory
FiFo #(
	.DATA_WIDTH		(ID_DEPTH+DATA_WIDTH),
	.FIFO_DEPTH		(FIFO_DEPTH)
) cmd_quene (
	.CLK			(CLK),
	.nCLR			(nRST),
	// push interface
	.nWE			(~I_EN),
	.DIN			({I_ID, I_DATA}),
	.FULL			(q_full),
	// pop interface
	.nRE			(~s_iready),
	.DOUT			(q_data),
	.EMPTY			(q_empty)
);

// control buffer slice register
ControlSlice #(
	.WIDTH			(ID_DEPTH+DATA_WIDTH),
	.CHAINED		(NONBLOCK_SLICE)
) cmd_slice (
	.CLK			(CLK),
	.nRST			(nRST),
	.IE				(~q_empty),
	.IDATA			(q_data),
	.IREADY			(s_iready),
	.OE				(s_oe),
	.ODATA			({s_id, O_DATA}),
	.OREADY			(s_oready)
);

// muxed enable output
mux_by_index #(
	.INDEX_BITS		(ID_DEPTH)
) enable (
	.EN				(s_oe),
	.INDEX			(s_id),
	.MUX			(O_EN)
);

endmodule

`endif//__TESTDRIVE_COMMAND_QUEUE_V__
