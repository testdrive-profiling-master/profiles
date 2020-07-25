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

module MTSP_ALU_MOV( CLK, nRST,
					 PC,
					 MO, MOMASK, SRCA, SRCB,
					 EN_FLFR, EN_CONV, EN_BW, EN_CMP, EN_MUL, EN_ADD, EN_SF, EN_BADD, EN_FMT,
					 OUT_FLFR, OUT_CONV, OUT_BW, OUT_CMP, OUT_MUL, OUT_ADD, OUT_SF, OUT_BADD, OUT_FMT,
					 WMASK, DEST);

// port definition -----------------------------------------------------------
// System
input						CLK, nRST;			// main clock & reset (active low)

// input
input	[`RANGE_PC]			PC;					// next instruction program counter @ SM output
input	[`RANGE_MODESC]		MO;					// MO description
input						MOMASK;				// MO mask
input	[`RANGE_DWORD]		SRCA, SRCB;			// source A & B
input						EN_FLFR, EN_CONV, EN_BW, EN_CMP, EN_MUL, EN_ADD, EN_SF, EN_BADD, EN_FMT;
input	[`RANGE_DWORD]		OUT_FLFR, OUT_CONV, OUT_BW, OUT_CMP, OUT_MUL, OUT_ADD, OUT_SF, OUT_BADD, OUT_FMT;
// output
output						WMASK;
output	[`RANGE_DWORD]		DEST;

// register definition & assignment ------------------------------------------
reg		[`RANGE_DWORD]	mov_data_0, mov_data_1, mov_data_2;			// mov register data
wire	[`RANGE_DWORD]	pipe_data_1, pipe_data_2, pipe_data_3;		// pipe data
reg		[2:0]			pipe_en;									// pipe enable
wire					nEN		= (MO[`RANGE_MODESC_MO]!=`MO_MOV) | MO[`RANGE_MODESC_nEN];
wire					sm_alt	= (MO[`RANGE_MODESC_MO]==`MO_MOV) & MO[`RANGE_MODESC_ALT];

// implementation ------------------------------------------------------------
// pre-stage : SM ----------------------- pipe : source modifier
reg						ex0_alt;

// stage #1 : EX0 -----------------------
assign	pipe_data_1		= EN_BW		? OUT_BW :
EN_CMP 	? OUT_CMP :
ex0_alt	? {20'b0, PC} : mov_data_0;

// stage #2 : EX1 -----------------------
assign	pipe_data_2		= EN_FLFR	? OUT_FLFR :
EN_CONV	? OUT_CONV :
EN_FMT	? OUT_FMT : mov_data_1;

// stage #3 : EX2 -----------------------
assign	pipe_data_3		= EN_ADD	? OUT_ADD :
EN_MUL	? OUT_MUL :
EN_BADD	? OUT_BADD :
EN_SF 	? OUT_SF : mov_data_2;

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		pipe_en	<= 0;
		ex0_alt	<= 0;
		//WMASK	<= `nFALSE;
		//DEST	<= 0;
		mov_data_0	<= 0;
		mov_data_1	<= 0;
		mov_data_2	<= 0;
	end
	else begin
		// pre-stage : SM -----------------------
		pipe_en[0]	<= ~nEN;
		ex0_alt		<= sm_alt;
		mov_data_0	<= nEN ? (mov_data_0) : (MOMASK ? SRCB : SRCA);
		// stage #1 : EX0 -----------------------
		pipe_en[1]	<= EN_BW|EN_CMP|pipe_en[0];
		mov_data_1	<= EN_BW|EN_CMP|pipe_en[0]			? pipe_data_1 : mov_data_1;
		// stage #2 : EX1 -----------------------
		pipe_en[2]	<= EN_FLFR|EN_CONV|EN_FMT|pipe_en[1];
		mov_data_2	<= EN_FLFR|EN_CONV|EN_BADD|EN_FMT|pipe_en[1]		? pipe_data_2 : mov_data_2;
		// stage #3 : EX2 -----------------------
		//WMASK		<= ~(EN_ADD|EN_MUL|EN_SF|pipe_en[2]);
		//DEST		<= EN_ADD|EN_MUL|EN_SF|pipe_en[2]	? pipe_data_3 : DEST;
	end
end

assign	WMASK		= ~(EN_ADD|EN_MUL|EN_BADD|EN_SF|pipe_en[2]),
DEST		= pipe_data_3;

endmodule
