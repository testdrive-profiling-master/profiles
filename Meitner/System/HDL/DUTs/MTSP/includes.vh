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
// Title : Meitner processor v1.1
// Rev.  : 6/18/2021 Fri (clonextop@gmail.com)
//================================================================================
/* verilator lint_off UNOPTFLAT */

`include "MTSP_Defines.vh"

// common library
/*verilator tracing_off*/
`include "library/ControlSlice.v"
`include "library/SRAM_Single.v"
`include "library/SRAM_Dual_Distributed.v"
`include "library/FiFo.v"
`include "library/mux_by_index.v"
`include "library/CommandQueue.v"
`include "library/Register_element.v"
`include "library/demux_by_enable.v"
`include "library/MultiCyclePath.v"
`include "library/MultiCyclePathEx.v"
`include "library/MultiCycleSlice.v"
/*verilator tracing_on*/

// core
/*verilator tracing_off*/
`include "DUTs/MTSP/Core/MTSP_SF_EXP.sv"
`include "DUTs/MTSP/Core/MTSP_SF_MAN.sv"
`include "DUTs/MTSP/Core/MTSP_SF_RCP_LUT.sv"
`include "DUTs/MTSP/Core/MTSP_SF_RCP.sv"
`include "DUTs/MTSP/Core/MTSP_SF_RSQ_LUT.sv"
`include "DUTs/MTSP/Core/MTSP_SF_RSQ.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_ADD.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_BA.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_BW.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_CMP.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_CONV.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_FLFR.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_FMT.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_MOV.sv"
`include "DUTs/MTSP/Core/MTSP_ALU_MUL.sv"
`include "DUTs/MTSP/Core/MTSP_BRC.sv"
/*verilator tracing_on*/
`include "DUTs/MTSP/Core/MTSP_Core.sv"
`include "DUTs/MTSP/Core/MTSP_EX.sv"
`include "DUTs/MTSP/Core/MTSP_EXT.sv"
`include "DUTs/MTSP/Core/MTSP_EX_Primitive.sv"
`include "DUTs/MTSP/Core/MTSP_GSCsCoordinate.sv"
`include "DUTs/MTSP/Core/MTSP_ID_0.sv"
`include "DUTs/MTSP/Core/MTSP_ID_1.sv"
`include "DUTs/MTSP/Core/MTSP_ID_2.sv"
`include "DUTs/MTSP/Core/MTSP_ID_3.sv"
`include "DUTs/MTSP/Core/MTSP_InstCoordinate.sv"
`include "DUTs/MTSP/Core/MTSP_InstOrganize.sv"
`include "DUTs/MTSP/Core/MTSP_MEM.sv"
`include "DUTs/MTSP/Core/MTSP_MO_Dispatch.sv"
`include "DUTs/MTSP/Core/MTSP_OF_0.sv"
`include "DUTs/MTSP/Core/MTSP_OF_1.sv"
`include "DUTs/MTSP/Core/MTSP_OF_2.sv"
`include "DUTs/MTSP/Core/MTSP_SF.sv"
`include "DUTs/MTSP/Core/MTSP_SrcCoordinate.sv"
`include "DUTs/MTSP/Core/MTSP_SrcOperate1D.sv"
`include "DUTs/MTSP/Core/MTSP_SrcOperate4D.sv"
`include "DUTs/MTSP/Core/MTSP_SrcSwizzle4D.sv"
`include "DUTs/MTSP/Core/MTSP_WB.sv"

`include "DUTs/MTSP/Single/MTSP_Bus.sv"
`include "DUTs/MTSP/Single/MTSP_EW1.sv"
`include "DUTs/MTSP/Single/MTSP_GPRs.sv"
`include "DUTs/MTSP/Single/MTSP_GPRs_Element.sv"
`include "DUTs/MTSP/Single/MTSP_GPRs_Primitive.sv"
`include "DUTs/MTSP/Single/MTSP_INSTx4.sv"
/*verilator tracing_off*/
`include "DUTs/MTSP/Single/MTSP_LMB.sv"
/*verilator tracing_on*/
`include "DUTs/MTSP/Single/MTSP_PackUnpack.sv"
`include "DUTs/MTSP/Single/MTSP_SCs.sv"
`include "DUTs/MTSP/Single/MTSP_SCs_Element.sv"
`include "DUTs/MTSP/Single/MTSP_SCs_Primitive.sv"
`include "DUTs/MTSP/Single/MTSP_Single.sv"
`include "DUTs/MTSP/Single/MTSP_TRDs.sv"
`include "DUTs/MTSP/Single/MTSP_TRDs_Primitive.sv"
`include "DUTs/MTSP/Single/MTSP_TRDs_Status.sv"
`include "DUTs/MTSP/Single/MTSP_UnpackElement.sv"

`include "DUTs/MTSP/System/MTSP_Busy.sv"
`include "DUTs/MTSP/System/MTSP_Synchronize.sv"
`include "DUTs/MTSP/System/MTSP_Multi.sv"
`include "DUTs/MTSP/System/MTSP_Command.sv"
`include "DUTs/MTSP/System/MTSP_MemoryCommandDispatch.sv"
`include "DUTs/MTSP/System/MTSP_MemoryCommandDemux.sv"
`include "DUTs/MTSP/System/MTSP_MemoryCommand.sv"
`include "DUTs/MTSP/System/MTSP_Cache.sv"
`include "DUTs/MTSP/System/MTSP_CacheLUT.sv"
/*verilator tracing_off*/
`include "DUTs/MTSP/System/MTSP_GMB.sv"
/*verilator tracing_on*/
`include "DUTs/MTSP/System/MTSP_InterBus.sv"
`include "DUTs/MTSP/System/MTSP_BusMaster.sv"
`include "DUTs/MTSP/System/MTSP_System.sv"
