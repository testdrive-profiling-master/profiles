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
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_SCs_Element(
	// system
	input						CLK, nRST,			// main clock & reset (active low)
	// input
	input						nEN,				// Enable (active low)
	input	[1:0]				OP,					// Operations
	input						CARRY_IN,			// Carry input
	input	[`RANGE_WORD]		DATA_IN,			// Data input
	// output
	output						CARRY_OUT,			// Carry output
	output	[`RANGE_WORD]		DATA_OUT			// Data output
);

// register definition & assignment ------------------------------------------
reg		[`RANGE_WORD]		Scratch;				// scratch counters
reg		[`RANGE_WORD]		Limit;					// Counter limit

// increase & overflow check
wire						Equal		= (Scratch == Limit);
wire						Op_Inc		= CARRY_IN | (OP == `SC_GETnINC);
wire						Op_Set		= (OP == `SC_SET);
wire	[`RANGE_WORD]		Inc_data	= Scratch + 1;			// scratch+1

// out carry
assign						CARRY_OUT	= Equal & Op_Inc;
// out scratch
assign						DATA_OUT	= Scratch;

// implementation ------------------------------------------------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		Scratch		<= 0;
		Limit		<= 16'hFFFF;
	end
	else if(!nEN) begin
		Limit		<= (OP == `SC_SET_LIMIT) ? DATA_IN : Limit;
		Scratch		<= Op_Set ? DATA_IN : Op_Inc ? (Equal ? 0 : Inc_data) : Scratch;
	end
end



endmodule
