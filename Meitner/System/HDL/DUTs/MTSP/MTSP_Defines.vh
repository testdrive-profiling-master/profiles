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
`ifndef __MTSP_DEFINES__				// rid of Warnning (Duplicated definition)
//----------------------------------------------------------------------------
`define __MTSP_DEFINES__
`include "system_defines.vh"
	
// Processor Configuration ---------------------------------------------------
`ifndef MEITNER_GPRs_SIZE
`define	MEITNER_GPRs_SIZE		128		// max 128 entries
`endif
	
// Core Descriptions ---------------------------------------------------------
// *Modifiable  Data & BUS Constant definitions
`define EXP_BIAS				63		// Exponent bias
`define	SIZE_SIGN				1		// Sign size
`define SIZE_EXP				7		// Exponent size
`define SIZE_FRACTION			16		// Fraction size
`define DATA_ZERO				24'b0_0000000_0000000000000000	// float point '0'
`define DATA_ONE				24'b0_0111111_0000000000000000	// float point '1'
`define DATA_nONE				24'b1_0111111_0000000000000000	// float point '-1'
`define	DATA_MAX				24'b0_1111111_1111111111111111	// max value
`define	DATA_MIN				24'b1_1111111_1111111111111111	// min value
`define	DATA_OVER				23'b1111111_1111111111111111	// over baundary
`define	DATA_ALLZ				24'bZ_ZZZZZZZ_ZZZZZZZZZZZZZZZZ	// 
`define	DATA4D_ALLZ				{`DATA_ALLZ, `DATA_ALLZ, `DATA_ALLZ, `DATA_ALLZ}
// Macro definitions below from here...
`define SIZE_DATA				(1 + `SIZE_EXP + `SIZE_FRACTION)
`define	SIZE_DATA4D				(`SIZE_DATA*4)
`define	FRAC_BIAS				(`EXP_BIAS-1)		// 소수를 기점으로 한 바이어스

// LSB
`define LSB_DATA				0
`define LSB_DATA_EXP			`SIZE_FRACTION
`define LSB_DATA4D				0
`define	LSB_DATA4D_X			(`SIZE_DATA*3)
`define	LSB_DATA4D_Y			(`SIZE_DATA*2)
`define	LSB_DATA4D_Z			(`SIZE_DATA)
`define	LSB_DATA4D_W			0

// MSB
`define MSB_EXP					`SIZE_EXP - 1
`define MSB_FRACTION			(`SIZE_FRACTION - 1)
`define MSB_DATA				(`SIZE_DATA - 1)
`define MSB_DATA_EXP			(`MSB_EXP +`LSB_DATA_EXP)
`define MSB_DATA_FRACTION		`MSB_FRACTION
`define MSB_DATA4D				(`SIZE_DATA4D-1)
`define	MSB_DATA4D_X			(`LSB_DATA4D_X + `MSB_DATA)
`define	MSB_DATA4D_Y			(`LSB_DATA4D_Y + `MSB_DATA)
`define	MSB_DATA4D_Z			(`LSB_DATA4D_Z + `MSB_DATA)
`define	MSB_DATA4D_W			(`LSB_DATA4D_W + `MSB_DATA)

// Range
`define	RANGE_EXP				`MSB_EXP:0
`define	RANGE_FRACTION			`MSB_FRACTION:0
`define	RANGE_DATA				`MSB_DATA:0
`define	RANGE_DATA_SIGN			`MSB_DATA
`define	RANGE_DATA_EXP			`MSB_DATA_EXP:`LSB_DATA_EXP
`define	RANGE_DATA_FRACTION		`MSB_FRACTION:0

/*
`define	RANGE_DATA4D			`MSB_DATA4D:0
`define	RANGE_DATA4D_X			`MSB_DATA4D_X:`LSB_DATA4D_X
`define	RANGE_DATA4D_Y			`MSB_DATA4D_Y:`LSB_DATA4D_Y
`define	RANGE_DATA4D_Z			`MSB_DATA4D_Z:`LSB_DATA4D_Z
`define	RANGE_DATA4D_W			`MSB_DATA4D_W:`LSB_DATA4D_W
`define	RANGE_DATA4D_X_NO_SIGN	(`MSB_DATA4D_X-1):`LSB_DATA4D_X
`define	RANGE_DATA4D_Y_NO_SIGN	(`MSB_DATA4D_Y-1):`LSB_DATA4D_Y
`define	RANGE_DATA4D_Z_NO_SIGN	(`MSB_DATA4D_Z-1):`LSB_DATA4D_Z
`define	RANGE_DATA4D_W_NO_SIGN	(`MSB_DATA4D_W-1):`LSB_DATA4D_W
`define	RANGE_DATA4D_X_EXP		(`MSB_DATA4D_X-1):(`MSB_DATA4D_X-`SIZE_EXP)
`define	RANGE_DATA4D_Y_EXP		(`MSB_DATA4D_Y-1):(`MSB_DATA4D_Y-`SIZE_EXP)
`define	RANGE_DATA4D_Z_EXP		(`MSB_DATA4D_Z-1):(`MSB_DATA4D_Z-`SIZE_EXP)
`define	RANGE_DATA4D_W_EXP		(`MSB_DATA4D_W-1):(`MSB_DATA4D_W-`SIZE_EXP)
`define RANGE_DATA4D_XY			`MSB_DATA4D_X:`LSB_DATA4D_Y
`define RANGE_DATA4D_XYZ		`MSB_DATA4D_X:`LSB_DATA4D_Z
*/

`define SIGN_DATA				`MSB_DATA

//----------------------------------------------------------------------------
// Threads
`define	SIZE_TRDs				12
`define MSB_TRDs				(`SIZE_TRDs-1)
`define RANGE_TRDs				`MSB_TRDs:0
`define DEFUALT_TRDs			12'b1111_1111_1110
`define RESET_TRDs				12'b1111_1111_1111
//----------------------------------------------------------------------------
// GPRs (General Purpose Registers)
`define	SIZE_GPRs				7							// max 7bits (128 entry)
`define COUNT_GPRs				(2**`SIZE_GPRs)
`define	RANGE_GPRs				(`SIZE_GPRs-1):0
//----------------------------------------------------------------------------
// LMB (Local memory block)
`define SIZE_LMB				11
`define COUNT_LMB				(2**`SIZE_LMB)				// 2048
`define RANGE_LMB				(`SIZE_LMB-1):0				// [10:0]
//----------------------------------------------------------------------------
// Program Counter
`define SIZE_PC					30
`define	RANGE_PC				(`SIZE_PC-1):0				// 32bit aligned
//----------------------------------------------------------------------------
// Common index : maximum range of GPRs/GLBs/Cache LUT
`define SIZE_INDEX				12
`define RANGE_INDEX				(`SIZE_INDEX-1):0
//----------------------------------------------------------------------------
// Branch operation
`define	SIZE_BR					4
`define	RANGE_BR				(`SIZE_BR-1):0
`define	BR_DEFAULT				4'd0	// BRC instruction default set
//----------------------------------------------------------------------------
// Branch operation
`define	SIZE_MEMOP				26
`define	RANGE_MEMOP				(`SIZE_MEMOP-1):0
`define	RANGE_MEMOP_ADDR		15:0
`define	RANGE_MEMOP_SRC			20:16
`define	RANGE_MEMOP_DEST		`RANGE_MEMOP_SRC
`define	RANGE_MEMOP_WRITE		21
`define	RANGE_MEMOP_SIZE		23:22
`define	RANGE_MEMOP_BYTECONV	24
`define	RANGE_MEMOP_USE_MAP		25
`define	RANGE_MEMOP_CS			`RANGE_MEMOP_SIZE
`define	RANGE_MEMOP_MID			20:17
`define	RANGE_MEMOP_STRIDER		16
//----------------------------------------------------------------------------
// Pipelines enum
`define	PIPE_ID_0				0
`define	PIPE_ID_1				(`PIPE_ID_0		+1)
`define	PIPE_SM					(`PIPE_ID_1		+1)
`define	PIPE_EX_0				(`PIPE_SM		+1)
`define	PIPE_EX_1				(`PIPE_EX_0		+1)
`define	PIPE_EX_2				(`PIPE_EX_1		+1)
`define	PIPE_WB_0				(`PIPE_EX_2		+1)
`define	PIPE_WB_1				(`PIPE_WB_0		+1)
`define	PIPE_SIZE				(`PIPE_WB_1		+1)
//----------------------------------------------------------------------------
// Memory operation ID
`define MEMID_nRESET			4'b1111		// (active low)
`define	MEMID_LMB				0			// local memory block
`define	MEMID_SCRATCH			1			// scratch counters
`define MEMID_STREAM			2			// stream operation
`define MEMID_PACKUNPACK		3			// pack & unpack
`define RANGE_MEMID				3:0

//----------------------------------------------------------------------------
// Scratch counter operations
`define SC_GET					2'd0		// get
`define SC_GETnINC				2'd1		// get & increase
`define SC_SET					2'd2		// set
`define SC_SET_LIMIT			2'd3		// set limit
//----------------------------------------------------------------------------
// Unit Instruction Definition
//  31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
// Arithmetic main/sub
// [E ][P ][MO            ][DEST/SRC          ][SRC               ][Al][Sl][Sa][Ne][WMASK         ][SelX  ] [SelY ] [SelZ ] [SelW ]
//         [MO            ][MO_MASK       ][SW][SRC               ][Al][Sl][Sa][Ne][STBs-A][STBs-B][SelX  ] [SelY ] [SelZ ] [SelW ]
// Process
//         [MO            ][EX_OP         ][SW][SRC               ][ADDR                                                          ]
// Coordinate
//         [MO            ][MO_MASK       ][SW][SRC               ][Option                                                        ]
//--- internal expression
// [nE]    [MO_ID]
`define	SIZE_UINST				32
`define	SIZE_UINSTx2			(`SIZE_UINST*2)
`define	SIZE_UINSTx4			(`SIZE_UINST*4)
`define	RANGE_UINST				(`SIZE_UINST-1):0
`define	RANGE_UINSTx2			(`SIZE_UINSTx2-1):0
`define	RANGE_UINSTx4			(`SIZE_UINSTx4-1):0
`define	RANGE_UINSTx4_0			(`SIZE_UINST*4-1):(`SIZE_UINST*3)
`define	RANGE_UINSTx4_1			(`SIZE_UINST*3-1):(`SIZE_UINST*2)
`define	RANGE_UINSTx4_2			(`SIZE_UINST*2-1):(`SIZE_UINST)
`define	RANGE_UINSTx4_3			(`SIZE_UINST  -1):0
`define RANGE_UINSTx4_0_nEN		(`SIZE_UINST*4-1)
`define RANGE_UINSTx4_1_nEN		(`SIZE_UINST*3-1)
`define RANGE_UINSTx4_2_nEN		(`SIZE_UINST*2-1)
`define RANGE_UINSTx4_3_nEN		(`SIZE_UINST  -1)

`define	RANGE_UINST_END			31
`define	RANGE_UINST_PHASE		30
`define	RANGE_UINST_MO			29:26
`define RANGE_UINST_MO_ALU		29:28
`define	RANGE_UINST_DEST		25:21
`define	RANGE_UINST_MO_MASK		25:22
`define	RANGE_UINST_OP			25:22
`define	RANGE_UINST_RELATIVE	25
`define	RANGE_UINST_SRC			20:16
`define	RANGE_UINST_ALTERNATE	15
`define	RANGE_UINST_SELECT		14
`define	RANGE_UINST_MODIFIER	13
`define	RANGE_UINST_NEGATE		12
`define	RANGE_UINST_WMASK		11:8
`define	RANGE_UINST_SI_A		11:10
`define	RANGE_UINST_SI_B		9 : 8
`define	RANGE_UINST_SWZ4D		7:0
`define	RANGE_UINST_SWZ4D_X		7:6
`define	RANGE_UINST_SWZ4D_Y		5:4
`define	RANGE_UINST_SWZ4D_Z		3:2
`define	RANGE_UINST_SWZ4D_W		1:0
`define	RANGE_UINST_EX_OP		25:22
`define	RANGE_UINST_SWITCH		21
`define	RANGE_UINST_ADDRESS		15:0
`define	RANGE_UINST_OPTION		15:0
`define	RANGE_UINST_CMPX		15:12
`define	RANGE_UINST_CMPY		11: 8
`define	RANGE_UINST_CMPZ		7 : 4
`define	RANGE_UINST_CMPW		3 : 0

// instruction properties
`define RANGE_UINST_nEN			31
`define	RANGE_UINST_nALU		30

`define	RANGE_MO				3:0

`define	MO_MOV					4'b0000		// A(0)
`define	MO_ADD					4'b0001		// A(0)
`define	MO_MUL					4'b0010		// A(0)
`define	MO_CMP					4'b0011		// A(0)
`define	MO_RCP					4'b0100		// A(0)
`define	MO_RSQ					4'b0100		// A(1)
`define	MO_LOG					4'b0101		// A(0)
`define	MO_EXP					4'b0101		// A(1)
`define	MO_FLR					4'b0110		// A(0)
`define	MO_FRC					4'b0110		// A(1)
`define	MO_CONV					4'b0111		// A(1)
`define	MO_AND					4'b1000		// A(0)
`define	MO_OR					4'b1000		// A(1)
`define	MO_XOR					4'b1001		// A(0)
`define	MO_BA					4'b1010		// A(0)
`define	MO_FMT					4'b1010		// A(1)

`define MO_FLFR					4'b0110
`define MO_BW					3'b100		// logical operation : 100X
//-- phase #0 only
`define	MO_PRED					4'b1100
`define	MO_ADDR					4'b1101
`define	MO_MVS					4'b1110
//-- phase #1 only
`define	MO_BRC					4'b1110
`define	MO_MEM					4'b1111

`define	RANGE_SRCOP				2:0
`define	RANGE_SRCOP_SELECT		2
`define	RANGE_SRCOP_MODIFIER	1
`define	RANGE_SRCOP_NEGATE		0

//----------------------------------------------------------------------------
// MO description
//----------------------------------------------------------------------------
// External mo input
`define	MSB_MODESC_MO			3
`define	LSB_MODESC_MO			0
`define	RANGE_MODESC			6:0
`define	RANGE_MODESC_nEN		6
`define	RANGE_MODESC_ALT		5
`define	RANGE_MODESC_SEL		4
`define	RANGE_MODESC_MO			`MSB_MODESC_MO:`LSB_MODESC_MO

// Internal mo bypass
`define	SIZE_MOEXT				4
`define	RAMGE_MOEXT				3:0
`define	RANGE_MOEXT_EN			3
`define	RANGE_MOEXT_PHASE		2
`define	RANGE_MOEXT_ALT			1
`define	RANGE_MOEXT_SEL			0
`define	RANGE_MOEXT_OP			1:0
`define	RANGE_MOEXT_NONE		4'b0

//----------------------------------------------------------------------------
// Standard definition
//----------------------------------------------------------------------------
`define MASKED				`TRUE	// operation masked
`define UNMASKED			`FALSE  // operation unmasked

//----------------------------------------------------------------------------
// Selection & Swizzling & data
`define	SELECT_0				0
`define	SELECT_1				1
`define	SELECT_2				2
`define	SELECT_3				3
`define	SELECT_X				`SELECT_3
`define	SELECT_Y				`SELECT_2
`define	SELECT_Z				`SELECT_1
`define	SELECT_W				`SELECT_0
`define	SELECT_R				`SELECT_X
`define	SELECT_G				`SELECT_Y
`define	SELECT_B				`SELECT_Z
`define	SELECT_A				`SELECT_W
`define DATA_X					2'd0
`define DATA_Y					2'd1
`define DATA_Z					2'd2
`define DATA_W					2'd3

`define	RANGE_SEL1D				1:0
`define	RANGE_SEL4D				7:0
`define	RANGE_SEL4D_X			7:6
`define	RANGE_SEL4D_Y			5:4
`define	RANGE_SEL4D_Z			3:2
`define	RANGE_SEL4D_W			1:0
`define	RANGE_SWZ1D				`RANGE_SEL1D
`define	RANGE_SWZ4D				`RANGE_SEL4D
`define	RANGE_SWZ4D_X			`RANGE_SEL4D_X
`define	RANGE_SWZ4D_Y			`RANGE_SEL4D_Y
`define	RANGE_SWZ4D_Z			`RANGE_SEL4D_Z
`define	RANGE_SWZ4D_W			`RANGE_SEL4D_W
//----------------------------------------------------------------------------
// Default data types
`define	SIZE_MASK4D				4
`define	SIZE_BYTE				8
`define	SIZE_WORD				16
`define	SIZE_DWORD				32
`define	SIZE_QWORD				64
`define	SIZE_OWORD				128
// MSB
`define	MSB_BYTE				(`SIZE_BYTE-1)
`define	MSB_WORD				(`SIZE_WORD-1)
`define MSB_DWORD				(`SIZE_DWORD-1)
`define	MSB_QWORD				(`SIZE_QWORD-1)
`define	MSB_OWORD				(`SIZE_OWORD-1)
`define RANGE_MASK4D			(`SIZE_MASK4D-1) : 0	// Write Mask Bits (W/Z/Y/X)
`define RANGE_MASK4D_0			3
`define RANGE_MASK4D_1			2
`define RANGE_MASK4D_2			1
`define RANGE_MASK4D_3			0
`define RANGE_MASK4D_X			`RANGE_MASK4D_0
`define RANGE_MASK4D_Y			`RANGE_MASK4D_1
`define RANGE_MASK4D_Z			`RANGE_MASK4D_2
`define RANGE_MASK4D_W			`RANGE_MASK4D_3
`define	RANGE_QWORD				`MSB_QWORD	: 0	// Quard Word : 64 bits
`define	RANGE_OWORD				`MSB_OWORD	: 0	// Octal Word : 128 bits
`define	RANGE_WORD_H			15: 8	// high byte in word
`define	RANGE_WORD_L			7 : 0	// low byte in word

`define LSB_DWORDx4				0
`define	LSB_DWORDx4_X			(`SIZE_DWORD*3)
`define	LSB_DWORDx4_Y			(`SIZE_DWORD*2)
`define	LSB_DWORDx4_Z			(`SIZE_DWORD)
`define	LSB_DWORDx4_W			0

`define SIZE_DWORDx4			128
`define	RANGE_DWORDx4			127:0
`define	RANGE_DWORDx4_0			127:96
`define	RANGE_DWORDx4_0H		127:112
`define	RANGE_DWORDx4_0L		111:96
`define	RANGE_DWORDx4_1			95:64
`define	RANGE_DWORDx4_1H		95:80
`define	RANGE_DWORDx4_1L		79:64
`define	RANGE_DWORDx4_2			63:32
`define	RANGE_DWORDx4_2H		63:48
`define	RANGE_DWORDx4_2L		47:32
`define	RANGE_DWORDx4_3			31:0
`define	RANGE_DWORDx4_3H		31:16
`define	RANGE_DWORDx4_3L		15:0
`define	RANGE_DWORDx4_X			`RANGE_DWORDx4_0
`define	RANGE_DWORDx4_XH		`RANGE_DWORDx4_0H
`define	RANGE_DWORDx4_XL		`RANGE_DWORDx4_0L
`define	RANGE_DWORDx4_Y			`RANGE_DWORDx4_1
`define	RANGE_DWORDx4_YH		`RANGE_DWORDx4_1H
`define	RANGE_DWORDx4_YL		`RANGE_DWORDx4_1L
`define	RANGE_DWORDx4_Z			`RANGE_DWORDx4_2
`define	RANGE_DWORDx4_ZH		`RANGE_DWORDx4_2H
`define	RANGE_DWORDx4_ZL		`RANGE_DWORDx4_2L
`define	RANGE_DWORDx4_W			`RANGE_DWORDx4_3
`define	RANGE_DWORDx4_WH		`RANGE_DWORDx4_3H
`define	RANGE_DWORDx4_WL		`RANGE_DWORDx4_3L
`define SIZE_DWORDx8			256
`define	RANGE_DWORDx8			255:0
`define RANGE_DWORDx8_1			255:128
`define RANGE_DWORDx8_0			127:0

`define RANGE_HWORD				3:0			// half word : 4bit
`define RANGE_HWORDx3			11:0		// half word x3 : 12bit
`define RANGE_DWORD_R			31:24
`define RANGE_DWORD_G			23:16
`define RANGE_DWORD_B			15:8
`define RANGE_DWORD_A			7:0

`define SIZE_WORDx4				64
`define	RANGE_WORDx4			(`SIZE_WORDx4-1):0
`define RANGE_WORDx4_0			63:48
`define RANGE_WORDx4_1			47:32
`define RANGE_WORDx4_2			31:16
`define RANGE_WORDx4_3			15:0
`define RANGE_WORDx4_X			`RANGE_WORDx4_0
`define RANGE_WORDx4_Y			`RANGE_WORDx4_1
`define RANGE_WORDx4_Z			`RANGE_WORDx4_2
`define RANGE_WORDx4_W			`RANGE_WORDx4_3

`define SIZE_HBYTEx4			16
`define	RANGE_HBYTEx4			(`SIZE_HBYTEx4-1):0
`define	RANGE_HBYTEx4_0			15:12
`define	RANGE_HBYTEx4_1			11:8
`define	RANGE_HBYTEx4_2			7:4
`define	RANGE_HBYTEx4_3			3:0
`define	RANGE_HBYTEx4_X			`RANGE_HBYTEx4_0
`define	RANGE_HBYTEx4_Y			`RANGE_HBYTEx4_1
`define	RANGE_HBYTEx4_Z			`RANGE_HBYTEx4_2
`define	RANGE_HBYTEx4_W			`RANGE_HBYTEx4_3

// Default data range
`define	RANGE_BYTE_SHIFT(n)		(7+n):n
`define	RANGE_WORD_SHIFT(n)		(15+n):n
`define	RANGE_DWORD_SHIFT(n)	(31+n):n

//----------------------------------------------------------------------------
`define SIZE_STATUS_THREAD		2
`define RANGE_STATUS_THREAD		1:0
`define STATUS_THREAD_DISABLED	2'b00
`define STATUS_THREAD_RESERVED	2'b01
`define STATUS_THREAD_WAIT		2'b10
`define STATUS_THREAD_ENABLED	2'b11
//----------------------------------------------------------------------------
// Selection/Mask Bits : Maskable bits setting of Vector components
`define	SEL4D_0					4'b1000
`define	SEL4D_1					4'b0100
`define	SEL4D_2					4'b0010
`define	SEL4D_3					4'b0001
`define	SEL4D_ALL				4'b1111
`define	SEL4D_NONE				4'b0000

`define MASK4D_X				`SEL4D_0
`define MASK4D_Y				`SEL4D_1
`define MASK4D_Z				`SEL4D_2
`define MASK4D_W				`SEL4D_3
`define MASK4D_R				`SEL4D_0
`define MASK4D_G				`SEL4D_1
`define MASK4D_B				`SEL4D_2
`define MASK4D_A				`SEL4D_3
`define	MASK4D_ALLMASKED		`SEL4D_ALL
`define	MASK4D_ALLUNMASKED		`SEL4D_NONE

//----------------------------------------------------------------------------
// color ranges
`define	RANGE_RGBA				`RANGE_DWORD
`define	RANGE_RGBA_R			31:24
`define	RANGE_RGBA_G			23:16
`define	RANGE_RGBA_B			15: 8
`define	RANGE_RGBA_A			7 : 0

// depth/stencil ranges
`define	RANGE_DnS				`RANGE_DWORD		// Depth and stencil
`define	RANGE_DEPTH				`RANGE_WORD			// Depth : 16 bits
`define	RANGE_STENCIL			`RANGE_BYTE			// stencil : 8 bits
`define	RANGE_DnS_DEPTH			31:16				// Depth in DnS
`define	RANGE_DnS_STENCIL		15: 8				// Stencil in DnS
`define	RANGE_DnS_WRITE_TRUE	7					// Write true
`define	RANGE_DnS_DEPTH_TRUE	6					// Depth true
`define	RANGE_DnS_STENCIL_TRUE	5					// Stencil true
`define	RANGE_DnS_ETC			4 : 0				// etc. in DnS

//----------------------------------------------------------------------------
// Compare
/*`define	RANGE_COMPARE			2:0
//   0	 : Never
//   1	 : Less
//   2	 : Equal
//   3	 : Less or Equal
//   4	 : Greater
//   5	 : Not Equal
//   6	 : Greater or Equal
//   7	 : Always
`define	COMPARE_NEVER			3'b000
`define	COMPARE_LESS			3'b001
`define	COMPARE_EQUAL			3'b010
`define	COMPARE_LESS_EQUAL		3'b011
`define	COMPARE_GREATER			3'b100
`define	COMPARE_NOT_EQUAL		3'b101
`define	COMPARE_GREATER_EQUAL	3'b110
`define	COMPARE_ALWAYS			3'b111
*/
`define	RANGE_CMP				3:0
//  [0] src0==src1, [1] src0!=src1, [2] src0>src1, [3] src0<src1
// 	0000 : Never
// 	0001 : EQ(Equal)
// 	0010 : NE(Not Equal)
// 	0101 : GE(Greater than or Equal)
// 	0100 : GT(Greater Than)
// 	1001 : LE(Less than or Equal)
// 	1000 : LT(Less Than)
// 	1111 : Always
`define	CMP_NEVER				4'b0000
`define	CMP_EQ					4'b0001
`define	CMP_NE					4'b0010
`define	CMP_GE					4'b0101
`define	CMP_GT					4'b0100
`define	CMP_LE					4'b1001
`define	CMP_LT					4'b1000
`define	CMP_AL					4'b1111

// ---------------------------------------------------------------------------
// Multi-core
//@TODO : MULTICORE_XX 지워져야 한다.
/*
// 16 cores
`define	MULTICORE_16										// "MULTICORE_#"에서 #는 반드시 COUNT_MULTICORE 값과 일치시킨다.
*/
/*
// 8 cores
`define	MULTICORE_8											// "MULTICORE_#"에서 #는 반드시 COUNT_MULTICORE 값과 일치시킨다.
*/
// 4 cores
`define	MULTICORE_4											// "MULTICORE_#"에서 #는 반드시 COUNT_MULTICORE 값과 일치시킨다.

// ---------------------------------------------------------------------------
// Cache controller
`define	SIZE_CACHE_LUT			11
`define	SIZE_CACHE_PADDR		(32 - 4)
`define	RANGE_CACHE_LUT			(`SIZE_CACHE_LUT-1):0
`define	RANGE_CACHE_PADDR		(`SIZE_CACHE_PADDR-1):0
// Global memory block
`define	SIZE_GMB				13
`define COUNT_GMB				(2**`SIZE_GMB)
`define	RANGE_GMB				(`SIZE_GMB-1):0
	
// ---------------------------------------------------------------------------
// interfaces

interface i_thread_control;
	logic						en;			// enable
	logic						op;			// operation end(0), new(1)
	logic	[`RANGE_PC]			pc;			// program counter

	modport s	(input  en, op, pc);
	modport m	(output en, op, pc);
endinterface

interface i_bus_slave;
	logic						en;			// enable
	logic						we;			// write enable
	logic	[3:0]				addr;		// address
	logic	[`RANGE_DWORD]		wdata;		// write data
	logic	[`RANGE_DWORD]		rdata;		// read data
	logic						ready;		// bus is ready

	modport s	(input en, we, addr, wdata, output rdata, ready);
	modport m	(output en, we, addr, wdata, input rdata, ready);
endinterface

interface i_bus_master;
	logic						rreq;			// read request
	logic	[`RANGE_DWORD]		raddr;			// read address
	logic	[`RANGE_BYTE]		rsize;			// read size
	logic						rgrant;			// read grant
	// read data
	logic	[`RANGE_DWORDx8]	rdata;			// read data
	logic						rvalid;			// read validation
	logic						rready;			// read ready
	logic						rlast;			// read last
	// write address
	logic						wreq;			// write request
	logic	[`RANGE_DWORD]		waddr;			// write address
	logic	[`RANGE_BYTE]		wsize;			// write size
	logic						wgrant;			// write grant
	// write data
	logic	[`RANGE_DWORDx8]	wdata;			// write data
	logic						wvalid;			// write validation
	logic						wready;			// write ready
	logic						wlast;			// write last

	modport s	(input rreq, raddr, rsize, rready, wreq, waddr, wsize, wdata, wvalid, output rgrant, rvalid, wgrant, rdata, wready, rlast, wlast);
	modport m	(output rreq, raddr, rsize, rready, wreq, waddr, wsize, wdata, wvalid, input rgrant, rvalid, wgrant, rdata, wready, rlast, wlast);
endinterface

interface i_memory_command;
	logic						en;			// enable
	logic	[`RANGE_DWORDx4]	desc;		// description (x[INDEX:ID], y[C-SIZE:SIZE:STRIDE], z[P-BASE], w[E:L-BASE:C-BASE])
	logic						valid;		// valid operation

	modport s	(input en, desc, output valid);
	modport m	(output en, desc, input valid);
endinterface

interface i_local_memory_control;
	logic							en;				// enable
	logic							we;				// write enable
	logic	[`RANGE_LMB]			addr;			// address
	logic	[`RANGE_DWORDx8]		wdata;			// write data
	logic	[`RANGE_DWORDx8]		rdata;			// read data
	logic							ready;			// operation ready
	logic							valid;			// output data valid
		
	modport s	(input en, we, addr, wdata, output rdata, ready, valid);
	modport m	(output en, we, addr, wdata, input rdata, ready, valid);
endinterface

interface i_sync;
	logic							en;				// enable sync.
	logic							ack;			// acknowledge sync.
		
	modport s	(input en, output ack);
	modport m	(output en, input ack);
endinterface

interface i_mem_command_desc;
	logic							cache_en;		// cache enable
	logic							cache_only;		// use cache only
	logic							req_master;		// master process request
	logic							req_ext;		// external bus request
	logic							req_cache;		// cache mastering request
	logic							we;				// write enable
	logic	[`RANGE_BYTE]			size;			// transfer size
	logic	[`RANGE_INDEX]			index;			// register index
	logic	[`RANGE_CACHE_PADDR]	paddr;			// physical memory address
	logic	[`RANGE_GMB]			gaddr;			// global memory block address

	modport s	(input cache_en, cache_only, req_master, req_ext, req_cache, we, size, index, paddr, gaddr);
	modport m	(output cache_en, cache_only, req_master, req_ext, req_cache, we, size, index, paddr, gaddr);
endinterface

`endif//__MTSP_DEFINES__
