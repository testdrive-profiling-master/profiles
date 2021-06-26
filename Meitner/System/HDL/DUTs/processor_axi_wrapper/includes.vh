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
// Title : processor AXI wrapper
// Rev.  : 6/26/2021 Sat (clonextop@gmail.com)
//================================================================================
`ifndef __PROCESSOR_AXI_WRAPPER_COMMON_VH__
`define __PROCESSOR_AXI_WRAPPER_COMMON_VH__
`include "system_defines.vh"

// common library
`include "library/SyncPipe.v"
`include "library/SRAM_Dual_Distributed.v"
`include "library/FiFo_Async.v"
`include "library/FiFo.v"

/*verilator tracing_off*/
`include "template/axi_master_wrapper.v"
`include "DUTs/processor_axi_wrapper/processor_clkgen.v"
`include "DUTs/processor_axi_wrapper/processor_clkgen_slave.v"
`include "DUTs/processor_axi_wrapper/processor_async_slave.v"
`include "DUTs/processor_axi_wrapper/processor_async_master.v"
/*verilator tracing_on*/

// processor axi wrapper top level module (Asynchronous)
`include "DUTs/processor_axi_wrapper/processor_axi_wrapper.v"

//-----------------------------------------------------------------------
// Top design selection
//-----------------------------------------------------------------------
// test design (simple slave & master read/write test...)
`ifdef TEST_DESIGN
	`include "DUTs/processor_axi_wrapper/processor_top.v"
`endif

`endif//__PROCESSOR_AXI_WRAPPER_COMMON_VH__

