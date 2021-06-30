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


// IP VLNV: xilinx.com:user:MSI_Generator:1.0
// IP Revision: 16

`timescale 1ns/1ps

(* DowngradeIPIdentifiedWarnings = "yes" *)
module System_MSI_Generator_0_0 (
  CLK,
  nRST,
  INTR_0,
  INTR_1,
  MSI_EN,
  MSI_REQ,
  MSI_NUM,
  MSI_GRANT
);

(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK, ASSOCIATED_RESET nRST, FREQ_HZ 125000000, PHASE 0.0, CLK_DOMAIN System_axi_pcie_0_1_axi_aclk_out" *)
(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK CLK" *)
input wire CLK;
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME nRST, POLARITY ACTIVE_LOW" *)
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 nRST RST" *)
input wire nRST;
input wire INTR_0;
input wire INTR_1;
input wire MSI_EN;
output wire MSI_REQ;
output wire [4 : 0] MSI_NUM;
input wire MSI_GRANT;

  MSI_Generator_v1_0 #(
    .INTERRUPT_SIZE(2)
  ) inst (
    .CLK(CLK),
    .nRST(nRST),
    .INTR_0(INTR_0),
    .INTR_1(INTR_1),
    .INTR_2(1'B0),
    .INTR_3(1'B0),
    .INTR_4(1'B0),
    .INTR_5(1'B0),
    .INTR_6(1'B0),
    .INTR_7(1'B0),
    .MSI_EN(MSI_EN),
    .MSI_REQ(MSI_REQ),
    .MSI_NUM(MSI_NUM),
    .MSI_GRANT(MSI_GRANT)
  );
endmodule
