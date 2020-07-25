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

module MTSP_GSCsCoordinate(	UINST,
							GSCs_RST, GSCs_INC);

// port list -----------------------------------------------------------------
// input
input	[`RANGE_UINST]			UINST;					// unit instructions input

// output
output	[`RANGE_MASK4D]			GSCs_RST;				// reset
output	[`RANGE_MASK4D]			GSCs_INC;				// increase

// definition & assignment ---------------------------------------------------
wire						w_nEN;
wire	[3:0]				OP_X, OP_Y, OP_Z, OP_W;

assign	w_nEN						= UINST[`RANGE_UINST_nEN] | (UINST[`RANGE_UINST_MO]!=`MO_MEM) | (UINST[`RANGE_UINST_MO_MASK]!=`MEMID_SCRATCH);
assign	{OP_X, OP_Y, OP_Z, OP_W}	= UINST[`RANGE_WORD];

assign	GSCs_RST[`RANGE_MASK4D_X]	= w_nEN	? `FALSE : (OP_X[3]&OP_X[1]);
assign	GSCs_RST[`RANGE_MASK4D_Y]	= w_nEN	? `FALSE : (OP_Y[3]&OP_Y[1]);
assign	GSCs_RST[`RANGE_MASK4D_Z]	= w_nEN	? `FALSE : (OP_Z[3]&OP_Z[1]);
assign	GSCs_RST[`RANGE_MASK4D_W]	= w_nEN	? `FALSE : (OP_W[3]&OP_W[1]);

assign	GSCs_INC[`RANGE_MASK4D_X]	= w_nEN	? `FALSE : (OP_X[3]&OP_X[0]);
assign	GSCs_INC[`RANGE_MASK4D_Y]	= w_nEN	? `FALSE : (OP_Y[3]&OP_Y[0]);
assign	GSCs_INC[`RANGE_MASK4D_Z]	= w_nEN	? `FALSE : (OP_Z[3]&OP_Z[0]);
assign	GSCs_INC[`RANGE_MASK4D_W]	= w_nEN	? `FALSE : (OP_W[3]&OP_W[0]);

// implementation ------------------------------------------------------------



endmodule
