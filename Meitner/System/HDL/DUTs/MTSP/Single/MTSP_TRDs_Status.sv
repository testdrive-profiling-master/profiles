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

module MTSP_TRDs_Status(
	// system
	input						CLK, nRST,
	input						EN_ALL,			// command enable to all
	input						nEN,			// command enable (active low)
	input	[1:0]				BOP,			// branch operation
	input	[`RANGE_PC]			PC_NEXT,		// next pc
	input						nWAIT,			// thread wait for memory transaction
	input						nAWAKE,			// thread awake to processing the thread
	input						nSOLITUDE,		// only one thread is running

	output	reg [`RANGE_PC]		PC,				// current pc
	output	reg					BUSY,
	output	reg					RUN
);

// register definition & assignment ------------------------------------------
//BOP	00 : jmp (jump current to PC_NEXT)
//		01 : all (revolt all thread to PC_NEXT)
//		10 : end (end current thread)
//		11 : seq (sequencing thread)
//										newall 명령 or 개별명령 이면 seq 일땐 ~nSOL. 아니면 jmp 일때만 enable
wire						busy_next = EN_ALL | (nEN|(BOP==2'b01) ? BUSY : (&BOP ?  ~nSOLITUDE : ~BOP[1]));

// implementation ------------------------------------------------------------
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		PC		<= 'd0;
		BUSY	<= `FALSE;
		RUN		<= 1'b0;
	end
	else begin
		//			개별명령이거나 newall 활성이면서 BUSY가 비활성일때 PC_NEXT 받음
		PC		<= (nEN & ((~EN_ALL)|BUSY)) ? PC : PC_NEXT;
		BUSY	<=	busy_next;
		//		   다음상태가 busy 이면서
		RUN		<= busy_next &
		//			nWAIT의한 정지		nWAK.활성,  newall 활성
		((RUN&(nWAIT|nEN)) | (~nAWAKE) | EN_ALL);
	end
end

endmodule
