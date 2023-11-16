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
// Rev.  : 11/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_MULTICYCLE_PATH_V__
`define __TESTDRIVE_MULTICYCLE_PATH_V__
`include "testdrive_system.vh"

`define __GEN_MULTIPATH_PIPE \
	(* dont_touch = "yes" *) reg	[(IWIDTH*CYCLE)-1:0]	i_data; \
	assign	PIPE_I		= i_data; \
	for(i=0;i<CYCLE;i=i+1) begin : i_pipe \
		always@(posedge CLK, negedge nRST) begin \
			if(!nRST) begin \
				i_data[`BUS_RANGE(IWIDTH, i)]	<= {(IWIDTH){1'b0}}; \
			end \
			else begin \
				if(ie_pipe[i] & IE) \
					i_data[`BUS_RANGE(IWIDTH, i)]	<= IDATA; \
			end \
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
		output	[(IWIDTH*CYCLE)-1:0]		PIPE_I,			// input bus
		input	[(OWIDTH*CYCLE)-1:0]		PIPE_O,			// output bus
		// output
		output								OE,				// output enable
		output	[OWIDTH-1:0]				ODATA			// output data
	);
	// synopsys template

	// register definition & assignment ------------------------------------------
	genvar i;

	reg		[CYCLE-1:0]			ie_pipe;		// input enable pipe
	reg		[CYCLE-1:0]			oe_pipe;		// output enable pipe
	reg		[CYCLE:0]			en_pipe;		// state enable pipe

	assign	OE					= oe_pipe[CYCLE-1];

	// implementation ------------------------------------------------------------
	// in/out enable pipe
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			ie_pipe		<= {{(CYCLE-1){1'b0}}, 1'b1};
			oe_pipe		<= {(CYCLE){1'b0}};
			en_pipe		<= {(CYCLE+1){1'b0}};
		end
		else begin
			if(IE || en_pipe[CYCLE]) begin
				oe_pipe		<= {oe_pipe[CYCLE-2:0], IE};
				en_pipe		<= {en_pipe[CYCLE-1:0] | {(CYCLE){IE}}, IE};
			end

			if(IE || en_pipe[CYCLE-2]) begin
				ie_pipe		<= {ie_pipe[CYCLE-2:0], ie_pipe[CYCLE-1]};
			end
		end
	end

	// demuxing result output
	demux_by_enable #(
		.WIDTH			(OWIDTH),
		.CHANNELS		(CYCLE),
		.TRISTATE		(1)
	) odata_demux (
		.EN_BUS			(ie_pipe),
		.DIN_BUS		(PIPE_O),
		.DOUT			(ODATA)
	);

	generate begin : gen_multicycle
			if(CYCLE==2) begin : path_2
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==3) begin : path_3
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==4) begin : path_4
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==5) begin : path_5
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==6) begin : path_6
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==7) begin : path_7
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==8) begin : path_8
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==9) begin : path_9
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==10) begin : path_10
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==11) begin : path_11
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==12) begin : path_12
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==13) begin : path_13
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==14) begin : path_14
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==15) begin : path_15
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==16) begin : path_16
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==17) begin : path_17
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==18) begin : path_18
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==19) begin : path_19
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==20) begin : path_20
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==21) begin : path_21
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==22) begin : path_22
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==23) begin : path_23
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==24) begin : path_24
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==25) begin : path_25
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==26) begin : path_26
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==27) begin : path_27
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==28) begin : path_28
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==29) begin : path_29
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==30) begin : path_30
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==31) begin : path_31
				`__GEN_MULTIPATH_PIPE
			end
			else if(CYCLE==32) begin : path_32
				`__GEN_MULTIPATH_PIPE
			end
			else begin : multicycle_path
				$error("The maximum allowed value for MulticyclePath's 'CYCLE' is 32");
			end
		end
	endgenerate

endmodule

`undef __GEN_MULTIPATH_PIPE

`endif//__TESTDRIVE_MULTICYCLE_PATH_V__
