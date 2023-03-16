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
// Title : TestDrive template design
// Rev.  : 3/16/2023 Thu (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"

/*verilator tracing_off*/
//-----------------------------------------------------------------------------
// edged interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_edge_bfm #(
	parameter			C_ACTIVE	= 1		// level : 1(active high), 0(active low)
) (
	input				CLK,		// clock
	input				nRST,		// reset active low
	input				EN,			// enable
	input				INTR		// interrupt
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION int SimulationAwakeInterrupt();
wire	_Active			= C_ACTIVE;
reg		_prev_INTR;

// implementation ------------------------------------------------------------

always@(posedge CLK, negedge nRST) begin
	if(nRST) begin
		_prev_INTR	<= INTR;
	end begin
		if(EN && (INTR^_prev_INTR) && (INTR == _Active)) begin
			/* verilator lint_off IGNOREDRETURN */ SimulationAwakeInterrupt();
		end
		_prev_INTR	<= INTR;
	end
end

endmodule

//-----------------------------------------------------------------------------
// leveled interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_level_bfm #(
	parameter			C_ACTIVE	= 1		// level : 1(active high), 0(active low)
) (
	input				CLK,		// clock
	input				nRST,		// reset active low
	input				EN,			// enable
	input				INTR		// interrupt
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION int SimulationAwakeInterrupt();
wire	_Active			= C_ACTIVE;

// implementation ------------------------------------------------------------

always@(posedge CLK, negedge nRST) begin
	if(nRST) begin
	end begin
		if(EN && (INTR == _Active)) begin
			/* verilator lint_off IGNOREDRETURN */ SimulationAwakeInterrupt() ;
		end
	end
end

endmodule

//-----------------------------------------------------------------------------
// edged/leveled interrupt
//-----------------------------------------------------------------------------
module testdrive_interrupt_bfm #(
	parameter			C_EDGE_DETECT	= 1,	// edge : 1(edge), 0(level)
	parameter			C_ACTIVE		= 1		// level : 1(high), 0(low)
) (
	input				CLK,		// clock
	input				nRST,		// reset active low
	input				EN,			// enable
	input				INTR		// interrupt
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
generate
if(C_EDGE_DETECT==1) begin : gen_edge
	testdrive_interrupt_edge_bfm #(.C_ACTIVE(C_ACTIVE)) intr (.*);
end
else begin : gen_level
	testdrive_interrupt_level_bfm #(.C_ACTIVE(C_ACTIVE)) intr (.*);
end
endgenerate

endmodule
/*verilator tracing_on*/
