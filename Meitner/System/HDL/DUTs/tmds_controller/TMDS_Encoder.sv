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
// Title : TMDS controller
// Rev.  : 7/29/2026 Wed (clonextop@gmail.com)
//================================================================================
`timescale 1ns/1ns

module TMDS_Encoder(
	//// system
	input							CLK,				// clock
	input							nRST,				// reset (active low)
	// I/O
	input        				    DE,					// data enable
	input	[1:0]					CTRL,				// control signal
	input	[7:0]					DIN,				// data input
	output	reg [9:0]				DOUT				// data output
);

wire	[3:0]	Nb1s;
wire			XNOR;
wire	[8:0]	q_m;
reg		[3:0]	balance_acc;
wire	[3:0]	balance;
wire			balance_sign_eq;
wire			invert_q_m;
wire	[3:0]	balance_acc_inc;
wire	[3:0]	balance_acc_new;
wire	[9:0]	TMDS_data;
wire	[9:0]	TMDS_code;

// definition & assignment ---------------------------------------------------
assign Nb1s				= (
	{3'b0, DIN[0]} +
	{3'b0, DIN[1]} +
	{3'b0, DIN[2]} +
	{3'b0, DIN[3]} +
	{3'b0, DIN[4]} +
	{3'b0, DIN[5]} +
	{3'b0, DIN[6]} +
	{3'b0, DIN[7]}
);
assign XNOR				= (Nb1s > 4'd4) || (Nb1s == 4'd4 && DIN[0] == 1'b0);
assign q_m				= {~XNOR, q_m[6:0] ^ DIN[7:1] ^ {7{XNOR}}, DIN[0]};

assign balance			= (
	{3'b0, q_m[0]} +
	{3'b0, q_m[1]} +
	{3'b0, q_m[2]} +
	{3'b0, q_m[3]} +
	{3'b0, q_m[4]} +
	{3'b0, q_m[5]} +
	{3'b0, q_m[6]} +
	{3'b0, q_m[7]} -
	4'd4
);
assign balance_sign_eq	= (balance[3] == balance_acc[3]);
assign invert_q_m		= (balance == 4'b0 || balance_acc == 4'b0) ? ~q_m[8] : balance_sign_eq;

assign balance_acc_inc	= balance - {3'b0, ({q_m[8] ^ ~balance_sign_eq} & ~(balance == 4'b0 || balance_acc == 4'b0))};
assign balance_acc_new	= invert_q_m ? balance_acc-balance_acc_inc : balance_acc+balance_acc_inc;
assign TMDS_data		= {invert_q_m, q_m[8], q_m[7:0] ^ {8{invert_q_m}}};
assign TMDS_code		= CTRL[1] ? (CTRL[0] ? 10'b1010101011 : 10'b0101010100) : (CTRL[0] ? 10'b0010101011 : 10'b1101010100);

// implementation ------------------------------------------------------------
always@(posedge CLK, negedge nRST) begin
	if(!nRST) begin
		DOUT			<= 0;
		balance_acc		<= 0;
	end
	else begin
		DOUT			<= DE ? TMDS_data : TMDS_code;
		balance_acc		<= DE ? balance_acc_new:4'h0;
	end
end

endmodule
