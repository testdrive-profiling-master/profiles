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

module MTSP_GPRs #(
	parameter	CORE_ID			= 0
) (
	// system
	input							CLK, nRST,			// main clock & reset (active low)
	input	[`RANGE_TRDs]			nTRD,				// Thread enable (active low)

	// source -----------------------------
	// Phase #0 source input
	input							SRC0A_nEN,			// Read enable (active low)
	input	[`RANGE_GPRs]			SRC0A_ADDR,			// Source A index
	output	reg	[`RANGE_DWORDx4]	SRC0A_DATA,			// Source A data
	input							SRC0B_nEN,			// Read enable (active low)
	input	[`RANGE_GPRs]			SRC0B_ADDR,			// Source B index
	output	reg	[`RANGE_DWORDx4]	SRC0B_DATA,			// Source B data
	// Phase #1 source input
	input							SRC1A_nEN,			// Read enable (active low)
	input	[`RANGE_GPRs]			SRC1A_ADDR,			// Source A index
	output	reg [`RANGE_DWORDx4]	SRC1A_DATA,			// Source A data
	input							SRC1B_nEN,			// Read enable (active low)
	input	[`RANGE_GPRs]			SRC1B_ADDR,			// Source B index
	output	reg [`RANGE_DWORDx4]	SRC1B_DATA,			// Source B data

	// write back -------------------------
	// Phase #0 output
	input							WB0_nEN,			// write enable (active low)
	input	[`RANGE_MASK4D]			WB0_WMASK,			// Write mask
	input	[`RANGE_GPRs]			WB0_ADDR,			// Write index
	input	[`RANGE_DWORDx4]		WB0_DATA,			// Write data
	// Phase #1 output
	input							WB1_nEN,			// write enable (active low)
	input	[`RANGE_MASK4D]			WB1_WMASK,			// Write mask
	input	[`RANGE_GPRs]			WB1_ADDR,			// Write index
	input	[`RANGE_DWORDx4]		WB1_DATA			// Write data
);

// register definition & assignment ------------------------------------------
wire	[5:0]				nEN_0, nEN_1, nEN_2, nEN_3, nEN_4, nEN_5;
wire	[`RANGE_DWORDx4]	rdata_0, rdata_1, rdata_2, rdata_3, rdata_4, rdata_5;
reg		[5:0]				src0a_0, src0a_1, src0b_0, src0b_1, src1a_0, src1a_1, src1b_0, src1b_1;
// WB1,                        WB0,                         S1B,                           S1A,                           S0B,                           S0A
assign	nEN_0	= {WB1_nEN|(nTRD[1]&nTRD[ 7]), WB0_nEN|(nTRD[ 0]&nTRD[ 6]), SRC1B_nEN|(nTRD[ 5]&nTRD[11]), SRC1A_nEN|(nTRD[ 4]&nTRD[10]), SRC0B_nEN|(nTRD[ 3]&nTRD[ 9]), SRC0A_nEN|(nTRD[ 2]&nTRD[ 8])};
assign	nEN_1	= {WB1_nEN|(nTRD[2]&nTRD[ 8]), WB0_nEN|(nTRD[ 1]&nTRD[ 7]), SRC1B_nEN|(nTRD[ 6]&nTRD[ 0]), SRC1A_nEN|(nTRD[ 5]&nTRD[11]), SRC0B_nEN|(nTRD[ 4]&nTRD[10]), SRC0A_nEN|(nTRD[ 3]&nTRD[ 9])};
assign	nEN_2	= {WB1_nEN|(nTRD[3]&nTRD[ 9]), WB0_nEN|(nTRD[ 2]&nTRD[ 8]), SRC1B_nEN|(nTRD[ 7]&nTRD[ 1]), SRC1A_nEN|(nTRD[ 6]&nTRD[ 0]), SRC0B_nEN|(nTRD[ 5]&nTRD[11]), SRC0A_nEN|(nTRD[ 4]&nTRD[10])};
assign	nEN_3	= {WB1_nEN|(nTRD[4]&nTRD[10]), WB0_nEN|(nTRD[ 3]&nTRD[ 9]), SRC1B_nEN|(nTRD[ 8]&nTRD[ 2]), SRC1A_nEN|(nTRD[ 7]&nTRD[ 1]), SRC0B_nEN|(nTRD[ 6]&nTRD[ 0]), SRC0A_nEN|(nTRD[ 5]&nTRD[11])};
assign	nEN_4	= {WB1_nEN|(nTRD[5]&nTRD[11]), WB0_nEN|(nTRD[ 4]&nTRD[10]), SRC1B_nEN|(nTRD[ 9]&nTRD[ 3]), SRC1A_nEN|(nTRD[ 8]&nTRD[ 2]), SRC0B_nEN|(nTRD[ 7]&nTRD[ 1]), SRC0A_nEN|(nTRD[ 6]&nTRD[ 0])};
assign	nEN_5	= {WB1_nEN|(nTRD[6]&nTRD[ 0]), WB0_nEN|(nTRD[ 5]&nTRD[11]), SRC1B_nEN|(nTRD[10]&nTRD[ 4]), SRC1A_nEN|(nTRD[ 9]&nTRD[ 3]), SRC0B_nEN|(nTRD[ 8]&nTRD[ 2]), SRC0A_nEN|(nTRD[ 7]&nTRD[ 1])};

// implementation ------------------------------------------------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		src0a_0		<= 6'b111111;
		src0a_1		<= 6'b111111;
		src0b_0		<= 6'b111111;
		src0b_1		<= 6'b111111;
		src1a_0		<= 6'b111111;
		src1a_1		<= 6'b111111;
		src1b_0		<= 6'b111111;
		src1b_1		<= 6'b111111;
	end
	else begin
		src0a_0		<= {nEN_5[0], nEN_4[0], nEN_3[0], nEN_2[0], nEN_1[0], nEN_0[0]};
		src0a_1		<= src0a_0;
		src0b_0		<= {nEN_5[1], nEN_4[1], nEN_3[1], nEN_2[1], nEN_1[1], nEN_0[1]};
		src0b_1		<= src0b_0;
		src1a_0		<= {nEN_5[2], nEN_4[2], nEN_3[2], nEN_2[2], nEN_1[2], nEN_0[2]};
		src1a_1		<= src1a_0;
		src1b_0		<= {nEN_5[3], nEN_4[3], nEN_3[3], nEN_2[3], nEN_1[3], nEN_0[3]};
		src1b_1		<= src1b_0;
		SRC0A_DATA	<= !src0a_1[0] ? rdata_0 :
		!src0a_1[1] ? rdata_1 :
		!src0a_1[2] ? rdata_2 :
		!src0a_1[3] ? rdata_3 :
		!src0a_1[4] ? rdata_4 : rdata_5;
		SRC0B_DATA	<= !src0b_1[0] ? rdata_0 :
		!src0b_1[1] ? rdata_1 :
		!src0b_1[2] ? rdata_2 :
		!src0b_1[3] ? rdata_3 :
		!src0b_1[4] ? rdata_4 : rdata_5;
		SRC1A_DATA	<= !src1a_1[0] ? rdata_0 :
		!src1a_1[1] ? rdata_1 :
		!src1a_1[2] ? rdata_2 :
		!src1a_1[3] ? rdata_3 :
		!src1a_1[4] ? rdata_4 : rdata_5;
		SRC1B_DATA	<= !src1b_1[0] ? rdata_0 :
		!src1b_1[1] ? rdata_1 :
		!src1b_1[2] ? rdata_2 :
		!src1b_1[3] ? rdata_3 :
		!src1b_1[4] ? rdata_4 : rdata_5;
	end
end

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(0)
) gprs_0 (
	CLK,nRST,
	nEN_0,
	{nTRD[ 2], SRC0A_ADDR},
	{nTRD[ 3], SRC0B_ADDR},
	{nTRD[ 4], SRC1A_ADDR},
	{nTRD[ 5], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 0], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 1], WB1_ADDR}, WB1_DATA,
	rdata_0
);

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(1)
) gprs_1 (
	CLK,nRST,
	nEN_1,
	{nTRD[ 3], SRC0A_ADDR},
	{nTRD[ 4], SRC0B_ADDR},
	{nTRD[ 5], SRC1A_ADDR},
	{nTRD[ 6], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 1], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 2], WB1_ADDR}, WB1_DATA,
	rdata_1
);

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(2)
) gprs_2 (
	CLK,nRST,
	nEN_2,
	{nTRD[ 4], SRC0A_ADDR},
	{nTRD[ 5], SRC0B_ADDR},
	{nTRD[ 6], SRC1A_ADDR},
	{nTRD[ 7], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 2], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 3], WB1_ADDR}, WB1_DATA,
	rdata_2
);

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(3)
) gprs_3 (
	CLK,nRST,
	nEN_3,
	{nTRD[ 5], SRC0A_ADDR},
	{nTRD[ 6], SRC0B_ADDR},
	{nTRD[ 7], SRC1A_ADDR},
	{nTRD[ 8], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 3], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 4], WB1_ADDR}, WB1_DATA,
	rdata_3
);

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(4)
) gprs_4 (
	CLK,nRST,
	nEN_4,
	{nTRD[ 6], SRC0A_ADDR},
	{nTRD[ 7], SRC0B_ADDR},
	{nTRD[ 8], SRC1A_ADDR},
	{nTRD[ 9], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 4], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 5], WB1_ADDR}, WB1_DATA,
	rdata_4
);

MTSP_GPRs_Primitive #(
	.CORE_ID		(CORE_ID),
	.PRIMITIVE_ID	(5)
) gprs_5 (
	CLK,nRST,
	nEN_5,
	{nTRD[ 7], SRC0A_ADDR},
	{nTRD[ 8], SRC0B_ADDR},
	{nTRD[ 9], SRC1A_ADDR},
	{nTRD[10], SRC1B_ADDR},
	WB0_WMASK, {nTRD[ 5], WB0_ADDR}, WB0_DATA,
	WB1_WMASK, {nTRD[ 6], WB1_ADDR}, WB1_DATA,
	rdata_5
);

endmodule
