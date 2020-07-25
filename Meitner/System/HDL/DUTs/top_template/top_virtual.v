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
// Title : Processor
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`define	VIRTUAL_SLAVE_BASE_ADDR		32'h0
`define	VIRTUAL_SLAVE_ADDR_BITS		16

`include "top/top_virtual.sv"

module dut_top (
	//// system ----------------------------
	input									CLK,				// clock
	input									nRST,				// reset (active low)
	output									BUSY,				// processor is busy
	output									INTR,				// interrupt signal

	// write
	input									S_WE,
	input	[`VIRTUAL_SLAVE_ADDR_BITS-1:0]	S_WADDR,
	input	[`RANGE_DWORD]					S_WDATA,

	// read
	input									S_RE,
	input	[`VIRTUAL_SLAVE_ADDR_BITS-1:0]	S_RADDR,
	output	[`RANGE_DWORD]					S_RDATA
);

// definition & assignment ---------------------------------------------------
// simulation will be run within 1000 clock
`define	END_OF_TIME		1000
reg	[31:0]			sim_counter;
assign	BUSY		= (sim_counter <  `END_OF_TIME);
assign	INTR		= (sim_counter == `END_OF_TIME);

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nRST) begin
		sim_counter	<= 'd0;
	end
	else begin
		sim_counter	<= sim_counter + 1;
	end
end

endmodule
