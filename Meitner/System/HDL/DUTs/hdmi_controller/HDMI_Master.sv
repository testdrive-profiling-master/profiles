//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : HDMI controller
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
module HDMI_Master #(
		parameter integer	C_ADDR_WIDTH		= 32
	) (
		// port list -----------------------------------------------------------------
		//// AXI slave interface
		// system
		input								ACLK,				// clock
		input								nRST,				// reset (active low)

		//// Simplified master interface
		output	reg							MR_REQ,				// read request
		input								MR_GRANT,			// read grant
		output	reg [C_ADDR_WIDTH-1:0]		MR_ADDR,			// read address
		output	[7:0]						MR_SIZE,			// read size
		input	[512-1:0]					MR_DATA,			// read data
		input								MR_VALID,			// read validation
		output								MR_READY,			// read ready
		input								MR_LAST,			// last read

		// video configuration
		input	[63:0]						VIDEO_BASE,			// base address
		input	[15:0]						VIDEO_FRAME,		// frame count
		// pixel operation
		input								PIXEL_nREQ,
		output								PIXEL_nVALID,
		input								PIXEL_nREADY,
		output	[384-1:0]					PIXEL_DATA
	);

	// definition & assignment ---------------------------------------------------
	reg		[15:0]			frame_count;

	assign	MR_SIZE			= 'd15;			// 16 beats
	assign	MR_READY		= (~PIXEL_nREADY);
	assign	PIXEL_nVALID	= (~MR_VALID);
	assign	PIXEL_DATA		= {
		MR_DATA[(32*15)+23:(32*15)],
		MR_DATA[(32*14)+23:(32*14)],
		MR_DATA[(32*13)+23:(32*13)],
		MR_DATA[(32*12)+23:(32*12)],
		MR_DATA[(32*11)+23:(32*11)],
		MR_DATA[(32*10)+23:(32*10)],
		MR_DATA[(32* 9)+23:(32* 9)],
		MR_DATA[(32* 8)+23:(32* 8)],
		MR_DATA[(32* 7)+23:(32* 7)],
		MR_DATA[(32* 6)+23:(32* 6)],
		MR_DATA[(32* 5)+23:(32* 5)],
		MR_DATA[(32* 4)+23:(32* 4)],
		MR_DATA[(32* 3)+23:(32* 3)],
		MR_DATA[(32* 2)+23:(32* 2)],
		MR_DATA[(32* 1)+23:(32* 1)],
		MR_DATA[(32* 0)+23:(32* 0)]
	};

	// implementation ------------------------------------------------------------
	always@(posedge ACLK, negedge nRST) begin
		if(!nRST) begin
			MR_REQ		<= 1'b0;
			MR_ADDR		<= {C_ADDR_WIDTH{1'b0}};
			frame_count	<= 'd0;
		end
		else begin
			if(!MR_REQ) begin
				MR_REQ		<= (!PIXEL_nREQ) | (|frame_count);
				if(!PIXEL_nREQ) begin
					MR_ADDR		<= VIDEO_BASE[C_ADDR_WIDTH-1:0];
					frame_count	<= VIDEO_FRAME;
				end
			end
			else
				if(MR_GRANT) begin
					MR_REQ		<= 1'b0;
					MR_ADDR		<= MR_ADDR + 32'd1024;	// 512bit(64) * 16 beats = 1024 entries
					frame_count	<= frame_count - 1'b1;
				end
		end
	end

endmodule
