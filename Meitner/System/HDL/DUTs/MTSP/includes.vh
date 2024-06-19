//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
`ifndef __MTSPU_TESTBENCH_WRAPPER_INCLUDES_VH__
`define __MTSPU_TESTBENCH_WRAPPER_INCLUDES_VH__
`include "testdrive_system.vh"

/* verilator lint_on WIDTH */
/* verilator lint_on BLKSEQ */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNDRIVEN */

// common library
`include "library/SyncPipe.v"
`include "library/SRAM_Dual_2R.v"
`include "library/SRAM_Dual_Distributed.v"
`include "library/FiFo_Async.v"
`include "library/FiFo.v"
`include "library/mux_by_index.v"
`include "library/demux_by_enable.v"
`include "library/ControlSlice.v"
`include "library/CommandQueue.v"
`include "library/MultiCyclePath.v"
`include "library/MultiCyclePathEx.v"

// BFMs
`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"

// BUS wrapper
`include "template/axi_master_wrapper.v"

//-----------------------------------------------------------------------
// DUT design selection
//-----------------------------------------------------------------------
`include "MTSP_wrapper_includes.vh"

// default definitions
`ifndef CORE_SIZE
`define	CORE_SIZE			4
`endif

`endif//__MTSPU_TESTBENCH_WRAPPER_INCLUDES_VH__
