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
// Title : MTSP(Multi-thread Stream Processor) 'Meitner' v1.1
// Rev.  : 7/20/2021 Tue (clonextop@gmail.com)
//================================================================================
`include "MTSP_Defines.vh"

module MTSP_Single #(
	parameter	CORE_ID			= 0
) (
	// System
	input						CLK, nRST,			// main clock & reset
	output						BUSY,				// system is busy

	i_local_memory_control.s	local_memory,		// local memory control (LMB)
	i_thread_control.s			thread_command,		// thread control
	i_sync.m					core_sync,			// sync. of multi-core

	// Global scratch counters
	output	[`RANGE_MASK4D]		GSCs_RST,			// reset mask
	output	[`RANGE_MASK4D]		GSCs_INC,			// increase mask
	input	[`RANGE_WORDx4]		GSCs_DATA,			// data

	i_memory_command.m			mem_command			// memory command
);

// definition & assignment ---------------------------------------------------
// Instruction
wire							IF_nEN;						// instruction enable (active low)
wire	[`RANGE_PC]				IF_PC;						// input program counter
wire	[`RANGE_UINSTx4]		IF_UINSTx4;					// instruction 4 units

// GPRs read
wire							SRC0A_nEN, SRC0B_nEN,		// source enable (active low)
SRC1A_nEN, SRC1B_nEN;
wire	[`RANGE_GPRs]			SRC0A_ADDR, SRC0B_ADDR,		// GPRs source address
SRC1A_ADDR, SRC1B_ADDR;
wire	[`RANGE_DWORDx4]		SRC0A, SRC0B, SRC1A, SRC1B;	// GPRs source data

// GPRs write
wire							WB0_nEN, WB1_nEN;			// destination enable (active low)
wire	[`RANGE_GPRs]			WB0_ADDR, WB1_ADDR;			// destination address
wire	[`RANGE_MASK4D]			WB0_MASK, WB1_MASK;			// destination mask
wire	[`RANGE_DWORDx4]		WB0_DATA, WB1_DATA;			// destination write data

// PC operation @ output EX0
wire							PC_nEN;						// PC operation enable (active low)
wire	[`RANGE_BR]				PC_BOP;						// branch operation
wire	[`RANGE_PC]				PC_NEXT;					// next PC output
wire							PC_nWAIT;					// thread wait for memory transaction (active low)
wire	[`RANGE_TRDs]			PC_nAWAKE;					// threa awake to processing the thread

// Memory operation @ output EX0
wire	[`RANGE_MEMID]			MEM_nEN;					// operatio enable (active low)
wire							MEM_WRITE;					// write enable
wire	[`RANGE_WORD]			MEM_ADDR;					// memory address
wire	[`RANGE_GPRs]			MEM_SRC;					// source index
wire	[`RANGE_DWORDx4]		MEM_DATA_0, MEM_DATA_1;		// memory source data
wire							MEM_STALL;					// memory stall

// Thread management
wire	[`RANGE_TRDs]			nTRD;						// Thread enable (active low)
wire 							TRD_nEN;					// Thread enable
wire	[`RANGE_PC]				TRD_PC;						// Thread program counter
wire							TRD_BUSY;					// Thread busy

// Local memory block extended write
wire							LMB_EW_nEN;				// write enable (active low) - phase #0/#1 common signal
wire	[`RANGE_GPRs]			LMB_EW0_ADDR, LMB_EW1_ADDR;		// address
wire	[`RANGE_DWORDx4]		LMB_EW0_DATA, LMB_EW1_DATA;		// data

// Scratch counter
wire							SCs_EW1_nEN;				// write enable (active low)
wire	[`RANGE_MASK4D]			SCs_EW1_MASK;				// write mask
wire	[`RANGE_DWORDx4]		SCs_EW1_DATAx4;				// write data

// pack & unpack
wire							PU_EW1_nWE;					// write enabe (active low)
wire	[`RANGE_MASK4D]			PU_EW1_MASK;				// write mask
wire	[`RANGE_DWORDx4]		PU_EW1_DATA;				// write data

// Extended write
wire							EW0_nEN, EW1_nEN;			// write enable (active low)
wire	[`RANGE_MASK4D]			EW0_MASK, EW1_MASK;			// write mask
wire	[`RANGE_GPRs]			EW0_ADDR, EW1_ADDR;			// write address
wire	[`RANGE_DWORDx4]		EW0_DATA, EW1_DATA;			// write data

// external
wire	[`RANGE_TRDs]			STBs_nEN;					// enable (active low)
wire	[`RANGE_DWORDx4]		STBs_RDATA;					// read data
wire	[5:0]					STBs_VALID;					// valid current signal

assign	EW0_nEN			= LMB_EW_nEN;
assign	EW0_MASK		= 4'b0000;							// all non-mask
assign	EW0_ADDR		= LMB_EW0_ADDR;
assign	EW0_DATA		= LMB_EW0_DATA;
assign	BUSY			= TRD_BUSY;

// implementation ------------------------------------------------------------
// Core
MTSP_Core #(
	.CORE_ID		(CORE_ID)
) core_inst (
	.CLK			(CLK),
	.nRST			(nRST),
	// Instruction
	.IF_nEN			(IF_nEN),
	.IF_PC			(IF_PC),
	.IF_UINSTx4		(IF_UINSTx4),
	// GPRs read
	.SRC0A_nEN		(SRC0A_nEN),
	.SRC0A_ADDR		(SRC0A_ADDR),
	.SRC0A			(SRC0A),
	.SRC0B_nEN		(SRC0B_nEN),
	.SRC0B_ADDR		(SRC0B_ADDR),
	.SRC0B			(SRC0B),
	.SRC1A_nEN		(SRC1A_nEN),
	.SRC1A_ADDR		(SRC1A_ADDR),
	.SRC1A			(SRC1A),
	.SRC1B_nEN		(SRC1B_nEN),
	.SRC1B_ADDR		(SRC1B_ADDR),
	.SRC1B			(SRC1B),
	// GPRs write back
	.WB0_nEN		(WB0_nEN),
	.WB0_MASK		(WB0_MASK),
	.WB0_ADDR		(WB0_ADDR),
	.WB0_DATA		(WB0_DATA),
	.WB1_nEN		(WB1_nEN),
	.WB1_MASK		(WB1_MASK),
	.WB1_ADDR		(WB1_ADDR),
	.WB1_DATA		(WB1_DATA),
	// Extended write back
	.EW0_nEN		(EW0_nEN),
	.EW0_MASK		(EW0_MASK),
	.EW0_ADDR		(EW0_ADDR),
	.EW0_DATA		(EW0_DATA),
	.EW1_nEN		(EW1_nEN),
	.EW1_MASK		(EW1_MASK),
	.EW1_ADDR		(EW1_ADDR),
	.EW1_DATA		(EW1_DATA),
	// PC control
	.PC_nEN			(PC_nEN),
	.PC_BO			(PC_BOP),
	.PC_NEXT		(PC_NEXT),
	.thread_command	(thread_command),
	// Global scratch counters
	.GSCs_RST		(GSCs_RST),
	.GSCs_INC		(GSCs_INC),
	// Memory control
	.MEM_nEN		(MEM_nEN),
	.MEM_WRITE		(MEM_WRITE),
	.MEM_ADDR		(MEM_ADDR),
	.MEM_SRC		(MEM_SRC),
	.MEM_DATA_0		(MEM_DATA_0),
	.MEM_DATA_1		(MEM_DATA_1),
	.MEM_STALL		(MEM_STALL)
);

// Thread managements
MTSP_TRDs	TRDs_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.PC_nEN			((PC_nEN|MEM_STALL)),
	.PC_BOP			(PC_BOP),
	.PC_NEXT		(PC_NEXT),
	.PC_nWAIT		(PC_nWAIT),
	.PC_nAWAKE		(PC_nAWAKE),
	.nTRD			(nTRD),
	.TRD_nEN		(TRD_nEN),
	.TRD_PC			(TRD_PC),
	.TRD_BUSY		(TRD_BUSY),
	.core_sync		(core_sync)
);

// Instruction registers
MTSP_INSTx4	INST_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.TRD_nEN		(TRD_nEN),
	.TRD_PC			(TRD_PC),
	.IF_nEN			(IF_nEN),
	.IF_PC			(IF_PC),
	.IF_UINSTx4		(IF_UINSTx4)
);

// General purpose registers
MTSP_GPRs #(
	.CORE_ID		(CORE_ID)
) GPRs_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.nTRD			(nTRD),
	.SRC0A_nEN		(SRC0A_nEN),
	.SRC0A_ADDR		(SRC0A_ADDR),
	.SRC0A_DATA		(SRC0A),
	.SRC0B_nEN		(SRC0B_nEN),
	.SRC0B_ADDR		(SRC0B_ADDR),
	.SRC0B_DATA		(SRC0B),
	.SRC1A_nEN		(SRC1A_nEN),
	.SRC1A_ADDR		(SRC1A_ADDR),
	.SRC1A_DATA		(SRC1A),
	.SRC1B_nEN		(SRC1B_nEN),
	.SRC1B_ADDR		(SRC1B_ADDR),
	.SRC1B_DATA		(SRC1B),
	.WB0_nEN		(WB0_nEN),
	.WB0_WMASK		(WB0_MASK),
	.WB0_ADDR		(WB0_ADDR),
	.WB0_DATA		(WB0_DATA),
	.WB1_nEN		(WB1_nEN),
	.WB1_WMASK		(WB1_MASK),
	.WB1_ADDR		(WB1_ADDR),
	.WB1_DATA		(WB1_DATA)
);

// Local memory block
MTSP_LMB #(
	.CORE_ID		(CORE_ID)
) LMB_inst (
	.CLK			(CLK),
	.nRST			(nRST),
	.local_memory	(local_memory),
	.MEM_nEN		(MEM_nEN[`MEMID_LMB]),
	.MEM_WRITE		(MEM_WRITE),
	.MEM_ADDR		(MEM_ADDR[`RANGE_LMB]),
	.MEM_WDATA		({MEM_DATA_0, MEM_DATA_1}),
	.MEM_SRC		(MEM_SRC),
	.EW_nEN			(LMB_EW_nEN),
	.EW0_ADDR		(LMB_EW0_ADDR),
	.EW0_DATA		(LMB_EW0_DATA),
	.EW1_ADDR		(LMB_EW1_ADDR),
	.EW1_DATA		(LMB_EW1_DATA)
);

// Scratch counters
MTSP_SCs # (
	.CORE_ID		(CORE_ID)
) SCs_inst (
	.CLK			(CLK),
	.nRST			(nRST),
	.MEM_nEN		(MEM_nEN[`MEMID_SCRATCH]),
	.MEM_SEL		(MEM_WRITE),
	.MEM_OP			(MEM_ADDR),
	.MEM_DATA		(MEM_DATA_0),
	.GSCs_DATA		(GSCs_DATA),
	.EW1_nEN		(SCs_EW1_nEN),
	.EW1_MASK		(SCs_EW1_MASK),
	.EW1_DATA		(SCs_EW1_DATAx4)
);

// Stream buffer
MTSP_Bus	Bus_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.nTRD			({nTRD[(`MSB_TRDs-3):0], nTRD[`MSB_TRDs:(`MSB_TRDs-2)]}),
	.MEM_nEN		(MEM_nEN[`MEMID_STREAM]),
	.MEM_WRITE		(MEM_WRITE),
	.MEM_ADDR		(MEM_ADDR),
	.MEM_DATA		(MEM_DATA_0),
	.MEM_STALL		(MEM_STALL),
	.PC_nWAIT		(PC_nWAIT),
	.PC_nAWAKE		(PC_nAWAKE),
	.mem_command	(mem_command)
);

// pack & unpack
MTSP_PackUnpack	pu_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.MEM_nEN		(MEM_nEN[`MEMID_PACKUNPACK]),
	.MEM_WRITE		(MEM_WRITE),
	.MEM_ADDR		(MEM_ADDR),
	.MEM_DATA		(MEM_DATA_0),
	.EW1_nEN		(PU_EW1_nWE),
	.EW1_MASK		(PU_EW1_MASK),
	.EW1_DATA		(PU_EW1_DATA)
);

// extended write phase #1 de-mux
MTSP_EW1	ew1_inst(
	.CLK			(CLK),
	.nRST			(nRST),
	.MEM_SRC		(MEM_SRC),
	.LMB_nEN		(LMB_EW_nEN),
	.LMB_ADDR		(LMB_EW1_ADDR),
	.LMB_DATA		(LMB_EW1_DATA),
	.SCs_nEN		(SCs_EW1_nEN),
	.SCs_MASK		(SCs_EW1_MASK),
	.SCs_DATA		(SCs_EW1_DATAx4),
	.PU_nEN			(PU_EW1_nWE),
	.PU_MASK		(PU_EW1_MASK),
	.PU_DATA		(PU_EW1_DATA),
	.EW1_nEN		(EW1_nEN),
	.EW1_MASK		(EW1_MASK),
	.EW1_ADDR		(EW1_ADDR),
	.EW1_DATA		(EW1_DATA)
);

endmodule
