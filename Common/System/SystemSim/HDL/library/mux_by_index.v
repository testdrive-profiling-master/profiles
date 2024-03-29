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
// Rev.  : 5/25/2023 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __TESTDRIVE_MUX_BY_INDEX_V__
`define __TESTDRIVE_MUX_BY_INDEX_V__
`timescale 1ns/1ns

module mux_by_index  #(
	parameter	INDEX_BITS			= 2
)(
	input										EN,
	input	[(INDEX_BITS-1):0]					INDEX,		// index
	output	[(2**INDEX_BITS)-1:0]				MUX			// enable output
);
// synopsys template

localparam	CHANNELS			= (2**INDEX_BITS);

// definition & assignment ---------------------------------------------------
genvar	i;

// implementation ------------------------------------------------------------
generate
for(i=0;i<CHANNELS;i=i+1) begin: enable_out
	wire	[(INDEX_BITS-1):0]	temp	= i[(INDEX_BITS-1):0];
	assign	MUX[i]	= EN & (INDEX == temp);
end
endgenerate

endmodule

`endif//__TESTDRIVE_MUX_BY_INDEX_V__
