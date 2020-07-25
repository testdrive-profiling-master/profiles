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
// Title : Processor
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
`ifndef __DPI_DEFINE_VH__
`define __DPI_DEFINE_VH__

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

`endif//__DPI_DEFINE_VH__
