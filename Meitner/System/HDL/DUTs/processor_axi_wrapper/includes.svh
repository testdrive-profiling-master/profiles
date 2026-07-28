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
// Title : processor AXI wrapper
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
`ifndef __PROCESSOR_AXI_WRAPPER_COMMON_SVH__
`define __PROCESSOR_AXI_WRAPPER_COMMON_SVH__
`include "testdrive_system.svh"

// common library
`include "library/SyncPipe.sv"
`include "library/SRAM_Dual_Distributed.sv"
`include "library/FiFo_Async.sv"
`include "library/FiFo.sv"

/*verilator tracing_off*/
`include "template/axi_master_wrapper.sv"
`include "template/axi_master_lite_wrapper.sv"
`include "DUTs/processor_axi_wrapper/processor_clkgen.sv"
`include "DUTs/processor_axi_wrapper/processor_clkgen_slave.sv"
`include "DUTs/processor_axi_wrapper/processor_async_slave.sv"
`include "DUTs/processor_axi_wrapper/processor_async_master.sv"
`include "DUTs/processor_axi_wrapper/processor_async_master_lite.sv"
/*verilator tracing_on*/

// processor axi wrapper top level module (Asynchronous)
`include "DUTs/processor_axi_wrapper/processor_axi_wrapper.sv"

//-----------------------------------------------------------------------
// Top design selection
//-----------------------------------------------------------------------
// test design (simple slave & master read/write test...)
`ifdef TEST_DESIGN
	`include "DUTs/processor_axi_wrapper/processor_top.sv"
`endif

`endif//__PROCESSOR_AXI_WRAPPER_COMMON_SVH__

