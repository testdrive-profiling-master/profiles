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

module MTSP_Core#(
	parameter	CORE_ID			= 0
) (
	// System
	input							CLK, nRST,					// main clock & reset (active low)
	// Instruction
	input							IF_nEN,						// instruction enable (active low)
	input	[`RANGE_PC]				IF_PC,						// input program counter
	input	[`RANGE_UINSTx4]		IF_UINSTx4,					// instruction 4 units
	// GPRs read
	output							SRC0A_nEN, SRC0B_nEN,		// source read enable (active low)
	SRC1A_nEN, SRC1B_nEN,
	output	[`RANGE_GPRs]			SRC0A_ADDR, SRC0B_ADDR,		// source address
	SRC1A_ADDR, SRC1B_ADDR,
	input	[`RANGE_DWORDx4]		SRC0A, SRC0B, SRC1A, SRC1B,	// source data
	// GPRs write
	output							WB0_nEN, WB1_nEN,			// destination write enable (active low)
	output	[`RANGE_MASK4D]			WB0_MASK, WB1_MASK,			// destination mask
	output	[`RANGE_GPRs]			WB0_ADDR, WB1_ADDR,			// destination extended address
	output	[`RANGE_DWORDx4]		WB0_DATA, WB1_DATA,			// destination write data
	// PC operation @ output EX0
	output							PC_nEN,						// PC operation enable (active low)
	output	[`RANGE_BR]				PC_BO,						// branch operation
	output	[`RANGE_PC]				PC_NEXT,					// next PC output
	// External thread control
	i_thread_control.s				thread_command,				// thread control
	// External global scratch counters
	output	[`RANGE_MASK4D]			GSCs_RST,					// reset mask
	output	[`RANGE_MASK4D]			GSCs_INC,					// increase mask
	// Memory operation @ output EX0
	output	[`RANGE_MEMID]			MEM_nEN,					// operatio enable (active low)
	output							MEM_WRITE,					// write enable
	output	[`RANGE_WORD]			MEM_ADDR,					// address
	output	[`RANGE_GPRs]			MEM_SRC,					// source index
	output	[`RANGE_DWORDx4]		MEM_DATA_0, MEM_DATA_1,		// source data
	input							MEM_STALL,					// memory stall
	// Extended write
	input							EW0_nEN, EW1_nEN,			// write enable (active low)
	input	[`RANGE_MASK4D]			EW0_MASK, EW1_MASK,			// write mask
	input	[`RANGE_GPRs]			EW0_ADDR, EW1_ADDR,			// write address
	input	[`RANGE_DWORDx4]		EW0_DATA, EW1_DATA			// write data
);

// definition & assignment ---------------------------------------------------
// pipeline wired signals
wire	[`RANGE_PC]				io_pc, of0_pc, of1_pc, of2_pc, id0_pc, id1_pc, id2_pc, id3_pc;
wire	[`RANGE_UINSTx4]		io_uinstx4, of0_uinstx4, of1_uinstx4, of2_uinstx4, id0_uinstx4, id1_uinstx4, id2_uinstx4, id3_uinstx4;
wire	[`RANGE_DWORDx4]		id0_src0a,
id1_src0a, id1_src0b,
id2_src0a, id2_src0b, id2_src1a,
id3_src0a, id3_src0b, id3_src1a, id3_src1b;
wire	[`RANGE_GPRs]			ref_addr_dest;
wire	[`RANGE_INDEX]			ref_addr_relative;
// execution signals
wire	[`RANGE_MODESC]			mo0_x, mo0_y, mo0_z, mo0_w,
mo1_x, mo1_y, mo1_z, mo1_w;
wire	[`RANGE_MASK4D]			mo0_mask, mo1_mask;
wire	[`RANGE_GPRs]			mo0_waddr, mo1_waddr;
wire	[`RANGE_DWORDx4]		dest1_data;
wire	[`RANGE_MASK4D]			dest1_wmask;
wire	[`RANGE_GPRs]			dest1_waddr;
wire	[`RANGE_GPRs]			ref_mem_src;

// implementation ------------------------------------------------------------
// instruction organize
MTSP_InstOrganize		IO_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.nEN				(IF_nEN),
	.PC					(IF_PC),
	.UINSTx4			(IF_UINSTx4),
	//-----------pipe-----------
	.PC_OUT				(io_pc),
	.UINSTx4_OUT		(io_uinstx4),
	.SRC0A_nEN			(SRC0A_nEN),
	.SRC0A_ADDR			(SRC0A_ADDR)
);

// operand fetch #0
MTSP_OF_0				OF_0_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(io_pc),
	.UINSTx4			(io_uinstx4),
	//-----------pipe-----------
	.PC_OUT				(of0_pc),
	.UINSTx4_OUT		(of0_uinstx4),
	.SRC0B_nEN			(SRC0B_nEN),
	.SRC0B_ADDR			(SRC0B_ADDR)
);

// operand fetch #1
MTSP_OF_1				OF_1_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(of0_pc),
	.UINSTx4			(of0_uinstx4),
	//-----------pipe-----------
	.PC_OUT				(of1_pc),
	.UINSTx4_OUT		(of1_uinstx4),
	.SRC1A_nEN			(SRC1A_nEN),
	.SRC1A_ADDR			(SRC1A_ADDR)
);

// operand fetch #2
MTSP_OF_2				OF_2_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(of1_pc),
	.UINSTx4			(of1_uinstx4),
	//-----------pipe-----------
	.PC_OUT				(of2_pc),
	.UINSTx4_OUT		(of2_uinstx4),
	.SRC1B_nEN			(SRC1B_nEN),
	.SRC1B_ADDR			(SRC1B_ADDR)
);

// instruction decode #0
MTSP_ID_0				ID_0_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(of2_pc),
	.UINSTx4			(of2_uinstx4),
	.SRC0A				(SRC0A),
	//-----------pipe-----------
	.PC_OUT				(id0_pc),
	.UINSTx4_OUT		(id0_uinstx4),
	.SRC0A_OUT			(id0_src0a)
);

// instruction decode #1
MTSP_ID_1				ID_1_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(id0_pc),
	.UINSTx4			(id0_uinstx4),
	.SRC0A				(id0_src0a),
	.SRC0B				(SRC0B),
	//-----------pipe-----------
	.PC_OUT				(id1_pc),
	.UINSTx4_OUT		(id1_uinstx4),
	.SRC0A_OUT			(id1_src0a),
	.SRC0B_OUT			(id1_src0b)
);

// instruction decode #2
MTSP_ID_2				ID_2_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(id1_pc),
	.UINSTx4			(id1_uinstx4),
	.SRC0A				(id1_src0a),
	.SRC0B				(id1_src0b),
	.SRC1A				(SRC1A),
	//-----------pipe-----------
	.PC_OUT				(id2_pc),
	.UINSTx4_OUT		(id2_uinstx4),
	.SRC0A_OUT			(id2_src0a),
	.SRC0B_OUT			(id2_src0b),
	.SRC1A_OUT			(id2_src1a),
	.REF_ADDR_DEST		(ref_addr_dest),
	.REF_ADDR_RELATIVE	(ref_addr_relative),
	.GSCs_RST			(GSCs_RST),
	.GSCs_INC			(GSCs_INC)
);

// instruction decode #3
MTSP_ID_3				ID_3_inst (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(id2_pc),
	.UINSTx4			(id2_uinstx4),
	.SRC0A				(id2_src0a),
	.SRC0B				(id2_src0b),
	.SRC1A				(id2_src1a),
	.SRC1B				(SRC1B),
	.REF_ADDR_DEST		(ref_addr_dest),
	//          -wire-
	.PC_OUT				(id3_pc),
	.UINSTx4_OUT		(id3_uinstx4),
	.REF_MEM_SRC		(ref_mem_src),
	.SRC0A_OUT			(id3_src0a),
	.SRC0B_OUT			(id3_src0b),
	.SRC1A_OUT			(id3_src1a),
	.SRC1B_OUT			(id3_src1b),
	.MO0_X				(mo0_x),
	.MO0_Y				(mo0_y),
	.MO0_Z				(mo0_z),
	.MO0_W				(mo0_w),
	.MO1_X				(mo1_x),
	.MO1_Y				(mo1_y),
	.MO1_Z				(mo1_z),
	.MO1_W				(mo1_w),
	.MO0_MASK			(mo0_mask),
	.MO1_MASK			(mo1_mask),
	.MO0_WADDR			(mo0_waddr),
	.MO1_WADDR			(mo1_waddr)
);

//---------------- Multi-Execution pipeline part ---------------------
// Branch & Memory operation
MTSP_EXT				EXT (
	.CLK				(CLK),
	.nRST				(nRST),
	.nEN_BRC			(id3_uinstx4[`RANGE_UINSTx4_0_nEN] & id3_uinstx4[`RANGE_UINSTx4_2_nEN]),
	.PC					(id3_pc),
	.UINST				(id3_uinstx4[`RANGE_UINSTx4_2]),
	.REF_ADDR_DEST		(ref_addr_dest),
	.REF_ADDR_RELATIVE	(ref_addr_relative),
	.REF_MEM_SRC		(ref_mem_src),
	.SRC0				(id3_src1a),
	.SRC1				(id3_src1b),
	.thread_command		(thread_command),
	.PC_nEN				(PC_nEN),
	.PC_BO				(PC_BO),
	.PC_NEXT			(PC_NEXT),
	.MEM_nEN			(MEM_nEN),
	.MEM_WRITE			(MEM_WRITE),
	.MEM_ADDR			(MEM_ADDR),
	.MEM_SRC			(MEM_SRC),
	.MEM_DATA_0			(MEM_DATA_0),
	.MEM_DATA_1			(MEM_DATA_1)
);

// Execution
MTSP_EX					EX (
	.CLK				(CLK),
	.nRST				(nRST),
	.PC					(id3_pc),
	.MEM_STALL			(MEM_STALL),
	.MO0_X				(mo0_x),
	.MO0_Y				(mo0_y),
	.MO0_Z				(mo0_z),
	.MO0_W				(mo0_w),
	.MO1_X				(mo1_x),
	.MO1_Y				(mo1_y),
	.MO1_Z				(mo1_z),
	.MO1_W				(mo1_w),
	.MO0_MASK			(mo0_mask),
	.MO1_MASK			(mo1_mask),
	.MO0_WADDR			(mo0_waddr),
	.MO1_WADDR			(mo1_waddr),
	.SRC0A				(id3_src0a),
	.SRC0B				(id3_src0b),
	.SRC1A				(id3_src1a),
	.SRC1B				(id3_src1b),
	.EW0_nEN			(EW0_nEN),
	.EW0_MASK			(EW0_MASK),
	.EW0_ADDR			(EW0_ADDR),
	.EW0_DATA			(EW0_DATA),
	.WB0_nEN			(WB0_nEN),
	.WB0_DATA			(WB0_DATA),
	.WB1_DATA			(dest1_data),
	.WB0_MASK			(WB0_MASK),
	.WB1_MASK			(dest1_wmask),
	.WB0_ADDR			(WB0_ADDR),
	.WB1_ADDR			(dest1_waddr)
);

// Write back #1 & #2
MTSP_WB					WB (
	.CLK				(CLK),
	.nRST				(nRST),
	.DEST1				(dest1_data),
	.WMASK1				(dest1_wmask),
	.WADDR1				(dest1_waddr),
	.EW1_nEN			(EW1_nEN),
	.EW1_MASK			(EW1_MASK),
	.EW1_ADDR			(EW1_ADDR),
	.EW1_DATA			(EW1_DATA),
	.WB1_nEN			(WB1_nEN),
	.WB1_MASK			(WB1_MASK),
	.WB1_ADDR			(WB1_ADDR),
	.WB1_DATA			(WB1_DATA)
);

endmodule
