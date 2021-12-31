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
// Rev.  : 12/31/2021 Fri (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

/*verilator tracing_off*/
`include "template/testdrive_clock_bfm.sv"
`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"
`include "template/testdrive_busy_bfm.sv"
`include "template/testdrive_interrupt_bfm.sv"

module top ();

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void StarterKit_Initialize();
`DPI_FUNCTION void StarterKit_LED(input bit [8:0] LED_pins);
`DPI_FUNCTION void StarterKit_NumericDisplay(input bit [13:0] pins);
`DPI_FUNCTION void StarterKit_Eval();
`DPI_FUNCTION void StarterKit_GetButtons(output bit [31:0] pins);
`DPI_FUNCTION void StarterKit_GetSwitches(output bit [31:0] pins);
`DPI_FUNCTION void StarterKit_Motor(input bit PWM, input bit DIR, output bit SENSOR);
`DPI_FUNCTION void StarterKit_TFT_LCD(input bit DISP, input bit HSYNC, input bit VSYNC, input bit DE, input bit [23:0] RGB);

wire					CLK, nRST, INTR;
wire					RSTn_Board;
wire	[13:0]			KW4_56NCWB_P_Y;
wire	[7:0]			LED;
wire					MOTOR_PWM, MOTOR_DIR;
reg						MOTOR_SENSOR, r_motor_sensor;
reg		[31:0]			r_button;
reg		[8:0]			BUTTONS;
reg		[31:0]			r_switch;
reg		[7:0]			SWITCHES;

// TFT LCD display
wire					tft_pclk, tft_disp, tft_hsync, tft_vsync, tft_de;
wire	[23:0]			tft_rgb;

assign	RSTn_Board		= r_button[31] & nRST;

initial	begin
	SetSystemDescription("FPGA Starter Kit");
	StarterKit_Initialize();
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
	BUTTONS			<= r_button[8:0];
	StarterKit_GetSwitches(r_switch);
	SWITCHES		<= r_switch[7:0];
	StarterKit_Motor(MOTOR_PWM, MOTOR_DIR, r_motor_sensor);
	MOTOR_SENSOR	<= r_motor_sensor;
end

always@(negedge tft_pclk) begin
	StarterKit_TFT_LCD(tft_disp, tft_hsync, tft_vsync, tft_de, tft_rgb);
end

// slave APB
wire					S_PCLK, S_PRESETn;
wire					S_PSEL;
wire					S_PENABLE;
wire					S_PWRITE;
wire	[15:0]			S_PADDR;
wire	[31:0]			S_PWDATA;
wire	[31:0]			S_PRDATA;
wire					S_PREADY;
wire					S_PSLVERR;

testdrive_apb_slave_bfm #(
	.C_BUS_TITLE		("APB slave"),
	.C_BASE_ADDR		(32'h0002_0000),
	.C_ADDR_BITS		(16)
) apb_slave_inst (
	// system
	.CLK				(S_PCLK),
	.nRST				(S_PRESETn),
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

// master AXI4
wire					M_ACLK, M_ARESETn;
wire					M_AWID;
wire	[31:0]			M_AWADDR;
wire	[7:0]			M_AWLEN;
wire	[2:0]			M_AWSIZE;
wire	[1:0]			M_AWBURST;
wire					M_AWLOCK;
wire	[3:0]			M_AWCACHE;
wire	[2:0]			M_AWPROT;
wire	[3:0]			M_AWREGION;
wire	[3:0]			M_AWQOS;
wire					M_AWVALID;
wire 					M_AWREADY;
// write data
wire 					M_WID;
wire	[127:0]			M_WDATA;
wire	[15:0]			M_WSTRB;
wire					M_WLAST;
wire					M_WVALID;
wire 					M_WREADY;
// bus
wire 					M_BID;
wire 	[1:0]			M_BRESP;
wire 					M_BVALID;
wire					M_BREADY;
// read address
wire					M_ARID;
wire	[31:0]			M_ARADDR;
wire	[7:0]			M_ARLEN;
wire	[2:0]			M_ARSIZE;
wire	[1:0]			M_ARBURST;
wire					M_ARLOCK;
wire	[3:0]			M_ARCACHE;
wire	[2:0]			M_ARPROT;
wire	[3:0]			M_ARREGION;
wire 	[3:0]			M_ARQOS;
wire					M_ARVALID;
wire 					M_ARREADY;
// read data
wire 					M_RID;
wire 	[127:0]			M_RDATA;
wire 	[1:0]			M_RRESP;
wire 					M_RLAST;
wire 					M_RVALID;
wire					M_RREADY;

testdrive_axi4_master_bfm #(
	.C_BUS_TITLE			("AXI4 master"),
	.C_THREAD_ID_WIDTH		(1),
	.C_ADDR_WIDTH			(32),
	.C_DATA_WIDTH			(128),
	.C_USE_AXI4				(1)
) axi4_master_inst (
	.CLK					(M_ACLK),
	.nRST					(M_ARESETn),
	// write address
	.AWID					(M_AWID),
	.AWADDR					(M_AWADDR),
	.AWLEN					(M_AWLEN),
	.AWSIZE					(M_AWSIZE),
	.AWBURST				(M_AWBURST),
	.AWLOCK					(M_AWLOCK),
	.AWCACHE				(M_AWCACHE),
	.AWPROT					(M_AWPROT),
	.AWREGION				(M_AWREGION),
	.AWQOS					(M_AWQOS),
	.AWVALID				(M_AWVALID),
	.AWREADY				(M_AWREADY),
	// write data
	.WID					(M_WID),
	.WDATA					(M_WDATA),
	.WSTRB					(M_WSTRB),
	.WLAST					(M_WLAST),
	.WVALID					(M_WVALID),
	.WREADY					(M_WREADY),
	// bus
	.BID					(M_BID),
	.BRESP					(M_BRESP),
	.BVALID					(M_BVALID),
	.BREADY					(M_BREADY),
	// read address
	.ARID					(M_ARID),
	.ARADDR					(M_ARADDR),
	.ARLEN					(M_ARLEN),
	.ARSIZE					(M_ARSIZE),
	.ARBURST				(M_ARBURST),
	.ARLOCK					(M_ARLOCK),
	.ARCACHE				(M_ARCACHE),
	.ARPROT					(M_ARPROT),
	.ARREGION				(M_ARREGION),
	.ARQOS					(M_ARQOS),
	.ARVALID				(M_ARVALID),
	.ARREADY				(M_ARREADY),
	// read data
	.RID					(M_RID),
	.RDATA					(M_RDATA),
	.RRESP					(M_RRESP),
	.RLAST					(M_RLAST),
	.RVALID					(M_RVALID),
	.RREADY					(M_RREADY)
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
	.S_PCLK					(S_PCLK),
	.S_PRESETn				(S_PRESETn),
	.S_PSEL					(S_PSEL),
	.S_PENABLE				(S_PENABLE),
	.S_PWRITE				(S_PWRITE),
	.S_PADDR				(S_PADDR),
	.S_PWDATA				(S_PWDATA),
	.S_PRDATA				(S_PRDATA),
	.S_PREADY				(S_PREADY),
	.S_PSLVERR				(S_PSLVERR),
	// AXI4 master
	.M_ACLK					(M_ACLK),
	.M_ARESETn				(M_ARESETn),
	// write address
	.M_AWID					(M_AWID),
	.M_AWADDR				(M_AWADDR),
	.M_AWLEN				(M_AWLEN),
	.M_AWSIZE				(M_AWSIZE),
	.M_AWBURST				(M_AWBURST),
	.M_AWLOCK				(M_AWLOCK),
	.M_AWCACHE				(M_AWCACHE),
	.M_AWPROT				(M_AWPROT),
	.M_AWREGION				(M_AWREGION),
	.M_AWQOS				(M_AWQOS),
	.M_AWVALID				(M_AWVALID),
	.M_AWREADY				(M_AWREADY),
	// write data
	.M_WID					(M_WID),
	.M_WDATA				(M_WDATA),
	.M_WSTRB				(M_WSTRB),
	.M_WLAST				(M_WLAST),
	.M_WVALID				(M_WVALID),
	.M_WREADY				(M_WREADY),
	// bus
	.M_BID					(M_BID),
	.M_BRESP				(M_BRESP),
	.M_BVALID				(M_BVALID),
	.M_BREADY				(M_BREADY),
	// read address
	.M_ARID					(M_ARID),
	.M_ARADDR				(M_ARADDR),
	.M_ARLEN				(M_ARLEN),
	.M_ARSIZE				(M_ARSIZE),
	.M_ARBURST				(M_ARBURST),
	.M_ARLOCK				(M_ARLOCK),
	.M_ARCACHE				(M_ARCACHE),
	.M_ARPROT				(M_ARPROT),
	.M_ARREGION				(M_ARREGION),
	.M_ARQOS				(M_ARQOS),
	.M_ARVALID				(M_ARVALID),
	.M_ARREADY				(M_ARREADY),
	// read data
	.M_RID					(M_RID),
	.M_RDATA				(M_RDATA),
	.M_RRESP				(M_RRESP),
	.M_RLAST				(M_RLAST),
	.M_RVALID				(M_RVALID),
	.M_RREADY				(M_RREADY),
	// I/Os
	.LED_pins				(LED),
	.KW4_56NCWB_P_Y_pins	(KW4_56NCWB_P_Y),
	.SWITCH_pins			(SWITCHES),
	.BUTTON_pins			(BUTTONS),
	// Motor
	.MOTOR_PWM				(MOTOR_PWM),
	.MOTOR_DIR				(MOTOR_DIR),
	.MOTOR_SENSOR			(MOTOR_SENSOR),
	// TFT LCD
	.TFT_PCLK				(tft_pclk),
	.TFT_DISP				(tft_disp),
	.TFT_HSYNC				(tft_hsync),
	.TFT_VSYNC				(tft_vsync),
	.TFT_DE					(tft_de),
	.TFT_RGB				(tft_rgb)
);

endmodule
