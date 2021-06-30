//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.1 (win64) Build 3247384 Thu Jun 10 19:36:33 MDT 2021
//Date        : Wed Jun 30 13:28:05 2021
//Host        : DESKTOP-F0P52DA running 64-bit major release  (build 9200)
//Command     : generate_target System_wrapper.bd
//Design      : System_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module System_wrapper
   (HDMI_CLK,
    HDMI_DATA,
    HDMI_DE,
    HDMI_HSYNC,
    HDMI_INTR,
    HDMI_SPDIF,
    HDMI_VSYNC,
    I2C_SCL,
    I2C_SDA,
    I2C_nRST,
    ddr3_addr,
    ddr3_ba,
    ddr3_cas_n,
    ddr3_ck_n,
    ddr3_ck_p,
    ddr3_cke,
    ddr3_cs_n,
    ddr3_dm,
    ddr3_dq,
    ddr3_dqs_n,
    ddr3_dqs_p,
    ddr3_odt,
    ddr3_ras_n,
    ddr3_reset_n,
    ddr3_we_n,
    pcie_diff_clock_clk_n,
    pcie_diff_clock_clk_p,
    pcie_perstn,
    pcie_rxn,
    pcie_rxp,
    pcie_txn,
    pcie_txp,
    sys_clk_clk_n,
    sys_clk_clk_p,
    sys_rst);
  output HDMI_CLK;
  output [35:0]HDMI_DATA;
  output HDMI_DE;
  output HDMI_HSYNC;
  input HDMI_INTR;
  output HDMI_SPDIF;
  output HDMI_VSYNC;
  inout I2C_SCL;
  inout I2C_SDA;
  output I2C_nRST;
  output [13:0]ddr3_addr;
  output [2:0]ddr3_ba;
  output ddr3_cas_n;
  output [0:0]ddr3_ck_n;
  output [0:0]ddr3_ck_p;
  output [0:0]ddr3_cke;
  output [0:0]ddr3_cs_n;
  output [7:0]ddr3_dm;
  inout [63:0]ddr3_dq;
  inout [7:0]ddr3_dqs_n;
  inout [7:0]ddr3_dqs_p;
  output [0:0]ddr3_odt;
  output ddr3_ras_n;
  output ddr3_reset_n;
  output ddr3_we_n;
  input [0:0]pcie_diff_clock_clk_n;
  input [0:0]pcie_diff_clock_clk_p;
  input pcie_perstn;
  input [3:0]pcie_rxn;
  input [3:0]pcie_rxp;
  output [3:0]pcie_txn;
  output [3:0]pcie_txp;
  input sys_clk_clk_n;
  input sys_clk_clk_p;
  input sys_rst;

  wire HDMI_CLK;
  wire [35:0]HDMI_DATA;
  wire HDMI_DE;
  wire HDMI_HSYNC;
  wire HDMI_INTR;
  wire HDMI_SPDIF;
  wire HDMI_VSYNC;
  wire I2C_SCL;
  wire I2C_SDA;
  wire I2C_nRST;
  wire [13:0]ddr3_addr;
  wire [2:0]ddr3_ba;
  wire ddr3_cas_n;
  wire [0:0]ddr3_ck_n;
  wire [0:0]ddr3_ck_p;
  wire [0:0]ddr3_cke;
  wire [0:0]ddr3_cs_n;
  wire [7:0]ddr3_dm;
  wire [63:0]ddr3_dq;
  wire [7:0]ddr3_dqs_n;
  wire [7:0]ddr3_dqs_p;
  wire [0:0]ddr3_odt;
  wire ddr3_ras_n;
  wire ddr3_reset_n;
  wire ddr3_we_n;
  wire [0:0]pcie_diff_clock_clk_n;
  wire [0:0]pcie_diff_clock_clk_p;
  wire pcie_perstn;
  wire [3:0]pcie_rxn;
  wire [3:0]pcie_rxp;
  wire [3:0]pcie_txn;
  wire [3:0]pcie_txp;
  wire sys_clk_clk_n;
  wire sys_clk_clk_p;
  wire sys_rst;

  System System_i
       (.HDMI_CLK(HDMI_CLK),
        .HDMI_DATA(HDMI_DATA),
        .HDMI_DE(HDMI_DE),
        .HDMI_HSYNC(HDMI_HSYNC),
        .HDMI_INTR(HDMI_INTR),
        .HDMI_SPDIF(HDMI_SPDIF),
        .HDMI_VSYNC(HDMI_VSYNC),
        .I2C_SCL(I2C_SCL),
        .I2C_SDA(I2C_SDA),
        .I2C_nRST(I2C_nRST),
        .ddr3_addr(ddr3_addr),
        .ddr3_ba(ddr3_ba),
        .ddr3_cas_n(ddr3_cas_n),
        .ddr3_ck_n(ddr3_ck_n),
        .ddr3_ck_p(ddr3_ck_p),
        .ddr3_cke(ddr3_cke),
        .ddr3_cs_n(ddr3_cs_n),
        .ddr3_dm(ddr3_dm),
        .ddr3_dq(ddr3_dq),
        .ddr3_dqs_n(ddr3_dqs_n),
        .ddr3_dqs_p(ddr3_dqs_p),
        .ddr3_odt(ddr3_odt),
        .ddr3_ras_n(ddr3_ras_n),
        .ddr3_reset_n(ddr3_reset_n),
        .ddr3_we_n(ddr3_we_n),
        .pcie_diff_clock_clk_n(pcie_diff_clock_clk_n),
        .pcie_diff_clock_clk_p(pcie_diff_clock_clk_p),
        .pcie_perstn(pcie_perstn),
        .pcie_rxn(pcie_rxn),
        .pcie_rxp(pcie_rxp),
        .pcie_txn(pcie_txn),
        .pcie_txp(pcie_txp),
        .sys_clk_clk_n(sys_clk_clk_n),
        .sys_clk_clk_p(sys_clk_clk_p),
        .sys_rst(sys_rst));
endmodule
