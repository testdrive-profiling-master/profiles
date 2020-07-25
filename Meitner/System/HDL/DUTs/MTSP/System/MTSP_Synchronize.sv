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

module MTSP_Synchronize #(
	parameter	CORE_SIZE		= `MEITNER_CORE_SIZE
) (
	// System
	input						CLK, nRST,					// main clock & reset
	i_sync.s					core_sync[(CORE_SIZE-1):0]	// sync. of multi-core
);

// definition & assignment ---------------------------------------------------
genvar							i;
reg		[(CORE_SIZE-1):0]		multicore_sync;
reg								multicore_ack;

// implementation ------------------------------------------------------------
// make acknowlege
`ALWAYS_CLOCK_RESET begin
	`ON_RESET begin
		multicore_ack		<= `FALSE;
	end
	else begin
		multicore_ack		<= &multicore_sync;
	end
end

generate
for(i=0;i<CORE_SIZE;i=i+1) begin : core_synchronize
	`ALWAYS_CLOCK_RESET begin
		`ON_RESET begin
			multicore_sync[i] 	<= `FALSE;
			core_sync[i].ack	<= `FALSE;
		end
		else begin
			if(multicore_ack) begin
				multicore_sync[i] 	<= `FALSE;
			end
			else if(core_sync[i].en) begin
				multicore_sync[i]	<= `TRUE;
			end
			core_sync[i].ack	<= multicore_ack;
		end
	end
end
endgenerate

endmodule
