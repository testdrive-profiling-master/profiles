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
// Title : processor AXI wrapper
// Rev.  : 5/25/2023 Thu (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns
`include "testdrive_system.vh"
`include "template/testdrive_virtual_slave_bfm.sv"
/*verilator tracing_off*/
/* HIDDEN */
// virtual hdmi controller
module hdmi_controller #(
	parameter		C_S_AXI_ADDR			= 64'h00010000,
	parameter		C_DEFAULT_VIDEO_BASE	= 32'h80000000
) (
	//// system
	input							CLK, nRST		// clock & reset (active low)
);

// definition & assignment ---------------------------------------------------
`DPI_FUNCTION void VirtualDisplayInitialize(input int bReverse);
`DPI_FUNCTION void VirtualDisplayBaseAddress(input int unsigned dwBaseAddress, input int bFront);
`DPI_FUNCTION void VirtualDisplayFormat(input int iWidth, input int iHeight, input int unsigned dwByteStride, input int unsigned Format);

initial begin
	SetSystemDescription("HDMI controller");
	VirtualDisplayInitialize(0);
	VirtualDisplayBaseAddress(C_DEFAULT_VIDEO_BASE, 0);
	VirtualDisplayBaseAddress(C_DEFAULT_VIDEO_BASE, 1);
	VirtualDisplayFormat(1920, 1080, 0, 8);
end

wire					we;
wire	[9:0]			waddr;
wire	[`RANGE_DWORD]	wdata;

// implementation ------------------------------------------------------------
testdrive_virtual_slave_bfm #(
	.C_BUS_TITLE	("hdmi_slave"),
	.C_BASE_ADDR	(C_S_AXI_ADDR),
	.C_ADDR_BITS	(10)
) slave (
	.CLK			(CLK),
	.nRST			(nRST),
	.WE				(we),
	.WADDR			(waddr),
	.WDATA			(wdata),
	.RE				(),
	.RADDR			(),
	.RDATA			({16'd0, 1'b0, 1'b1, 1'b1, 10'd0, 3'd0})	// clkgen_RDATA(0), HDMI_nEN(0), clkgen_READY(1), clkgen_LOCKED(1), dummy(0), i2c(0)
);

int unsigned hdmi_reg[8];

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
	end
	else begin
		if(we) begin
			if(waddr[9:2] == 'd1) begin
				for(int i=0;i<7;i++) begin
					hdmi_reg[i] = hdmi_reg[i+1];
				end
				hdmi_reg[7]	= wdata;
				VirtualDisplayFormat(hdmi_reg[1] - hdmi_reg[3], hdmi_reg[0] - hdmi_reg[2], 0, 7/*DISPLAY_FORMAT_ABGR_8888*/);
			end
			else
				if(waddr[9:2] == 'd3) begin
					if(wdata[31]) begin
						VirtualDisplayBaseAddress(wdata, 1/*Front*/);
					end
				end

		end
	end
end

endmodule
/*verilator tracing_on*/
