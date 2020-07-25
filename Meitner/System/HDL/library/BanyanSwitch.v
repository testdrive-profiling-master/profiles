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
`ifndef __TESTDRIVE_BANYAN_SWITCH_V__
`define __TESTDRIVE_BANYAN_SWITCH_V__
`timescale 1ns/1ns
/* TOP_MODULE = BanyanSwitch */

module BanyanSwitch_element #(
	parameter		WIDTH			= 8,		// data width
	parameter		ID				= 0			// output id
) (
	// system
	input							CLK,		// clock
	input							nRST,		// reset (active low)
	// inputs
	input	[WIDTH+1:0]				A,			// {en, id, data}
	output							A_READY,	// ready
	input	[WIDTH+1:0]				B,			// {en, id, data}
	output							B_READY,	// ready
	// outputs
	output	[WIDTH:0]				O,			// {en, data}
	input							O_VALID		// output valid
);
// synopsys template

// register definition & assignment ------------------------------------------
// input dispatch
wire								a_en, b_en;
wire								a_id, b_id;
wire	[WIDTH-1:0]					a_data, b_data;
assign	{a_en, a_id, a_data}		= A;
assign	{b_en, b_id, b_data}		= B;

// signals
reg		sw;						// prefered switching (round robine)
wire	i_a_en					= a_en & (ID ? a_id : ~a_id);	// input 'A' enable
wire	i_b_en					= b_en & (ID ? b_id : ~b_id);	// input 'B' enable
/* prefered switching algorithm
	iA iB  ^  S oA oB
	 1  0  1  0  1  X
	 0  1  1  0  X  1
	 1  1  0  0  1  0
	 1  0  1  1  1  X
	 0  1  1  1  X  1
	 1  1  0  1  0  1
 */
wire	i_en					= i_a_en ^ i_b_en;
wire	i_ready					= O_VALID | (~o_en);
wire	A_READY					= i_ready & (i_en | (i_en ^ sw));
wire	B_READY					= i_ready & (i_en | ~(i_en ^ sw));

reg		o_en;
reg		o_data;
assign	O						= {o_en, o_data};

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		sw			<= 1'b0;
		o_en		<= 1'b0;
		o_data		<= {(WIDTH-1){1'b0}};
	end
	else begin
		if(o_en & O_VALID)
			sw			<= ~sw;
		o_en		<= i_a_en | i_b_en;
		if(a_en & A_READY)
			o_data		<= a_data;
		else if(b_en & B_READY)
			o_data		<= b_data;
	end
end

endmodule

module BanyanSwitch #(
	parameter		WIDTH			= 8			// data width
) (
	// system
	input							CLK,		// clock
	input							nRST,		// reset (active low)
	// inputs
	input	[WIDTH+1:0]				I_A,		// {en, id, data}
	output							I_A_READY,	// ready
	input	[WIDTH+1:0]				I_B,		// {en, id, data}
	output							I_B_READY,	// ready
	// outputs
	output	[WIDTH:0]				O_A,		// {en, data}
	input							O_A_VALID,
	output	[WIDTH:0]				O_B,		// {en, data}
	input							O_B_VALID
);
// synopsys template

// register definition & assignment ------------------------------------------
wire		i_a0_ready, i_a1_ready;
wire		i_b0_ready, i_b1_ready;

assign		i_A_READY	= i_a0_ready | i_a1_ready;
assign		i_B_READY	= i_b0_ready | i_b1_ready;

// implementation ------------------------------------------------------------
BanyanSwitch_element #(
	.WIDTH		(WIDTH),
	.ID			(0)
) switch_a (
	.CLK		(CLK),
	.nRST		(nRST),
	.A			(I_A),
	.A_READY	(i_a0_ready),
	.B			(I_B),
	.B_READY	(i_a1_ready),
	.O			(O_A),
	.O_VALID	(O_A_VALID)
);

BanyanSwitch_element #(
	.WIDTH		(WIDTH),
	.ID			(1)
) switch_b (
	.CLK		(CLK),
	.nRST		(nRST),
	.A			(I_A),
	.A_READY	(i_b0_ready),
	.B			(I_B),
	.B_READY	(i_b1_ready),
	.O			(O_B),
	.O_VALID	(O_B_VALID)
);

endmodule

`endif//__TESTDRIVE_BANYAN_SWITCH_V__
