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
// Rev.  : 4/26/2023 Wed (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`define __TESTDRIVE_SRAM_SINGLE_MULTICYCLE_V__
`include "testdrive_system.vh"

`define __GEN_MULTIPATH_SRAM \
	reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0]; \
	(* keep="true"*) reg		[ADDR_WIDTH-1:0]	i_addr; \
	(* keep="true"*) reg		[DATA_WIDTH-1:0]	i_din; \
	(* keep="true"*) reg							i_wen; \
	\
	SRAM_Single #( \
		.ADDR_WIDTH		(ADDR_WIDTH), \
		.DATA_WIDTH		(DATA_WIDTH) \
	) sram ( \
		.CLK			(CLK), \
		.nCE			(~sram_en), \
		.nWE			(i_wen), \
		.ADDR			(i_addr), \
		.DIN			(i_din), \
		.DOUT			(DOUT) \
	); \
	\
	always@(posedge CLK, negedge nRST) begin \
		if(!nRST) begin \
			i_addr		<= 'd0;	\
			i_din		<= 'd0; \
			i_wen		<= 1'b1; \
		end \
		else begin \
			if(en) begin \
				i_wen		<= ~WE; \
				i_addr		<= ADDR; \
				if(WE) begin \
					i_din	<= DIN; \
				end \
			end \
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
reg		[CYCLE:0]			oe_pipe;			// output enable pipe
reg		[CYCLE-1:0]			rd_pipe;			// ready pipe
reg		[CYCLE-1:0]			se_pipe;			// sram enable pipe

wire	en					= EN & READY;		// enable
wire	oe					= en & ~WE;			// output enable
wire	sram_en				= se_pipe[CYCLE-1];	// sram enable
assign	READY				= rd_pipe[CYCLE-1];
assign	OE					= oe_pipe[CYCLE];

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		rd_pipe		<= {(CYCLE){1'b1}};
		oe_pipe		<= {(CYCLE+1){1'b0}};
		se_pipe		<= {(CYCLE){1'b0}};
	end
	else begin
		rd_pipe		<= {rd_pipe[(CYCLE-2):0] & {(CYCLE - 1){~en}}, 1'b1};
		oe_pipe		<= {oe_pipe[(CYCLE-1):0], oe};
		se_pipe		<= {se_pipe[(CYCLE-2):0], en};
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
