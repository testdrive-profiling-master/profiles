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
`ifndef __TESTDRIVE_MULTICYCLE_PIPE_V__
`define __TESTDRIVE_MULTICYCLE_PIPE_V__
`include "system_defines.vh"

`define __GEN_MULTIPATH_PIPE(name) \
	(* keep="true"*) reg		[OWIDTH-1:0]		name; \
	assign ODATA	= name; \
	always@(posedge CLK, negedge nRST) begin \
		if(!nRST) begin \
			name	<= {(OWIDTH){1'b0}}; \
		end \
		else if(oe_pipe[CYCLE-1]) begin \
			name	<= demux_dout; \
		end \
	end

module MultiCyclePath #(
	parameter		IWIDTH				= 8,			// input total width
	parameter		OWIDTH				= 8,			// output total width
	parameter		CYCLE				= 2				// instance pipe cycles (must be >= 2)
) (
	input								CLK,			// clock
	input								nRST,			// reset (active low)
	// input
	input								IE,				// input enable
	input	[IWIDTH-1:0]				IDATA,			// input data or control
	// i/o pipes
	output	reg [(IWIDTH*CYCLE)-1:0]	PIPE_I,			// input bus
	input	[(OWIDTH*CYCLE)-1:0]		PIPE_O,			// output bus
	// output
	output								OE,				// output enable
	output	[OWIDTH-1:0]				ODATA			// output data
);
// synopsys template

// register definition & assignment ------------------------------------------
genvar i;

reg		[CYCLE-1:0]			ie_pipe;		// input enable pipe
reg		[CYCLE:0]			oe_pipe;		// output enable pipe
wire	[OWIDTH-1:0]		demux_dout;		// demuxed data out

assign	OE					= oe_pipe[CYCLE];

// implementation ------------------------------------------------------------
// in/out enable pipe
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		ie_pipe		<= {{(CYCLE-1){1'b0}}, 1'b1};
		oe_pipe		<= {(CYCLE+1){1'b0}};
	end
	else begin
		if(IE || (|oe_pipe)) begin
			ie_pipe		<= {ie_pipe[CYCLE-2:0], ie_pipe[CYCLE-1]};
			oe_pipe		<= {oe_pipe[CYCLE-1:0], IE};
		end
	end
end

// input bus generation
generate
for(i=0;i<CYCLE;i=i+1) begin : level_gen
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			PIPE_I[`BUS_RANGE(IWIDTH, i)]	<= {(IWIDTH){1'b0}};
		end
		else begin
			if(ie_pipe[i] & IE)
				PIPE_I[`BUS_RANGE(IWIDTH, i)]	<= IDATA;
		end
	end
end
endgenerate

// demuxing result output
demux_by_enable #(
	.WIDTH			(OWIDTH),
	.CHANNELS		(CYCLE)
) odata_demux (
	.EN_BUS			(ie_pipe),
	.DIN_BUS		(PIPE_O),
	.DOUT			(demux_dout)
);

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

`endif//__TESTDRIVE_MULTICYCLE_PIPE_V__
