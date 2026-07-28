//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
`ifndef __MTSPU_TESTBENCH_WRAPPER_INCLUDES_SVH__
`define __MTSPU_TESTBENCH_WRAPPER_INCLUDES_SVH__
`include "testdrive_system.svh"

/* verilator lint_on WIDTH */
/* verilator lint_on BLKSEQ */
/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNDRIVEN */

// common library
`include "library/SyncPipe.sv"
`include "library/SRAM_Dual_2R.sv"
`include "library/SRAM_Dual_Distributed.sv"
`include "library/FiFo_Async.sv"
`include "library/FiFo.sv"
`include "library/mux_by_index.sv"
`include "library/demux_by_enable.sv"
`include "library/ControlSlice.sv"
`include "library/CommandQueue.sv"
`include "library/MultiCyclePath.sv"
`include "library/MultiCyclePathEx.sv"

// BFMs
`include "template/testdrive_apb_slave_bfm.sv"
`include "template/testdrive_axi4_master_bfm.sv"

// BUS wrapper
`include "template/axi_master_wrapper.sv"

//-----------------------------------------------------------------------
// DUT design selection
//-----------------------------------------------------------------------
`include "MTSP_wrapper_includes.svh"

// default definitions
`ifndef CORE_SIZE
`define	CORE_SIZE			4
`endif

`endif//__MTSPU_TESTBENCH_WRAPPER_INCLUDES_SVH__
