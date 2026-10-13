//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive template design
// Rev.  : 10/13/2026 Tue (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.svh"
`DPI_FUNCTION void SimulationAwakeInterrupt(int iVector);

/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// edged interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_edge_bfm #(
	parameter			C_VECTOR	= 0,	// interrupt vector's start number
	parameter			C_WIDTH		= 1		// vector size
) (
	input						CLK,		// clock
	input						nRST,		// reset active low
	input						EN,			// enable
	input	[(C_WIDTH-1):0]		INTR		// interrupt
);

// definition & assignment ---------------------------------------------------
genvar	i;
reg		[(C_WIDTH-1):0]			_prev_INTR;

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	_prev_INTR	<= INTR;
end

generate
	for(i=0;i<C_WIDTH;i++) begin
		always@(posedge CLK, negedge nRST) begin
			if(nRST) begin
				if(EN && INTR[i] && !_prev_INTR[i]) begin
					SimulationAwakeInterrupt(C_VECTOR + i);
				end
			end
		end
	end
endgenerate

endmodule

//-----------------------------------------------------------------------------
// leveled interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_level_bfm #(
	parameter			C_VECTOR	= 0,	// interrupt vector's start number
	parameter			C_WIDTH		= 1		// vector size
) (
	input						CLK,		// clock
	input						nRST,		// reset active low
	input						EN,			// enable
	input	[(C_WIDTH-1):0]		INTR		// interrupt
);

// definition & assignment ---------------------------------------------------
genvar	i;

// implementation ------------------------------------------------------------
generate
	for(i=0;i<C_WIDTH;i++) begin
		always@(posedge CLK, negedge nRST) begin
			if(nRST) begin
				if(EN && INTR[i]) begin
					SimulationAwakeInterrupt(C_VECTOR + i);
				end
			end
		end
	end
endgenerate

endmodule

//-----------------------------------------------------------------------------
// edged/leveled interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_bfm #(
	parameter			C_EDGE		= 1,	// edge : 1(edge), 0(level)
	parameter			C_VECTOR	= 0,	// interrupt vector's start number
	parameter			C_WIDTH		= 1		// vector size
) (
	input						CLK,		// clock
	input						nRST,		// reset active low
	input						EN,			// enable
	input	[(C_WIDTH-1):0]		INTR		// interrupt
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
generate
	if(C_EDGE == 1) begin : gen_edge
		testdrive_interrupt_edge_bfm #(.C_VECTOR(C_VECTOR), .C_WIDTH(C_WIDTH)) intr (.*);
	end
	else begin : gen_level
		testdrive_interrupt_level_bfm #(.C_VECTOR(C_VECTOR), .C_WIDTH(C_WIDTH)) intr (.*);
	end
endgenerate

endmodule
/*verilator tracing_on*/
