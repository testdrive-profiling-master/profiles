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
`ifndef __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`define __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`include "testdrive_system.vh"

`define __GEN_MULTIPATH_PIPE(name) \
	(* keep="true"*) reg		[DATA_WIDTH-1:0]		name; \
	assign DOUT		= name; \
	always@(posedge CLK, negedge nRST) begin \
		if(!nRST) begin \
			name	<= {(DATA_WIDTH){1'b0}}; \
		end \
		else if(o_en) begin \
			name	<= sram_out; \
		end \
	end

module SRAM_Single_Multicycle #(
	parameter	ADDR_WIDTH			= 7,
	parameter	DATA_WIDTH			= 32,
	parameter	CYCLE				= 2
) (
	input							CLK,	// clock
	input							nRST,	// reset (active low)
	input							nCE,	// chip enable (active low)
	input							nWE,	// write enable (active low)
	output	reg						READY,	// input ready
	input	[ADDR_WIDTH-1:0]		ADDR,	// address
	input	[DATA_WIDTH-1:0]		DIN,	// data input
	output	reg						OE,		// output enable
	output	[DATA_WIDTH-1:0]		DOUT	// data output
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[CYCLE-1:0]			oe_pipe;
wire	[DATA_WIDTH-1:0]	sram_out;

wire	re					= (~nCE) & nWE;
wire	o_en				= oe_pipe[CYCLE-1];

// implementation ------------------------------------------------------------
SRAM_Single #(
	.ADDR_WIDTH		(ADDR_WIDTH),
	.DATA_WIDTH		(DATA_WIDTH)
) sram (
	.CLK			(CLK),
	.nCE			(nCE),
	.nWE			(nWE),
	.ADDR			(ADDR),
	.DIN			(DIN),
	.DOUT			(sram_out)
);

always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		oe_pipe		<= {(CYCLE){1'b0}};
		READY		<= 1'b0;
		OE			<= 1'b0;
	end
	else begin
		oe_pipe		<= {oe_pipe[(CYCLE-2):0], re};
		READY		<= |{re, oe_pipe};
		OE			<= o_en;
	end
end

generate begin : gen_multicycle
	if(CYCLE==2) begin : path_2
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==3) begin : path_3
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==4) begin : path_4
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==5) begin : path_5
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==6) begin : path_6
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==7) begin : path_7
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==8) begin : path_8
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==9) begin : path_9
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==10) begin : path_10
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==11) begin : path_11
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else if(CYCLE==12) begin : path_12
		`__GEN_MULTIPATH_PIPE(o_data)
	end
	else begin : multicycle_path
		`__GEN_MULTIPATH_PIPE(o_data)
	end
end
endgenerate

endmodule

`undef __GEN_MULTIPATH_PIPE

`endif//__TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
