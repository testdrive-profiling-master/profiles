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
// Title : Template design
// Rev.  : 12/21/2021 Tue (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

/*verilator tracing_off*/
module top (
	//// system ----------------------------
	input									MCLK,				// clock
	input									nRST,				// reset (active low)
	output									BUSY,				// processor is busy
	output									INTR				// interrupt signal
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void StarterKit_Initialize();
`DPI_FUNCTION void StarterKit_LED(input bit [7:0] LED_pins);
`DPI_FUNCTION void StarterKit_NumericDisplay(input bit [13:0] pins);
`DPI_FUNCTION void StarterKit_Eval();
`DPI_FUNCTION void StarterKit_GetButtons(output bit [12:0] pins);

wire					RSTn_Board;
wire					CLK_10MHz;
wire					CLK_10MHz_RSTn;
wire	[13:0]			KW4_56NCWB_P_Y;
wire	[7:0]			LED;
reg		[12:0]			r_button;
reg		[7:0]			TOGGLE_SWITCH;
reg		[4:0]			ARROW_BUTTON;

initial	begin
	StarterKit_Initialize();
	TOGGLE_SWITCH	= 'd0;
	ARROW_BUTTON	= 'd0;
	//	INTR	= 1'b0;
	//	BUSY	= 1'b0;
end

testdrive_clock_bfm #(
	.C_CLOCK_ID				(0),		// clock ID#
	.C_CLOCK_PERIOD_PS		(100000),	// default 100MHz
	.C_RESET_CYCLES			(32),		// first reset cycles
	.C_RESET_POLARITY		(0)			// default active low reset
) board_10MHz_clock (
	.CLK					(CLK_10MHz),
	.RST					(CLK_10MHz_RSTn)
);

always@(posedge CLK_10MHz) begin
	StarterKit_LED(LED);
	StarterKit_NumericDisplay(KW4_56NCWB_P_Y);
	StarterKit_Eval();
	StarterKit_GetButtons(r_button);
	{TOGGLE_SWITCH, ARROW_BUTTON}	<= r_button;
end

/*verilator tracing_on*/

// implementation ------------------------------------------------------------
dut_top	system (
	// system
	.MCLK					(MCLK),
	.nRST					(nRST),
	.BUSY					(BUSY),
	.INTR					(INTR),
	.LED_pins				(LED),
	.RSTn_Board				(RSTn_Board & CLK_10MHz_RSTn),
	.CLK_10MHz				(CLK_10MHz),
	.KW4_56NCWB_P_Y_pins	(KW4_56NCWB_P_Y),
	.TOGGLE_SWITCH			(TOGGLE_SWITCH),
	.ARROW_BUTTON			(ARROW_BUTTON)
);

endmodule
