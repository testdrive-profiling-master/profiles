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
// Rev.  : 1/23/2024 Tue (clonextop@gmail.com)
//================================================================================
module HDMI_LineBuffer #(
		parameter	DEPTH				= 8				// 2^8 * 8 = 256 * 8 = 2048
	) (
		input							nRST,			// reset (active low)
		// write
		input							W_CLK,			// clock
		output	reg						W_nREQ,			// transaction request
		input							W_nVALID,		// write enable
		output							W_nREADY,		// frame toogle
		input	[384-1:0]				W_DATA,			// data (for 512 bits bus)
		// read
		input							R_CLK,			// clock
		input							R_RE,			// read enable
		input							R_FT,			// frame toggle
		output	[23:0]					R_DATA			// data
	);

	// definition & assignment ---------------------------------------------------
	reg		[1:0]		re_history;
	reg		[1:0]		ft_history;
	reg		[384-1:0]	pixel_data;
	wire	[384-1:0]	fifo_rdata;
	wire				fifo_empty;
	reg					fifo_rst_n;
	reg		[3:0]		r_sel;

	assign	R_DATA		= pixel_data[23:0];

	// implementation ------------------------------------------------------------
	FiFo_Async #(
		.FIFO_DEPTH		(DEPTH),
		.DATA_WIDTH		(384)
	) pixel_fifo (
		// push interface
		.W_CLK			(W_CLK),
		.W_nRST			(nRST & fifo_rst_n),
		.W_nEN			(W_nVALID),
		.W_DATA			(W_DATA),
		.W_FULL			(W_nREADY),
		// pop interface
		.R_CLK			(R_CLK),
		.R_nRST			(nRST & fifo_rst_n),
		.R_nEN			(~&{R_RE, r_sel}),
		.R_DATA			(fifo_rdata),
		.R_EMPTY		(fifo_empty)
	);

	always@(posedge W_CLK) begin
		re_history	<= {re_history[0], R_RE};
		ft_history	<= {ft_history[0], R_FT};
		fifo_rst_n	<= (
			((re_history == 2'b10) & ft_history[0])		? 1'b0 :
			(ft_history == 2'b10)						? 1'b1 : fifo_rst_n
		);
		W_nREQ		<= (ft_history != 2'b10);
	end

	always@(posedge R_CLK) begin
		r_sel		<= R_RE ? (r_sel + 1'b1) : 4'd0;
		if(R_RE) begin
			pixel_data	<= (r_sel == 4'd0) ? (fifo_empty ? 'd0 : fifo_rdata) : {24'd0, pixel_data[384-1:24]};
		end
	end

endmodule
