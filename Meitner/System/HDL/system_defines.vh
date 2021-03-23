//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Processor
// Rev.  : 3/23/2021 Tue (clonextop@gmail.com)
//================================================================================
`ifndef __SYSTEM_DEFINE_VH__
`define __SYSTEM_DEFINE_VH__
`timescale 1ns/1ns

`ifdef VERILATOR
`define	USE_TESTDRIVE
`elsif SVEDITOR
`define	USE_TESTDRIVE
`endif

`define TRUE					1'b1
`define FALSE					1'b0
`define nTRUE					`FALSE
`define nFALSE					`TRUE

`define SIZE_BYTE				8
`define SIZE_WORD				16
`define SIZE_DWORD				32
`define SIZE_DWORDx2			64
`define SIZE_DWORDx4			128
`define SIZE_DWORDx8			256

`define RANGE_BYTE				7:0
`define RANGE_WORD				15:0
`define RANGE_DWORD				31:0
`define RANGE_DWORDx2			63:0
`define RANGE_DWORDx4			127:0
`define RANGE_DWORDx8			255:0

`define RANGE_FLOAT				31:0
`define RANGE_DOUBLE			63:0
`define RANGE_FLOAT16			15:0
`define RANGE_FLOAT32			`RANGE_FLOAT
`define RANGE_FLOAT64			`RANGE_DOUBLE

`define RANGE_BYTE_0			((8*1)-1):0
`define RANGE_BYTE_1			((8*2)-1):(8*1)
`define RANGE_BYTE_2			((8*3)-1):(8*2)
`define RANGE_BYTE_3			((8*4)-1):(8*3)

`define RANGE_WORD_0			((16*1)-1):0
`define RANGE_WORD_1			((16*2)-1):(16*1)
`define RANGE_WORD_2			((16*3)-1):(16*2)
`define RANGE_WORD_3			((16*4)-1):(16*3)

`define RANGE_DWORD_0			((32*1)-1):0
`define RANGE_DWORD_1			((32*2)-1):(32*1)
`define RANGE_DWORD_2			((32*3)-1):(32*2)
`define RANGE_DWORD_3			((32*4)-1):(32*3)

`define ZERO					'd0
`define ZERO_BIT				1'd0
`define ZERO_BYTE				8'd0
`define ZERO_WORD				16'd0
`define ZERO_DWORD				32'd0
`define ZERO_DWORDx4			{`ZERO_DWORD, `ZERO_DWORD, `ZERO_DWORD, `ZERO_DWORD}
`define ZERO_DWORDx8			{`ZERO_DWORDx4, `ZERO_DWORDx4}

`define ROTATE_LEFT(data,size,rotate)		{data[(size-rotate-1):0], data[(size-1):(size-rotate)]}		// rotate left (rotate != 0)
`define ROTATE_RIGHT(data,size,rotate)		{data[(rotate-1):0], data[(size-1):rotate]}					// rotate right (rotate != 0)
`define SHIFT_LEFT(data,size,shift)			{data[(size-shift-1):0], {shift{1'b0}}}						// shift left (shift != 0)
`define SHIFT_RIGHT(data,size,shift)		{{shift{1'b0}}, data[(size-1):shift]}						// logical shift right (shift != 0)
`define SHIFT_RIGHT_S(data,size,shift)		{{shift{data[size-1]}}, data[(size-1):shift]}				// signed shift left (shift != 0)

`define BUS_RANGE(w,i)						((i+1)*(w))-1:((i)*(w))
`define BUS_RANGE2D(w,s,x,y)				((((s)*(y))+x+1)*(w))-1:((((s)*(y))+x)*(w))

// clocking & registering
`define ALWAYS_CLOCK						always@(posedge CLK)
`define ALWAYS_CLOCK_RESET					always@(posedge CLK, negedge nRST)
`define ON_RESET							if(!nRST)
`define PIPE_SETi(w,s,i,o,v)				reg [(w)-1:0] o; `ALWAYS_CLOCK_RESET `ON_RESET o <= v; else if(s) o <= i;
`define PIPE_SET(w,s,i,o)					`PIPE_SETi(w,s,i,o,{(w){1'b0}})
`define PIPE_BYPASS(w,i,o)					reg [(w)-1:0] o; `ALWAYS_CLOCK o <= i;
`define REG_BUFFER(size, i, iv, o, ov)		reg [size-1:0] o;wire ov; assign iv=((~o[size-1])|ov); `ALWAYS_CLOCK_RESET `ON_RESET begin o <= {(size){1'b0}}; end else begin if(iv & i[size-1]) o <= i; else if(ov & o[size-1]) o <= {(size){1'b0}}; end

//------------------------------------------------------------------------------------------------------
// system verilog DPI functions
//------------------------------------------------------------------------------------------------------
`define DPI_FUNCTION			import "DPI-C" function

`ifdef USE_TESTDRIVE
// meomory interface ---------------------------------------------------------
`DPI_FUNCTION void MemoryWrite32 (input int ID, input int unsigned ADDR, input  int      unsigned DATA);
`DPI_FUNCTION void MemoryWrite16 (input int ID, input int unsigned ADDR, input  shortint unsigned DATA);
`DPI_FUNCTION void MemoryWrite8  (input int ID, input int unsigned ADDR, input  byte     unsigned DATA);
`DPI_FUNCTION void MemoryRead32  (input int ID, input int unsigned ADDR, output int      unsigned DATA);
`DPI_FUNCTION void MemoryRead16  (input int ID, input int unsigned ADDR, output shortint unsigned DATA);
`DPI_FUNCTION void MemoryRead8   (input int ID, input int unsigned ADDR, output byte     unsigned DATA);

`DPI_FUNCTION void BreakPoint (input string sFileName, input int iLine);
`DPI_FUNCTION longint unsigned SimulationTime();
`DPI_FUNCTION void SetSystemDescription(string sDesc);

`define	BREAK_POINT			BreakPoint(`__FILE__, `__LINE__);
`else//!USE_TESTDRIVE
`define	BREAK_POINT			// for non-testdrive system
`endif//USE_TESTDRIVE

`endif//__SYSTEM_DEFINE_VH__
