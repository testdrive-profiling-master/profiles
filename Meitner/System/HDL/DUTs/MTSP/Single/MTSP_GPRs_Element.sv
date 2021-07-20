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

module MTSP_GPRs_Element #(
	parameter	CORE_ID				= 0,
	parameter	PRIMITIVE_ID		= 0
) (
	// system signals
	input							CLK, nRST,				// clock & reset (active low)

	// Temporary Register Interface 4D
	input	[`RANGE_MASK4D]			WMASK,					// write mask
	input	[`SIZE_GPRs:0]			ADDR,					// address x2
	input	[`RANGE_DWORDx4]		WDATA,					// write data
	output	[`RANGE_DWORDx4]		RDATA					// read data
);

// definition & assignment ---------------------------------------------------
localparam	PHY_GPRs_BITS			= $clog2(`MEITNER_GPRs_SIZE);
wire	[PHY_GPRs_BITS:0]			phy_addr	= {ADDR[`SIZE_GPRs], ADDR[PHY_GPRs_BITS-1:0]};

// implementation ------------------------------------------------------------
// GPRs 4D vector dual port sram
SRAM_Single #(
	.ADDR_WIDTH		(PHY_GPRs_BITS+1),
	.DATA_WIDTH		(`SIZE_DWORD)
) GPRs_X (
	.CLK			(CLK),
	.nCE			(`nTRUE),
	.nWE			(WMASK[`SELECT_X]),
	.ADDR			(phy_addr),
	.DIN			(WDATA[`RANGE_DWORDx4_X]),
	.DOUT			(RDATA[`RANGE_DWORDx4_X])
);

SRAM_Single #(
	.ADDR_WIDTH		(PHY_GPRs_BITS+1),
	.DATA_WIDTH		(`SIZE_DWORD)
) GPRs_Y (
	.CLK			(CLK),
	.nCE			(`nTRUE),
	.nWE			(WMASK[`SELECT_Y]),
	.ADDR			(phy_addr),
	.DIN			(WDATA[`RANGE_DWORDx4_Y]),
	.DOUT			(RDATA[`RANGE_DWORDx4_Y])
);

SRAM_Single #(
	.ADDR_WIDTH		(PHY_GPRs_BITS+1),
	.DATA_WIDTH		(`SIZE_DWORD)
) GPRs_Z (
	.CLK			(CLK),
	.nCE			(`nTRUE),
	.nWE			(WMASK[`SELECT_Z]),
	.ADDR			(phy_addr),
	.DIN			(WDATA[`RANGE_DWORDx4_Z]),
	.DOUT			(RDATA[`RANGE_DWORDx4_Z])
);

SRAM_Single #(
	.ADDR_WIDTH		(PHY_GPRs_BITS+1),
	.DATA_WIDTH		(`SIZE_DWORD)
) GPRs_W (
	.CLK			(CLK),
	.nCE			(`nTRUE),
	.nWE			(WMASK[`SELECT_W]),
	.ADDR			(phy_addr),
	.DIN			(WDATA[`RANGE_DWORDx4_W]),
	.DOUT			(RDATA[`RANGE_DWORDx4_W])
);

`ifdef USE_TESTDRIVE
`DPI_FUNCTION void SetGPR(input int unsigned dwCoreID, input int unsigned dwPrimitiveID, input int unsigned dwIndex, input int unsigned dwComponent, input bit [`RANGE_DWORD] DATA);
int i;
initial begin
	for(i=0;i<`MEITNER_GPRs_SIZE;i=i+1) begin
		SetGPR(CORE_ID, PRIMITIVE_ID,   i, 0, GPRs_X.mem[i]);
		SetGPR(CORE_ID, PRIMITIVE_ID+6, i, 0, GPRs_X.mem[i+`MEITNER_GPRs_SIZE]);
		SetGPR(CORE_ID, PRIMITIVE_ID,   i, 1, GPRs_Y.mem[i]);
		SetGPR(CORE_ID, PRIMITIVE_ID+6, i, 1, GPRs_Y.mem[i+`MEITNER_GPRs_SIZE]);
		SetGPR(CORE_ID, PRIMITIVE_ID,   i, 2, GPRs_Z.mem[i]);
		SetGPR(CORE_ID, PRIMITIVE_ID+6, i, 2, GPRs_Z.mem[i+`MEITNER_GPRs_SIZE]);
		SetGPR(CORE_ID, PRIMITIVE_ID,   i, 3, GPRs_W.mem[i]);
		SetGPR(CORE_ID, PRIMITIVE_ID+6, i, 3, GPRs_W.mem[i+`MEITNER_GPRs_SIZE]);
	end
end

`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		// nothing to do!
	end
	else begin
		if(!WMASK[`SELECT_X])
			SetGPR(CORE_ID, PRIMITIVE_ID+(ADDR[`SIZE_GPRs] ? 6 : 0), ADDR[PHY_GPRs_BITS-1:0], 0, WDATA[`RANGE_DWORDx4_X]);
		if(!WMASK[`SELECT_Y])
			SetGPR(CORE_ID, PRIMITIVE_ID+(ADDR[`SIZE_GPRs] ? 6 : 0), ADDR[PHY_GPRs_BITS-1:0], 1, WDATA[`RANGE_DWORDx4_Y]);
		if(!WMASK[`SELECT_Z])
			SetGPR(CORE_ID, PRIMITIVE_ID+(ADDR[`SIZE_GPRs] ? 6 : 0), ADDR[PHY_GPRs_BITS-1:0], 2, WDATA[`RANGE_DWORDx4_Z]);
		if(!WMASK[`SELECT_W])
			SetGPR(CORE_ID, PRIMITIVE_ID+(ADDR[`SIZE_GPRs] ? 6 : 0), ADDR[PHY_GPRs_BITS-1:0], 3, WDATA[`RANGE_DWORDx4_W]);
	end
end
`endif

endmodule
