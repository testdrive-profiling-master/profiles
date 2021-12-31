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
// Title : Virtual FPGA Starter Kit
// Rev.  : 12/31/2021 Fri (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`include "template/StarterKit/includes.vh"

module dut_top (
	input						CLK,					// clock (10MHz)
	input						nRST,					// reset (active low)
	output						INTR,					// interrupt signal (active high, edged mode)

	//// APB slave bus (base address : 0x20000000, search keywords on google : "APB protocol filetype:pdf")
	output						S_PCLK,					// slave clock output
	output						S_PRESETn,				// slave reset (active low)
	input						S_PSEL,					// select
	input						S_PENABLE,				// enable
	input						S_PWRITE,				// write enable
	input	[15:0]				S_PADDR,				// address
	input	[31:0]				S_PWDATA,				// write data
	output	[31:0]				S_PRDATA,				// read data
	output						S_PREADY,				// ready
	output						S_PSLVERR,				// slave error

	//// AXI4 master bus (128bit narrow transfer supports, search keywords on google : "axi4 protocol filetype:pdf")
	output						M_ACLK,					// master clock
	output						M_ARESETn,				// reset (active low)
	// write address
	output						M_AWID,					// The ID tag for the write address group of signals
	output	[31:0]				M_AWADDR,				// Write address
	output	[7:0]				M_AWLEN,				// Burst_Length = AxLEN + 1
	output	[2:0]				M_AWSIZE,				// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]				M_AWBURST,				// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output						M_AWLOCK,				// b0(Normal), b1(Exclusive)
	output	[3:0]				M_AWCACHE,				// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]				M_AWPROT,				// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	output	[3:0]				M_AWREGION,				// Write region identifier (not defined in this)
	output	[3:0]				M_AWQOS,				// Write Quality of Service (not defined in this)
	output						M_AWVALID,				// Write address valid
	input 						M_AWREADY,				// Write address ready (1 = slave ready, 0 = slave not ready)
	// write data
	output 						M_WID,					// Write ID tag
	output	[127:0]				M_WDATA,				// Write data
	output	[15:0]				M_WSTRB,				// Write strobes (WSTRB[n] = WDATA[(8  n) + 7:(8  n)])
	output						M_WLAST,				// Write last
	output						M_WVALID,				// Write valid
	input 						M_WREADY,				// Write ready
	// bus
	input 						M_BID,					// The ID tag for the write response
	input 	[1:0]				M_BRESP,				// b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input 						M_BVALID,				// Write response valid
	output						M_BREADY,				// Response ready
	// read address
	output						M_ARID,					// Read address ID tag
	output	[31:0]				M_ARADDR,				// Read address
	output	[7:0]				M_ARLEN,				// Burst_Length = AxLEN + 1
	output	[2:0]				M_ARSIZE,				// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]				M_ARBURST,				// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output						M_ARLOCK,				// b0(Normal), b1(Exclusive)
	output	[3:0]				M_ARCACHE,				// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]				M_ARPROT,				// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	input 	[3:0]				M_ARREGION,				// Read region identifier (not defined in this)
	input	[3:0]				M_ARQOS,				// Read Quality of Service (not defined in this)
	output						M_ARVALID,				// Read address valid
	input						M_ARREADY,				// Read address ready (1 = slave ready, 0 = slave not ready)
	// read data
	input 						M_RID,					// Read ID tag
	input	[127:0]				M_RDATA,				// Read data
	input	[1:0]				M_RRESP,				// Read response. b00(OKAY), b01(EXOKAY), b10(SLVERR), b11(DECERR)
	input						M_RLAST,				// Read last. This signal indicates the last transfer in a read burst.
	input						M_RVALID,				// Read valid (1 = read data available, 0 = read data not available)
	output						M_RREADY,				// Read ready (1= master ready 0 = master not ready)

	//// GPIOs
	output	[7:0]				LED_pins,				// LED pins
	output	[13:0]				KW4_56NCWB_P_Y_pins,	// Quadruple Digit Numeric Displays (Model # : KW4_56NCWB_P_Y)
	input	[7:0]				SWITCH_pins,			// toggle switch pins
	input	[8:0]				BUTTON_pins,			// buttons (active low)

	//// Motor
	output						MOTOR_PWM,				// motor PWM control
	output						MOTOR_DIR,				// motor direction
	input						MOTOR_SENSOR,			// motor hole sensor

	//// TFT LCD display
	output						TFT_PCLK,				// pixel clock
	output						TFT_DISP,				// display enable
	output						TFT_HSYNC,				// horizontal sync input in RGB mode.
	output						TFT_VSYNC,				// vertical sync input in RGB mode.
	output						TFT_DE,					// data enable
	output	[23:0]				TFT_RGB					// red/green/blue data
);

// definition & assignment ---------------------------------------------------
reg		[16:0]	t_count;
reg		[7:0]	led_data;

assign	LED_pins	= led_data;

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nRST) begin
		t_count		<= 'd0;
		led_data	<= 'd0;

	end
	else begin
		t_count		<= t_count + 1'b1;
		if (t_count == 'd0) begin
			led_data	<= led_data + 1'b1;
		end
	end
end

endmodule
