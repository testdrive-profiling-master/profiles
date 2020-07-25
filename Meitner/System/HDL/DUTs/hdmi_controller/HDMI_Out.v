//================================================================================
// Copyright (c) 2013 ~ 2020. HyungKi Jeong(clonextop@gmail.com)
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
// Title : HDMI controller
// Rev.  : 3/2/2020 Mon (clonextop@gmail.com)
//================================================================================
`timescale 1 ns / 100 ps
module	HDMI_Out(
	// port list -----------------------------------------------------------------
	input					CLK, RST,
	input					HSYNC, VSYNC, DE,
	input	[35:0]			RGB,
	output					HDMI_CLK,
`ifdef USE_TESTDRIVE
	output	reg				HDMI_DE, HDMI_HSYNC, HDMI_VSYNC,
	output	reg [35:0]		HDMI_DATA
`else
    output					HDMI_DE, HDMI_HSYNC, HDMI_VSYNC,
    output	[35:0]			HDMI_DATA
`endif
);

// definition & assignment ---------------------------------------------------

// implementation ------------------------------------------------------------
`ifdef USE_TESTDRIVE
assign	HDMI_CLK	= CLK;

always@(posedge CLK) begin
	HDMI_HSYNC		<= HSYNC;
	HDMI_VSYNC		<= VSYNC;
	HDMI_DE			<= DE;
	HDMI_DATA		<= RGB;
end
`else
// inverted clock output for RGB set-up timing
ODDR #(
    .DDR_CLK_EDGE ("OPPOSITE_EDGE"),
    .INIT(1'b0),
    .SRTYPE("SYNC")
) HDMI_CLK_ODDR	(.Q(HDMI_CLK), .C(CLK), .CE(1'b1), .R(RST), .D1(1'b0), .D2(1'b1), .S(1'b0));	/* synthesis syn_useioff = 1 */

FDRE FDRE_HSYNC		(.Q(HDMI_HSYNC), .C(CLK), .CE(1'b1), .R(RST), .D(HSYNC))		/* synthesis syn_useioff = 1 */;
FDRE FDRE_VSYNC		(.Q(HDMI_VSYNC), .C(CLK), .CE(1'b1), .R(RST), .D(VSYNC))		/* synthesis syn_useioff = 1 */;
FDRE FDRE_DE		(.Q(HDMI_DE),    .C(CLK), .CE(1'b1), .R(RST), .D(DE))			/* synthesis syn_useioff = 1 */;

FDRE FDRE_DATA_0	(.Q(HDMI_DATA[ 0]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 0]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_1	(.Q(HDMI_DATA[ 1]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 1]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_2	(.Q(HDMI_DATA[ 2]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 2]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_3	(.Q(HDMI_DATA[ 3]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 3]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_4	(.Q(HDMI_DATA[ 4]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 4]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_5	(.Q(HDMI_DATA[ 5]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 5]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_6	(.Q(HDMI_DATA[ 6]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 6]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_7	(.Q(HDMI_DATA[ 7]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 7]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_8	(.Q(HDMI_DATA[ 8]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 8]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_9	(.Q(HDMI_DATA[ 9]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[ 9]))	/* synthesis syn_useioff = 1 */;

FDRE FDRE_DATA_10	(.Q(HDMI_DATA[10]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[10]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_11	(.Q(HDMI_DATA[11]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[11]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_12	(.Q(HDMI_DATA[12]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[12]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_13	(.Q(HDMI_DATA[13]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[13]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_14	(.Q(HDMI_DATA[14]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[14]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_15	(.Q(HDMI_DATA[15]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[15]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_16	(.Q(HDMI_DATA[16]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[16]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_17	(.Q(HDMI_DATA[17]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[17]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_18	(.Q(HDMI_DATA[18]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[18]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_19	(.Q(HDMI_DATA[19]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[19]))	/* synthesis syn_useioff = 1 */;

FDRE FDRE_DATA_20	(.Q(HDMI_DATA[20]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[20]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_21	(.Q(HDMI_DATA[21]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[21]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_22	(.Q(HDMI_DATA[22]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[22]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_23	(.Q(HDMI_DATA[23]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[23]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_24	(.Q(HDMI_DATA[24]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[24]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_25	(.Q(HDMI_DATA[25]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[25]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_26	(.Q(HDMI_DATA[26]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[26]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_27	(.Q(HDMI_DATA[27]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[27]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_28	(.Q(HDMI_DATA[28]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[28]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_29	(.Q(HDMI_DATA[29]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[29]))	/* synthesis syn_useioff = 1 */;

FDRE FDRE_DATA_30	(.Q(HDMI_DATA[30]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[30]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_31	(.Q(HDMI_DATA[31]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[31]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_32	(.Q(HDMI_DATA[32]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[32]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_33	(.Q(HDMI_DATA[33]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[33]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_34	(.Q(HDMI_DATA[34]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[34]))	/* synthesis syn_useioff = 1 */;
FDRE FDRE_DATA_35	(.Q(HDMI_DATA[35]), .C(CLK), .CE(1'b1), .R(RST), .D(RGB[35]))	/* synthesis syn_useioff = 1 */;
`endif

endmodule
