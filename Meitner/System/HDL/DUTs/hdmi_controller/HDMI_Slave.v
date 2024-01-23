//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : HDMI controller
// Rev.  : 1/23/2024 Tue (clonextop@gmail.com)
//================================================================================
module HDMI_Slave # (
		parameter			C_DEFAULT_BASE_ADDR	= 64'h00000000_80000000,
		parameter			C_CLKIN_PERIOD		= 5
	) (
		// port list -----------------------------------------------------------------
		//// AXI slave interface
		// system
		input								CLK,			// clock
		input								nRST,			// reset (active low)
		//// Simplified Slave interface
		input								S_EN,			// enable
		input								S_WE,			// write enable
		input	[3:0]						S_ADDR,			// address
		input	[31:0]						S_WDATA,		// write data
		output	[31:0]						S_RDATA,		// read data
		//// I2C interface
		output	reg							I2C_nRST,		// i2c reset (active low)
		input								I2C_SCL_I,		// i2c SCL input
		output	reg							I2C_SCL_T,		// i2c SCL tri-state enable
		input								I2C_SDA_I,		// i2c SDA input
		output	reg							I2C_SDA_T,		// i2c SDA tri-state enable
		//// HDMI interface
		// control
		output	reg							HDMI_nEN,		// hdmi enable (active low)
		output								HDMI_CLK,		// hdmi clock
		// configuration
		output								CFG_WE,			// write configuration enable
		output	[11:0]						CFG_DATA,		// write configuration data
		// video info.
		output	reg [63:0]					VIDEO_BASE,		// base address
		output	reg [15:0]					VIDEO_FRAME		// frame count
	);

	// Parameters ----------------------------------------------------------------

	// definition & assignment ---------------------------------------------------
	// clkgen
	reg							clkgen_EN, clkgen_WE, clkgen_RST;
	reg		[6:0]				clkgen_ADDR;
	reg		[15:0]				clkgen_WDATA;
	wire	[15:0]				clkgen_RDATA;
	wire						clkgen_READY, clkgen_LOCKED;

	// read bypass
	assign	S_RDATA				= {
		clkgen_RDATA,
		HDMI_nEN,
		clkgen_READY,
		clkgen_LOCKED,
		10'd0,
		I2C_nRST, I2C_SCL_I, I2C_SDA_I
	};

	assign	CFG_WE				= S_EN & S_WE & (S_ADDR == 'd1);
	assign	CFG_DATA			= S_WDATA[11:0];

	// implementation ------------------------------------------------------------
	// HDMI control & I2C setting
	always@(posedge CLK) begin
		if(!nRST) begin
			clkgen_EN			<= 1'b0;
			clkgen_WE			<= 1'b0;
			clkgen_RST			<= 1'b1;
			clkgen_ADDR			<= 7'd0;
			clkgen_WDATA		<= 16'd0;
			HDMI_nEN			<= 1'b1;
			I2C_nRST			<= 1'b0;
			I2C_SCL_T			<= 1'b1;
			I2C_SDA_T			<= 1'b1;
			VIDEO_BASE			<= C_DEFAULT_BASE_ADDR;
			VIDEO_FRAME			<= 'd1;		// minimum count
		end
		else begin
			if(S_EN & S_WE) begin
				if(S_ADDR=='d0) begin
					I2C_nRST		<= S_WDATA[2];
					I2C_SCL_T		<= S_WDATA[1];
					I2C_SDA_T		<= S_WDATA[0];
				end
				//if(S_ADDR=='d1) // #1 : MMCM configuration write
				if(S_ADDR=='d2) begin
					HDMI_nEN		<= S_WDATA[31];
					clkgen_RST		<= S_WDATA[25];
					clkgen_EN		<= S_WDATA[24];
					clkgen_WE		<= S_WDATA[23];
					clkgen_ADDR		<= S_WDATA[22:16];
					clkgen_WDATA	<= S_WDATA[15:0];
				end
				if(S_ADDR=='d3) begin
					VIDEO_BASE[31:0]	<= S_WDATA;
				end
				if(S_ADDR=='d4) begin
					VIDEO_BASE[63:32]	<= S_WDATA;
				end
				if(S_ADDR=='d5) begin
					VIDEO_FRAME			<= S_WDATA[15:0];
				end
			end
		end
	end

	// ClkGen
	HDMI_ClkGen #(
		.C_CLKIN_PERIOD	(C_CLKIN_PERIOD)
	) ClkGen_inst (
		// clock in/out
		.REF_CLK		(CLK),
		.CLK_OUT		(HDMI_CLK),

		// control signals
		.EN				(clkgen_EN),
		.WE				(clkgen_WE),
		.RST			(clkgen_RST),
		.ADDR			(clkgen_ADDR),
		.WDATA			(clkgen_WDATA),
		.RDATA			(clkgen_RDATA),
		.READY			(clkgen_READY),
		.LOCKED			(clkgen_LOCKED)
	);

endmodule
