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
// Rev.  : 4/25/2023 Tue (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`define __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`include "testdrive_system.vh"

`define __GEN_MULTIPATH_SRAM \
	(* keep="true"*) reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0]; \
	(* keep="true"*) reg		[DATA_WIDTH-1:0]		o_data; \
	assign DOUT		= o_data; \
	always@(posedge CLK) begin \
		if(en) begin \
			if(r_we) \
				mem[r_addr]	<= r_din; \
			o_data	<= mem[r_addr]; \
		end \
	end

module SRAM_Single_Multicycle #(
	parameter	ADDR_WIDTH			= 7,
	parameter	DATA_WIDTH			= 32,
	parameter	CYCLE				= 2
) (
	input							CLK,		// clock
	input							nRST,		// reset (active low)
	input							EN,			// enable
	input							WE,			// write enable
	output							READY,		// input ready
	input	[ADDR_WIDTH-1:0]		ADDR,		// address
	input	[DATA_WIDTH-1:0]		DIN,		// data input
	output							OE,			// output enable
	output	[DATA_WIDTH-1:0]		DOUT		// data output
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[CYCLE-1:0]			oe_pipe;
reg		[CYCLE-1:0]			en_pipe;
reg		[ADDR_WIDTH-1:0]	r_addr;
reg		[DATA_WIDTH-1:0]	r_din;
reg							r_we;
wire	[DATA_WIDTH-1:0]	sram_out;

wire	en					= EN & READY;
wire	re					= en & ~WE;

assign	READY				= ~en_pipe[CYCLE-1];
assign	OE					= oe_pipe[CYCLE-1];

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		en_pipe		<= {(CYCLE){1'b0}};
		oe_pipe		<= {(CYCLE){1'b0}};
		r_addr		<= 'd0;
		r_din		<= 'd0;
		r_we		<= 1'b0;
	end
	else begin
		en_pipe		<= {en_pipe[(CYCLE-2):0] | {(CYCLE - 1){en}}, 1'b0};
		oe_pipe		<= {oe_pipe[(CYCLE-2):0], re};
		if(en) begin
			r_we		<= WE;
			r_addr		<= ADDR;
			//if(~WE)
			r_din		<= DIN;
		end
	end
end

generate begin : gen_multicycle
	if(CYCLE==2) begin : path_2
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==3) begin : path_3
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==4) begin : path_4
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==5) begin : path_5
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==6) begin : path_6
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==7) begin : path_7
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==8) begin : path_8
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==9) begin : path_9
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==10) begin : path_10
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==11) begin : path_11
		`__GEN_MULTIPATH_SRAM
	end
	else if(CYCLE==12) begin : path_12
		`__GEN_MULTIPATH_SRAM
	end
	else begin : multicycle_path
		`__GEN_MULTIPATH_SRAM
	end
end
endgenerate

endmodule

`undef __GEN_MULTIPATH_PIPE

`endif//__TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
