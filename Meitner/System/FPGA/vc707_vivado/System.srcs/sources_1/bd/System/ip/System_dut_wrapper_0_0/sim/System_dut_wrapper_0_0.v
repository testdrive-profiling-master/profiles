// (c) Copyright 1995-2021 Xilinx, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


// IP VLNV: xilinx.com:user:dut_wrapper:1.0
// IP Revision: 13

`timescale 1ns/1ps

(* DowngradeIPIdentifiedWarnings = "yes" *)
module System_dut_wrapper_0_0 (
  ACLK,
  nRST,
  INTR,
  S_PSEL,
  S_PENABLE,
  S_PWRITE,
  S_PADDR,
  S_PWDATA,
  S_PRDATA,
  S_PREADY,
  S_PSLVERR,
  M_AWID,
  M_AWADDR,
  M_AWLEN,
  M_AWSIZE,
  M_AWBURST,
  M_AWLOCK,
  M_AWCACHE,
  M_AWPROT,
  M_AWQOS,
  M_AWVALID,
  M_AWREADY,
  M_WID,
  M_WDATA,
  M_WSTRB,
  M_WLAST,
  M_WVALID,
  M_WREADY,
  M_ARID,
  M_ARADDR,
  M_ARLEN,
  M_ARSIZE,
  M_ARBURST,
  M_ARLOCK,
  M_ARCACHE,
  M_ARPROT,
  M_ARQOS,
  M_ARVALID,
  M_ARREADY,
  M_RID,
  M_RDATA,
  M_RRESP,
  M_RLAST,
  M_RVALID,
  M_RREADY,
  M_BID,
  M_BRESP,
  M_BVALID,
  M_BREADY
);

(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ACLK, ASSOCIATED_BUSIF M_AXI:S_APB, FREQ_HZ 200000000, PHASE 0, CLK_DOMAIN System_mig_7series_0_0_ui_clk" *)
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ACLK CLK" *)
input wire ACLK;
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME nRST, POLARITY ACTIVE_LOW" *)
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 nRST RST" *)
input wire nRST;
output wire INTR;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PSEL" *)
input wire S_PSEL;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PENABLE" *)
input wire S_PENABLE;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PWRITE" *)
input wire S_PWRITE;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PADDR" *)
input wire [15 : 0] S_PADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PWDATA" *)
input wire [31 : 0] S_PWDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PRDATA" *)
output wire [31 : 0] S_PRDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PREADY" *)
output wire S_PREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PSLVERR" *)
output wire S_PSLVERR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWID" *)
output wire [0 : 0] M_AWID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWADDR" *)
output wire [31 : 0] M_AWADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWLEN" *)
output wire [7 : 0] M_AWLEN;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWSIZE" *)
output wire [2 : 0] M_AWSIZE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWBURST" *)
output wire [1 : 0] M_AWBURST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWLOCK" *)
output wire M_AWLOCK;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWCACHE" *)
output wire [3 : 0] M_AWCACHE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWPROT" *)
output wire [2 : 0] M_AWPROT;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWQOS" *)
output wire [3 : 0] M_AWQOS;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWVALID" *)
output wire M_AWVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI AWREADY" *)
input wire M_AWREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WID" *)
output wire [0 : 0] M_WID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WDATA" *)
output wire [511 : 0] M_WDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WSTRB" *)
output wire [63 : 0] M_WSTRB;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WLAST" *)
output wire M_WLAST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WVALID" *)
output wire M_WVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI WREADY" *)
input wire M_WREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARID" *)
output wire [0 : 0] M_ARID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARADDR" *)
output wire [31 : 0] M_ARADDR;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARLEN" *)
output wire [7 : 0] M_ARLEN;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARSIZE" *)
output wire [2 : 0] M_ARSIZE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARBURST" *)
output wire [1 : 0] M_ARBURST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARLOCK" *)
output wire M_ARLOCK;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARCACHE" *)
output wire [3 : 0] M_ARCACHE;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARPROT" *)
output wire [2 : 0] M_ARPROT;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARQOS" *)
output wire [3 : 0] M_ARQOS;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARVALID" *)
output wire M_ARVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI ARREADY" *)
input wire M_ARREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RID" *)
input wire [0 : 0] M_RID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RDATA" *)
input wire [511 : 0] M_RDATA;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RRESP" *)
input wire [1 : 0] M_RRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RLAST" *)
input wire M_RLAST;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RVALID" *)
input wire M_RVALID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI RREADY" *)
output wire M_RREADY;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI BID" *)
input wire [0 : 0] M_BID;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI BRESP" *)
input wire [1 : 0] M_BRESP;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI BVALID" *)
input wire M_BVALID;
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M_AXI, DATA_WIDTH 512, PROTOCOL AXI4, FREQ_HZ 200000000, ID_WIDTH 1, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 1, HAS_PROT 1, HAS_CACHE 1, HAS_QOS 1, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 256, PHASE 0, CLK_DOMAIN System_mig_7series_0_0_ui_clk, NUM_READ_THREADS 1, NUM_WRITE_TH\
READS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0" *)
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 M_AXI BREADY" *)
output wire M_BREADY;

  dut_wrapper_v1_0 #(
    .C_M_AXI_ID_WIDTH(1),  // Thread ID Width
    .C_M_AXI_ADDR_WIDTH(32),  // Width of Address Bus
    .C_M_AXI_DATA_WIDTH(512)  // Width of Data Bus
  ) inst (
    .ACLK(ACLK),
    .nRST(nRST),
    .INTR(INTR),
    .S_PSEL(S_PSEL),
    .S_PENABLE(S_PENABLE),
    .S_PWRITE(S_PWRITE),
    .S_PADDR(S_PADDR),
    .S_PWDATA(S_PWDATA),
    .S_PRDATA(S_PRDATA),
    .S_PREADY(S_PREADY),
    .S_PSLVERR(S_PSLVERR),
    .M_AWID(M_AWID),
    .M_AWADDR(M_AWADDR),
    .M_AWLEN(M_AWLEN),
    .M_AWSIZE(M_AWSIZE),
    .M_AWBURST(M_AWBURST),
    .M_AWLOCK(M_AWLOCK),
    .M_AWCACHE(M_AWCACHE),
    .M_AWPROT(M_AWPROT),
    .M_AWQOS(M_AWQOS),
    .M_AWVALID(M_AWVALID),
    .M_AWREADY(M_AWREADY),
    .M_WID(M_WID),
    .M_WDATA(M_WDATA),
    .M_WSTRB(M_WSTRB),
    .M_WLAST(M_WLAST),
    .M_WVALID(M_WVALID),
    .M_WREADY(M_WREADY),
    .M_ARID(M_ARID),
    .M_ARADDR(M_ARADDR),
    .M_ARLEN(M_ARLEN),
    .M_ARSIZE(M_ARSIZE),
    .M_ARBURST(M_ARBURST),
    .M_ARLOCK(M_ARLOCK),
    .M_ARCACHE(M_ARCACHE),
    .M_ARPROT(M_ARPROT),
    .M_ARQOS(M_ARQOS),
    .M_ARVALID(M_ARVALID),
    .M_ARREADY(M_ARREADY),
    .M_RID(M_RID),
    .M_RDATA(M_RDATA),
    .M_RRESP(M_RRESP),
    .M_RLAST(M_RLAST),
    .M_RVALID(M_RVALID),
    .M_RREADY(M_RREADY),
    .M_BID(M_BID),
    .M_BRESP(M_BRESP),
    .M_BVALID(M_BVALID),
    .M_BREADY(M_BREADY)
  );
endmodule
