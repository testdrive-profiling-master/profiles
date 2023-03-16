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
// Clock manager
//-----------------------------------------------------------------------------
module testdrive_clock_bfm #(
	parameter	int unsigned	C_CLOCK_ID				= 0,		// clock ID#
	parameter	int	unsigned	C_CLOCK_PERIOD_PS		= 10000,	// default 100MHz
	parameter	bit unsigned	C_CLOCK_INIT_VALUE		= 0,
	parameter	int unsigned	C_CLOCK_PHASE_PS		= 0,
	parameter	bit unsigned	C_CLOCK_POLARITY		= 1,		// default active high
	parameter	int unsigned	C_RESET_CYCLES			= 16,
	parameter	bit unsigned	C_RESET_POLARITY		= 0			// default active low reset
) (
	output	reg							CLK /*verilator public*/,	// clock
	output	reg							RST	/*verilator public*/	// reset
);

// definition & assignment ---------------------------------------------------
chandle	hHandle /*verilator public*/;

`DPI_FUNCTION void SimulationSetClock(input chandle hHandle, input int unsigned dwID, input int unsigned dwPeriod, input bit unsigned InitValue, input int unsigned dwPhase, input bit unsigned ClockPolarity, input bit unsigned ResetPolarity);
`DPI_FUNCTION void SimulationSetReset(input chandle hHandle, input int unsigned dwCycles);

// implementation ------------------------------------------------------------
`ifdef VERILATOR
`systemc_header
void* SimulationClockCreate(unsigned char* pCLK, unsigned char* pRST);
`systemc_ctor
hHandle	= (QData)SimulationClockCreate(&CLK, &RST);
`verilog
`endif

initial begin
	CLK	= C_CLOCK_INIT_VALUE;
	RST = C_RESET_POLARITY;
	SimulationSetClock(hHandle, C_CLOCK_ID, C_CLOCK_PERIOD_PS/2, C_CLOCK_INIT_VALUE, C_CLOCK_PHASE_PS, C_CLOCK_POLARITY, C_RESET_POLARITY);
	SimulationSetReset(hHandle, C_RESET_CYCLES);
end

endmodule
/*verilator tracing_on*/
