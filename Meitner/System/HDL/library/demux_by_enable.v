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
`ifndef __TESTDRIVE_DEMUX_BY_ENABLE_V__
`define __TESTDRIVE_DEMUX_BY_ENABLE_V__
`include "system_defines.vh"

module demux_by_enable  #(
	parameter	WIDTH				= 32,
	parameter	CHANNELS			= 2,
	parameter	TRISTATE			= 0
)(
	// instruction input
	input	[(CHANNELS-1):0]			EN_BUS,		// enable
	input	[((WIDTH*CHANNELS)-1):0]	DIN_BUS,	// data in
	output	[(WIDTH-1):0]				DOUT		// data output
);
// synopsys template

// definition & assignment ---------------------------------------------------
genvar	i_d, i_c;

wire	[((WIDTH*CHANNELS)-1):0]		input_swizzle;

// implementation ------------------------------------------------------------
generate
if(TRISTATE) begin
	for(i_c=0;i_c<CHANNELS;i_c=i_c+1) begin: channel_assignments
		assign	DOUT	= EN_BUS[i_c] ? DIN_BUS[`BUS_RANGE(WIDTH, i_c)] : {(WIDTH){1'bZ}};
	end
end
else begin
	for(i_d=0;i_d<WIDTH;i_d=i_d+1) begin : width_assignment
		for(i_c=0;i_c<CHANNELS;i_c=i_c+1) begin: channel_assignments
			assign	input_swizzle[(i_d*CHANNELS)+i_c]	= DIN_BUS[(i_c*WIDTH)+i_d];
		end
	end

	for(i_d=0;i_d<WIDTH;i_d=i_d+1) begin: array_dout_assignments
		assign	DOUT[i_d]	= |(input_swizzle[(((i_d+1)*CHANNELS)-1):(i_d*CHANNELS)] & EN_BUS);
	end
end
endgenerate

endmodule

`endif//__TESTDRIVE_DEMUX_BY_ENABLE_V__
