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
`ifndef __TESTDRIVE_MULTICYCLE_SLICE_V__
`define __TESTDRIVE_MULTICYCLE_SLICE_V__
`include "testdrive_system.vh"

`define __GEN_MULTIPATH_PIPE \
	(* dont_touch = "yes" *) reg		[WIDTH-1:0]		o_data; \
	assign ODATA	= o_data; \
	always@(posedge CLK, negedge nRST) begin \
		if(!nRST) begin \
			o_data	<= INITIAL; \
		end \
		else if(ie & IREADY) begin \
			o_data	<= IDATA; \
		end \
	end

module MultiCycleSlice #(
	parameter		WIDTH			= 4,				// control width
	parameter		INITIAL			= {WIDTH{1'b0}},	// initial value
	parameter		CYCLE			= 2,				// must >= 2
	parameter		CHAINED			= 1					// chained control?
) (
	input							CLK,				// clock
	input							nRST,				// reset (active low)
	// control input
	input							IE,					// input enable
	input	[WIDTH-1:0]				IDATA,				// input data
	output							IREADY,				// input ready
	// control output
	output							OE,					// output enable
	output	[WIDTH-1:0]				ODATA,				// output data
	input							OREADY				// output ready
);
// synopsys template

// register definition & assignment ------------------------------------------
reg							occupied;		// occupied control
reg		[CYCLE-2:0]			ie_pipe;		// output enable pipe

wire	ie					= ie_pipe[CYCLE-2];
assign	OE					= occupied;

// implementation ------------------------------------------------------------
// input cycle management
generate if(CYCLE==2) begin : gen_ie_2
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			ie_pipe		<= 1'b0;
		end
		else begin
			ie_pipe		<= IE & (~IREADY);
		end
	end
end
else begin : gen_ie
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			ie_pipe		<= {(CYCLE-1){1'b0}};
		end
		else begin
			ie_pipe		<= (IE & (~IREADY)) ? {ie_pipe[CYCLE-3:0], IE} : {(CYCLE-1){1'b0}};
		end
	end
end
endgenerate

generate if(CHAINED) begin : chained_control
	assign	IREADY				= ((~occupied) | OREADY) & ie;
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			occupied		<= 1'b0;
		end
		else begin
			if(occupied) begin
				if((~ie) & OREADY) begin
					occupied	<= 1'b0;
				end
			end
			else begin
				if(ie) begin
					occupied	<= 1'b1;
				end
			end
		end
	end
end
else begin : unchained_control
	assign	IREADY				= (~occupied) & ie;
	always@(posedge CLK, negedge nRST) begin
		if(!nRST) begin
			occupied		<= 1'b0;
		end
		else begin
			if(occupied) begin
				if(OREADY) begin
					occupied	<= 1'b0;
				end
			end
			else begin
				if(ie) begin
					occupied	<= 1'b1;
				end
			end
		end
	end
end
endgenerate

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
	else begin : multicycle_path
		`__GEN_MULTIPATH_PIPE
	end
end
endgenerate

endmodule

`undef __GEN_MULTIPATH_PIPE

`endif//__TESTDRIVE_MULTICYCLE_SLICE_V__
