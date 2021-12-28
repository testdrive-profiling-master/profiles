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
// Rev.  : 12/29/2021 Wed (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

/*verilator tracing_off*/
module top ();

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void StarterKit_Initialize();
`DPI_FUNCTION void StarterKit_LED(input bit [8:0] LED_pins);
`DPI_FUNCTION void StarterKit_NumericDisplay(input bit [13:0] pins);
`DPI_FUNCTION void StarterKit_Eval();
`DPI_FUNCTION void StarterKit_GetButtons(output bit [31:0] pins);
`DPI_FUNCTION void StarterKit_GetSwitches(output bit [31:0] pins);

wire					CLK, nRST, INTR;
wire					RSTn_Board;
wire	[13:0]			KW4_56NCWB_P_Y;
wire	[7:0]			LED;
reg		[31:0]			r_button;
reg		[8:0]			BUTTONS;
reg		[31:0]			r_switch;
reg		[7:0]			SWITCHES;

//// slave APB -------------------------
wire									S_PSEL;
wire									S_PENABLE;
wire									S_PWRITE;
wire	[15:0]							S_PADDR;
wire	[31:0]							S_PWDATA;
wire	[31:0]							S_PRDATA;
wire									S_PREADY;
wire									S_PSLVERR;

assign	RSTn_Board		= r_button[31] & nRST;

initial	begin
	SetSystemDescription("FPGA Starter Kit");
	StarterKit_Initialize();
	//	INTR	= 1'b0;
end

testdrive_clock_bfm #(
	.C_CLOCK_ID				(0),		// clock ID#
	.C_CLOCK_PERIOD_PS		(100000),	// default 100MHz
	.C_RESET_CYCLES			(32),		// first reset cycles
	.C_RESET_POLARITY		(0)			// default active low reset
) board_10MHz_clock (
	.CLK					(CLK),
	.RST					(nRST)
);

testdrive_interrupt_bfm intr_inst (
	.CLK		(CLK),
	.nRST		(RSTn_Board),
	.EN			(1'b1),
	.INTR		(INTR)
);

always@(posedge CLK) begin
	StarterKit_LED({RSTn_Board, LED});
	StarterKit_NumericDisplay(RSTn_Board ? KW4_56NCWB_P_Y : 14'b00_0000_0000_0000);
	StarterKit_Eval();
	StarterKit_GetButtons(r_button);
	BUTTONS		<= r_button[8:0];
	StarterKit_GetSwitches(r_switch);
	SWITCHES	<= r_switch[7:0];
end

testdrive_apb_slave_bfm #(
	.C_BUS_TITLE		("DUT"),
	.C_BASE_ADDR		(32'h0002_0000),
	.C_ADDR_BITS		(16)
) apb_slave_bfm (
	// system
	.CLK				(CLK),
	.nRST				(RSTn_Board),
	// Write
	.PSEL				(S_PSEL),
	.PENABLE			(S_PENABLE),
	.PWRITE				(S_PWRITE),
	.PADDR				(S_PADDR),
	.PWDATA				(S_PWDATA),
	.PSTRB				(),				// not using! all 32bit data operation
	.PRDATA				(S_PRDATA),
	.PREADY				(S_PREADY),
	.PSLVERR			(S_PSLVERR)
);

testdrive_busy_bfm busy_inst (.BUSY(1'b1));	// always FPGA is busy!

/*verilator tracing_on*/

// implementation ------------------------------------------------------------
dut_top	system (
	// system
	.CLK					(CLK),
	.nRST					(RSTn_Board),
	.INTR					(INTR),
	// APB slave
	.S_PSEL					(S_PSEL),
	.S_PENABLE				(S_PENABLE),
	.S_PWRITE				(S_PWRITE),
	.S_PADDR				(S_PADDR),
	.S_PWDATA				(S_PWDATA),
	.S_PRDATA				(S_PRDATA),
	.S_PREADY				(S_PREADY),
	.S_PSLVERR				(S_PSLVERR),
	// I/Os
	.LED_pins				(LED),
	.KW4_56NCWB_P_Y_pins	(KW4_56NCWB_P_Y),
	.SWITCH_pins			(SWITCHES),
	.BUTTON_pins			(BUTTONS)
);

endmodule
