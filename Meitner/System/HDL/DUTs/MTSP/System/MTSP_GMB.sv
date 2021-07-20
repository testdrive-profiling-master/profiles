//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : MTSP(Multi-thread Stream Processor) 'Meitner' v1.1
// Rev.  : 7/20/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_GMB(
	// System
	input							CLK, nRST,		// clock & reset (active low)
	// Input
	input							CE, WE,			// chip enable, write enable
	input	[`RANGE_GMB]			ADDR,			// address
	input	[`RANGE_DWORDx8]		DIN,			// data input
	// output
	output	reg						OE,				// output enable, 2 clock delayed
	output	reg [`RANGE_DWORDx8]	DOUT			// data output
);

// definition & assignment ---------------------------------------------------
genvar i;

reg		[3:0]					r_CE, r_OE;
reg								r_WE;
reg		[`RANGE_DWORDx8]		r_DIN;
reg		[`SIZE_GMB-1:2]			r_ADDR;
wire	[`SIZE_GMB-1:2]			bank_addr;
wire	[1:0]					node;
wire 	[3:0][`RANGE_DWORDx8]	w_DOUT;

assign	{bank_addr, node}		= ADDR;

// implementation ------------------------------------------------------------
// GMB instances
generate
for(i=0;i<4;i=i+1) begin : GMB_memory
	SRAM_Single#(
		.ADDR_WIDTH		(`SIZE_GMB-2),
		.DATA_WIDTH		(`SIZE_DWORDx8)
	) sram (
		.CLK			(CLK),
		.nCE			(~r_CE[i]),
		.nWE			(~r_WE),
		.ADDR			(r_ADDR),
		.DIN			(r_DIN),
		.DOUT			(w_DOUT[i])
	);
end
endgenerate

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		r_CE		<= {`FALSE, `FALSE, `FALSE, `FALSE};
		r_WE		<= `FALSE;
		r_ADDR		<= 'd0;
		r_DIN		<= 256'b0;

		// for delayed output
		r_OE		<= {`FALSE, `FALSE, `FALSE, `FALSE};
		OE			<= `FALSE;
		DOUT		<= 256'b0;
	end
	else begin
		r_CE		<= {(node==2'd3)&CE, (node==2'd2)&CE, (node==2'd1)&CE, (node==2'd0)&CE};
		if(CE) begin
			r_WE		<= WE;
			r_ADDR		<= bank_addr;
			r_DIN		<= DIN;
		end

		// for delayed output
		r_OE		<= r_CE & (~r_WE);	// CE & not write
		OE			<= |r_OE;
		if(|r_OE) begin
			DOUT		<= (
				r_OE[0] ? w_DOUT[0] :
				r_OE[1] ? w_DOUT[1] :
				r_OE[2] ? w_DOUT[2] : w_DOUT[3]
			);
		end
	end
end

`ifdef USE_TESTDRIVE
int t;
`DPI_FUNCTION void MTSP_set_GMB(input int unsigned dwIndex, input bit [`RANGE_DWORDx8] DATA);

initial begin
	for(t=0;t<(`COUNT_GMB/4);t++) begin
		MTSP_set_GMB((t*4)+0, GMB_memory[0].sram.mem[t]);
		MTSP_set_GMB((t*4)+1, GMB_memory[1].sram.mem[t]);
		MTSP_set_GMB((t*4)+2, GMB_memory[2].sram.mem[t]);
		MTSP_set_GMB((t*4)+3, GMB_memory[3].sram.mem[t]);
	end
end

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
	end
	else begin
		if(CE && WE)
			MTSP_set_GMB(ADDR, DIN);
	end
end
`endif

endmodule
