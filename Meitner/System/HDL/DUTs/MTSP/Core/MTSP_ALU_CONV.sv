//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_ALU_CONV(	CLK, nRST,
					  MO0, MO0_MASK, SRC0A, SRC0B,
					  MO1, MO1_MASK, SRC1A, SRC1B,
					  PHASE_EN, DOUT);

// port definition -----------------------------------------------------------
// System
input					CLK, nRST;				// main clock & reset (active low)
// input
input	[`RANGE_MODESC]	MO0, MO1;				// phase #0/#1 micro-operation description
input					MO0_MASK, MO1_MASK;		// phase #0/#1 micro-operation mask
input	[`RANGE_DWORD]	SRC0A, SRC0B,			// phase #0 source data
SRC1A, SRC1B;			// phase #1 source data
// output
output	[1:0]			PHASE_EN;				// output enable
output	[`RANGE_DWORD]	DOUT;					// result output

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]	DIN;
reg		[`RAMGE_MOEXT]	moext_sm, moext_ex0;
wire					MO0_en	= (~MO0[`RANGE_MODESC_nEN]) & (MO0[`RANGE_MODESC_MO]==`MO_CONV) & (~MO0[`RANGE_MODESC_ALT]),
MO1_en	= (~MO1[`RANGE_MODESC_nEN]) & (MO1[`RANGE_MODESC_MO]==`MO_CONV) & (~MO1[`RANGE_MODESC_ALT]),
MO_en	= MO0_en | MO1_en;
// implementation ------------------------------------------------------------
// pre-stage : SM -----------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		DIN				<= 0;
		moext_sm		<= 0;
	end
	else begin
		DIN				<= MO_en ? (MO1_en ? (MO1_MASK ? SRC1B : SRC1A) : (MO0_MASK ? SRC0B : SRC0A)) : DIN;
		moext_sm[`RANGE_MOEXT_EN]		<= MO_en;
		moext_sm[`RANGE_MOEXT_PHASE]	<= MO1_en;	// phase #1 priority is high
		moext_sm[`RANGE_MOEXT_OP]		<= MO_en ? (MO1_en ? MO1[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL] : MO0[`RANGE_MODESC_ALT:`RANGE_MODESC_SEL]) : moext_sm[`RANGE_MOEXT_OP];
	end
end

// stage #1 : EX0 -----------------------
// fp 2 int
wire						ex0_int_ZERO= (DIN[`RANGE_DATA_EXP]<63),
ex0_int_MAX	= (DIN[`RANGE_DATA_EXP]>86),
ex0_int_SET	= ex0_int_ZERO | ex0_int_MAX;
wire						ex0_int_S	= ex0_int_ZERO ? 0 : DIN[`RANGE_DATA_SIGN];
wire	[`RANGE_FRACTION]	ex0_int_F	= ex0_int_SET ? 0  : DIN[`RANGE_DATA_FRACTION];
wire	[`RANGE_EXP]		ex0_int_EL	= DIN[`RANGE_DATA_EXP]-7'd79,									// FP2INT : exponential for shift left
ex0_int_ER	= -ex0_int_EL,													// FP2INT : exponential fot shift right
ex0_int_E	= ex0_int_ZERO	? 0 :
ex0_int_MAX	? 7'b1111111 :
{ex0_int_EL[`MSB_EXP], (ex0_int_EL[`MSB_EXP] ? ex0_int_ER[`MSB_EXP-1:0] : ex0_int_EL[`MSB_EXP-1:0])};

// int 2 fp
wire						ex0_fp_S	= DIN[`RANGE_DATA_SIGN];// & moext_sm[`RANGE_MOEXT_SEL];			// unsigned signed operation
wire	[`RANGE_DATA]		ex0_fp_abs	= ex0_fp_S ? -DIN[`RANGE_DATA] : DIN[`RANGE_DATA];
wire	[`RANGE_EXP]		ex0_fp_E	= ex0_fp_abs[23] ? 7'd86 :	// 63+23=86
ex0_fp_abs[22] ? 7'd85 :
ex0_fp_abs[21] ? 7'd84 :
ex0_fp_abs[20] ? 7'd83 :
ex0_fp_abs[19] ? 7'd82 :
ex0_fp_abs[18] ? 7'd81 :
ex0_fp_abs[17] ? 7'd80 :
ex0_fp_abs[16] ? 7'd79 :
ex0_fp_abs[15] ? 7'd78 :
ex0_fp_abs[14] ? 7'd77 :
ex0_fp_abs[13] ? 7'd76 : 7'd75;
wire	[`RANGE_FRACTION]	ex0_fp_F	= ex0_fp_abs[23] ? ex0_fp_abs[22:7] :
ex0_fp_abs[22] ? ex0_fp_abs[21:6] :
ex0_fp_abs[21] ? ex0_fp_abs[20:5] :
ex0_fp_abs[20] ? ex0_fp_abs[19:4] :
ex0_fp_abs[19] ? ex0_fp_abs[18:3] :
ex0_fp_abs[18] ? ex0_fp_abs[17:2] :
ex0_fp_abs[17] ? ex0_fp_abs[16:1] :
ex0_fp_abs[16] ? ex0_fp_abs[15:0] :
ex0_fp_abs[15] ? {ex0_fp_abs[14:0], 1'b0} :
ex0_fp_abs[14] ? {ex0_fp_abs[13:0], 2'b0} :
ex0_fp_abs[13] ? {ex0_fp_abs[12:0], 3'b0} : {ex0_fp_abs[11:0], 4'b0};
wire						ex0_fp_SET	= |ex0_fp_abs[23:12];	// found

reg							ex1_int_S;
reg		[`RANGE_FRACTION]	ex1_int_F;
reg		[`RANGE_EXP]		ex1_int_E;
reg							ex1_int_SET;
reg							ex1_fp_S, ex1_fp_SET;
reg		[`RANGE_EXP]		ex1_fp_E;
reg		[`RANGE_FRACTION]	ex1_fp_F;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		{ex1_int_S, ex1_int_E, ex1_int_F, ex1_int_SET}	<= 0;
		{ex1_fp_S, ex1_fp_E, ex1_fp_F, ex1_fp_SET}		<= 0;
		moext_ex0		<= 0;
	end
	else begin
		{ex1_int_S, ex1_int_E, ex1_int_F, ex1_int_SET}		// fp -> int
		<= {ex0_int_S, ex0_int_E, ex0_int_F, ex0_int_SET};
		{ex1_fp_S, ex1_fp_E, ex1_fp_F, ex1_fp_SET}			// int -> fp
		<= {ex0_fp_S, ex0_fp_E, ex0_fp_F, ex0_fp_SET};
		moext_ex0		<= moext_sm;
	end
end

// stage #2 : EX1 -----------------------
// fp 2 int
wire	[`RANGE_DATA]		ex1_int_SL	= {8'b1, ex1_int_F} << ex1_int_E[`MSB_EXP-1:0],		// FP2INT : shift left
ex1_int_SR	= {8'b1, ex1_int_F} >> ex1_int_E[`MSB_EXP-1:0],		// FP2INT : shift right
ex1_int_abs	= ex1_int_E[`MSB_EXP]	? ex1_int_SR : ex1_int_SL,
ex1_int_out	= ex1_int_SET			? {ex1_int_S, ex1_int_E, ex1_int_F} :
ex1_int_S				? -ex1_int_abs : ex1_int_abs;

// int 2 fp
wire						ex1_fp_S2	= ex1_fp_S & (|ex1_fp_F[15:4]);
wire	[`RANGE_EXP]		ex1_fp_E2	= ex1_fp_F[15] ? 7'd74 :
ex1_fp_F[14] ? 7'd73 :
ex1_fp_F[13] ? 7'd72 :
ex1_fp_F[12] ? 7'd71 :
ex1_fp_F[11] ? 7'd70 :
ex1_fp_F[10] ? 7'd69 :
ex1_fp_F[ 9] ? 7'd68 :
ex1_fp_F[ 8] ? 7'd67 :
ex1_fp_F[ 7] ? 7'd66 :
ex1_fp_F[ 6] ? 7'd65 :
ex1_fp_F[ 5] ? 7'd64 :
ex1_fp_F[ 4] ? 7'd63 : 7'd0;
wire	[`RANGE_FRACTION]	ex1_fp_F2	= ex1_fp_F[15] ? {ex1_fp_F[14:4],  5'b0} :
ex1_fp_F[14] ? {ex1_fp_F[13:4],  6'b0} :
ex1_fp_F[13] ? {ex1_fp_F[12:4],  7'b0} :
ex1_fp_F[12] ? {ex1_fp_F[11:4],  8'b0} :
ex1_fp_F[11] ? {ex1_fp_F[10:4],  9'b0} :
ex1_fp_F[10] ? {ex1_fp_F[ 9:4], 10'b0} :
ex1_fp_F[ 9] ? {ex1_fp_F[ 8:4], 11'b0} :
ex1_fp_F[ 8] ? {ex1_fp_F[ 7:4], 12'b0} :
ex1_fp_F[ 7] ? {ex1_fp_F[ 6:4], 13'b0} :
ex1_fp_F[ 6] ? {ex1_fp_F[ 5:4], 14'b0} :
ex1_fp_F[ 5] ? {ex1_fp_F[   4], 15'b0} : 16'd0;
wire	[`RANGE_DATA]		ex1_fp_out	= ex1_fp_SET ? {ex1_fp_S, ex1_fp_E, ex1_fp_F} : {ex1_fp_S2, ex1_fp_E2, ex1_fp_F2};

assign	DOUT		= moext_ex0[`RANGE_MOEXT_SEL] ?	{8'b0, ex1_fp_out} : {ex1_int_out[23], ex1_int_out[23], ex1_int_out[23], ex1_int_out[23],
		ex1_int_out[23], ex1_int_out[23], ex1_int_out[23], ex1_int_out[23], ex1_int_out};
assign	PHASE_EN	= {moext_ex0[`RANGE_MOEXT_EN]&moext_ex0[`RANGE_MOEXT_PHASE], moext_ex0[`RANGE_MOEXT_EN]&(~moext_ex0[`RANGE_MOEXT_PHASE])};

endmodule
