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
`ifndef __TESTDRIVE_REGISTER_ELEMENT_V__
`define __TESTDRIVE_REGISTER_ELEMENT_V__
`include "system_defines.vh"

// register element
module Register_element #(
	parameter	WIDTH				= 32,			// data width
	parameter	INITIAL				= 32'd0,		// initial value
	parameter	FAST_VALID_RECOVERY	= 1,			// fast validation recovery when write operation
	parameter	PRE_OCCUPY			= 0				// pre-write occupied
)
(
	// system signals
	input							CLK,			// clock
	input							nRST,			// reset (active low)
	// register read/write
	input							R_EN,			// read enable
	output	[(WIDTH-1):0]			R_DATA,			// read data
	input							W_EN,			// write enable
	input	[(WIDTH-1):0]			W_DATA,			// write data
	output	[(WIDTH-1):0]			REGISTER,		// register data

	// register control
	input							OCCUPY,			// set write occupy
	output							VALID,			// data validation
	output	reg						nOCCUPIED		// register occupied
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg [(WIDTH-1):0]			element;				// register element

// write/read first operation
assign	R_DATA				= W_EN ? W_DATA : element;	// write first data output
assign	REGISTER			= element;					// read first data output

// register validation
generate
if(FAST_VALID_RECOVERY) begin : on_fast_valid_recovery	// fast validation recovery
	assign	VALID			= W_EN | nOCCUPIED | (~R_EN);
end
else begin : on_not_fast_valid_recovery				// not fast validation recovery
	assign	VALID			= nOCCUPIED | (~R_EN);
end
endgenerate

// implementation ------------------------------------------------------------
// write occupy control
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		nOCCUPIED		<= (PRE_OCCUPY!=0) ? `FALSE : `TRUE;
	end
	else begin
		nOCCUPIED		<= (~OCCUPY) & (W_EN | nOCCUPIED);
	end
end

// register write
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		element			<= INITIAL;
	end
	else begin
		if(W_EN) begin
			element		<= W_DATA;
		end
	end
end

endmodule

`endif//__TESTDRIVE_REGISTER_ELEMENT_V__
