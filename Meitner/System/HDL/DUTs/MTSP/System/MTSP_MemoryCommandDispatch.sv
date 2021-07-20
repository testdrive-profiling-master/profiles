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

module MTSP_MemoryCommandDispatch (
	input	[`RANGE_DWORDx4]			command,		// memory command
	i_mem_command_desc.m				desc			// memory description
);

// definition & assignment ---------------------------------------------------
// requested memory command contents
wire							cmd_we;			// write enable
wire							cmd_cache_en;	// cache enable (read only)
wire							cmd_dir;		// memory direction (0:external to GMB, 1:GMB to LMB)
wire							cmd_ex;			// extended operation
wire	[`RANGE_INDEX]			cmd_index;		// cache index
wire	[`RANGE_WORD]			cmd_id, cmd_mask, cmd_gbase, cmd_lbase;	// id, index mask, GMB base, LMB base
wire	[`RANGE_BYTE]			cmd_stride, cmd_size;	// memory block stride, transfer size
wire	[`RANGE_DWORD]			cmd_pbase;		// physical address

assign	{cmd_we, cmd_cache_en, cmd_dir, cmd_ex, cmd_index, cmd_id}	= command[`RANGE_DWORDx4_X];
assign	{cmd_stride, cmd_size, cmd_mask}							= command[`RANGE_DWORDx4_Y];
assign	cmd_pbase													= command[`RANGE_DWORDx4_Z];
assign	{cmd_lbase, cmd_gbase}										= command[`RANGE_DWORDx4_W];

wire	[`RANGE_WORD]			cmd_masked_id	= cmd_id & cmd_mask;

// dispatch command to description
assign	desc.cache_en		= cmd_cache_en;
assign	desc.cache_only		= cmd_cache_only;
assign	desc.req_master		= (~cmd_dir);
//assign	desc.req_ext		= ;
//assign	desc.req_cache		= ;
assign	desc.we				= cmd_we;
assign	desc.size			= cmd_size;
assign	desc.index			= cmd_index + cmd_masked_id;
assign	desc.paddr			= cmd_pbase + (cmd_stride * cmd_id);
assign	desc.gaddr			= cmd_lbase + (cmd_size * cmd_masked_id);

// implementation ------------------------------------------------------------

endmodule
