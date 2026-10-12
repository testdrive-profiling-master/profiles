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
// Title : Virtual FPGA Starter Kit
// Rev.  : 10/12/2026 Mon (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`include "template/StarterKit/includes.svh"
`include "library/FiFo.sv"
`include "library/SRAM_Dual_Distributed.sv"

module dut_top (
	input						CLK,					// clock (10MHz)
	input						nRST,					// reset (active low)
	output						INTR,					// interrupt signal (active high, edged mode)

	//// APB slave bus (base address : 0x00020000 ~ 0x0002FFFF, search keywords on google : "APB protocol filetype:pdf")
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
	output	reg [31:0]			M_ARADDR,				// Read address
	output	[7:0]				M_ARLEN,				// Burst_Length = AxLEN + 1
	output	[2:0]				M_ARSIZE,				// bytes in transfer b000(1:8bit), b001(2:16bit), b010(4:32bit), b011(8:64bit), b100(16:128bit), b101(32:256bit), b110(64:512bit), b111(128:1024bit)
	output	[1:0]				M_ARBURST,				// b00(FIXED), b01(INCR), b10(WRAP), b11(Reserved)
	output						M_ARLOCK,				// b0(Normal), b1(Exclusive)
	output	[3:0]				M_ARCACHE,				// [0] Bufferable, [1] Cacheable, [2] Read Allocate, [3] Write Allocate
	output	[2:0]				M_ARPROT,				// Protection level : [0] privileged(1)/normal(0) access, [1] nonesecure(1)/secure(0) access, [2] instruction(1)/data(0) access
	input 	[3:0]				M_ARREGION,				// Read region identifier (not defined in this)
	input	[3:0]				M_ARQOS,				// Read Quality of Service (not defined in this)
	output	reg					M_ARVALID,				// Read address valid
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
	output	reg					TFT_DISP,				// display enable
	output	reg					TFT_HSYNC,				// horizontal sync input in RGB mode.
	output	reg					TFT_VSYNC,				// vertical sync input in RGB mode.
	output	reg					TFT_DE,					// data enable
	output	reg [23:0]			TFT_RGB					// red/green/blue data
);

// definition & assignment ---------------------------------------------------
// master bus
assign	M_ACLK		= CLK;
assign	M_ARESETn	= nRST;
assign	M_AWID		= 'b0;
assign	M_AWVALID	= 'b0;
// write address (not using)
assign	M_AWLEN		= 'd0;
assign	M_AWSIZE	= 'b100;	// bytes in transfer b100(16:128bit)
assign	M_AWBURST	= 'b01;		// b01(INCR)
assign	M_AWLOCK	= 'b0;		// b0(Normal)
assign	M_AWCACHE	= 'b0;		// [0] Bufferable
assign	M_AWPROT	= 'b0;		// [0] privileged(1)/normal(0) access
assign	M_AWREGION	= 'b0;
assign	M_AWQOS		= 'b0;
assign	M_WLAST		= 1'b0;
assign	M_WVALID	= 1'b1;

// bus
assign	M_BREADY	= 1'b1;

// read
assign	M_ARID		= 'b0;
assign	M_ARLEN		= 'd119;
assign	M_ARSIZE	= 'b100;	// (16:128bit)
assign	M_ARBURST	= 'b01;		// b01(INCR)
assign	M_ARLOCK	= 'b0;		// b0(Normal)
assign	M_ARCACHE	= 'b0;		// [0] Bufferable
assign	M_ARPROT	= 'b0;		// [0] privileged(1)/normal(0) access

assign	M_RREADY	= 1'b1;


// slave bus
assign	S_PCLK		= CLK;
assign	S_PRESETn	= nRST;
assign	S_PREADY	= 1'b1;
assign	S_PSLVERR	= 1'b0;

// TFT LCD
assign	TFT_PCLK	= CLK;

// implementation ------------------------------------------------------------
`ifdef USE_TESTDRIVE
initial
	SetSystemDescription("TFT_LCD_solved");
`endif

reg	[31:0]	base_address;
always@(posedge CLK) begin
	if(!nRST) begin
		base_address	<= 'd0;
		TFT_DISP		<= 1'b0;
	end
	else begin
		if (S_PSEL && S_PENABLE && S_PWRITE) begin
			if (S_PADDR == 'd0) begin	// set base address
				base_address	<= S_PWDATA;
			end
			if (S_PADDR == 'd4) begin	// start/end display
				TFT_DISP		<= S_PWDATA[0];
			end
		end
	end
end


// display timing
reg	[9:0]	h_count;
reg	[8:0]	v_count;

wire	[9:0]	h_count_next	= (h_count < 620) ? (h_count + 1) : 0;
wire	[8:0]	v_count_next	= (v_count < 380) ? (v_count + 1) : 0;
wire	hsync_next				= (h_count < 480);
wire	vsync_next				= (v_count >= 1) & (v_count <= 272);
wire	de_next					= hsync_next & vsync_next;
wire	start_frame				= (h_count == 'd1) & (v_count == 0);
wire	line_end				= TFT_VSYNC & TFT_DE & (de_next == 1'b0);
wire	next_line				= line_end && (v_count != 'd272);

// line buffer fifo
wire	fifo_read				= TFT_DE & (h_count[1:0] == 'b11);
wire	[127:0]	fifo_rdata;

always@(posedge CLK) begin
	if(!nRST) begin
		h_count		<= 'd0;
		v_count		<= 'd0;
		TFT_HSYNC	<= 1'b0;
		TFT_VSYNC	<= 1'b0;
		TFT_DE		<= 1'b0;
		TFT_RGB		<= 'hFF0000;
		M_ARADDR	<= 'h0;
		M_ARVALID	<= 1'b0;
	end
	else begin
		if (TFT_DISP) begin
			h_count	<= h_count_next;
			if(h_count_next == 0) begin
				v_count = v_count_next;
			end
			TFT_HSYNC	<= hsync_next;
			TFT_VSYNC	<= vsync_next;
			TFT_DE		<= de_next;
			case(h_count[1:0])
				'b00:
					TFT_RGB	<= fifo_rdata[23:0];
				'b01:
					TFT_RGB	<= fifo_rdata[(23+32):32];
				'b10:
					TFT_RGB	<= fifo_rdata[(23+64):64];
				'b11:
					TFT_RGB	<= fifo_rdata[(23+96):96];
			endcase
		end
		else begin
			h_count		<= 'd0;
			v_count		<= 'd0;
			TFT_HSYNC	<= 'b0;
			TFT_VSYNC	<= 'b0;
			TFT_DE		<= 'b0;
			TFT_RGB		<= 'h0;
		end

		// update frame buffer address
		if (start_frame) begin
			M_ARADDR	<= base_address;
		end
		else if (next_line) begin
			M_ARADDR	<= M_ARADDR + (512 * 4);
		end

		// do bus mastering
		if (M_ARVALID) begin
			M_ARVALID	<= ~M_ARREADY;
		end
		else if(start_frame || next_line) begin
			M_ARVALID	<= 1'b1;
		end
	end
end

FiFo #(
	.FIFO_DEPTH		(8),
	.DATA_WIDTH		(128)
) line_buffer (
	// system signals
	.CLK			(CLK),
	.nCLR			(nRST),

	// push interface
	.nWE			(~M_RVALID),	// write enable (active low)
	.DIN			(M_RDATA),		// input data
	.FULL			(),				// fifo is full (not using in here)
	// pop interface
	.nRE			(~fifo_read),	// read enable (active low)
	.DOUT			(fifo_rdata),	// output data
	.EMPTY			()				// fifo is empty (not using in here)
);

endmodule
