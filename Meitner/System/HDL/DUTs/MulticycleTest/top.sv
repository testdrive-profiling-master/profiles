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
// Title : Processor
// Rev.  : 5/30/2023 Tue (clonextop@gmail.com)
//================================================================================
`include "testdrive_system.vh"
`include "library/demux_by_enable.v"
`include "library/MultiCyclePath.v"
`include "library/MultiCyclePathEx.v"
`include "library/SRAM_Single.v"
`include "library/SRAM_Single_Multicycle.v"
`include "MAD.v"
`include "MAD_multicycled.v"
`include "MAD_multicycled_Ex.v"
`include "SRAM_Single_multicycled.v"

/* HIDDEN */
module top (
	//// system ----------------------------
	input							MCLK,			// clock
	input							nRST,			// reset
	output	reg						BUSY,			// processor is busy
	output							INTR			// interrupt signal
);

// definition & assignment ---------------------------------------------------
reg	[31:0]					sim_counter;
assign	INTR				= `FALSE;

reg							ie;
wire						iready;
reg		[63:0]				data_a, data_b, data_c;
wire						oe;
wire	[63:0]				odata;

`define ON_TIME(t)	if(sim_counter==t)

// implementation ------------------------------------------------------------
always@(posedge MCLK, negedge nRST) begin
	if(!nRST) begin
		sim_counter		<= 'd0;
		ie				<= `FALSE;
		data_a			<= 'd0;
		data_b			<= 'd0;
		data_c			<= 'd0;
		BUSY			<= `TRUE;
	end
	else begin
		sim_counter		<= sim_counter + 1;

		`ON_TIME(2) begin
			ie			<= `TRUE;
			data_a		<= 3;
			data_b		<= 4;
			data_c		<= 5;	// 3 * 4 + 5 = 17
		end
		`ON_TIME(3) begin
			ie			<= `TRUE;
			data_a		<= 5;
			data_b		<= 6;
			data_c		<= 7;	// 5 * 6 + 7 = 37
		end
		`ON_TIME(4) begin
			ie			<= `TRUE;
			data_a		<= 7;
			data_b		<= 8;
			data_c		<= 9;	// 7 * 8 + 9 = 65
		end
		`ON_TIME(5) begin
			ie			<= `TRUE;
			data_a		<= 9;
			data_b		<= 10;
			data_c		<= 11;	// 7 * 8 + 9 = 101
		end
		`ON_TIME(6) begin
			ie			<= `TRUE;
			data_a		<= 11;
			data_b		<= 12;
			data_c		<= 13;	// 11 * 12 + 13 = 145
		end
		`ON_TIME(7) begin
			ie			<= `TRUE;
			data_a		<= 13;
			data_b		<= 14;
			data_c		<= 15;	// 13 * 14 + 15 = 197
		end
		`ON_TIME(8) begin
			ie			<= `TRUE;
			data_a		<= 15;
			data_b		<= 16;
			data_c		<= 17;	// 15 * 16 + 17 = 257
		end
		`ON_TIME(9) begin
			ie			<= `TRUE;
			data_a		<= 17;
			data_b		<= 18;
			data_c		<= 19;	// 17 * 18 + 19 = 325
		end
		`ON_TIME(10) begin
			ie			<= `TRUE;
			data_a		<= 19;
			data_b		<= 20;
			data_c		<= 21;	// 19 * 20 + 21 = 401
		end
		`ON_TIME(11) begin
			ie			<= `FALSE;
			BUSY		<= `FALSE;	// simulation end
		end
	end
end

//----------------------------------------------------------------------------
// Combinational MAD
//----------------------------------------------------------------------------
`ifdef COMBINATIONAL
MAD mad (
	.A			(data_a),
	.B			(data_b),
	.C			(data_c),
	.O			(odata)
);
assign	iready	= 1'b1;
assign	oe		= 1'b1;
`endif

//----------------------------------------------------------------------------
// Multi-cycle MAD
//----------------------------------------------------------------------------
`ifdef MULTICYCLE
MAD_multicycled #(
	.CYCLE		(3)
) mad (
	.CLK		(MCLK),
	.nRST		(nRST),
	.IE			(ie),
	.A			(data_a),
	.B			(data_b),
	.C			(data_c),
	.OE			(oe),
	.O			(odata)
);
assign	iready	= 1'b1;
`endif

//----------------------------------------------------------------------------
// Multi-cycle extension MAD
//----------------------------------------------------------------------------
`ifdef MULTICYCLE_EX
MAD_multicycled_Ex #(
	.CYCLE		(3),
	.COUNT		(2)
) mad (
	.CLK		(MCLK),
	.nRST		(nRST),
	.IE			(ie),
	.IREADY		(iready),
	.A			(data_a),
	.B			(data_b),
	.C			(data_c),
	.OE			(oe),
	.O			(odata)
);
`endif

//----------------------------------------------------------------------------
// Multi-cycle slice MAD
//----------------------------------------------------------------------------
`ifdef MULTICYCLE_SLICE
MAD_multicycled_Slice #(
	.CYCLE		(3)
) mad (
	.CLK		(MCLK),
	.nRST		(nRST),
	.IE			(ie),
	.IREADY		(iready),
	.A			(data_a),
	.B			(data_b),
	.C			(data_c),
	.OE			(oe),
	.O			(odata)
);
`endif

//----------------------------------------------------------------------------
// Multi-cycle Single SRAM
//----------------------------------------------------------------------------
`ifdef MULTICYCLE_SRAM
SRAM_Single_multicycled #(
	.CYCLE		(3)
) multicycle_sram (
	.CLK		(MCLK),
	.nRST		(nRST),
	.EN			(ie),
	.WE			(sim_counter[1]),
	.READY		(iready),
	.ADDR		(data_a[9:0]),
	.DIN		(data_b),
	.OE			(oe),
	.DOUT		(odata)
);
`endif

endmodule
