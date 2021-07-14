//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.1 (win64) Build 3247384 Thu Jun 10 19:36:33 MDT 2021
//Date        : Wed Jul 14 11:24:43 2021
//Host        : DESKTOP-F0P52DA running 64-bit major release  (build 9200)
//Command     : generate_target System.bd
//Design      : System
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "System,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=System,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=37,numReposBlks=26,numNonXlnxBlks=0,numHierBlks=11,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,da_axi4_cnt=1,da_board_cnt=2,synth_mode=Global}" *) (* HW_HANDOFF = "System.hwdef" *) 
module System
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
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 ADDR" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ddr3, AXI_ARBITRATION_SCHEME TDM, BURST_LENGTH 8, CAN_DEBUG false, CAS_LATENCY 11, CAS_WRITE_LATENCY 11, CS_ENABLED true, DATA_MASK_ENABLED true, DATA_WIDTH 8, MEMORY_TYPE COMPONENTS, MEM_ADDR_MAP ROW_COLUMN_BANK, SLOT Single, TIMEPERIOD_PS 1250" *) output [13:0]ddr3_addr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 BA" *) output [2:0]ddr3_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 CAS_N" *) output ddr3_cas_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 CK_N" *) output [0:0]ddr3_ck_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 CK_P" *) output [0:0]ddr3_ck_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 CKE" *) output [0:0]ddr3_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 CS_N" *) output [0:0]ddr3_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 DM" *) output [7:0]ddr3_dm;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 DQ" *) inout [63:0]ddr3_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 DQS_N" *) inout [7:0]ddr3_dqs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 DQS_P" *) inout [7:0]ddr3_dqs_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 ODT" *) output [0:0]ddr3_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 RAS_N" *) output ddr3_ras_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 RESET_N" *) output ddr3_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddrx:1.0 ddr3 WE_N" *) output ddr3_we_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pcie_diff_clock CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME pcie_diff_clock, CAN_DEBUG false, FREQ_HZ 100000000" *) input [0:0]pcie_diff_clock_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 pcie_diff_clock CLK_P" *) input [0:0]pcie_diff_clock_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.PCIE_PERSTN RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.PCIE_PERSTN, INSERT_VIP 0, POLARITY ACTIVE_LOW" *) input pcie_perstn;
  (* X_INTERFACE_INFO = "xilinx.com:interface:pcie_7x_mgt:1.0 pcie rxn" *) input [3:0]pcie_rxn;
  (* X_INTERFACE_INFO = "xilinx.com:interface:pcie_7x_mgt:1.0 pcie rxp" *) input [3:0]pcie_rxp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:pcie_7x_mgt:1.0 pcie txn" *) output [3:0]pcie_txn;
  (* X_INTERFACE_INFO = "xilinx.com:interface:pcie_7x_mgt:1.0 pcie txp" *) output [3:0]pcie_txp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_clk CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME sys_clk, CAN_DEBUG false, FREQ_HZ 200000000" *) input sys_clk_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_clk CLK_P" *) input sys_clk_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.SYS_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.SYS_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input sys_rst;

  wire [0:0]ARESETN_1;
  wire AXI_PCIe_INTX_MSI_Grant;
  wire AXI_PCIe_MSI_enable;
  wire [31:0]AXI_PCIe_M_AXI_ARADDR;
  wire [1:0]AXI_PCIe_M_AXI_ARBURST;
  wire [3:0]AXI_PCIe_M_AXI_ARCACHE;
  wire [7:0]AXI_PCIe_M_AXI_ARLEN;
  wire AXI_PCIe_M_AXI_ARLOCK;
  wire [2:0]AXI_PCIe_M_AXI_ARPROT;
  wire AXI_PCIe_M_AXI_ARREADY;
  wire [2:0]AXI_PCIe_M_AXI_ARSIZE;
  wire AXI_PCIe_M_AXI_ARVALID;
  wire [31:0]AXI_PCIe_M_AXI_AWADDR;
  wire [1:0]AXI_PCIe_M_AXI_AWBURST;
  wire [3:0]AXI_PCIe_M_AXI_AWCACHE;
  wire [7:0]AXI_PCIe_M_AXI_AWLEN;
  wire AXI_PCIe_M_AXI_AWLOCK;
  wire [2:0]AXI_PCIe_M_AXI_AWPROT;
  wire AXI_PCIe_M_AXI_AWREADY;
  wire [2:0]AXI_PCIe_M_AXI_AWSIZE;
  wire AXI_PCIe_M_AXI_AWVALID;
  wire AXI_PCIe_M_AXI_BREADY;
  wire [1:0]AXI_PCIe_M_AXI_BRESP;
  wire AXI_PCIe_M_AXI_BVALID;
  wire [127:0]AXI_PCIe_M_AXI_RDATA;
  wire AXI_PCIe_M_AXI_RLAST;
  wire AXI_PCIe_M_AXI_RREADY;
  wire [1:0]AXI_PCIe_M_AXI_RRESP;
  wire AXI_PCIe_M_AXI_RVALID;
  wire [127:0]AXI_PCIe_M_AXI_WDATA;
  wire AXI_PCIe_M_AXI_WLAST;
  wire AXI_PCIe_M_AXI_WREADY;
  wire [15:0]AXI_PCIe_M_AXI_WSTRB;
  wire AXI_PCIe_M_AXI_WVALID;
  wire AXI_PCIe_axi_aclk_out;
  wire AXI_PCIe_axi_ctl_aclk_out;
  wire AXI_PCIe_mmcm_lock;
  wire [3:0]AXI_PCIe_pcie_7x_mgt_rxn;
  wire [3:0]AXI_PCIe_pcie_7x_mgt_rxp;
  wire [3:0]AXI_PCIe_pcie_7x_mgt_txn;
  wire [3:0]AXI_PCIe_pcie_7x_mgt_txp;
  wire [13:0]DDR3_SDRAM_DDR3_ADDR;
  wire [2:0]DDR3_SDRAM_DDR3_BA;
  wire DDR3_SDRAM_DDR3_CAS_N;
  wire [0:0]DDR3_SDRAM_DDR3_CKE;
  wire [0:0]DDR3_SDRAM_DDR3_CK_N;
  wire [0:0]DDR3_SDRAM_DDR3_CK_P;
  wire [0:0]DDR3_SDRAM_DDR3_CS_N;
  wire [7:0]DDR3_SDRAM_DDR3_DM;
  wire [63:0]DDR3_SDRAM_DDR3_DQ;
  wire [7:0]DDR3_SDRAM_DDR3_DQS_N;
  wire [7:0]DDR3_SDRAM_DDR3_DQS_P;
  wire [0:0]DDR3_SDRAM_DDR3_ODT;
  wire DDR3_SDRAM_DDR3_RAS_N;
  wire DDR3_SDRAM_DDR3_RESET_N;
  wire DDR3_SDRAM_DDR3_WE_N;
  wire DDR3_SDRAM_mmcm_locked;
  wire DDR3_SDRAM_ui_clk;
  wire DDR3_SDRAM_ui_clk_sync_rst;
  wire HDMI_INTR_1;
  wire [4:0]MSI_Generator_0_MSI_NUM;
  wire MSI_Generator_0_MSI_REQ;
  wire Net;
  wire Net1;
  wire [0:0]PCIe_diff_clk_I_IBUF_OUT;
  wire [0:0]S00_ARESETN_1;
  wire SYS_CLK_1_CLK_N;
  wire SYS_CLK_1_CLK_P;
  wire [31:0]axi4_0_M00_AXI_ARADDR;
  wire [1:0]axi4_0_M00_AXI_ARBURST;
  wire [3:0]axi4_0_M00_AXI_ARCACHE;
  wire [2:0]axi4_0_M00_AXI_ARID;
  wire [7:0]axi4_0_M00_AXI_ARLEN;
  wire axi4_0_M00_AXI_ARLOCK;
  wire [2:0]axi4_0_M00_AXI_ARPROT;
  wire [3:0]axi4_0_M00_AXI_ARQOS;
  wire axi4_0_M00_AXI_ARREADY;
  wire [2:0]axi4_0_M00_AXI_ARSIZE;
  wire axi4_0_M00_AXI_ARVALID;
  wire [31:0]axi4_0_M00_AXI_AWADDR;
  wire [1:0]axi4_0_M00_AXI_AWBURST;
  wire [3:0]axi4_0_M00_AXI_AWCACHE;
  wire [2:0]axi4_0_M00_AXI_AWID;
  wire [7:0]axi4_0_M00_AXI_AWLEN;
  wire axi4_0_M00_AXI_AWLOCK;
  wire [2:0]axi4_0_M00_AXI_AWPROT;
  wire [3:0]axi4_0_M00_AXI_AWQOS;
  wire axi4_0_M00_AXI_AWREADY;
  wire [2:0]axi4_0_M00_AXI_AWSIZE;
  wire axi4_0_M00_AXI_AWVALID;
  wire [2:0]axi4_0_M00_AXI_BID;
  wire axi4_0_M00_AXI_BREADY;
  wire [1:0]axi4_0_M00_AXI_BRESP;
  wire axi4_0_M00_AXI_BVALID;
  wire [511:0]axi4_0_M00_AXI_RDATA;
  wire [2:0]axi4_0_M00_AXI_RID;
  wire axi4_0_M00_AXI_RLAST;
  wire axi4_0_M00_AXI_RREADY;
  wire [1:0]axi4_0_M00_AXI_RRESP;
  wire axi4_0_M00_AXI_RVALID;
  wire [511:0]axi4_0_M00_AXI_WDATA;
  wire axi4_0_M00_AXI_WLAST;
  wire axi4_0_M00_AXI_WREADY;
  wire [63:0]axi4_0_M00_AXI_WSTRB;
  wire axi4_0_M00_AXI_WVALID;
  wire [31:0]axi_apb_bridge_0_APB_M2_PRDATA;
  wire axi_apb_bridge_0_APB_M2_PREADY;
  wire [1:1]axi_apb_bridge_0_APB_M2_PSEL;
  wire axi_apb_bridge_0_APB_M2_PSLVERR;
  wire [31:0]axi_apb_bridge_0_APB_M_PADDR;
  wire axi_apb_bridge_0_APB_M_PENABLE;
  wire [31:0]axi_apb_bridge_0_APB_M_PRDATA;
  wire axi_apb_bridge_0_APB_M_PREADY;
  wire [0:0]axi_apb_bridge_0_APB_M_PSEL;
  wire axi_apb_bridge_0_APB_M_PSLVERR;
  wire [31:0]axi_apb_bridge_0_APB_M_PWDATA;
  wire axi_apb_bridge_0_APB_M_PWRITE;
  wire dut_wrapper_0_INTR;
  wire [31:0]dut_wrapper_0_M_AXI_ARADDR;
  wire [1:0]dut_wrapper_0_M_AXI_ARBURST;
  wire [3:0]dut_wrapper_0_M_AXI_ARCACHE;
  wire [0:0]dut_wrapper_0_M_AXI_ARID;
  wire [7:0]dut_wrapper_0_M_AXI_ARLEN;
  wire dut_wrapper_0_M_AXI_ARLOCK;
  wire [2:0]dut_wrapper_0_M_AXI_ARPROT;
  wire [3:0]dut_wrapper_0_M_AXI_ARQOS;
  wire dut_wrapper_0_M_AXI_ARREADY;
  wire [2:0]dut_wrapper_0_M_AXI_ARSIZE;
  wire dut_wrapper_0_M_AXI_ARVALID;
  wire [31:0]dut_wrapper_0_M_AXI_AWADDR;
  wire [1:0]dut_wrapper_0_M_AXI_AWBURST;
  wire [3:0]dut_wrapper_0_M_AXI_AWCACHE;
  wire [0:0]dut_wrapper_0_M_AXI_AWID;
  wire [7:0]dut_wrapper_0_M_AXI_AWLEN;
  wire dut_wrapper_0_M_AXI_AWLOCK;
  wire [2:0]dut_wrapper_0_M_AXI_AWPROT;
  wire [3:0]dut_wrapper_0_M_AXI_AWQOS;
  wire dut_wrapper_0_M_AXI_AWREADY;
  wire [2:0]dut_wrapper_0_M_AXI_AWSIZE;
  wire dut_wrapper_0_M_AXI_AWVALID;
  wire [0:0]dut_wrapper_0_M_AXI_BID;
  wire dut_wrapper_0_M_AXI_BREADY;
  wire [1:0]dut_wrapper_0_M_AXI_BRESP;
  wire dut_wrapper_0_M_AXI_BVALID;
  wire [511:0]dut_wrapper_0_M_AXI_RDATA;
  wire [0:0]dut_wrapper_0_M_AXI_RID;
  wire dut_wrapper_0_M_AXI_RLAST;
  wire dut_wrapper_0_M_AXI_RREADY;
  wire [1:0]dut_wrapper_0_M_AXI_RRESP;
  wire dut_wrapper_0_M_AXI_RVALID;
  wire [511:0]dut_wrapper_0_M_AXI_WDATA;
  wire dut_wrapper_0_M_AXI_WLAST;
  wire dut_wrapper_0_M_AXI_WREADY;
  wire [63:0]dut_wrapper_0_M_AXI_WSTRB;
  wire dut_wrapper_0_M_AXI_WVALID;
  wire [31:0]from_pcie_M00_AXI_ARADDR;
  wire [1:0]from_pcie_M00_AXI_ARBURST;
  wire [3:0]from_pcie_M00_AXI_ARCACHE;
  wire [7:0]from_pcie_M00_AXI_ARLEN;
  wire [0:0]from_pcie_M00_AXI_ARLOCK;
  wire [2:0]from_pcie_M00_AXI_ARPROT;
  wire [3:0]from_pcie_M00_AXI_ARQOS;
  wire from_pcie_M00_AXI_ARREADY;
  wire [3:0]from_pcie_M00_AXI_ARREGION;
  wire [2:0]from_pcie_M00_AXI_ARSIZE;
  wire from_pcie_M00_AXI_ARVALID;
  wire [31:0]from_pcie_M00_AXI_AWADDR;
  wire [1:0]from_pcie_M00_AXI_AWBURST;
  wire [3:0]from_pcie_M00_AXI_AWCACHE;
  wire [7:0]from_pcie_M00_AXI_AWLEN;
  wire [0:0]from_pcie_M00_AXI_AWLOCK;
  wire [2:0]from_pcie_M00_AXI_AWPROT;
  wire [3:0]from_pcie_M00_AXI_AWQOS;
  wire from_pcie_M00_AXI_AWREADY;
  wire [3:0]from_pcie_M00_AXI_AWREGION;
  wire [2:0]from_pcie_M00_AXI_AWSIZE;
  wire from_pcie_M00_AXI_AWVALID;
  wire from_pcie_M00_AXI_BREADY;
  wire [1:0]from_pcie_M00_AXI_BRESP;
  wire from_pcie_M00_AXI_BVALID;
  wire [511:0]from_pcie_M00_AXI_RDATA;
  wire from_pcie_M00_AXI_RLAST;
  wire from_pcie_M00_AXI_RREADY;
  wire [1:0]from_pcie_M00_AXI_RRESP;
  wire from_pcie_M00_AXI_RVALID;
  wire [511:0]from_pcie_M00_AXI_WDATA;
  wire from_pcie_M00_AXI_WLAST;
  wire from_pcie_M00_AXI_WREADY;
  wire [63:0]from_pcie_M00_AXI_WSTRB;
  wire from_pcie_M00_AXI_WVALID;
  wire [31:0]from_pcie_M01_AXI_ARADDR;
  wire from_pcie_M01_AXI_ARREADY;
  wire from_pcie_M01_AXI_ARVALID;
  wire [31:0]from_pcie_M01_AXI_AWADDR;
  wire from_pcie_M01_AXI_AWREADY;
  wire from_pcie_M01_AXI_AWVALID;
  wire from_pcie_M01_AXI_BREADY;
  wire [1:0]from_pcie_M01_AXI_BRESP;
  wire from_pcie_M01_AXI_BVALID;
  wire [31:0]from_pcie_M01_AXI_RDATA;
  wire from_pcie_M01_AXI_RREADY;
  wire [1:0]from_pcie_M01_AXI_RRESP;
  wire from_pcie_M01_AXI_RVALID;
  wire [31:0]from_pcie_M01_AXI_WDATA;
  wire from_pcie_M01_AXI_WREADY;
  wire from_pcie_M01_AXI_WVALID;
  wire [31:0]from_pcie_M02_AXI_ARADDR;
  wire from_pcie_M02_AXI_ARREADY;
  wire from_pcie_M02_AXI_ARVALID;
  wire [31:0]from_pcie_M02_AXI_AWADDR;
  wire from_pcie_M02_AXI_AWREADY;
  wire from_pcie_M02_AXI_AWVALID;
  wire from_pcie_M02_AXI_BREADY;
  wire [1:0]from_pcie_M02_AXI_BRESP;
  wire from_pcie_M02_AXI_BVALID;
  wire [31:0]from_pcie_M02_AXI_RDATA;
  wire from_pcie_M02_AXI_RREADY;
  wire [1:0]from_pcie_M02_AXI_RRESP;
  wire from_pcie_M02_AXI_RVALID;
  wire [31:0]from_pcie_M02_AXI_WDATA;
  wire from_pcie_M02_AXI_WREADY;
  wire [3:0]from_pcie_M02_AXI_WSTRB;
  wire from_pcie_M02_AXI_WVALID;
  wire hdmi_controller_0_HDMI_CLK;
  wire [35:0]hdmi_controller_0_HDMI_DATA;
  wire hdmi_controller_0_HDMI_DE;
  wire hdmi_controller_0_HDMI_HSYNC;
  wire hdmi_controller_0_HDMI_SPDIF;
  wire hdmi_controller_0_HDMI_VSYNC;
  wire hdmi_controller_0_I2C_nRST;
  wire hdmi_controller_0_INTR;
  wire [31:0]hdmi_controller_0_M_AXI_ARADDR;
  wire [1:0]hdmi_controller_0_M_AXI_ARBURST;
  wire [3:0]hdmi_controller_0_M_AXI_ARCACHE;
  wire [0:0]hdmi_controller_0_M_AXI_ARID;
  wire [7:0]hdmi_controller_0_M_AXI_ARLEN;
  wire hdmi_controller_0_M_AXI_ARLOCK;
  wire [2:0]hdmi_controller_0_M_AXI_ARPROT;
  wire [3:0]hdmi_controller_0_M_AXI_ARQOS;
  wire hdmi_controller_0_M_AXI_ARREADY;
  wire [3:0]hdmi_controller_0_M_AXI_ARREGION;
  wire [2:0]hdmi_controller_0_M_AXI_ARSIZE;
  wire hdmi_controller_0_M_AXI_ARVALID;
  wire [31:0]hdmi_controller_0_M_AXI_AWADDR;
  wire [1:0]hdmi_controller_0_M_AXI_AWBURST;
  wire [3:0]hdmi_controller_0_M_AXI_AWCACHE;
  wire [0:0]hdmi_controller_0_M_AXI_AWID;
  wire [7:0]hdmi_controller_0_M_AXI_AWLEN;
  wire hdmi_controller_0_M_AXI_AWLOCK;
  wire [2:0]hdmi_controller_0_M_AXI_AWPROT;
  wire [3:0]hdmi_controller_0_M_AXI_AWQOS;
  wire hdmi_controller_0_M_AXI_AWREADY;
  wire [3:0]hdmi_controller_0_M_AXI_AWREGION;
  wire [2:0]hdmi_controller_0_M_AXI_AWSIZE;
  wire hdmi_controller_0_M_AXI_AWVALID;
  wire [0:0]hdmi_controller_0_M_AXI_BID;
  wire hdmi_controller_0_M_AXI_BREADY;
  wire [1:0]hdmi_controller_0_M_AXI_BRESP;
  wire hdmi_controller_0_M_AXI_BVALID;
  wire [511:0]hdmi_controller_0_M_AXI_RDATA;
  wire [0:0]hdmi_controller_0_M_AXI_RID;
  wire hdmi_controller_0_M_AXI_RLAST;
  wire hdmi_controller_0_M_AXI_RREADY;
  wire [1:0]hdmi_controller_0_M_AXI_RRESP;
  wire hdmi_controller_0_M_AXI_RVALID;
  wire [511:0]hdmi_controller_0_M_AXI_WDATA;
  wire hdmi_controller_0_M_AXI_WLAST;
  wire hdmi_controller_0_M_AXI_WREADY;
  wire [63:0]hdmi_controller_0_M_AXI_WSTRB;
  wire hdmi_controller_0_M_AXI_WVALID;
  wire [0:0]pcie_diff_clk_1_CLK_N;
  wire [0:0]pcie_diff_clk_1_CLK_P;
  wire pcie_perstn_1;
  wire reset_1;
  wire [0:0]rst_DDR3_SDRAM_200M_interconnect_aresetn;
  wire [0:0]rst_DDR3_SDRAM_200M_peripheral_aresetn;
  wire [31:0]to_memory_M01_AXI_ARADDR;
  wire [1:0]to_memory_M01_AXI_ARBURST;
  wire [7:0]to_memory_M01_AXI_ARLEN;
  wire to_memory_M01_AXI_ARREADY;
  wire [3:0]to_memory_M01_AXI_ARREGION;
  wire [2:0]to_memory_M01_AXI_ARSIZE;
  wire to_memory_M01_AXI_ARVALID;
  wire [31:0]to_memory_M01_AXI_AWADDR;
  wire [1:0]to_memory_M01_AXI_AWBURST;
  wire [7:0]to_memory_M01_AXI_AWLEN;
  wire to_memory_M01_AXI_AWREADY;
  wire [3:0]to_memory_M01_AXI_AWREGION;
  wire [2:0]to_memory_M01_AXI_AWSIZE;
  wire to_memory_M01_AXI_AWVALID;
  wire to_memory_M01_AXI_BREADY;
  wire [1:0]to_memory_M01_AXI_BRESP;
  wire to_memory_M01_AXI_BVALID;
  wire [127:0]to_memory_M01_AXI_RDATA;
  wire to_memory_M01_AXI_RLAST;
  wire to_memory_M01_AXI_RREADY;
  wire [1:0]to_memory_M01_AXI_RRESP;
  wire to_memory_M01_AXI_RVALID;
  wire [127:0]to_memory_M01_AXI_WDATA;
  wire to_memory_M01_AXI_WLAST;
  wire to_memory_M01_AXI_WREADY;
  wire [15:0]to_memory_M01_AXI_WSTRB;
  wire to_memory_M01_AXI_WVALID;
  wire [0:0]util_vector_logic_0_Res;

  assign AXI_PCIe_pcie_7x_mgt_rxn = pcie_rxn[3:0];
  assign AXI_PCIe_pcie_7x_mgt_rxp = pcie_rxp[3:0];
  assign HDMI_CLK = hdmi_controller_0_HDMI_CLK;
  assign HDMI_DATA[35:0] = hdmi_controller_0_HDMI_DATA;
  assign HDMI_DE = hdmi_controller_0_HDMI_DE;
  assign HDMI_HSYNC = hdmi_controller_0_HDMI_HSYNC;
  assign HDMI_INTR_1 = HDMI_INTR;
  assign HDMI_SPDIF = hdmi_controller_0_HDMI_SPDIF;
  assign HDMI_VSYNC = hdmi_controller_0_HDMI_VSYNC;
  assign I2C_nRST = hdmi_controller_0_I2C_nRST;
  assign SYS_CLK_1_CLK_N = sys_clk_clk_n;
  assign SYS_CLK_1_CLK_P = sys_clk_clk_p;
  assign ddr3_addr[13:0] = DDR3_SDRAM_DDR3_ADDR;
  assign ddr3_ba[2:0] = DDR3_SDRAM_DDR3_BA;
  assign ddr3_cas_n = DDR3_SDRAM_DDR3_CAS_N;
  assign ddr3_ck_n[0] = DDR3_SDRAM_DDR3_CK_N;
  assign ddr3_ck_p[0] = DDR3_SDRAM_DDR3_CK_P;
  assign ddr3_cke[0] = DDR3_SDRAM_DDR3_CKE;
  assign ddr3_cs_n[0] = DDR3_SDRAM_DDR3_CS_N;
  assign ddr3_dm[7:0] = DDR3_SDRAM_DDR3_DM;
  assign ddr3_odt[0] = DDR3_SDRAM_DDR3_ODT;
  assign ddr3_ras_n = DDR3_SDRAM_DDR3_RAS_N;
  assign ddr3_reset_n = DDR3_SDRAM_DDR3_RESET_N;
  assign ddr3_we_n = DDR3_SDRAM_DDR3_WE_N;
  assign pcie_diff_clk_1_CLK_N = pcie_diff_clock_clk_n[0];
  assign pcie_diff_clk_1_CLK_P = pcie_diff_clock_clk_p[0];
  assign pcie_perstn_1 = pcie_perstn;
  assign pcie_txn[3:0] = AXI_PCIe_pcie_7x_mgt_txn;
  assign pcie_txp[3:0] = AXI_PCIe_pcie_7x_mgt_txp;
  assign reset_1 = sys_rst;
  System_axi_pcie_0_1 AXI_PCIe
       (.INTX_MSI_Grant(AXI_PCIe_INTX_MSI_Grant),
        .INTX_MSI_Request(MSI_Generator_0_MSI_REQ),
        .MSI_Vector_Num(MSI_Generator_0_MSI_NUM),
        .MSI_enable(AXI_PCIe_MSI_enable),
        .REFCLK(PCIe_diff_clk_I_IBUF_OUT),
        .axi_aclk_out(AXI_PCIe_axi_aclk_out),
        .axi_aresetn(S00_ARESETN_1),
        .axi_ctl_aclk_out(AXI_PCIe_axi_ctl_aclk_out),
        .m_axi_araddr(AXI_PCIe_M_AXI_ARADDR),
        .m_axi_arburst(AXI_PCIe_M_AXI_ARBURST),
        .m_axi_arcache(AXI_PCIe_M_AXI_ARCACHE),
        .m_axi_arlen(AXI_PCIe_M_AXI_ARLEN),
        .m_axi_arlock(AXI_PCIe_M_AXI_ARLOCK),
        .m_axi_arprot(AXI_PCIe_M_AXI_ARPROT),
        .m_axi_arready(AXI_PCIe_M_AXI_ARREADY),
        .m_axi_arsize(AXI_PCIe_M_AXI_ARSIZE),
        .m_axi_arvalid(AXI_PCIe_M_AXI_ARVALID),
        .m_axi_awaddr(AXI_PCIe_M_AXI_AWADDR),
        .m_axi_awburst(AXI_PCIe_M_AXI_AWBURST),
        .m_axi_awcache(AXI_PCIe_M_AXI_AWCACHE),
        .m_axi_awlen(AXI_PCIe_M_AXI_AWLEN),
        .m_axi_awlock(AXI_PCIe_M_AXI_AWLOCK),
        .m_axi_awprot(AXI_PCIe_M_AXI_AWPROT),
        .m_axi_awready(AXI_PCIe_M_AXI_AWREADY),
        .m_axi_awsize(AXI_PCIe_M_AXI_AWSIZE),
        .m_axi_awvalid(AXI_PCIe_M_AXI_AWVALID),
        .m_axi_bready(AXI_PCIe_M_AXI_BREADY),
        .m_axi_bresp(AXI_PCIe_M_AXI_BRESP),
        .m_axi_bvalid(AXI_PCIe_M_AXI_BVALID),
        .m_axi_rdata(AXI_PCIe_M_AXI_RDATA),
        .m_axi_rlast(AXI_PCIe_M_AXI_RLAST),
        .m_axi_rready(AXI_PCIe_M_AXI_RREADY),
        .m_axi_rresp(AXI_PCIe_M_AXI_RRESP),
        .m_axi_rvalid(AXI_PCIe_M_AXI_RVALID),
        .m_axi_wdata(AXI_PCIe_M_AXI_WDATA),
        .m_axi_wlast(AXI_PCIe_M_AXI_WLAST),
        .m_axi_wready(AXI_PCIe_M_AXI_WREADY),
        .m_axi_wstrb(AXI_PCIe_M_AXI_WSTRB),
        .m_axi_wvalid(AXI_PCIe_M_AXI_WVALID),
        .mmcm_lock(AXI_PCIe_mmcm_lock),
        .pci_exp_rxn(AXI_PCIe_pcie_7x_mgt_rxn),
        .pci_exp_rxp(AXI_PCIe_pcie_7x_mgt_rxp),
        .pci_exp_txn(AXI_PCIe_pcie_7x_mgt_txn),
        .pci_exp_txp(AXI_PCIe_pcie_7x_mgt_txp),
        .s_axi_araddr(to_memory_M01_AXI_ARADDR),
        .s_axi_arburst(to_memory_M01_AXI_ARBURST),
        .s_axi_arid({1'b0,1'b0,1'b0}),
        .s_axi_arlen(to_memory_M01_AXI_ARLEN),
        .s_axi_arready(to_memory_M01_AXI_ARREADY),
        .s_axi_arregion(to_memory_M01_AXI_ARREGION),
        .s_axi_arsize(to_memory_M01_AXI_ARSIZE),
        .s_axi_arvalid(to_memory_M01_AXI_ARVALID),
        .s_axi_awaddr(to_memory_M01_AXI_AWADDR),
        .s_axi_awburst(to_memory_M01_AXI_AWBURST),
        .s_axi_awid({1'b0,1'b0,1'b0}),
        .s_axi_awlen(to_memory_M01_AXI_AWLEN),
        .s_axi_awready(to_memory_M01_AXI_AWREADY),
        .s_axi_awregion(to_memory_M01_AXI_AWREGION),
        .s_axi_awsize(to_memory_M01_AXI_AWSIZE),
        .s_axi_awvalid(to_memory_M01_AXI_AWVALID),
        .s_axi_bready(to_memory_M01_AXI_BREADY),
        .s_axi_bresp(to_memory_M01_AXI_BRESP),
        .s_axi_bvalid(to_memory_M01_AXI_BVALID),
        .s_axi_ctl_araddr(from_pcie_M02_AXI_ARADDR),
        .s_axi_ctl_arready(from_pcie_M02_AXI_ARREADY),
        .s_axi_ctl_arvalid(from_pcie_M02_AXI_ARVALID),
        .s_axi_ctl_awaddr(from_pcie_M02_AXI_AWADDR),
        .s_axi_ctl_awready(from_pcie_M02_AXI_AWREADY),
        .s_axi_ctl_awvalid(from_pcie_M02_AXI_AWVALID),
        .s_axi_ctl_bready(from_pcie_M02_AXI_BREADY),
        .s_axi_ctl_bresp(from_pcie_M02_AXI_BRESP),
        .s_axi_ctl_bvalid(from_pcie_M02_AXI_BVALID),
        .s_axi_ctl_rdata(from_pcie_M02_AXI_RDATA),
        .s_axi_ctl_rready(from_pcie_M02_AXI_RREADY),
        .s_axi_ctl_rresp(from_pcie_M02_AXI_RRESP),
        .s_axi_ctl_rvalid(from_pcie_M02_AXI_RVALID),
        .s_axi_ctl_wdata(from_pcie_M02_AXI_WDATA),
        .s_axi_ctl_wready(from_pcie_M02_AXI_WREADY),
        .s_axi_ctl_wstrb(from_pcie_M02_AXI_WSTRB),
        .s_axi_ctl_wvalid(from_pcie_M02_AXI_WVALID),
        .s_axi_rdata(to_memory_M01_AXI_RDATA),
        .s_axi_rlast(to_memory_M01_AXI_RLAST),
        .s_axi_rready(to_memory_M01_AXI_RREADY),
        .s_axi_rresp(to_memory_M01_AXI_RRESP),
        .s_axi_rvalid(to_memory_M01_AXI_RVALID),
        .s_axi_wdata(to_memory_M01_AXI_WDATA),
        .s_axi_wlast(to_memory_M01_AXI_WLAST),
        .s_axi_wready(to_memory_M01_AXI_WREADY),
        .s_axi_wstrb(to_memory_M01_AXI_WSTRB),
        .s_axi_wvalid(to_memory_M01_AXI_WVALID));
  System_mig_7series_0_0 DDR3_SDRAM
       (.aresetn(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .ddr3_addr(DDR3_SDRAM_DDR3_ADDR),
        .ddr3_ba(DDR3_SDRAM_DDR3_BA),
        .ddr3_cas_n(DDR3_SDRAM_DDR3_CAS_N),
        .ddr3_ck_n(DDR3_SDRAM_DDR3_CK_N),
        .ddr3_ck_p(DDR3_SDRAM_DDR3_CK_P),
        .ddr3_cke(DDR3_SDRAM_DDR3_CKE),
        .ddr3_cs_n(DDR3_SDRAM_DDR3_CS_N),
        .ddr3_dm(DDR3_SDRAM_DDR3_DM),
        .ddr3_dq(ddr3_dq[63:0]),
        .ddr3_dqs_n(ddr3_dqs_n[7:0]),
        .ddr3_dqs_p(ddr3_dqs_p[7:0]),
        .ddr3_odt(DDR3_SDRAM_DDR3_ODT),
        .ddr3_ras_n(DDR3_SDRAM_DDR3_RAS_N),
        .ddr3_reset_n(DDR3_SDRAM_DDR3_RESET_N),
        .ddr3_we_n(DDR3_SDRAM_DDR3_WE_N),
        .mmcm_locked(DDR3_SDRAM_mmcm_locked),
        .s_axi_araddr(axi4_0_M00_AXI_ARADDR[29:0]),
        .s_axi_arburst(axi4_0_M00_AXI_ARBURST),
        .s_axi_arcache(axi4_0_M00_AXI_ARCACHE),
        .s_axi_arid(axi4_0_M00_AXI_ARID),
        .s_axi_arlen(axi4_0_M00_AXI_ARLEN),
        .s_axi_arlock(axi4_0_M00_AXI_ARLOCK),
        .s_axi_arprot(axi4_0_M00_AXI_ARPROT),
        .s_axi_arqos(axi4_0_M00_AXI_ARQOS),
        .s_axi_arready(axi4_0_M00_AXI_ARREADY),
        .s_axi_arsize(axi4_0_M00_AXI_ARSIZE),
        .s_axi_arvalid(axi4_0_M00_AXI_ARVALID),
        .s_axi_awaddr(axi4_0_M00_AXI_AWADDR[29:0]),
        .s_axi_awburst(axi4_0_M00_AXI_AWBURST),
        .s_axi_awcache(axi4_0_M00_AXI_AWCACHE),
        .s_axi_awid(axi4_0_M00_AXI_AWID),
        .s_axi_awlen(axi4_0_M00_AXI_AWLEN),
        .s_axi_awlock(axi4_0_M00_AXI_AWLOCK),
        .s_axi_awprot(axi4_0_M00_AXI_AWPROT),
        .s_axi_awqos(axi4_0_M00_AXI_AWQOS),
        .s_axi_awready(axi4_0_M00_AXI_AWREADY),
        .s_axi_awsize(axi4_0_M00_AXI_AWSIZE),
        .s_axi_awvalid(axi4_0_M00_AXI_AWVALID),
        .s_axi_bid(axi4_0_M00_AXI_BID),
        .s_axi_bready(axi4_0_M00_AXI_BREADY),
        .s_axi_bresp(axi4_0_M00_AXI_BRESP),
        .s_axi_bvalid(axi4_0_M00_AXI_BVALID),
        .s_axi_rdata(axi4_0_M00_AXI_RDATA),
        .s_axi_rid(axi4_0_M00_AXI_RID),
        .s_axi_rlast(axi4_0_M00_AXI_RLAST),
        .s_axi_rready(axi4_0_M00_AXI_RREADY),
        .s_axi_rresp(axi4_0_M00_AXI_RRESP),
        .s_axi_rvalid(axi4_0_M00_AXI_RVALID),
        .s_axi_wdata(axi4_0_M00_AXI_WDATA),
        .s_axi_wlast(axi4_0_M00_AXI_WLAST),
        .s_axi_wready(axi4_0_M00_AXI_WREADY),
        .s_axi_wstrb(axi4_0_M00_AXI_WSTRB),
        .s_axi_wvalid(axi4_0_M00_AXI_WVALID),
        .sys_clk_n(SYS_CLK_1_CLK_N),
        .sys_clk_p(SYS_CLK_1_CLK_P),
        .sys_rst(reset_1),
        .ui_clk(DDR3_SDRAM_ui_clk),
        .ui_clk_sync_rst(DDR3_SDRAM_ui_clk_sync_rst));
  System_MSI_Generator_0_0 MSI_Generator_0
       (.CLK(AXI_PCIe_axi_aclk_out),
        .INTR_0(hdmi_controller_0_INTR),
        .INTR_1(dut_wrapper_0_INTR),
        .MSI_EN(AXI_PCIe_MSI_enable),
        .MSI_GRANT(AXI_PCIe_INTX_MSI_Grant),
        .MSI_NUM(MSI_Generator_0_MSI_NUM),
        .MSI_REQ(MSI_Generator_0_MSI_REQ),
        .nRST(S00_ARESETN_1));
  System_util_ds_buf_0_0 PCIe_diff_clk_I
       (.IBUF_DS_N(pcie_diff_clk_1_CLK_N),
        .IBUF_DS_P(pcie_diff_clk_1_CLK_P),
        .IBUF_OUT(PCIe_diff_clk_I_IBUF_OUT));
  System_axi_apb_bridge_0_0 axi_apb_bridge_0
       (.m_apb_paddr(axi_apb_bridge_0_APB_M_PADDR),
        .m_apb_penable(axi_apb_bridge_0_APB_M_PENABLE),
        .m_apb_prdata(axi_apb_bridge_0_APB_M_PRDATA),
        .m_apb_prdata2(axi_apb_bridge_0_APB_M2_PRDATA),
        .m_apb_pready({axi_apb_bridge_0_APB_M2_PREADY,axi_apb_bridge_0_APB_M_PREADY}),
        .m_apb_psel({axi_apb_bridge_0_APB_M2_PSEL,axi_apb_bridge_0_APB_M_PSEL}),
        .m_apb_pslverr({axi_apb_bridge_0_APB_M2_PSLVERR,axi_apb_bridge_0_APB_M_PSLVERR}),
        .m_apb_pwdata(axi_apb_bridge_0_APB_M_PWDATA),
        .m_apb_pwrite(axi_apb_bridge_0_APB_M_PWRITE),
        .s_axi_aclk(DDR3_SDRAM_ui_clk),
        .s_axi_araddr(from_pcie_M01_AXI_ARADDR),
        .s_axi_aresetn(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .s_axi_arready(from_pcie_M01_AXI_ARREADY),
        .s_axi_arvalid(from_pcie_M01_AXI_ARVALID),
        .s_axi_awaddr(from_pcie_M01_AXI_AWADDR),
        .s_axi_awready(from_pcie_M01_AXI_AWREADY),
        .s_axi_awvalid(from_pcie_M01_AXI_AWVALID),
        .s_axi_bready(from_pcie_M01_AXI_BREADY),
        .s_axi_bresp(from_pcie_M01_AXI_BRESP),
        .s_axi_bvalid(from_pcie_M01_AXI_BVALID),
        .s_axi_rdata(from_pcie_M01_AXI_RDATA),
        .s_axi_rready(from_pcie_M01_AXI_RREADY),
        .s_axi_rresp(from_pcie_M01_AXI_RRESP),
        .s_axi_rvalid(from_pcie_M01_AXI_RVALID),
        .s_axi_wdata(from_pcie_M01_AXI_WDATA),
        .s_axi_wready(from_pcie_M01_AXI_WREADY),
        .s_axi_wvalid(from_pcie_M01_AXI_WVALID));
  System_rst_DDR3_SDRAM_200M_0 bus_reset_0
       (.aux_reset_in(pcie_perstn_1),
        .dcm_locked(util_vector_logic_0_Res),
        .ext_reset_in(DDR3_SDRAM_ui_clk_sync_rst),
        .interconnect_aresetn(rst_DDR3_SDRAM_200M_interconnect_aresetn),
        .mb_debug_sys_rst(1'b0),
        .peripheral_aresetn(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .slowest_sync_clk(DDR3_SDRAM_ui_clk));
  System_dut_wrapper_0_0 dut_wrapper_0
       (.ACLK(DDR3_SDRAM_ui_clk),
        .INTR(dut_wrapper_0_INTR),
        .M_ARADDR(dut_wrapper_0_M_AXI_ARADDR),
        .M_ARBURST(dut_wrapper_0_M_AXI_ARBURST),
        .M_ARCACHE(dut_wrapper_0_M_AXI_ARCACHE),
        .M_ARID(dut_wrapper_0_M_AXI_ARID),
        .M_ARLEN(dut_wrapper_0_M_AXI_ARLEN),
        .M_ARLOCK(dut_wrapper_0_M_AXI_ARLOCK),
        .M_ARPROT(dut_wrapper_0_M_AXI_ARPROT),
        .M_ARQOS(dut_wrapper_0_M_AXI_ARQOS),
        .M_ARREADY(dut_wrapper_0_M_AXI_ARREADY),
        .M_ARSIZE(dut_wrapper_0_M_AXI_ARSIZE),
        .M_ARVALID(dut_wrapper_0_M_AXI_ARVALID),
        .M_AWADDR(dut_wrapper_0_M_AXI_AWADDR),
        .M_AWBURST(dut_wrapper_0_M_AXI_AWBURST),
        .M_AWCACHE(dut_wrapper_0_M_AXI_AWCACHE),
        .M_AWID(dut_wrapper_0_M_AXI_AWID),
        .M_AWLEN(dut_wrapper_0_M_AXI_AWLEN),
        .M_AWLOCK(dut_wrapper_0_M_AXI_AWLOCK),
        .M_AWPROT(dut_wrapper_0_M_AXI_AWPROT),
        .M_AWQOS(dut_wrapper_0_M_AXI_AWQOS),
        .M_AWREADY(dut_wrapper_0_M_AXI_AWREADY),
        .M_AWSIZE(dut_wrapper_0_M_AXI_AWSIZE),
        .M_AWVALID(dut_wrapper_0_M_AXI_AWVALID),
        .M_BID(dut_wrapper_0_M_AXI_BID),
        .M_BREADY(dut_wrapper_0_M_AXI_BREADY),
        .M_BRESP(dut_wrapper_0_M_AXI_BRESP),
        .M_BVALID(dut_wrapper_0_M_AXI_BVALID),
        .M_RDATA(dut_wrapper_0_M_AXI_RDATA),
        .M_RID(dut_wrapper_0_M_AXI_RID),
        .M_RLAST(dut_wrapper_0_M_AXI_RLAST),
        .M_RREADY(dut_wrapper_0_M_AXI_RREADY),
        .M_RRESP(dut_wrapper_0_M_AXI_RRESP),
        .M_RVALID(dut_wrapper_0_M_AXI_RVALID),
        .M_WDATA(dut_wrapper_0_M_AXI_WDATA),
        .M_WLAST(dut_wrapper_0_M_AXI_WLAST),
        .M_WREADY(dut_wrapper_0_M_AXI_WREADY),
        .M_WSTRB(dut_wrapper_0_M_AXI_WSTRB),
        .M_WVALID(dut_wrapper_0_M_AXI_WVALID),
        .S_PADDR(axi_apb_bridge_0_APB_M_PADDR[15:0]),
        .S_PENABLE(axi_apb_bridge_0_APB_M_PENABLE),
        .S_PRDATA(axi_apb_bridge_0_APB_M2_PRDATA),
        .S_PREADY(axi_apb_bridge_0_APB_M2_PREADY),
        .S_PSEL(axi_apb_bridge_0_APB_M2_PSEL),
        .S_PSLVERR(axi_apb_bridge_0_APB_M2_PSLVERR),
        .S_PWDATA(axi_apb_bridge_0_APB_M_PWDATA),
        .S_PWRITE(axi_apb_bridge_0_APB_M_PWRITE),
        .nRST(rst_DDR3_SDRAM_200M_peripheral_aresetn));
  System_axi_interconnect_0_0 from_pcie
       (.ACLK(AXI_PCIe_axi_aclk_out),
        .ARESETN(ARESETN_1),
        .M00_ACLK(DDR3_SDRAM_ui_clk),
        .M00_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .M00_AXI_araddr(from_pcie_M00_AXI_ARADDR),
        .M00_AXI_arburst(from_pcie_M00_AXI_ARBURST),
        .M00_AXI_arcache(from_pcie_M00_AXI_ARCACHE),
        .M00_AXI_arlen(from_pcie_M00_AXI_ARLEN),
        .M00_AXI_arlock(from_pcie_M00_AXI_ARLOCK),
        .M00_AXI_arprot(from_pcie_M00_AXI_ARPROT),
        .M00_AXI_arqos(from_pcie_M00_AXI_ARQOS),
        .M00_AXI_arready(from_pcie_M00_AXI_ARREADY),
        .M00_AXI_arregion(from_pcie_M00_AXI_ARREGION),
        .M00_AXI_arsize(from_pcie_M00_AXI_ARSIZE),
        .M00_AXI_arvalid(from_pcie_M00_AXI_ARVALID),
        .M00_AXI_awaddr(from_pcie_M00_AXI_AWADDR),
        .M00_AXI_awburst(from_pcie_M00_AXI_AWBURST),
        .M00_AXI_awcache(from_pcie_M00_AXI_AWCACHE),
        .M00_AXI_awlen(from_pcie_M00_AXI_AWLEN),
        .M00_AXI_awlock(from_pcie_M00_AXI_AWLOCK),
        .M00_AXI_awprot(from_pcie_M00_AXI_AWPROT),
        .M00_AXI_awqos(from_pcie_M00_AXI_AWQOS),
        .M00_AXI_awready(from_pcie_M00_AXI_AWREADY),
        .M00_AXI_awregion(from_pcie_M00_AXI_AWREGION),
        .M00_AXI_awsize(from_pcie_M00_AXI_AWSIZE),
        .M00_AXI_awvalid(from_pcie_M00_AXI_AWVALID),
        .M00_AXI_bready(from_pcie_M00_AXI_BREADY),
        .M00_AXI_bresp(from_pcie_M00_AXI_BRESP),
        .M00_AXI_bvalid(from_pcie_M00_AXI_BVALID),
        .M00_AXI_rdata(from_pcie_M00_AXI_RDATA),
        .M00_AXI_rlast(from_pcie_M00_AXI_RLAST),
        .M00_AXI_rready(from_pcie_M00_AXI_RREADY),
        .M00_AXI_rresp(from_pcie_M00_AXI_RRESP),
        .M00_AXI_rvalid(from_pcie_M00_AXI_RVALID),
        .M00_AXI_wdata(from_pcie_M00_AXI_WDATA),
        .M00_AXI_wlast(from_pcie_M00_AXI_WLAST),
        .M00_AXI_wready(from_pcie_M00_AXI_WREADY),
        .M00_AXI_wstrb(from_pcie_M00_AXI_WSTRB),
        .M00_AXI_wvalid(from_pcie_M00_AXI_WVALID),
        .M01_ACLK(DDR3_SDRAM_ui_clk),
        .M01_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .M01_AXI_araddr(from_pcie_M01_AXI_ARADDR),
        .M01_AXI_arready(from_pcie_M01_AXI_ARREADY),
        .M01_AXI_arvalid(from_pcie_M01_AXI_ARVALID),
        .M01_AXI_awaddr(from_pcie_M01_AXI_AWADDR),
        .M01_AXI_awready(from_pcie_M01_AXI_AWREADY),
        .M01_AXI_awvalid(from_pcie_M01_AXI_AWVALID),
        .M01_AXI_bready(from_pcie_M01_AXI_BREADY),
        .M01_AXI_bresp(from_pcie_M01_AXI_BRESP),
        .M01_AXI_bvalid(from_pcie_M01_AXI_BVALID),
        .M01_AXI_rdata(from_pcie_M01_AXI_RDATA),
        .M01_AXI_rready(from_pcie_M01_AXI_RREADY),
        .M01_AXI_rresp(from_pcie_M01_AXI_RRESP),
        .M01_AXI_rvalid(from_pcie_M01_AXI_RVALID),
        .M01_AXI_wdata(from_pcie_M01_AXI_WDATA),
        .M01_AXI_wready(from_pcie_M01_AXI_WREADY),
        .M01_AXI_wvalid(from_pcie_M01_AXI_WVALID),
        .M02_ACLK(AXI_PCIe_axi_ctl_aclk_out),
        .M02_ARESETN(S00_ARESETN_1),
        .M02_AXI_araddr(from_pcie_M02_AXI_ARADDR),
        .M02_AXI_arready(from_pcie_M02_AXI_ARREADY),
        .M02_AXI_arvalid(from_pcie_M02_AXI_ARVALID),
        .M02_AXI_awaddr(from_pcie_M02_AXI_AWADDR),
        .M02_AXI_awready(from_pcie_M02_AXI_AWREADY),
        .M02_AXI_awvalid(from_pcie_M02_AXI_AWVALID),
        .M02_AXI_bready(from_pcie_M02_AXI_BREADY),
        .M02_AXI_bresp(from_pcie_M02_AXI_BRESP),
        .M02_AXI_bvalid(from_pcie_M02_AXI_BVALID),
        .M02_AXI_rdata(from_pcie_M02_AXI_RDATA),
        .M02_AXI_rready(from_pcie_M02_AXI_RREADY),
        .M02_AXI_rresp(from_pcie_M02_AXI_RRESP),
        .M02_AXI_rvalid(from_pcie_M02_AXI_RVALID),
        .M02_AXI_wdata(from_pcie_M02_AXI_WDATA),
        .M02_AXI_wready(from_pcie_M02_AXI_WREADY),
        .M02_AXI_wstrb(from_pcie_M02_AXI_WSTRB),
        .M02_AXI_wvalid(from_pcie_M02_AXI_WVALID),
        .S00_ACLK(AXI_PCIe_axi_aclk_out),
        .S00_ARESETN(S00_ARESETN_1),
        .S00_AXI_araddr(AXI_PCIe_M_AXI_ARADDR),
        .S00_AXI_arburst(AXI_PCIe_M_AXI_ARBURST),
        .S00_AXI_arcache(AXI_PCIe_M_AXI_ARCACHE),
        .S00_AXI_arlen(AXI_PCIe_M_AXI_ARLEN),
        .S00_AXI_arlock(AXI_PCIe_M_AXI_ARLOCK),
        .S00_AXI_arprot(AXI_PCIe_M_AXI_ARPROT),
        .S00_AXI_arready(AXI_PCIe_M_AXI_ARREADY),
        .S00_AXI_arsize(AXI_PCIe_M_AXI_ARSIZE),
        .S00_AXI_arvalid(AXI_PCIe_M_AXI_ARVALID),
        .S00_AXI_awaddr(AXI_PCIe_M_AXI_AWADDR),
        .S00_AXI_awburst(AXI_PCIe_M_AXI_AWBURST),
        .S00_AXI_awcache(AXI_PCIe_M_AXI_AWCACHE),
        .S00_AXI_awlen(AXI_PCIe_M_AXI_AWLEN),
        .S00_AXI_awlock(AXI_PCIe_M_AXI_AWLOCK),
        .S00_AXI_awprot(AXI_PCIe_M_AXI_AWPROT),
        .S00_AXI_awready(AXI_PCIe_M_AXI_AWREADY),
        .S00_AXI_awsize(AXI_PCIe_M_AXI_AWSIZE),
        .S00_AXI_awvalid(AXI_PCIe_M_AXI_AWVALID),
        .S00_AXI_bready(AXI_PCIe_M_AXI_BREADY),
        .S00_AXI_bresp(AXI_PCIe_M_AXI_BRESP),
        .S00_AXI_bvalid(AXI_PCIe_M_AXI_BVALID),
        .S00_AXI_rdata(AXI_PCIe_M_AXI_RDATA),
        .S00_AXI_rlast(AXI_PCIe_M_AXI_RLAST),
        .S00_AXI_rready(AXI_PCIe_M_AXI_RREADY),
        .S00_AXI_rresp(AXI_PCIe_M_AXI_RRESP),
        .S00_AXI_rvalid(AXI_PCIe_M_AXI_RVALID),
        .S00_AXI_wdata(AXI_PCIe_M_AXI_WDATA),
        .S00_AXI_wlast(AXI_PCIe_M_AXI_WLAST),
        .S00_AXI_wready(AXI_PCIe_M_AXI_WREADY),
        .S00_AXI_wstrb(AXI_PCIe_M_AXI_WSTRB),
        .S00_AXI_wvalid(AXI_PCIe_M_AXI_WVALID));
  System_hdmi_controller_0_1 hdmi_controller_0
       (.ACLK(DDR3_SDRAM_ui_clk),
        .HDMI_CLK(hdmi_controller_0_HDMI_CLK),
        .HDMI_DATA(hdmi_controller_0_HDMI_DATA),
        .HDMI_DE(hdmi_controller_0_HDMI_DE),
        .HDMI_HSYNC(hdmi_controller_0_HDMI_HSYNC),
        .HDMI_INTR(HDMI_INTR_1),
        .HDMI_SPDIF(hdmi_controller_0_HDMI_SPDIF),
        .HDMI_VSYNC(hdmi_controller_0_HDMI_VSYNC),
        .I2C_SCL(I2C_SCL),
        .I2C_SDA(I2C_SDA),
        .I2C_nRST(hdmi_controller_0_I2C_nRST),
        .INTR(hdmi_controller_0_INTR),
        .M_ARADDR(hdmi_controller_0_M_AXI_ARADDR),
        .M_ARBURST(hdmi_controller_0_M_AXI_ARBURST),
        .M_ARCACHE(hdmi_controller_0_M_AXI_ARCACHE),
        .M_ARID(hdmi_controller_0_M_AXI_ARID),
        .M_ARLEN(hdmi_controller_0_M_AXI_ARLEN),
        .M_ARLOCK(hdmi_controller_0_M_AXI_ARLOCK),
        .M_ARPROT(hdmi_controller_0_M_AXI_ARPROT),
        .M_ARQOS(hdmi_controller_0_M_AXI_ARQOS),
        .M_ARREADY(hdmi_controller_0_M_AXI_ARREADY),
        .M_ARREGION(hdmi_controller_0_M_AXI_ARREGION),
        .M_ARSIZE(hdmi_controller_0_M_AXI_ARSIZE),
        .M_ARVALID(hdmi_controller_0_M_AXI_ARVALID),
        .M_AWADDR(hdmi_controller_0_M_AXI_AWADDR),
        .M_AWBURST(hdmi_controller_0_M_AXI_AWBURST),
        .M_AWCACHE(hdmi_controller_0_M_AXI_AWCACHE),
        .M_AWID(hdmi_controller_0_M_AXI_AWID),
        .M_AWLEN(hdmi_controller_0_M_AXI_AWLEN),
        .M_AWLOCK(hdmi_controller_0_M_AXI_AWLOCK),
        .M_AWPROT(hdmi_controller_0_M_AXI_AWPROT),
        .M_AWQOS(hdmi_controller_0_M_AXI_AWQOS),
        .M_AWREADY(hdmi_controller_0_M_AXI_AWREADY),
        .M_AWREGION(hdmi_controller_0_M_AXI_AWREGION),
        .M_AWSIZE(hdmi_controller_0_M_AXI_AWSIZE),
        .M_AWVALID(hdmi_controller_0_M_AXI_AWVALID),
        .M_BID(hdmi_controller_0_M_AXI_BID),
        .M_BREADY(hdmi_controller_0_M_AXI_BREADY),
        .M_BRESP(hdmi_controller_0_M_AXI_BRESP),
        .M_BVALID(hdmi_controller_0_M_AXI_BVALID),
        .M_RDATA(hdmi_controller_0_M_AXI_RDATA),
        .M_RID(hdmi_controller_0_M_AXI_RID),
        .M_RLAST(hdmi_controller_0_M_AXI_RLAST),
        .M_RREADY(hdmi_controller_0_M_AXI_RREADY),
        .M_RRESP(hdmi_controller_0_M_AXI_RRESP),
        .M_RVALID(hdmi_controller_0_M_AXI_RVALID),
        .M_WDATA(hdmi_controller_0_M_AXI_WDATA),
        .M_WLAST(hdmi_controller_0_M_AXI_WLAST),
        .M_WREADY(hdmi_controller_0_M_AXI_WREADY),
        .M_WSTRB(hdmi_controller_0_M_AXI_WSTRB),
        .M_WVALID(hdmi_controller_0_M_AXI_WVALID),
        .S_PADDR(axi_apb_bridge_0_APB_M_PADDR[15:0]),
        .S_PENABLE(axi_apb_bridge_0_APB_M_PENABLE),
        .S_PRDATA(axi_apb_bridge_0_APB_M_PRDATA),
        .S_PREADY(axi_apb_bridge_0_APB_M_PREADY),
        .S_PSEL(axi_apb_bridge_0_APB_M_PSEL),
        .S_PSLVERR(axi_apb_bridge_0_APB_M_PSLVERR),
        .S_PWDATA(axi_apb_bridge_0_APB_M_PWDATA),
        .S_PWRITE(axi_apb_bridge_0_APB_M_PWRITE),
        .nRST(rst_DDR3_SDRAM_200M_peripheral_aresetn));
  System_proc_sys_reset_0_0 pcie_reset_0
       (.aux_reset_in(pcie_perstn_1),
        .dcm_locked(util_vector_logic_0_Res),
        .ext_reset_in(DDR3_SDRAM_ui_clk_sync_rst),
        .interconnect_aresetn(ARESETN_1),
        .mb_debug_sys_rst(1'b0),
        .peripheral_aresetn(S00_ARESETN_1),
        .slowest_sync_clk(AXI_PCIe_axi_ctl_aclk_out));
  System_axi_interconnect_0_1 to_memory
       (.ACLK(DDR3_SDRAM_ui_clk),
        .ARESETN(rst_DDR3_SDRAM_200M_interconnect_aresetn),
        .M00_ACLK(DDR3_SDRAM_ui_clk),
        .M00_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .M00_AXI_araddr(axi4_0_M00_AXI_ARADDR),
        .M00_AXI_arburst(axi4_0_M00_AXI_ARBURST),
        .M00_AXI_arcache(axi4_0_M00_AXI_ARCACHE),
        .M00_AXI_arid(axi4_0_M00_AXI_ARID),
        .M00_AXI_arlen(axi4_0_M00_AXI_ARLEN),
        .M00_AXI_arlock(axi4_0_M00_AXI_ARLOCK),
        .M00_AXI_arprot(axi4_0_M00_AXI_ARPROT),
        .M00_AXI_arqos(axi4_0_M00_AXI_ARQOS),
        .M00_AXI_arready(axi4_0_M00_AXI_ARREADY),
        .M00_AXI_arsize(axi4_0_M00_AXI_ARSIZE),
        .M00_AXI_arvalid(axi4_0_M00_AXI_ARVALID),
        .M00_AXI_awaddr(axi4_0_M00_AXI_AWADDR),
        .M00_AXI_awburst(axi4_0_M00_AXI_AWBURST),
        .M00_AXI_awcache(axi4_0_M00_AXI_AWCACHE),
        .M00_AXI_awid(axi4_0_M00_AXI_AWID),
        .M00_AXI_awlen(axi4_0_M00_AXI_AWLEN),
        .M00_AXI_awlock(axi4_0_M00_AXI_AWLOCK),
        .M00_AXI_awprot(axi4_0_M00_AXI_AWPROT),
        .M00_AXI_awqos(axi4_0_M00_AXI_AWQOS),
        .M00_AXI_awready(axi4_0_M00_AXI_AWREADY),
        .M00_AXI_awsize(axi4_0_M00_AXI_AWSIZE),
        .M00_AXI_awvalid(axi4_0_M00_AXI_AWVALID),
        .M00_AXI_bid(axi4_0_M00_AXI_BID),
        .M00_AXI_bready(axi4_0_M00_AXI_BREADY),
        .M00_AXI_bresp(axi4_0_M00_AXI_BRESP),
        .M00_AXI_bvalid(axi4_0_M00_AXI_BVALID),
        .M00_AXI_rdata(axi4_0_M00_AXI_RDATA),
        .M00_AXI_rid(axi4_0_M00_AXI_RID),
        .M00_AXI_rlast(axi4_0_M00_AXI_RLAST),
        .M00_AXI_rready(axi4_0_M00_AXI_RREADY),
        .M00_AXI_rresp(axi4_0_M00_AXI_RRESP),
        .M00_AXI_rvalid(axi4_0_M00_AXI_RVALID),
        .M00_AXI_wdata(axi4_0_M00_AXI_WDATA),
        .M00_AXI_wlast(axi4_0_M00_AXI_WLAST),
        .M00_AXI_wready(axi4_0_M00_AXI_WREADY),
        .M00_AXI_wstrb(axi4_0_M00_AXI_WSTRB),
        .M00_AXI_wvalid(axi4_0_M00_AXI_WVALID),
        .M01_ACLK(AXI_PCIe_axi_aclk_out),
        .M01_ARESETN(S00_ARESETN_1),
        .M01_AXI_araddr(to_memory_M01_AXI_ARADDR),
        .M01_AXI_arburst(to_memory_M01_AXI_ARBURST),
        .M01_AXI_arlen(to_memory_M01_AXI_ARLEN),
        .M01_AXI_arready(to_memory_M01_AXI_ARREADY),
        .M01_AXI_arregion(to_memory_M01_AXI_ARREGION),
        .M01_AXI_arsize(to_memory_M01_AXI_ARSIZE),
        .M01_AXI_arvalid(to_memory_M01_AXI_ARVALID),
        .M01_AXI_awaddr(to_memory_M01_AXI_AWADDR),
        .M01_AXI_awburst(to_memory_M01_AXI_AWBURST),
        .M01_AXI_awlen(to_memory_M01_AXI_AWLEN),
        .M01_AXI_awready(to_memory_M01_AXI_AWREADY),
        .M01_AXI_awregion(to_memory_M01_AXI_AWREGION),
        .M01_AXI_awsize(to_memory_M01_AXI_AWSIZE),
        .M01_AXI_awvalid(to_memory_M01_AXI_AWVALID),
        .M01_AXI_bready(to_memory_M01_AXI_BREADY),
        .M01_AXI_bresp(to_memory_M01_AXI_BRESP),
        .M01_AXI_bvalid(to_memory_M01_AXI_BVALID),
        .M01_AXI_rdata(to_memory_M01_AXI_RDATA),
        .M01_AXI_rlast(to_memory_M01_AXI_RLAST),
        .M01_AXI_rready(to_memory_M01_AXI_RREADY),
        .M01_AXI_rresp(to_memory_M01_AXI_RRESP),
        .M01_AXI_rvalid(to_memory_M01_AXI_RVALID),
        .M01_AXI_wdata(to_memory_M01_AXI_WDATA),
        .M01_AXI_wlast(to_memory_M01_AXI_WLAST),
        .M01_AXI_wready(to_memory_M01_AXI_WREADY),
        .M01_AXI_wstrb(to_memory_M01_AXI_WSTRB),
        .M01_AXI_wvalid(to_memory_M01_AXI_WVALID),
        .S00_ACLK(DDR3_SDRAM_ui_clk),
        .S00_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .S00_AXI_araddr(from_pcie_M00_AXI_ARADDR),
        .S00_AXI_arburst(from_pcie_M00_AXI_ARBURST),
        .S00_AXI_arcache(from_pcie_M00_AXI_ARCACHE),
        .S00_AXI_arlen(from_pcie_M00_AXI_ARLEN),
        .S00_AXI_arlock(from_pcie_M00_AXI_ARLOCK),
        .S00_AXI_arprot(from_pcie_M00_AXI_ARPROT),
        .S00_AXI_arqos(from_pcie_M00_AXI_ARQOS),
        .S00_AXI_arready(from_pcie_M00_AXI_ARREADY),
        .S00_AXI_arregion(from_pcie_M00_AXI_ARREGION),
        .S00_AXI_arsize(from_pcie_M00_AXI_ARSIZE),
        .S00_AXI_arvalid(from_pcie_M00_AXI_ARVALID),
        .S00_AXI_awaddr(from_pcie_M00_AXI_AWADDR),
        .S00_AXI_awburst(from_pcie_M00_AXI_AWBURST),
        .S00_AXI_awcache(from_pcie_M00_AXI_AWCACHE),
        .S00_AXI_awlen(from_pcie_M00_AXI_AWLEN),
        .S00_AXI_awlock(from_pcie_M00_AXI_AWLOCK),
        .S00_AXI_awprot(from_pcie_M00_AXI_AWPROT),
        .S00_AXI_awqos(from_pcie_M00_AXI_AWQOS),
        .S00_AXI_awready(from_pcie_M00_AXI_AWREADY),
        .S00_AXI_awregion(from_pcie_M00_AXI_AWREGION),
        .S00_AXI_awsize(from_pcie_M00_AXI_AWSIZE),
        .S00_AXI_awvalid(from_pcie_M00_AXI_AWVALID),
        .S00_AXI_bready(from_pcie_M00_AXI_BREADY),
        .S00_AXI_bresp(from_pcie_M00_AXI_BRESP),
        .S00_AXI_bvalid(from_pcie_M00_AXI_BVALID),
        .S00_AXI_rdata(from_pcie_M00_AXI_RDATA),
        .S00_AXI_rlast(from_pcie_M00_AXI_RLAST),
        .S00_AXI_rready(from_pcie_M00_AXI_RREADY),
        .S00_AXI_rresp(from_pcie_M00_AXI_RRESP),
        .S00_AXI_rvalid(from_pcie_M00_AXI_RVALID),
        .S00_AXI_wdata(from_pcie_M00_AXI_WDATA),
        .S00_AXI_wlast(from_pcie_M00_AXI_WLAST),
        .S00_AXI_wready(from_pcie_M00_AXI_WREADY),
        .S00_AXI_wstrb(from_pcie_M00_AXI_WSTRB),
        .S00_AXI_wvalid(from_pcie_M00_AXI_WVALID),
        .S01_ACLK(DDR3_SDRAM_ui_clk),
        .S01_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .S01_AXI_araddr(hdmi_controller_0_M_AXI_ARADDR),
        .S01_AXI_arburst(hdmi_controller_0_M_AXI_ARBURST),
        .S01_AXI_arcache(hdmi_controller_0_M_AXI_ARCACHE),
        .S01_AXI_arid(hdmi_controller_0_M_AXI_ARID),
        .S01_AXI_arlen(hdmi_controller_0_M_AXI_ARLEN),
        .S01_AXI_arlock(hdmi_controller_0_M_AXI_ARLOCK),
        .S01_AXI_arprot(hdmi_controller_0_M_AXI_ARPROT),
        .S01_AXI_arqos(hdmi_controller_0_M_AXI_ARQOS),
        .S01_AXI_arready(hdmi_controller_0_M_AXI_ARREADY),
        .S01_AXI_arregion(hdmi_controller_0_M_AXI_ARREGION),
        .S01_AXI_arsize(hdmi_controller_0_M_AXI_ARSIZE),
        .S01_AXI_arvalid(hdmi_controller_0_M_AXI_ARVALID),
        .S01_AXI_awaddr(hdmi_controller_0_M_AXI_AWADDR),
        .S01_AXI_awburst(hdmi_controller_0_M_AXI_AWBURST),
        .S01_AXI_awcache(hdmi_controller_0_M_AXI_AWCACHE),
        .S01_AXI_awid(hdmi_controller_0_M_AXI_AWID),
        .S01_AXI_awlen(hdmi_controller_0_M_AXI_AWLEN),
        .S01_AXI_awlock(hdmi_controller_0_M_AXI_AWLOCK),
        .S01_AXI_awprot(hdmi_controller_0_M_AXI_AWPROT),
        .S01_AXI_awqos(hdmi_controller_0_M_AXI_AWQOS),
        .S01_AXI_awready(hdmi_controller_0_M_AXI_AWREADY),
        .S01_AXI_awregion(hdmi_controller_0_M_AXI_AWREGION),
        .S01_AXI_awsize(hdmi_controller_0_M_AXI_AWSIZE),
        .S01_AXI_awvalid(hdmi_controller_0_M_AXI_AWVALID),
        .S01_AXI_bid(hdmi_controller_0_M_AXI_BID),
        .S01_AXI_bready(hdmi_controller_0_M_AXI_BREADY),
        .S01_AXI_bresp(hdmi_controller_0_M_AXI_BRESP),
        .S01_AXI_bvalid(hdmi_controller_0_M_AXI_BVALID),
        .S01_AXI_rdata(hdmi_controller_0_M_AXI_RDATA),
        .S01_AXI_rid(hdmi_controller_0_M_AXI_RID),
        .S01_AXI_rlast(hdmi_controller_0_M_AXI_RLAST),
        .S01_AXI_rready(hdmi_controller_0_M_AXI_RREADY),
        .S01_AXI_rresp(hdmi_controller_0_M_AXI_RRESP),
        .S01_AXI_rvalid(hdmi_controller_0_M_AXI_RVALID),
        .S01_AXI_wdata(hdmi_controller_0_M_AXI_WDATA),
        .S01_AXI_wlast(hdmi_controller_0_M_AXI_WLAST),
        .S01_AXI_wready(hdmi_controller_0_M_AXI_WREADY),
        .S01_AXI_wstrb(hdmi_controller_0_M_AXI_WSTRB),
        .S01_AXI_wvalid(hdmi_controller_0_M_AXI_WVALID),
        .S02_ACLK(DDR3_SDRAM_ui_clk),
        .S02_ARESETN(rst_DDR3_SDRAM_200M_peripheral_aresetn),
        .S02_AXI_araddr(dut_wrapper_0_M_AXI_ARADDR),
        .S02_AXI_arburst(dut_wrapper_0_M_AXI_ARBURST),
        .S02_AXI_arcache(dut_wrapper_0_M_AXI_ARCACHE),
        .S02_AXI_arid(dut_wrapper_0_M_AXI_ARID),
        .S02_AXI_arlen(dut_wrapper_0_M_AXI_ARLEN),
        .S02_AXI_arlock(dut_wrapper_0_M_AXI_ARLOCK),
        .S02_AXI_arprot(dut_wrapper_0_M_AXI_ARPROT),
        .S02_AXI_arqos(dut_wrapper_0_M_AXI_ARQOS),
        .S02_AXI_arready(dut_wrapper_0_M_AXI_ARREADY),
        .S02_AXI_arsize(dut_wrapper_0_M_AXI_ARSIZE),
        .S02_AXI_arvalid(dut_wrapper_0_M_AXI_ARVALID),
        .S02_AXI_awaddr(dut_wrapper_0_M_AXI_AWADDR),
        .S02_AXI_awburst(dut_wrapper_0_M_AXI_AWBURST),
        .S02_AXI_awcache(dut_wrapper_0_M_AXI_AWCACHE),
        .S02_AXI_awid(dut_wrapper_0_M_AXI_AWID),
        .S02_AXI_awlen(dut_wrapper_0_M_AXI_AWLEN),
        .S02_AXI_awlock(dut_wrapper_0_M_AXI_AWLOCK),
        .S02_AXI_awprot(dut_wrapper_0_M_AXI_AWPROT),
        .S02_AXI_awqos(dut_wrapper_0_M_AXI_AWQOS),
        .S02_AXI_awready(dut_wrapper_0_M_AXI_AWREADY),
        .S02_AXI_awsize(dut_wrapper_0_M_AXI_AWSIZE),
        .S02_AXI_awvalid(dut_wrapper_0_M_AXI_AWVALID),
        .S02_AXI_bid(dut_wrapper_0_M_AXI_BID),
        .S02_AXI_bready(dut_wrapper_0_M_AXI_BREADY),
        .S02_AXI_bresp(dut_wrapper_0_M_AXI_BRESP),
        .S02_AXI_bvalid(dut_wrapper_0_M_AXI_BVALID),
        .S02_AXI_rdata(dut_wrapper_0_M_AXI_RDATA),
        .S02_AXI_rid(dut_wrapper_0_M_AXI_RID),
        .S02_AXI_rlast(dut_wrapper_0_M_AXI_RLAST),
        .S02_AXI_rready(dut_wrapper_0_M_AXI_RREADY),
        .S02_AXI_rresp(dut_wrapper_0_M_AXI_RRESP),
        .S02_AXI_rvalid(dut_wrapper_0_M_AXI_RVALID),
        .S02_AXI_wdata(dut_wrapper_0_M_AXI_WDATA),
        .S02_AXI_wlast(dut_wrapper_0_M_AXI_WLAST),
        .S02_AXI_wready(dut_wrapper_0_M_AXI_WREADY),
        .S02_AXI_wstrb(dut_wrapper_0_M_AXI_WSTRB),
        .S02_AXI_wvalid(dut_wrapper_0_M_AXI_WVALID));
  System_util_vector_logic_0_1 util_vector_logic_0
       (.Op1(DDR3_SDRAM_mmcm_locked),
        .Op2(AXI_PCIe_mmcm_lock),
        .Res(util_vector_logic_0_Res));
endmodule

module System_axi_interconnect_0_0
   (ACLK,
    ARESETN,
    M00_ACLK,
    M00_ARESETN,
    M00_AXI_araddr,
    M00_AXI_arburst,
    M00_AXI_arcache,
    M00_AXI_arlen,
    M00_AXI_arlock,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arready,
    M00_AXI_arregion,
    M00_AXI_arsize,
    M00_AXI_arvalid,
    M00_AXI_awaddr,
    M00_AXI_awburst,
    M00_AXI_awcache,
    M00_AXI_awlen,
    M00_AXI_awlock,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awready,
    M00_AXI_awregion,
    M00_AXI_awsize,
    M00_AXI_awvalid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_rdata,
    M00_AXI_rlast,
    M00_AXI_rready,
    M00_AXI_rresp,
    M00_AXI_rvalid,
    M00_AXI_wdata,
    M00_AXI_wlast,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    M01_ACLK,
    M01_ARESETN,
    M01_AXI_araddr,
    M01_AXI_arready,
    M01_AXI_arvalid,
    M01_AXI_awaddr,
    M01_AXI_awready,
    M01_AXI_awvalid,
    M01_AXI_bready,
    M01_AXI_bresp,
    M01_AXI_bvalid,
    M01_AXI_rdata,
    M01_AXI_rready,
    M01_AXI_rresp,
    M01_AXI_rvalid,
    M01_AXI_wdata,
    M01_AXI_wready,
    M01_AXI_wvalid,
    M02_ACLK,
    M02_ARESETN,
    M02_AXI_araddr,
    M02_AXI_arready,
    M02_AXI_arvalid,
    M02_AXI_awaddr,
    M02_AXI_awready,
    M02_AXI_awvalid,
    M02_AXI_bready,
    M02_AXI_bresp,
    M02_AXI_bvalid,
    M02_AXI_rdata,
    M02_AXI_rready,
    M02_AXI_rresp,
    M02_AXI_rvalid,
    M02_AXI_wdata,
    M02_AXI_wready,
    M02_AXI_wstrb,
    M02_AXI_wvalid,
    S00_ACLK,
    S00_ARESETN,
    S00_AXI_araddr,
    S00_AXI_arburst,
    S00_AXI_arcache,
    S00_AXI_arlen,
    S00_AXI_arlock,
    S00_AXI_arprot,
    S00_AXI_arready,
    S00_AXI_arsize,
    S00_AXI_arvalid,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awlen,
    S00_AXI_awlock,
    S00_AXI_awprot,
    S00_AXI_awready,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_rdata,
    S00_AXI_rlast,
    S00_AXI_rready,
    S00_AXI_rresp,
    S00_AXI_rvalid,
    S00_AXI_wdata,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid);
  input ACLK;
  input ARESETN;
  input M00_ACLK;
  input M00_ARESETN;
  output [31:0]M00_AXI_araddr;
  output [1:0]M00_AXI_arburst;
  output [3:0]M00_AXI_arcache;
  output [7:0]M00_AXI_arlen;
  output [0:0]M00_AXI_arlock;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  input M00_AXI_arready;
  output [3:0]M00_AXI_arregion;
  output [2:0]M00_AXI_arsize;
  output M00_AXI_arvalid;
  output [31:0]M00_AXI_awaddr;
  output [1:0]M00_AXI_awburst;
  output [3:0]M00_AXI_awcache;
  output [7:0]M00_AXI_awlen;
  output [0:0]M00_AXI_awlock;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  input M00_AXI_awready;
  output [3:0]M00_AXI_awregion;
  output [2:0]M00_AXI_awsize;
  output M00_AXI_awvalid;
  output M00_AXI_bready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  input [511:0]M00_AXI_rdata;
  input M00_AXI_rlast;
  output M00_AXI_rready;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rvalid;
  output [511:0]M00_AXI_wdata;
  output M00_AXI_wlast;
  input M00_AXI_wready;
  output [63:0]M00_AXI_wstrb;
  output M00_AXI_wvalid;
  input M01_ACLK;
  input M01_ARESETN;
  output [31:0]M01_AXI_araddr;
  input M01_AXI_arready;
  output M01_AXI_arvalid;
  output [31:0]M01_AXI_awaddr;
  input M01_AXI_awready;
  output M01_AXI_awvalid;
  output M01_AXI_bready;
  input [1:0]M01_AXI_bresp;
  input M01_AXI_bvalid;
  input [31:0]M01_AXI_rdata;
  output M01_AXI_rready;
  input [1:0]M01_AXI_rresp;
  input M01_AXI_rvalid;
  output [31:0]M01_AXI_wdata;
  input M01_AXI_wready;
  output M01_AXI_wvalid;
  input M02_ACLK;
  input M02_ARESETN;
  output [31:0]M02_AXI_araddr;
  input M02_AXI_arready;
  output M02_AXI_arvalid;
  output [31:0]M02_AXI_awaddr;
  input M02_AXI_awready;
  output M02_AXI_awvalid;
  output M02_AXI_bready;
  input [1:0]M02_AXI_bresp;
  input M02_AXI_bvalid;
  input [31:0]M02_AXI_rdata;
  output M02_AXI_rready;
  input [1:0]M02_AXI_rresp;
  input M02_AXI_rvalid;
  output [31:0]M02_AXI_wdata;
  input M02_AXI_wready;
  output [3:0]M02_AXI_wstrb;
  output M02_AXI_wvalid;
  input S00_ACLK;
  input S00_ARESETN;
  input [31:0]S00_AXI_araddr;
  input [1:0]S00_AXI_arburst;
  input [3:0]S00_AXI_arcache;
  input [7:0]S00_AXI_arlen;
  input S00_AXI_arlock;
  input [2:0]S00_AXI_arprot;
  output S00_AXI_arready;
  input [2:0]S00_AXI_arsize;
  input S00_AXI_arvalid;
  input [31:0]S00_AXI_awaddr;
  input [1:0]S00_AXI_awburst;
  input [3:0]S00_AXI_awcache;
  input [7:0]S00_AXI_awlen;
  input S00_AXI_awlock;
  input [2:0]S00_AXI_awprot;
  output S00_AXI_awready;
  input [2:0]S00_AXI_awsize;
  input S00_AXI_awvalid;
  input S00_AXI_bready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  output [127:0]S00_AXI_rdata;
  output S00_AXI_rlast;
  input S00_AXI_rready;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rvalid;
  input [127:0]S00_AXI_wdata;
  input S00_AXI_wlast;
  output S00_AXI_wready;
  input [15:0]S00_AXI_wstrb;
  input S00_AXI_wvalid;

  wire M00_ACLK_1;
  wire M00_ARESETN_1;
  wire M01_ACLK_1;
  wire M01_ARESETN_1;
  wire M02_ACLK_1;
  wire M02_ARESETN_1;
  wire S00_ACLK_1;
  wire S00_ARESETN_1;
  wire from_pcie_ACLK_net;
  wire from_pcie_ARESETN_net;
  wire [31:0]from_pcie_to_s00_couplers_ARADDR;
  wire [1:0]from_pcie_to_s00_couplers_ARBURST;
  wire [3:0]from_pcie_to_s00_couplers_ARCACHE;
  wire [7:0]from_pcie_to_s00_couplers_ARLEN;
  wire from_pcie_to_s00_couplers_ARLOCK;
  wire [2:0]from_pcie_to_s00_couplers_ARPROT;
  wire from_pcie_to_s00_couplers_ARREADY;
  wire [2:0]from_pcie_to_s00_couplers_ARSIZE;
  wire from_pcie_to_s00_couplers_ARVALID;
  wire [31:0]from_pcie_to_s00_couplers_AWADDR;
  wire [1:0]from_pcie_to_s00_couplers_AWBURST;
  wire [3:0]from_pcie_to_s00_couplers_AWCACHE;
  wire [7:0]from_pcie_to_s00_couplers_AWLEN;
  wire from_pcie_to_s00_couplers_AWLOCK;
  wire [2:0]from_pcie_to_s00_couplers_AWPROT;
  wire from_pcie_to_s00_couplers_AWREADY;
  wire [2:0]from_pcie_to_s00_couplers_AWSIZE;
  wire from_pcie_to_s00_couplers_AWVALID;
  wire from_pcie_to_s00_couplers_BREADY;
  wire [1:0]from_pcie_to_s00_couplers_BRESP;
  wire from_pcie_to_s00_couplers_BVALID;
  wire [127:0]from_pcie_to_s00_couplers_RDATA;
  wire from_pcie_to_s00_couplers_RLAST;
  wire from_pcie_to_s00_couplers_RREADY;
  wire [1:0]from_pcie_to_s00_couplers_RRESP;
  wire from_pcie_to_s00_couplers_RVALID;
  wire [127:0]from_pcie_to_s00_couplers_WDATA;
  wire from_pcie_to_s00_couplers_WLAST;
  wire from_pcie_to_s00_couplers_WREADY;
  wire [15:0]from_pcie_to_s00_couplers_WSTRB;
  wire from_pcie_to_s00_couplers_WVALID;
  wire [31:0]m00_couplers_to_from_pcie_ARADDR;
  wire [1:0]m00_couplers_to_from_pcie_ARBURST;
  wire [3:0]m00_couplers_to_from_pcie_ARCACHE;
  wire [7:0]m00_couplers_to_from_pcie_ARLEN;
  wire [0:0]m00_couplers_to_from_pcie_ARLOCK;
  wire [2:0]m00_couplers_to_from_pcie_ARPROT;
  wire [3:0]m00_couplers_to_from_pcie_ARQOS;
  wire m00_couplers_to_from_pcie_ARREADY;
  wire [3:0]m00_couplers_to_from_pcie_ARREGION;
  wire [2:0]m00_couplers_to_from_pcie_ARSIZE;
  wire m00_couplers_to_from_pcie_ARVALID;
  wire [31:0]m00_couplers_to_from_pcie_AWADDR;
  wire [1:0]m00_couplers_to_from_pcie_AWBURST;
  wire [3:0]m00_couplers_to_from_pcie_AWCACHE;
  wire [7:0]m00_couplers_to_from_pcie_AWLEN;
  wire [0:0]m00_couplers_to_from_pcie_AWLOCK;
  wire [2:0]m00_couplers_to_from_pcie_AWPROT;
  wire [3:0]m00_couplers_to_from_pcie_AWQOS;
  wire m00_couplers_to_from_pcie_AWREADY;
  wire [3:0]m00_couplers_to_from_pcie_AWREGION;
  wire [2:0]m00_couplers_to_from_pcie_AWSIZE;
  wire m00_couplers_to_from_pcie_AWVALID;
  wire m00_couplers_to_from_pcie_BREADY;
  wire [1:0]m00_couplers_to_from_pcie_BRESP;
  wire m00_couplers_to_from_pcie_BVALID;
  wire [511:0]m00_couplers_to_from_pcie_RDATA;
  wire m00_couplers_to_from_pcie_RLAST;
  wire m00_couplers_to_from_pcie_RREADY;
  wire [1:0]m00_couplers_to_from_pcie_RRESP;
  wire m00_couplers_to_from_pcie_RVALID;
  wire [511:0]m00_couplers_to_from_pcie_WDATA;
  wire m00_couplers_to_from_pcie_WLAST;
  wire m00_couplers_to_from_pcie_WREADY;
  wire [63:0]m00_couplers_to_from_pcie_WSTRB;
  wire m00_couplers_to_from_pcie_WVALID;
  wire [31:0]m01_couplers_to_from_pcie_ARADDR;
  wire m01_couplers_to_from_pcie_ARREADY;
  wire m01_couplers_to_from_pcie_ARVALID;
  wire [31:0]m01_couplers_to_from_pcie_AWADDR;
  wire m01_couplers_to_from_pcie_AWREADY;
  wire m01_couplers_to_from_pcie_AWVALID;
  wire m01_couplers_to_from_pcie_BREADY;
  wire [1:0]m01_couplers_to_from_pcie_BRESP;
  wire m01_couplers_to_from_pcie_BVALID;
  wire [31:0]m01_couplers_to_from_pcie_RDATA;
  wire m01_couplers_to_from_pcie_RREADY;
  wire [1:0]m01_couplers_to_from_pcie_RRESP;
  wire m01_couplers_to_from_pcie_RVALID;
  wire [31:0]m01_couplers_to_from_pcie_WDATA;
  wire m01_couplers_to_from_pcie_WREADY;
  wire m01_couplers_to_from_pcie_WVALID;
  wire [31:0]m02_couplers_to_from_pcie_ARADDR;
  wire m02_couplers_to_from_pcie_ARREADY;
  wire m02_couplers_to_from_pcie_ARVALID;
  wire [31:0]m02_couplers_to_from_pcie_AWADDR;
  wire m02_couplers_to_from_pcie_AWREADY;
  wire m02_couplers_to_from_pcie_AWVALID;
  wire m02_couplers_to_from_pcie_BREADY;
  wire [1:0]m02_couplers_to_from_pcie_BRESP;
  wire m02_couplers_to_from_pcie_BVALID;
  wire [31:0]m02_couplers_to_from_pcie_RDATA;
  wire m02_couplers_to_from_pcie_RREADY;
  wire [1:0]m02_couplers_to_from_pcie_RRESP;
  wire m02_couplers_to_from_pcie_RVALID;
  wire [31:0]m02_couplers_to_from_pcie_WDATA;
  wire m02_couplers_to_from_pcie_WREADY;
  wire [3:0]m02_couplers_to_from_pcie_WSTRB;
  wire m02_couplers_to_from_pcie_WVALID;
  wire [31:0]s00_couplers_to_xbar_ARADDR;
  wire [1:0]s00_couplers_to_xbar_ARBURST;
  wire [3:0]s00_couplers_to_xbar_ARCACHE;
  wire [7:0]s00_couplers_to_xbar_ARLEN;
  wire s00_couplers_to_xbar_ARLOCK;
  wire [2:0]s00_couplers_to_xbar_ARPROT;
  wire [0:0]s00_couplers_to_xbar_ARREADY;
  wire [2:0]s00_couplers_to_xbar_ARSIZE;
  wire s00_couplers_to_xbar_ARVALID;
  wire [31:0]s00_couplers_to_xbar_AWADDR;
  wire [1:0]s00_couplers_to_xbar_AWBURST;
  wire [3:0]s00_couplers_to_xbar_AWCACHE;
  wire [7:0]s00_couplers_to_xbar_AWLEN;
  wire s00_couplers_to_xbar_AWLOCK;
  wire [2:0]s00_couplers_to_xbar_AWPROT;
  wire [0:0]s00_couplers_to_xbar_AWREADY;
  wire [2:0]s00_couplers_to_xbar_AWSIZE;
  wire s00_couplers_to_xbar_AWVALID;
  wire s00_couplers_to_xbar_BREADY;
  wire [1:0]s00_couplers_to_xbar_BRESP;
  wire [0:0]s00_couplers_to_xbar_BVALID;
  wire [127:0]s00_couplers_to_xbar_RDATA;
  wire [0:0]s00_couplers_to_xbar_RLAST;
  wire s00_couplers_to_xbar_RREADY;
  wire [1:0]s00_couplers_to_xbar_RRESP;
  wire [0:0]s00_couplers_to_xbar_RVALID;
  wire [127:0]s00_couplers_to_xbar_WDATA;
  wire s00_couplers_to_xbar_WLAST;
  wire [0:0]s00_couplers_to_xbar_WREADY;
  wire [15:0]s00_couplers_to_xbar_WSTRB;
  wire s00_couplers_to_xbar_WVALID;
  wire [31:0]xbar_to_m00_couplers_ARADDR;
  wire [1:0]xbar_to_m00_couplers_ARBURST;
  wire [3:0]xbar_to_m00_couplers_ARCACHE;
  wire [7:0]xbar_to_m00_couplers_ARLEN;
  wire [0:0]xbar_to_m00_couplers_ARLOCK;
  wire [2:0]xbar_to_m00_couplers_ARPROT;
  wire [3:0]xbar_to_m00_couplers_ARQOS;
  wire xbar_to_m00_couplers_ARREADY;
  wire [3:0]xbar_to_m00_couplers_ARREGION;
  wire [2:0]xbar_to_m00_couplers_ARSIZE;
  wire [0:0]xbar_to_m00_couplers_ARVALID;
  wire [31:0]xbar_to_m00_couplers_AWADDR;
  wire [1:0]xbar_to_m00_couplers_AWBURST;
  wire [3:0]xbar_to_m00_couplers_AWCACHE;
  wire [7:0]xbar_to_m00_couplers_AWLEN;
  wire [0:0]xbar_to_m00_couplers_AWLOCK;
  wire [2:0]xbar_to_m00_couplers_AWPROT;
  wire [3:0]xbar_to_m00_couplers_AWQOS;
  wire xbar_to_m00_couplers_AWREADY;
  wire [3:0]xbar_to_m00_couplers_AWREGION;
  wire [2:0]xbar_to_m00_couplers_AWSIZE;
  wire [0:0]xbar_to_m00_couplers_AWVALID;
  wire [0:0]xbar_to_m00_couplers_BREADY;
  wire [1:0]xbar_to_m00_couplers_BRESP;
  wire xbar_to_m00_couplers_BVALID;
  wire [127:0]xbar_to_m00_couplers_RDATA;
  wire xbar_to_m00_couplers_RLAST;
  wire [0:0]xbar_to_m00_couplers_RREADY;
  wire [1:0]xbar_to_m00_couplers_RRESP;
  wire xbar_to_m00_couplers_RVALID;
  wire [127:0]xbar_to_m00_couplers_WDATA;
  wire [0:0]xbar_to_m00_couplers_WLAST;
  wire xbar_to_m00_couplers_WREADY;
  wire [15:0]xbar_to_m00_couplers_WSTRB;
  wire [0:0]xbar_to_m00_couplers_WVALID;
  wire [63:32]xbar_to_m01_couplers_ARADDR;
  wire [3:2]xbar_to_m01_couplers_ARBURST;
  wire [7:4]xbar_to_m01_couplers_ARCACHE;
  wire [15:8]xbar_to_m01_couplers_ARLEN;
  wire [1:1]xbar_to_m01_couplers_ARLOCK;
  wire [5:3]xbar_to_m01_couplers_ARPROT;
  wire [7:4]xbar_to_m01_couplers_ARQOS;
  wire xbar_to_m01_couplers_ARREADY;
  wire [7:4]xbar_to_m01_couplers_ARREGION;
  wire [5:3]xbar_to_m01_couplers_ARSIZE;
  wire [1:1]xbar_to_m01_couplers_ARVALID;
  wire [63:32]xbar_to_m01_couplers_AWADDR;
  wire [3:2]xbar_to_m01_couplers_AWBURST;
  wire [7:4]xbar_to_m01_couplers_AWCACHE;
  wire [15:8]xbar_to_m01_couplers_AWLEN;
  wire [1:1]xbar_to_m01_couplers_AWLOCK;
  wire [5:3]xbar_to_m01_couplers_AWPROT;
  wire [7:4]xbar_to_m01_couplers_AWQOS;
  wire xbar_to_m01_couplers_AWREADY;
  wire [7:4]xbar_to_m01_couplers_AWREGION;
  wire [5:3]xbar_to_m01_couplers_AWSIZE;
  wire [1:1]xbar_to_m01_couplers_AWVALID;
  wire [1:1]xbar_to_m01_couplers_BREADY;
  wire [1:0]xbar_to_m01_couplers_BRESP;
  wire xbar_to_m01_couplers_BVALID;
  wire [127:0]xbar_to_m01_couplers_RDATA;
  wire xbar_to_m01_couplers_RLAST;
  wire [1:1]xbar_to_m01_couplers_RREADY;
  wire [1:0]xbar_to_m01_couplers_RRESP;
  wire xbar_to_m01_couplers_RVALID;
  wire [255:128]xbar_to_m01_couplers_WDATA;
  wire [1:1]xbar_to_m01_couplers_WLAST;
  wire xbar_to_m01_couplers_WREADY;
  wire [31:16]xbar_to_m01_couplers_WSTRB;
  wire [1:1]xbar_to_m01_couplers_WVALID;
  wire [95:64]xbar_to_m02_couplers_ARADDR;
  wire [5:4]xbar_to_m02_couplers_ARBURST;
  wire [11:8]xbar_to_m02_couplers_ARCACHE;
  wire [23:16]xbar_to_m02_couplers_ARLEN;
  wire [2:2]xbar_to_m02_couplers_ARLOCK;
  wire [8:6]xbar_to_m02_couplers_ARPROT;
  wire [11:8]xbar_to_m02_couplers_ARQOS;
  wire xbar_to_m02_couplers_ARREADY;
  wire [11:8]xbar_to_m02_couplers_ARREGION;
  wire [8:6]xbar_to_m02_couplers_ARSIZE;
  wire [2:2]xbar_to_m02_couplers_ARVALID;
  wire [95:64]xbar_to_m02_couplers_AWADDR;
  wire [5:4]xbar_to_m02_couplers_AWBURST;
  wire [11:8]xbar_to_m02_couplers_AWCACHE;
  wire [23:16]xbar_to_m02_couplers_AWLEN;
  wire [2:2]xbar_to_m02_couplers_AWLOCK;
  wire [8:6]xbar_to_m02_couplers_AWPROT;
  wire [11:8]xbar_to_m02_couplers_AWQOS;
  wire xbar_to_m02_couplers_AWREADY;
  wire [11:8]xbar_to_m02_couplers_AWREGION;
  wire [8:6]xbar_to_m02_couplers_AWSIZE;
  wire [2:2]xbar_to_m02_couplers_AWVALID;
  wire [2:2]xbar_to_m02_couplers_BREADY;
  wire [1:0]xbar_to_m02_couplers_BRESP;
  wire xbar_to_m02_couplers_BVALID;
  wire [127:0]xbar_to_m02_couplers_RDATA;
  wire xbar_to_m02_couplers_RLAST;
  wire [2:2]xbar_to_m02_couplers_RREADY;
  wire [1:0]xbar_to_m02_couplers_RRESP;
  wire xbar_to_m02_couplers_RVALID;
  wire [383:256]xbar_to_m02_couplers_WDATA;
  wire [2:2]xbar_to_m02_couplers_WLAST;
  wire xbar_to_m02_couplers_WREADY;
  wire [47:32]xbar_to_m02_couplers_WSTRB;
  wire [2:2]xbar_to_m02_couplers_WVALID;

  assign M00_ACLK_1 = M00_ACLK;
  assign M00_ARESETN_1 = M00_ARESETN;
  assign M00_AXI_araddr[31:0] = m00_couplers_to_from_pcie_ARADDR;
  assign M00_AXI_arburst[1:0] = m00_couplers_to_from_pcie_ARBURST;
  assign M00_AXI_arcache[3:0] = m00_couplers_to_from_pcie_ARCACHE;
  assign M00_AXI_arlen[7:0] = m00_couplers_to_from_pcie_ARLEN;
  assign M00_AXI_arlock[0] = m00_couplers_to_from_pcie_ARLOCK;
  assign M00_AXI_arprot[2:0] = m00_couplers_to_from_pcie_ARPROT;
  assign M00_AXI_arqos[3:0] = m00_couplers_to_from_pcie_ARQOS;
  assign M00_AXI_arregion[3:0] = m00_couplers_to_from_pcie_ARREGION;
  assign M00_AXI_arsize[2:0] = m00_couplers_to_from_pcie_ARSIZE;
  assign M00_AXI_arvalid = m00_couplers_to_from_pcie_ARVALID;
  assign M00_AXI_awaddr[31:0] = m00_couplers_to_from_pcie_AWADDR;
  assign M00_AXI_awburst[1:0] = m00_couplers_to_from_pcie_AWBURST;
  assign M00_AXI_awcache[3:0] = m00_couplers_to_from_pcie_AWCACHE;
  assign M00_AXI_awlen[7:0] = m00_couplers_to_from_pcie_AWLEN;
  assign M00_AXI_awlock[0] = m00_couplers_to_from_pcie_AWLOCK;
  assign M00_AXI_awprot[2:0] = m00_couplers_to_from_pcie_AWPROT;
  assign M00_AXI_awqos[3:0] = m00_couplers_to_from_pcie_AWQOS;
  assign M00_AXI_awregion[3:0] = m00_couplers_to_from_pcie_AWREGION;
  assign M00_AXI_awsize[2:0] = m00_couplers_to_from_pcie_AWSIZE;
  assign M00_AXI_awvalid = m00_couplers_to_from_pcie_AWVALID;
  assign M00_AXI_bready = m00_couplers_to_from_pcie_BREADY;
  assign M00_AXI_rready = m00_couplers_to_from_pcie_RREADY;
  assign M00_AXI_wdata[511:0] = m00_couplers_to_from_pcie_WDATA;
  assign M00_AXI_wlast = m00_couplers_to_from_pcie_WLAST;
  assign M00_AXI_wstrb[63:0] = m00_couplers_to_from_pcie_WSTRB;
  assign M00_AXI_wvalid = m00_couplers_to_from_pcie_WVALID;
  assign M01_ACLK_1 = M01_ACLK;
  assign M01_ARESETN_1 = M01_ARESETN;
  assign M01_AXI_araddr[31:0] = m01_couplers_to_from_pcie_ARADDR;
  assign M01_AXI_arvalid = m01_couplers_to_from_pcie_ARVALID;
  assign M01_AXI_awaddr[31:0] = m01_couplers_to_from_pcie_AWADDR;
  assign M01_AXI_awvalid = m01_couplers_to_from_pcie_AWVALID;
  assign M01_AXI_bready = m01_couplers_to_from_pcie_BREADY;
  assign M01_AXI_rready = m01_couplers_to_from_pcie_RREADY;
  assign M01_AXI_wdata[31:0] = m01_couplers_to_from_pcie_WDATA;
  assign M01_AXI_wvalid = m01_couplers_to_from_pcie_WVALID;
  assign M02_ACLK_1 = M02_ACLK;
  assign M02_ARESETN_1 = M02_ARESETN;
  assign M02_AXI_araddr[31:0] = m02_couplers_to_from_pcie_ARADDR;
  assign M02_AXI_arvalid = m02_couplers_to_from_pcie_ARVALID;
  assign M02_AXI_awaddr[31:0] = m02_couplers_to_from_pcie_AWADDR;
  assign M02_AXI_awvalid = m02_couplers_to_from_pcie_AWVALID;
  assign M02_AXI_bready = m02_couplers_to_from_pcie_BREADY;
  assign M02_AXI_rready = m02_couplers_to_from_pcie_RREADY;
  assign M02_AXI_wdata[31:0] = m02_couplers_to_from_pcie_WDATA;
  assign M02_AXI_wstrb[3:0] = m02_couplers_to_from_pcie_WSTRB;
  assign M02_AXI_wvalid = m02_couplers_to_from_pcie_WVALID;
  assign S00_ACLK_1 = S00_ACLK;
  assign S00_ARESETN_1 = S00_ARESETN;
  assign S00_AXI_arready = from_pcie_to_s00_couplers_ARREADY;
  assign S00_AXI_awready = from_pcie_to_s00_couplers_AWREADY;
  assign S00_AXI_bresp[1:0] = from_pcie_to_s00_couplers_BRESP;
  assign S00_AXI_bvalid = from_pcie_to_s00_couplers_BVALID;
  assign S00_AXI_rdata[127:0] = from_pcie_to_s00_couplers_RDATA;
  assign S00_AXI_rlast = from_pcie_to_s00_couplers_RLAST;
  assign S00_AXI_rresp[1:0] = from_pcie_to_s00_couplers_RRESP;
  assign S00_AXI_rvalid = from_pcie_to_s00_couplers_RVALID;
  assign S00_AXI_wready = from_pcie_to_s00_couplers_WREADY;
  assign from_pcie_ACLK_net = ACLK;
  assign from_pcie_ARESETN_net = ARESETN;
  assign from_pcie_to_s00_couplers_ARADDR = S00_AXI_araddr[31:0];
  assign from_pcie_to_s00_couplers_ARBURST = S00_AXI_arburst[1:0];
  assign from_pcie_to_s00_couplers_ARCACHE = S00_AXI_arcache[3:0];
  assign from_pcie_to_s00_couplers_ARLEN = S00_AXI_arlen[7:0];
  assign from_pcie_to_s00_couplers_ARLOCK = S00_AXI_arlock;
  assign from_pcie_to_s00_couplers_ARPROT = S00_AXI_arprot[2:0];
  assign from_pcie_to_s00_couplers_ARSIZE = S00_AXI_arsize[2:0];
  assign from_pcie_to_s00_couplers_ARVALID = S00_AXI_arvalid;
  assign from_pcie_to_s00_couplers_AWADDR = S00_AXI_awaddr[31:0];
  assign from_pcie_to_s00_couplers_AWBURST = S00_AXI_awburst[1:0];
  assign from_pcie_to_s00_couplers_AWCACHE = S00_AXI_awcache[3:0];
  assign from_pcie_to_s00_couplers_AWLEN = S00_AXI_awlen[7:0];
  assign from_pcie_to_s00_couplers_AWLOCK = S00_AXI_awlock;
  assign from_pcie_to_s00_couplers_AWPROT = S00_AXI_awprot[2:0];
  assign from_pcie_to_s00_couplers_AWSIZE = S00_AXI_awsize[2:0];
  assign from_pcie_to_s00_couplers_AWVALID = S00_AXI_awvalid;
  assign from_pcie_to_s00_couplers_BREADY = S00_AXI_bready;
  assign from_pcie_to_s00_couplers_RREADY = S00_AXI_rready;
  assign from_pcie_to_s00_couplers_WDATA = S00_AXI_wdata[127:0];
  assign from_pcie_to_s00_couplers_WLAST = S00_AXI_wlast;
  assign from_pcie_to_s00_couplers_WSTRB = S00_AXI_wstrb[15:0];
  assign from_pcie_to_s00_couplers_WVALID = S00_AXI_wvalid;
  assign m00_couplers_to_from_pcie_ARREADY = M00_AXI_arready;
  assign m00_couplers_to_from_pcie_AWREADY = M00_AXI_awready;
  assign m00_couplers_to_from_pcie_BRESP = M00_AXI_bresp[1:0];
  assign m00_couplers_to_from_pcie_BVALID = M00_AXI_bvalid;
  assign m00_couplers_to_from_pcie_RDATA = M00_AXI_rdata[511:0];
  assign m00_couplers_to_from_pcie_RLAST = M00_AXI_rlast;
  assign m00_couplers_to_from_pcie_RRESP = M00_AXI_rresp[1:0];
  assign m00_couplers_to_from_pcie_RVALID = M00_AXI_rvalid;
  assign m00_couplers_to_from_pcie_WREADY = M00_AXI_wready;
  assign m01_couplers_to_from_pcie_ARREADY = M01_AXI_arready;
  assign m01_couplers_to_from_pcie_AWREADY = M01_AXI_awready;
  assign m01_couplers_to_from_pcie_BRESP = M01_AXI_bresp[1:0];
  assign m01_couplers_to_from_pcie_BVALID = M01_AXI_bvalid;
  assign m01_couplers_to_from_pcie_RDATA = M01_AXI_rdata[31:0];
  assign m01_couplers_to_from_pcie_RRESP = M01_AXI_rresp[1:0];
  assign m01_couplers_to_from_pcie_RVALID = M01_AXI_rvalid;
  assign m01_couplers_to_from_pcie_WREADY = M01_AXI_wready;
  assign m02_couplers_to_from_pcie_ARREADY = M02_AXI_arready;
  assign m02_couplers_to_from_pcie_AWREADY = M02_AXI_awready;
  assign m02_couplers_to_from_pcie_BRESP = M02_AXI_bresp[1:0];
  assign m02_couplers_to_from_pcie_BVALID = M02_AXI_bvalid;
  assign m02_couplers_to_from_pcie_RDATA = M02_AXI_rdata[31:0];
  assign m02_couplers_to_from_pcie_RRESP = M02_AXI_rresp[1:0];
  assign m02_couplers_to_from_pcie_RVALID = M02_AXI_rvalid;
  assign m02_couplers_to_from_pcie_WREADY = M02_AXI_wready;
  m00_couplers_imp_13MYVT9 m00_couplers
       (.M_ACLK(M00_ACLK_1),
        .M_ARESETN(M00_ARESETN_1),
        .M_AXI_araddr(m00_couplers_to_from_pcie_ARADDR),
        .M_AXI_arburst(m00_couplers_to_from_pcie_ARBURST),
        .M_AXI_arcache(m00_couplers_to_from_pcie_ARCACHE),
        .M_AXI_arlen(m00_couplers_to_from_pcie_ARLEN),
        .M_AXI_arlock(m00_couplers_to_from_pcie_ARLOCK),
        .M_AXI_arprot(m00_couplers_to_from_pcie_ARPROT),
        .M_AXI_arqos(m00_couplers_to_from_pcie_ARQOS),
        .M_AXI_arready(m00_couplers_to_from_pcie_ARREADY),
        .M_AXI_arregion(m00_couplers_to_from_pcie_ARREGION),
        .M_AXI_arsize(m00_couplers_to_from_pcie_ARSIZE),
        .M_AXI_arvalid(m00_couplers_to_from_pcie_ARVALID),
        .M_AXI_awaddr(m00_couplers_to_from_pcie_AWADDR),
        .M_AXI_awburst(m00_couplers_to_from_pcie_AWBURST),
        .M_AXI_awcache(m00_couplers_to_from_pcie_AWCACHE),
        .M_AXI_awlen(m00_couplers_to_from_pcie_AWLEN),
        .M_AXI_awlock(m00_couplers_to_from_pcie_AWLOCK),
        .M_AXI_awprot(m00_couplers_to_from_pcie_AWPROT),
        .M_AXI_awqos(m00_couplers_to_from_pcie_AWQOS),
        .M_AXI_awready(m00_couplers_to_from_pcie_AWREADY),
        .M_AXI_awregion(m00_couplers_to_from_pcie_AWREGION),
        .M_AXI_awsize(m00_couplers_to_from_pcie_AWSIZE),
        .M_AXI_awvalid(m00_couplers_to_from_pcie_AWVALID),
        .M_AXI_bready(m00_couplers_to_from_pcie_BREADY),
        .M_AXI_bresp(m00_couplers_to_from_pcie_BRESP),
        .M_AXI_bvalid(m00_couplers_to_from_pcie_BVALID),
        .M_AXI_rdata(m00_couplers_to_from_pcie_RDATA),
        .M_AXI_rlast(m00_couplers_to_from_pcie_RLAST),
        .M_AXI_rready(m00_couplers_to_from_pcie_RREADY),
        .M_AXI_rresp(m00_couplers_to_from_pcie_RRESP),
        .M_AXI_rvalid(m00_couplers_to_from_pcie_RVALID),
        .M_AXI_wdata(m00_couplers_to_from_pcie_WDATA),
        .M_AXI_wlast(m00_couplers_to_from_pcie_WLAST),
        .M_AXI_wready(m00_couplers_to_from_pcie_WREADY),
        .M_AXI_wstrb(m00_couplers_to_from_pcie_WSTRB),
        .M_AXI_wvalid(m00_couplers_to_from_pcie_WVALID),
        .S_ACLK(from_pcie_ACLK_net),
        .S_ARESETN(from_pcie_ARESETN_net),
        .S_AXI_araddr(xbar_to_m00_couplers_ARADDR),
        .S_AXI_arburst(xbar_to_m00_couplers_ARBURST),
        .S_AXI_arcache(xbar_to_m00_couplers_ARCACHE),
        .S_AXI_arlen(xbar_to_m00_couplers_ARLEN),
        .S_AXI_arlock(xbar_to_m00_couplers_ARLOCK),
        .S_AXI_arprot(xbar_to_m00_couplers_ARPROT),
        .S_AXI_arqos(xbar_to_m00_couplers_ARQOS),
        .S_AXI_arready(xbar_to_m00_couplers_ARREADY),
        .S_AXI_arregion(xbar_to_m00_couplers_ARREGION),
        .S_AXI_arsize(xbar_to_m00_couplers_ARSIZE),
        .S_AXI_arvalid(xbar_to_m00_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m00_couplers_AWADDR),
        .S_AXI_awburst(xbar_to_m00_couplers_AWBURST),
        .S_AXI_awcache(xbar_to_m00_couplers_AWCACHE),
        .S_AXI_awlen(xbar_to_m00_couplers_AWLEN),
        .S_AXI_awlock(xbar_to_m00_couplers_AWLOCK),
        .S_AXI_awprot(xbar_to_m00_couplers_AWPROT),
        .S_AXI_awqos(xbar_to_m00_couplers_AWQOS),
        .S_AXI_awready(xbar_to_m00_couplers_AWREADY),
        .S_AXI_awregion(xbar_to_m00_couplers_AWREGION),
        .S_AXI_awsize(xbar_to_m00_couplers_AWSIZE),
        .S_AXI_awvalid(xbar_to_m00_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m00_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m00_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m00_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m00_couplers_RDATA),
        .S_AXI_rlast(xbar_to_m00_couplers_RLAST),
        .S_AXI_rready(xbar_to_m00_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m00_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m00_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m00_couplers_WDATA),
        .S_AXI_wlast(xbar_to_m00_couplers_WLAST),
        .S_AXI_wready(xbar_to_m00_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m00_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m00_couplers_WVALID));
  m01_couplers_imp_92MQL8 m01_couplers
       (.M_ACLK(M01_ACLK_1),
        .M_ARESETN(M01_ARESETN_1),
        .M_AXI_araddr(m01_couplers_to_from_pcie_ARADDR),
        .M_AXI_arready(m01_couplers_to_from_pcie_ARREADY),
        .M_AXI_arvalid(m01_couplers_to_from_pcie_ARVALID),
        .M_AXI_awaddr(m01_couplers_to_from_pcie_AWADDR),
        .M_AXI_awready(m01_couplers_to_from_pcie_AWREADY),
        .M_AXI_awvalid(m01_couplers_to_from_pcie_AWVALID),
        .M_AXI_bready(m01_couplers_to_from_pcie_BREADY),
        .M_AXI_bresp(m01_couplers_to_from_pcie_BRESP),
        .M_AXI_bvalid(m01_couplers_to_from_pcie_BVALID),
        .M_AXI_rdata(m01_couplers_to_from_pcie_RDATA),
        .M_AXI_rready(m01_couplers_to_from_pcie_RREADY),
        .M_AXI_rresp(m01_couplers_to_from_pcie_RRESP),
        .M_AXI_rvalid(m01_couplers_to_from_pcie_RVALID),
        .M_AXI_wdata(m01_couplers_to_from_pcie_WDATA),
        .M_AXI_wready(m01_couplers_to_from_pcie_WREADY),
        .M_AXI_wvalid(m01_couplers_to_from_pcie_WVALID),
        .S_ACLK(from_pcie_ACLK_net),
        .S_ARESETN(from_pcie_ARESETN_net),
        .S_AXI_araddr(xbar_to_m01_couplers_ARADDR),
        .S_AXI_arburst(xbar_to_m01_couplers_ARBURST),
        .S_AXI_arcache(xbar_to_m01_couplers_ARCACHE),
        .S_AXI_arlen(xbar_to_m01_couplers_ARLEN),
        .S_AXI_arlock(xbar_to_m01_couplers_ARLOCK),
        .S_AXI_arprot(xbar_to_m01_couplers_ARPROT),
        .S_AXI_arqos(xbar_to_m01_couplers_ARQOS),
        .S_AXI_arready(xbar_to_m01_couplers_ARREADY),
        .S_AXI_arregion(xbar_to_m01_couplers_ARREGION),
        .S_AXI_arsize(xbar_to_m01_couplers_ARSIZE),
        .S_AXI_arvalid(xbar_to_m01_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m01_couplers_AWADDR),
        .S_AXI_awburst(xbar_to_m01_couplers_AWBURST),
        .S_AXI_awcache(xbar_to_m01_couplers_AWCACHE),
        .S_AXI_awlen(xbar_to_m01_couplers_AWLEN),
        .S_AXI_awlock(xbar_to_m01_couplers_AWLOCK),
        .S_AXI_awprot(xbar_to_m01_couplers_AWPROT),
        .S_AXI_awqos(xbar_to_m01_couplers_AWQOS),
        .S_AXI_awready(xbar_to_m01_couplers_AWREADY),
        .S_AXI_awregion(xbar_to_m01_couplers_AWREGION),
        .S_AXI_awsize(xbar_to_m01_couplers_AWSIZE),
        .S_AXI_awvalid(xbar_to_m01_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m01_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m01_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m01_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m01_couplers_RDATA),
        .S_AXI_rlast(xbar_to_m01_couplers_RLAST),
        .S_AXI_rready(xbar_to_m01_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m01_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m01_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m01_couplers_WDATA),
        .S_AXI_wlast(xbar_to_m01_couplers_WLAST),
        .S_AXI_wready(xbar_to_m01_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m01_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m01_couplers_WVALID));
  m02_couplers_imp_2MPU5Q m02_couplers
       (.M_ACLK(M02_ACLK_1),
        .M_ARESETN(M02_ARESETN_1),
        .M_AXI_araddr(m02_couplers_to_from_pcie_ARADDR),
        .M_AXI_arready(m02_couplers_to_from_pcie_ARREADY),
        .M_AXI_arvalid(m02_couplers_to_from_pcie_ARVALID),
        .M_AXI_awaddr(m02_couplers_to_from_pcie_AWADDR),
        .M_AXI_awready(m02_couplers_to_from_pcie_AWREADY),
        .M_AXI_awvalid(m02_couplers_to_from_pcie_AWVALID),
        .M_AXI_bready(m02_couplers_to_from_pcie_BREADY),
        .M_AXI_bresp(m02_couplers_to_from_pcie_BRESP),
        .M_AXI_bvalid(m02_couplers_to_from_pcie_BVALID),
        .M_AXI_rdata(m02_couplers_to_from_pcie_RDATA),
        .M_AXI_rready(m02_couplers_to_from_pcie_RREADY),
        .M_AXI_rresp(m02_couplers_to_from_pcie_RRESP),
        .M_AXI_rvalid(m02_couplers_to_from_pcie_RVALID),
        .M_AXI_wdata(m02_couplers_to_from_pcie_WDATA),
        .M_AXI_wready(m02_couplers_to_from_pcie_WREADY),
        .M_AXI_wstrb(m02_couplers_to_from_pcie_WSTRB),
        .M_AXI_wvalid(m02_couplers_to_from_pcie_WVALID),
        .S_ACLK(from_pcie_ACLK_net),
        .S_ARESETN(from_pcie_ARESETN_net),
        .S_AXI_araddr(xbar_to_m02_couplers_ARADDR),
        .S_AXI_arburst(xbar_to_m02_couplers_ARBURST),
        .S_AXI_arcache(xbar_to_m02_couplers_ARCACHE),
        .S_AXI_arlen(xbar_to_m02_couplers_ARLEN),
        .S_AXI_arlock(xbar_to_m02_couplers_ARLOCK),
        .S_AXI_arprot(xbar_to_m02_couplers_ARPROT),
        .S_AXI_arqos(xbar_to_m02_couplers_ARQOS),
        .S_AXI_arready(xbar_to_m02_couplers_ARREADY),
        .S_AXI_arregion(xbar_to_m02_couplers_ARREGION),
        .S_AXI_arsize(xbar_to_m02_couplers_ARSIZE),
        .S_AXI_arvalid(xbar_to_m02_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m02_couplers_AWADDR),
        .S_AXI_awburst(xbar_to_m02_couplers_AWBURST),
        .S_AXI_awcache(xbar_to_m02_couplers_AWCACHE),
        .S_AXI_awlen(xbar_to_m02_couplers_AWLEN),
        .S_AXI_awlock(xbar_to_m02_couplers_AWLOCK),
        .S_AXI_awprot(xbar_to_m02_couplers_AWPROT),
        .S_AXI_awqos(xbar_to_m02_couplers_AWQOS),
        .S_AXI_awready(xbar_to_m02_couplers_AWREADY),
        .S_AXI_awregion(xbar_to_m02_couplers_AWREGION),
        .S_AXI_awsize(xbar_to_m02_couplers_AWSIZE),
        .S_AXI_awvalid(xbar_to_m02_couplers_AWVALID),
        .S_AXI_bready(xbar_to_m02_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m02_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m02_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m02_couplers_RDATA),
        .S_AXI_rlast(xbar_to_m02_couplers_RLAST),
        .S_AXI_rready(xbar_to_m02_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m02_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m02_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m02_couplers_WDATA),
        .S_AXI_wlast(xbar_to_m02_couplers_WLAST),
        .S_AXI_wready(xbar_to_m02_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m02_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m02_couplers_WVALID));
  s00_couplers_imp_1FF9K4C s00_couplers
       (.M_ACLK(from_pcie_ACLK_net),
        .M_ARESETN(from_pcie_ARESETN_net),
        .M_AXI_araddr(s00_couplers_to_xbar_ARADDR),
        .M_AXI_arburst(s00_couplers_to_xbar_ARBURST),
        .M_AXI_arcache(s00_couplers_to_xbar_ARCACHE),
        .M_AXI_arlen(s00_couplers_to_xbar_ARLEN),
        .M_AXI_arlock(s00_couplers_to_xbar_ARLOCK),
        .M_AXI_arprot(s00_couplers_to_xbar_ARPROT),
        .M_AXI_arready(s00_couplers_to_xbar_ARREADY),
        .M_AXI_arsize(s00_couplers_to_xbar_ARSIZE),
        .M_AXI_arvalid(s00_couplers_to_xbar_ARVALID),
        .M_AXI_awaddr(s00_couplers_to_xbar_AWADDR),
        .M_AXI_awburst(s00_couplers_to_xbar_AWBURST),
        .M_AXI_awcache(s00_couplers_to_xbar_AWCACHE),
        .M_AXI_awlen(s00_couplers_to_xbar_AWLEN),
        .M_AXI_awlock(s00_couplers_to_xbar_AWLOCK),
        .M_AXI_awprot(s00_couplers_to_xbar_AWPROT),
        .M_AXI_awready(s00_couplers_to_xbar_AWREADY),
        .M_AXI_awsize(s00_couplers_to_xbar_AWSIZE),
        .M_AXI_awvalid(s00_couplers_to_xbar_AWVALID),
        .M_AXI_bready(s00_couplers_to_xbar_BREADY),
        .M_AXI_bresp(s00_couplers_to_xbar_BRESP),
        .M_AXI_bvalid(s00_couplers_to_xbar_BVALID),
        .M_AXI_rdata(s00_couplers_to_xbar_RDATA),
        .M_AXI_rlast(s00_couplers_to_xbar_RLAST),
        .M_AXI_rready(s00_couplers_to_xbar_RREADY),
        .M_AXI_rresp(s00_couplers_to_xbar_RRESP),
        .M_AXI_rvalid(s00_couplers_to_xbar_RVALID),
        .M_AXI_wdata(s00_couplers_to_xbar_WDATA),
        .M_AXI_wlast(s00_couplers_to_xbar_WLAST),
        .M_AXI_wready(s00_couplers_to_xbar_WREADY),
        .M_AXI_wstrb(s00_couplers_to_xbar_WSTRB),
        .M_AXI_wvalid(s00_couplers_to_xbar_WVALID),
        .S_ACLK(S00_ACLK_1),
        .S_ARESETN(S00_ARESETN_1),
        .S_AXI_araddr(from_pcie_to_s00_couplers_ARADDR),
        .S_AXI_arburst(from_pcie_to_s00_couplers_ARBURST),
        .S_AXI_arcache(from_pcie_to_s00_couplers_ARCACHE),
        .S_AXI_arlen(from_pcie_to_s00_couplers_ARLEN),
        .S_AXI_arlock(from_pcie_to_s00_couplers_ARLOCK),
        .S_AXI_arprot(from_pcie_to_s00_couplers_ARPROT),
        .S_AXI_arready(from_pcie_to_s00_couplers_ARREADY),
        .S_AXI_arsize(from_pcie_to_s00_couplers_ARSIZE),
        .S_AXI_arvalid(from_pcie_to_s00_couplers_ARVALID),
        .S_AXI_awaddr(from_pcie_to_s00_couplers_AWADDR),
        .S_AXI_awburst(from_pcie_to_s00_couplers_AWBURST),
        .S_AXI_awcache(from_pcie_to_s00_couplers_AWCACHE),
        .S_AXI_awlen(from_pcie_to_s00_couplers_AWLEN),
        .S_AXI_awlock(from_pcie_to_s00_couplers_AWLOCK),
        .S_AXI_awprot(from_pcie_to_s00_couplers_AWPROT),
        .S_AXI_awready(from_pcie_to_s00_couplers_AWREADY),
        .S_AXI_awsize(from_pcie_to_s00_couplers_AWSIZE),
        .S_AXI_awvalid(from_pcie_to_s00_couplers_AWVALID),
        .S_AXI_bready(from_pcie_to_s00_couplers_BREADY),
        .S_AXI_bresp(from_pcie_to_s00_couplers_BRESP),
        .S_AXI_bvalid(from_pcie_to_s00_couplers_BVALID),
        .S_AXI_rdata(from_pcie_to_s00_couplers_RDATA),
        .S_AXI_rlast(from_pcie_to_s00_couplers_RLAST),
        .S_AXI_rready(from_pcie_to_s00_couplers_RREADY),
        .S_AXI_rresp(from_pcie_to_s00_couplers_RRESP),
        .S_AXI_rvalid(from_pcie_to_s00_couplers_RVALID),
        .S_AXI_wdata(from_pcie_to_s00_couplers_WDATA),
        .S_AXI_wlast(from_pcie_to_s00_couplers_WLAST),
        .S_AXI_wready(from_pcie_to_s00_couplers_WREADY),
        .S_AXI_wstrb(from_pcie_to_s00_couplers_WSTRB),
        .S_AXI_wvalid(from_pcie_to_s00_couplers_WVALID));
  System_xbar_2 xbar
       (.aclk(from_pcie_ACLK_net),
        .aresetn(from_pcie_ARESETN_net),
        .m_axi_araddr({xbar_to_m02_couplers_ARADDR,xbar_to_m01_couplers_ARADDR,xbar_to_m00_couplers_ARADDR}),
        .m_axi_arburst({xbar_to_m02_couplers_ARBURST,xbar_to_m01_couplers_ARBURST,xbar_to_m00_couplers_ARBURST}),
        .m_axi_arcache({xbar_to_m02_couplers_ARCACHE,xbar_to_m01_couplers_ARCACHE,xbar_to_m00_couplers_ARCACHE}),
        .m_axi_arlen({xbar_to_m02_couplers_ARLEN,xbar_to_m01_couplers_ARLEN,xbar_to_m00_couplers_ARLEN}),
        .m_axi_arlock({xbar_to_m02_couplers_ARLOCK,xbar_to_m01_couplers_ARLOCK,xbar_to_m00_couplers_ARLOCK}),
        .m_axi_arprot({xbar_to_m02_couplers_ARPROT,xbar_to_m01_couplers_ARPROT,xbar_to_m00_couplers_ARPROT}),
        .m_axi_arqos({xbar_to_m02_couplers_ARQOS,xbar_to_m01_couplers_ARQOS,xbar_to_m00_couplers_ARQOS}),
        .m_axi_arready({xbar_to_m02_couplers_ARREADY,xbar_to_m01_couplers_ARREADY,xbar_to_m00_couplers_ARREADY}),
        .m_axi_arregion({xbar_to_m02_couplers_ARREGION,xbar_to_m01_couplers_ARREGION,xbar_to_m00_couplers_ARREGION}),
        .m_axi_arsize({xbar_to_m02_couplers_ARSIZE,xbar_to_m01_couplers_ARSIZE,xbar_to_m00_couplers_ARSIZE}),
        .m_axi_arvalid({xbar_to_m02_couplers_ARVALID,xbar_to_m01_couplers_ARVALID,xbar_to_m00_couplers_ARVALID}),
        .m_axi_awaddr({xbar_to_m02_couplers_AWADDR,xbar_to_m01_couplers_AWADDR,xbar_to_m00_couplers_AWADDR}),
        .m_axi_awburst({xbar_to_m02_couplers_AWBURST,xbar_to_m01_couplers_AWBURST,xbar_to_m00_couplers_AWBURST}),
        .m_axi_awcache({xbar_to_m02_couplers_AWCACHE,xbar_to_m01_couplers_AWCACHE,xbar_to_m00_couplers_AWCACHE}),
        .m_axi_awlen({xbar_to_m02_couplers_AWLEN,xbar_to_m01_couplers_AWLEN,xbar_to_m00_couplers_AWLEN}),
        .m_axi_awlock({xbar_to_m02_couplers_AWLOCK,xbar_to_m01_couplers_AWLOCK,xbar_to_m00_couplers_AWLOCK}),
        .m_axi_awprot({xbar_to_m02_couplers_AWPROT,xbar_to_m01_couplers_AWPROT,xbar_to_m00_couplers_AWPROT}),
        .m_axi_awqos({xbar_to_m02_couplers_AWQOS,xbar_to_m01_couplers_AWQOS,xbar_to_m00_couplers_AWQOS}),
        .m_axi_awready({xbar_to_m02_couplers_AWREADY,xbar_to_m01_couplers_AWREADY,xbar_to_m00_couplers_AWREADY}),
        .m_axi_awregion({xbar_to_m02_couplers_AWREGION,xbar_to_m01_couplers_AWREGION,xbar_to_m00_couplers_AWREGION}),
        .m_axi_awsize({xbar_to_m02_couplers_AWSIZE,xbar_to_m01_couplers_AWSIZE,xbar_to_m00_couplers_AWSIZE}),
        .m_axi_awvalid({xbar_to_m02_couplers_AWVALID,xbar_to_m01_couplers_AWVALID,xbar_to_m00_couplers_AWVALID}),
        .m_axi_bready({xbar_to_m02_couplers_BREADY,xbar_to_m01_couplers_BREADY,xbar_to_m00_couplers_BREADY}),
        .m_axi_bresp({xbar_to_m02_couplers_BRESP,xbar_to_m01_couplers_BRESP,xbar_to_m00_couplers_BRESP}),
        .m_axi_bvalid({xbar_to_m02_couplers_BVALID,xbar_to_m01_couplers_BVALID,xbar_to_m00_couplers_BVALID}),
        .m_axi_rdata({xbar_to_m02_couplers_RDATA,xbar_to_m01_couplers_RDATA,xbar_to_m00_couplers_RDATA}),
        .m_axi_rlast({xbar_to_m02_couplers_RLAST,xbar_to_m01_couplers_RLAST,xbar_to_m00_couplers_RLAST}),
        .m_axi_rready({xbar_to_m02_couplers_RREADY,xbar_to_m01_couplers_RREADY,xbar_to_m00_couplers_RREADY}),
        .m_axi_rresp({xbar_to_m02_couplers_RRESP,xbar_to_m01_couplers_RRESP,xbar_to_m00_couplers_RRESP}),
        .m_axi_rvalid({xbar_to_m02_couplers_RVALID,xbar_to_m01_couplers_RVALID,xbar_to_m00_couplers_RVALID}),
        .m_axi_wdata({xbar_to_m02_couplers_WDATA,xbar_to_m01_couplers_WDATA,xbar_to_m00_couplers_WDATA}),
        .m_axi_wlast({xbar_to_m02_couplers_WLAST,xbar_to_m01_couplers_WLAST,xbar_to_m00_couplers_WLAST}),
        .m_axi_wready({xbar_to_m02_couplers_WREADY,xbar_to_m01_couplers_WREADY,xbar_to_m00_couplers_WREADY}),
        .m_axi_wstrb({xbar_to_m02_couplers_WSTRB,xbar_to_m01_couplers_WSTRB,xbar_to_m00_couplers_WSTRB}),
        .m_axi_wvalid({xbar_to_m02_couplers_WVALID,xbar_to_m01_couplers_WVALID,xbar_to_m00_couplers_WVALID}),
        .s_axi_araddr(s00_couplers_to_xbar_ARADDR),
        .s_axi_arburst(s00_couplers_to_xbar_ARBURST),
        .s_axi_arcache(s00_couplers_to_xbar_ARCACHE),
        .s_axi_arlen(s00_couplers_to_xbar_ARLEN),
        .s_axi_arlock(s00_couplers_to_xbar_ARLOCK),
        .s_axi_arprot(s00_couplers_to_xbar_ARPROT),
        .s_axi_arqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(s00_couplers_to_xbar_ARREADY),
        .s_axi_arsize(s00_couplers_to_xbar_ARSIZE),
        .s_axi_arvalid(s00_couplers_to_xbar_ARVALID),
        .s_axi_awaddr(s00_couplers_to_xbar_AWADDR),
        .s_axi_awburst(s00_couplers_to_xbar_AWBURST),
        .s_axi_awcache(s00_couplers_to_xbar_AWCACHE),
        .s_axi_awlen(s00_couplers_to_xbar_AWLEN),
        .s_axi_awlock(s00_couplers_to_xbar_AWLOCK),
        .s_axi_awprot(s00_couplers_to_xbar_AWPROT),
        .s_axi_awqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(s00_couplers_to_xbar_AWREADY),
        .s_axi_awsize(s00_couplers_to_xbar_AWSIZE),
        .s_axi_awvalid(s00_couplers_to_xbar_AWVALID),
        .s_axi_bready(s00_couplers_to_xbar_BREADY),
        .s_axi_bresp(s00_couplers_to_xbar_BRESP),
        .s_axi_bvalid(s00_couplers_to_xbar_BVALID),
        .s_axi_rdata(s00_couplers_to_xbar_RDATA),
        .s_axi_rlast(s00_couplers_to_xbar_RLAST),
        .s_axi_rready(s00_couplers_to_xbar_RREADY),
        .s_axi_rresp(s00_couplers_to_xbar_RRESP),
        .s_axi_rvalid(s00_couplers_to_xbar_RVALID),
        .s_axi_wdata(s00_couplers_to_xbar_WDATA),
        .s_axi_wlast(s00_couplers_to_xbar_WLAST),
        .s_axi_wready(s00_couplers_to_xbar_WREADY),
        .s_axi_wstrb(s00_couplers_to_xbar_WSTRB),
        .s_axi_wvalid(s00_couplers_to_xbar_WVALID));
endmodule

module System_axi_interconnect_0_1
   (ACLK,
    ARESETN,
    M00_ACLK,
    M00_ARESETN,
    M00_AXI_araddr,
    M00_AXI_arburst,
    M00_AXI_arcache,
    M00_AXI_arid,
    M00_AXI_arlen,
    M00_AXI_arlock,
    M00_AXI_arprot,
    M00_AXI_arqos,
    M00_AXI_arready,
    M00_AXI_arsize,
    M00_AXI_arvalid,
    M00_AXI_awaddr,
    M00_AXI_awburst,
    M00_AXI_awcache,
    M00_AXI_awid,
    M00_AXI_awlen,
    M00_AXI_awlock,
    M00_AXI_awprot,
    M00_AXI_awqos,
    M00_AXI_awready,
    M00_AXI_awsize,
    M00_AXI_awvalid,
    M00_AXI_bid,
    M00_AXI_bready,
    M00_AXI_bresp,
    M00_AXI_bvalid,
    M00_AXI_rdata,
    M00_AXI_rid,
    M00_AXI_rlast,
    M00_AXI_rready,
    M00_AXI_rresp,
    M00_AXI_rvalid,
    M00_AXI_wdata,
    M00_AXI_wlast,
    M00_AXI_wready,
    M00_AXI_wstrb,
    M00_AXI_wvalid,
    M01_ACLK,
    M01_ARESETN,
    M01_AXI_araddr,
    M01_AXI_arburst,
    M01_AXI_arlen,
    M01_AXI_arready,
    M01_AXI_arregion,
    M01_AXI_arsize,
    M01_AXI_arvalid,
    M01_AXI_awaddr,
    M01_AXI_awburst,
    M01_AXI_awlen,
    M01_AXI_awready,
    M01_AXI_awregion,
    M01_AXI_awsize,
    M01_AXI_awvalid,
    M01_AXI_bready,
    M01_AXI_bresp,
    M01_AXI_bvalid,
    M01_AXI_rdata,
    M01_AXI_rlast,
    M01_AXI_rready,
    M01_AXI_rresp,
    M01_AXI_rvalid,
    M01_AXI_wdata,
    M01_AXI_wlast,
    M01_AXI_wready,
    M01_AXI_wstrb,
    M01_AXI_wvalid,
    S00_ACLK,
    S00_ARESETN,
    S00_AXI_araddr,
    S00_AXI_arburst,
    S00_AXI_arcache,
    S00_AXI_arlen,
    S00_AXI_arlock,
    S00_AXI_arprot,
    S00_AXI_arqos,
    S00_AXI_arready,
    S00_AXI_arregion,
    S00_AXI_arsize,
    S00_AXI_arvalid,
    S00_AXI_awaddr,
    S00_AXI_awburst,
    S00_AXI_awcache,
    S00_AXI_awlen,
    S00_AXI_awlock,
    S00_AXI_awprot,
    S00_AXI_awqos,
    S00_AXI_awready,
    S00_AXI_awregion,
    S00_AXI_awsize,
    S00_AXI_awvalid,
    S00_AXI_bready,
    S00_AXI_bresp,
    S00_AXI_bvalid,
    S00_AXI_rdata,
    S00_AXI_rlast,
    S00_AXI_rready,
    S00_AXI_rresp,
    S00_AXI_rvalid,
    S00_AXI_wdata,
    S00_AXI_wlast,
    S00_AXI_wready,
    S00_AXI_wstrb,
    S00_AXI_wvalid,
    S01_ACLK,
    S01_ARESETN,
    S01_AXI_araddr,
    S01_AXI_arburst,
    S01_AXI_arcache,
    S01_AXI_arid,
    S01_AXI_arlen,
    S01_AXI_arlock,
    S01_AXI_arprot,
    S01_AXI_arqos,
    S01_AXI_arready,
    S01_AXI_arregion,
    S01_AXI_arsize,
    S01_AXI_arvalid,
    S01_AXI_awaddr,
    S01_AXI_awburst,
    S01_AXI_awcache,
    S01_AXI_awid,
    S01_AXI_awlen,
    S01_AXI_awlock,
    S01_AXI_awprot,
    S01_AXI_awqos,
    S01_AXI_awready,
    S01_AXI_awregion,
    S01_AXI_awsize,
    S01_AXI_awvalid,
    S01_AXI_bid,
    S01_AXI_bready,
    S01_AXI_bresp,
    S01_AXI_bvalid,
    S01_AXI_rdata,
    S01_AXI_rid,
    S01_AXI_rlast,
    S01_AXI_rready,
    S01_AXI_rresp,
    S01_AXI_rvalid,
    S01_AXI_wdata,
    S01_AXI_wlast,
    S01_AXI_wready,
    S01_AXI_wstrb,
    S01_AXI_wvalid,
    S02_ACLK,
    S02_ARESETN,
    S02_AXI_araddr,
    S02_AXI_arburst,
    S02_AXI_arcache,
    S02_AXI_arid,
    S02_AXI_arlen,
    S02_AXI_arlock,
    S02_AXI_arprot,
    S02_AXI_arqos,
    S02_AXI_arready,
    S02_AXI_arsize,
    S02_AXI_arvalid,
    S02_AXI_awaddr,
    S02_AXI_awburst,
    S02_AXI_awcache,
    S02_AXI_awid,
    S02_AXI_awlen,
    S02_AXI_awlock,
    S02_AXI_awprot,
    S02_AXI_awqos,
    S02_AXI_awready,
    S02_AXI_awsize,
    S02_AXI_awvalid,
    S02_AXI_bid,
    S02_AXI_bready,
    S02_AXI_bresp,
    S02_AXI_bvalid,
    S02_AXI_rdata,
    S02_AXI_rid,
    S02_AXI_rlast,
    S02_AXI_rready,
    S02_AXI_rresp,
    S02_AXI_rvalid,
    S02_AXI_wdata,
    S02_AXI_wlast,
    S02_AXI_wready,
    S02_AXI_wstrb,
    S02_AXI_wvalid);
  input ACLK;
  input ARESETN;
  input M00_ACLK;
  input M00_ARESETN;
  output [31:0]M00_AXI_araddr;
  output [1:0]M00_AXI_arburst;
  output [3:0]M00_AXI_arcache;
  output [2:0]M00_AXI_arid;
  output [7:0]M00_AXI_arlen;
  output M00_AXI_arlock;
  output [2:0]M00_AXI_arprot;
  output [3:0]M00_AXI_arqos;
  input M00_AXI_arready;
  output [2:0]M00_AXI_arsize;
  output M00_AXI_arvalid;
  output [31:0]M00_AXI_awaddr;
  output [1:0]M00_AXI_awburst;
  output [3:0]M00_AXI_awcache;
  output [2:0]M00_AXI_awid;
  output [7:0]M00_AXI_awlen;
  output M00_AXI_awlock;
  output [2:0]M00_AXI_awprot;
  output [3:0]M00_AXI_awqos;
  input M00_AXI_awready;
  output [2:0]M00_AXI_awsize;
  output M00_AXI_awvalid;
  input [2:0]M00_AXI_bid;
  output M00_AXI_bready;
  input [1:0]M00_AXI_bresp;
  input M00_AXI_bvalid;
  input [511:0]M00_AXI_rdata;
  input [2:0]M00_AXI_rid;
  input M00_AXI_rlast;
  output M00_AXI_rready;
  input [1:0]M00_AXI_rresp;
  input M00_AXI_rvalid;
  output [511:0]M00_AXI_wdata;
  output M00_AXI_wlast;
  input M00_AXI_wready;
  output [63:0]M00_AXI_wstrb;
  output M00_AXI_wvalid;
  input M01_ACLK;
  input M01_ARESETN;
  output [31:0]M01_AXI_araddr;
  output [1:0]M01_AXI_arburst;
  output [7:0]M01_AXI_arlen;
  input M01_AXI_arready;
  output [3:0]M01_AXI_arregion;
  output [2:0]M01_AXI_arsize;
  output M01_AXI_arvalid;
  output [31:0]M01_AXI_awaddr;
  output [1:0]M01_AXI_awburst;
  output [7:0]M01_AXI_awlen;
  input M01_AXI_awready;
  output [3:0]M01_AXI_awregion;
  output [2:0]M01_AXI_awsize;
  output M01_AXI_awvalid;
  output M01_AXI_bready;
  input [1:0]M01_AXI_bresp;
  input M01_AXI_bvalid;
  input [127:0]M01_AXI_rdata;
  input M01_AXI_rlast;
  output M01_AXI_rready;
  input [1:0]M01_AXI_rresp;
  input M01_AXI_rvalid;
  output [127:0]M01_AXI_wdata;
  output M01_AXI_wlast;
  input M01_AXI_wready;
  output [15:0]M01_AXI_wstrb;
  output M01_AXI_wvalid;
  input S00_ACLK;
  input S00_ARESETN;
  input [31:0]S00_AXI_araddr;
  input [1:0]S00_AXI_arburst;
  input [3:0]S00_AXI_arcache;
  input [7:0]S00_AXI_arlen;
  input [0:0]S00_AXI_arlock;
  input [2:0]S00_AXI_arprot;
  input [3:0]S00_AXI_arqos;
  output S00_AXI_arready;
  input [3:0]S00_AXI_arregion;
  input [2:0]S00_AXI_arsize;
  input S00_AXI_arvalid;
  input [31:0]S00_AXI_awaddr;
  input [1:0]S00_AXI_awburst;
  input [3:0]S00_AXI_awcache;
  input [7:0]S00_AXI_awlen;
  input [0:0]S00_AXI_awlock;
  input [2:0]S00_AXI_awprot;
  input [3:0]S00_AXI_awqos;
  output S00_AXI_awready;
  input [3:0]S00_AXI_awregion;
  input [2:0]S00_AXI_awsize;
  input S00_AXI_awvalid;
  input S00_AXI_bready;
  output [1:0]S00_AXI_bresp;
  output S00_AXI_bvalid;
  output [511:0]S00_AXI_rdata;
  output S00_AXI_rlast;
  input S00_AXI_rready;
  output [1:0]S00_AXI_rresp;
  output S00_AXI_rvalid;
  input [511:0]S00_AXI_wdata;
  input S00_AXI_wlast;
  output S00_AXI_wready;
  input [63:0]S00_AXI_wstrb;
  input S00_AXI_wvalid;
  input S01_ACLK;
  input S01_ARESETN;
  input [31:0]S01_AXI_araddr;
  input [1:0]S01_AXI_arburst;
  input [3:0]S01_AXI_arcache;
  input [0:0]S01_AXI_arid;
  input [7:0]S01_AXI_arlen;
  input [0:0]S01_AXI_arlock;
  input [2:0]S01_AXI_arprot;
  input [3:0]S01_AXI_arqos;
  output S01_AXI_arready;
  input [3:0]S01_AXI_arregion;
  input [2:0]S01_AXI_arsize;
  input S01_AXI_arvalid;
  input [31:0]S01_AXI_awaddr;
  input [1:0]S01_AXI_awburst;
  input [3:0]S01_AXI_awcache;
  input [0:0]S01_AXI_awid;
  input [7:0]S01_AXI_awlen;
  input [0:0]S01_AXI_awlock;
  input [2:0]S01_AXI_awprot;
  input [3:0]S01_AXI_awqos;
  output S01_AXI_awready;
  input [3:0]S01_AXI_awregion;
  input [2:0]S01_AXI_awsize;
  input S01_AXI_awvalid;
  output [0:0]S01_AXI_bid;
  input S01_AXI_bready;
  output [1:0]S01_AXI_bresp;
  output S01_AXI_bvalid;
  output [511:0]S01_AXI_rdata;
  output [0:0]S01_AXI_rid;
  output S01_AXI_rlast;
  input S01_AXI_rready;
  output [1:0]S01_AXI_rresp;
  output S01_AXI_rvalid;
  input [511:0]S01_AXI_wdata;
  input S01_AXI_wlast;
  output S01_AXI_wready;
  input [63:0]S01_AXI_wstrb;
  input S01_AXI_wvalid;
  input S02_ACLK;
  input S02_ARESETN;
  input [31:0]S02_AXI_araddr;
  input [1:0]S02_AXI_arburst;
  input [3:0]S02_AXI_arcache;
  input [0:0]S02_AXI_arid;
  input [7:0]S02_AXI_arlen;
  input S02_AXI_arlock;
  input [2:0]S02_AXI_arprot;
  input [3:0]S02_AXI_arqos;
  output S02_AXI_arready;
  input [2:0]S02_AXI_arsize;
  input S02_AXI_arvalid;
  input [31:0]S02_AXI_awaddr;
  input [1:0]S02_AXI_awburst;
  input [3:0]S02_AXI_awcache;
  input [0:0]S02_AXI_awid;
  input [7:0]S02_AXI_awlen;
  input S02_AXI_awlock;
  input [2:0]S02_AXI_awprot;
  input [3:0]S02_AXI_awqos;
  output S02_AXI_awready;
  input [2:0]S02_AXI_awsize;
  input S02_AXI_awvalid;
  output [0:0]S02_AXI_bid;
  input S02_AXI_bready;
  output [1:0]S02_AXI_bresp;
  output S02_AXI_bvalid;
  output [511:0]S02_AXI_rdata;
  output [0:0]S02_AXI_rid;
  output S02_AXI_rlast;
  input S02_AXI_rready;
  output [1:0]S02_AXI_rresp;
  output S02_AXI_rvalid;
  input [511:0]S02_AXI_wdata;
  input S02_AXI_wlast;
  output S02_AXI_wready;
  input [63:0]S02_AXI_wstrb;
  input S02_AXI_wvalid;

  wire M00_ACLK_1;
  wire M00_ARESETN_1;
  wire M01_ACLK_1;
  wire M01_ARESETN_1;
  wire S00_ACLK_1;
  wire S00_ARESETN_1;
  wire S01_ACLK_1;
  wire S01_ARESETN_1;
  wire S02_ACLK_1;
  wire S02_ARESETN_1;
  wire [31:0]m00_couplers_to_to_memory_ARADDR;
  wire [1:0]m00_couplers_to_to_memory_ARBURST;
  wire [3:0]m00_couplers_to_to_memory_ARCACHE;
  wire [2:0]m00_couplers_to_to_memory_ARID;
  wire [7:0]m00_couplers_to_to_memory_ARLEN;
  wire m00_couplers_to_to_memory_ARLOCK;
  wire [2:0]m00_couplers_to_to_memory_ARPROT;
  wire [3:0]m00_couplers_to_to_memory_ARQOS;
  wire m00_couplers_to_to_memory_ARREADY;
  wire [2:0]m00_couplers_to_to_memory_ARSIZE;
  wire m00_couplers_to_to_memory_ARVALID;
  wire [31:0]m00_couplers_to_to_memory_AWADDR;
  wire [1:0]m00_couplers_to_to_memory_AWBURST;
  wire [3:0]m00_couplers_to_to_memory_AWCACHE;
  wire [2:0]m00_couplers_to_to_memory_AWID;
  wire [7:0]m00_couplers_to_to_memory_AWLEN;
  wire m00_couplers_to_to_memory_AWLOCK;
  wire [2:0]m00_couplers_to_to_memory_AWPROT;
  wire [3:0]m00_couplers_to_to_memory_AWQOS;
  wire m00_couplers_to_to_memory_AWREADY;
  wire [2:0]m00_couplers_to_to_memory_AWSIZE;
  wire m00_couplers_to_to_memory_AWVALID;
  wire [2:0]m00_couplers_to_to_memory_BID;
  wire m00_couplers_to_to_memory_BREADY;
  wire [1:0]m00_couplers_to_to_memory_BRESP;
  wire m00_couplers_to_to_memory_BVALID;
  wire [511:0]m00_couplers_to_to_memory_RDATA;
  wire [2:0]m00_couplers_to_to_memory_RID;
  wire m00_couplers_to_to_memory_RLAST;
  wire m00_couplers_to_to_memory_RREADY;
  wire [1:0]m00_couplers_to_to_memory_RRESP;
  wire m00_couplers_to_to_memory_RVALID;
  wire [511:0]m00_couplers_to_to_memory_WDATA;
  wire m00_couplers_to_to_memory_WLAST;
  wire m00_couplers_to_to_memory_WREADY;
  wire [63:0]m00_couplers_to_to_memory_WSTRB;
  wire m00_couplers_to_to_memory_WVALID;
  wire [31:0]m01_couplers_to_to_memory_ARADDR;
  wire [1:0]m01_couplers_to_to_memory_ARBURST;
  wire [7:0]m01_couplers_to_to_memory_ARLEN;
  wire m01_couplers_to_to_memory_ARREADY;
  wire [3:0]m01_couplers_to_to_memory_ARREGION;
  wire [2:0]m01_couplers_to_to_memory_ARSIZE;
  wire m01_couplers_to_to_memory_ARVALID;
  wire [31:0]m01_couplers_to_to_memory_AWADDR;
  wire [1:0]m01_couplers_to_to_memory_AWBURST;
  wire [7:0]m01_couplers_to_to_memory_AWLEN;
  wire m01_couplers_to_to_memory_AWREADY;
  wire [3:0]m01_couplers_to_to_memory_AWREGION;
  wire [2:0]m01_couplers_to_to_memory_AWSIZE;
  wire m01_couplers_to_to_memory_AWVALID;
  wire m01_couplers_to_to_memory_BREADY;
  wire [1:0]m01_couplers_to_to_memory_BRESP;
  wire m01_couplers_to_to_memory_BVALID;
  wire [127:0]m01_couplers_to_to_memory_RDATA;
  wire m01_couplers_to_to_memory_RLAST;
  wire m01_couplers_to_to_memory_RREADY;
  wire [1:0]m01_couplers_to_to_memory_RRESP;
  wire m01_couplers_to_to_memory_RVALID;
  wire [127:0]m01_couplers_to_to_memory_WDATA;
  wire m01_couplers_to_to_memory_WLAST;
  wire m01_couplers_to_to_memory_WREADY;
  wire [15:0]m01_couplers_to_to_memory_WSTRB;
  wire m01_couplers_to_to_memory_WVALID;
  wire [31:0]s00_couplers_to_xbar_ARADDR;
  wire [1:0]s00_couplers_to_xbar_ARBURST;
  wire [3:0]s00_couplers_to_xbar_ARCACHE;
  wire [7:0]s00_couplers_to_xbar_ARLEN;
  wire [0:0]s00_couplers_to_xbar_ARLOCK;
  wire [2:0]s00_couplers_to_xbar_ARPROT;
  wire [3:0]s00_couplers_to_xbar_ARQOS;
  wire [0:0]s00_couplers_to_xbar_ARREADY;
  wire [2:0]s00_couplers_to_xbar_ARSIZE;
  wire s00_couplers_to_xbar_ARVALID;
  wire [31:0]s00_couplers_to_xbar_AWADDR;
  wire [1:0]s00_couplers_to_xbar_AWBURST;
  wire [3:0]s00_couplers_to_xbar_AWCACHE;
  wire [7:0]s00_couplers_to_xbar_AWLEN;
  wire [0:0]s00_couplers_to_xbar_AWLOCK;
  wire [2:0]s00_couplers_to_xbar_AWPROT;
  wire [3:0]s00_couplers_to_xbar_AWQOS;
  wire [0:0]s00_couplers_to_xbar_AWREADY;
  wire [2:0]s00_couplers_to_xbar_AWSIZE;
  wire s00_couplers_to_xbar_AWVALID;
  wire s00_couplers_to_xbar_BREADY;
  wire [1:0]s00_couplers_to_xbar_BRESP;
  wire [0:0]s00_couplers_to_xbar_BVALID;
  wire [511:0]s00_couplers_to_xbar_RDATA;
  wire [0:0]s00_couplers_to_xbar_RLAST;
  wire s00_couplers_to_xbar_RREADY;
  wire [1:0]s00_couplers_to_xbar_RRESP;
  wire [0:0]s00_couplers_to_xbar_RVALID;
  wire [511:0]s00_couplers_to_xbar_WDATA;
  wire s00_couplers_to_xbar_WLAST;
  wire [0:0]s00_couplers_to_xbar_WREADY;
  wire [63:0]s00_couplers_to_xbar_WSTRB;
  wire s00_couplers_to_xbar_WVALID;
  wire [31:0]s01_couplers_to_xbar_ARADDR;
  wire [1:0]s01_couplers_to_xbar_ARBURST;
  wire [3:0]s01_couplers_to_xbar_ARCACHE;
  wire [0:0]s01_couplers_to_xbar_ARID;
  wire [7:0]s01_couplers_to_xbar_ARLEN;
  wire [0:0]s01_couplers_to_xbar_ARLOCK;
  wire [2:0]s01_couplers_to_xbar_ARPROT;
  wire [3:0]s01_couplers_to_xbar_ARQOS;
  wire [1:1]s01_couplers_to_xbar_ARREADY;
  wire [2:0]s01_couplers_to_xbar_ARSIZE;
  wire s01_couplers_to_xbar_ARVALID;
  wire [31:0]s01_couplers_to_xbar_AWADDR;
  wire [1:0]s01_couplers_to_xbar_AWBURST;
  wire [3:0]s01_couplers_to_xbar_AWCACHE;
  wire [0:0]s01_couplers_to_xbar_AWID;
  wire [7:0]s01_couplers_to_xbar_AWLEN;
  wire [0:0]s01_couplers_to_xbar_AWLOCK;
  wire [2:0]s01_couplers_to_xbar_AWPROT;
  wire [3:0]s01_couplers_to_xbar_AWQOS;
  wire [1:1]s01_couplers_to_xbar_AWREADY;
  wire [2:0]s01_couplers_to_xbar_AWSIZE;
  wire s01_couplers_to_xbar_AWVALID;
  wire [5:3]s01_couplers_to_xbar_BID;
  wire s01_couplers_to_xbar_BREADY;
  wire [3:2]s01_couplers_to_xbar_BRESP;
  wire [1:1]s01_couplers_to_xbar_BVALID;
  wire [1023:512]s01_couplers_to_xbar_RDATA;
  wire [5:3]s01_couplers_to_xbar_RID;
  wire [1:1]s01_couplers_to_xbar_RLAST;
  wire s01_couplers_to_xbar_RREADY;
  wire [3:2]s01_couplers_to_xbar_RRESP;
  wire [1:1]s01_couplers_to_xbar_RVALID;
  wire [511:0]s01_couplers_to_xbar_WDATA;
  wire s01_couplers_to_xbar_WLAST;
  wire [1:1]s01_couplers_to_xbar_WREADY;
  wire [63:0]s01_couplers_to_xbar_WSTRB;
  wire s01_couplers_to_xbar_WVALID;
  wire [31:0]s02_couplers_to_xbar_ARADDR;
  wire [1:0]s02_couplers_to_xbar_ARBURST;
  wire [3:0]s02_couplers_to_xbar_ARCACHE;
  wire [0:0]s02_couplers_to_xbar_ARID;
  wire [7:0]s02_couplers_to_xbar_ARLEN;
  wire [0:0]s02_couplers_to_xbar_ARLOCK;
  wire [2:0]s02_couplers_to_xbar_ARPROT;
  wire [3:0]s02_couplers_to_xbar_ARQOS;
  wire [2:2]s02_couplers_to_xbar_ARREADY;
  wire [2:0]s02_couplers_to_xbar_ARSIZE;
  wire s02_couplers_to_xbar_ARVALID;
  wire [31:0]s02_couplers_to_xbar_AWADDR;
  wire [1:0]s02_couplers_to_xbar_AWBURST;
  wire [3:0]s02_couplers_to_xbar_AWCACHE;
  wire [0:0]s02_couplers_to_xbar_AWID;
  wire [7:0]s02_couplers_to_xbar_AWLEN;
  wire [0:0]s02_couplers_to_xbar_AWLOCK;
  wire [2:0]s02_couplers_to_xbar_AWPROT;
  wire [3:0]s02_couplers_to_xbar_AWQOS;
  wire [2:2]s02_couplers_to_xbar_AWREADY;
  wire [2:0]s02_couplers_to_xbar_AWSIZE;
  wire s02_couplers_to_xbar_AWVALID;
  wire [8:6]s02_couplers_to_xbar_BID;
  wire s02_couplers_to_xbar_BREADY;
  wire [5:4]s02_couplers_to_xbar_BRESP;
  wire [2:2]s02_couplers_to_xbar_BVALID;
  wire [1535:1024]s02_couplers_to_xbar_RDATA;
  wire [8:6]s02_couplers_to_xbar_RID;
  wire [2:2]s02_couplers_to_xbar_RLAST;
  wire s02_couplers_to_xbar_RREADY;
  wire [5:4]s02_couplers_to_xbar_RRESP;
  wire [2:2]s02_couplers_to_xbar_RVALID;
  wire [511:0]s02_couplers_to_xbar_WDATA;
  wire s02_couplers_to_xbar_WLAST;
  wire [2:2]s02_couplers_to_xbar_WREADY;
  wire [63:0]s02_couplers_to_xbar_WSTRB;
  wire s02_couplers_to_xbar_WVALID;
  wire to_memory_ACLK_net;
  wire to_memory_ARESETN_net;
  wire [31:0]to_memory_to_s00_couplers_ARADDR;
  wire [1:0]to_memory_to_s00_couplers_ARBURST;
  wire [3:0]to_memory_to_s00_couplers_ARCACHE;
  wire [7:0]to_memory_to_s00_couplers_ARLEN;
  wire [0:0]to_memory_to_s00_couplers_ARLOCK;
  wire [2:0]to_memory_to_s00_couplers_ARPROT;
  wire [3:0]to_memory_to_s00_couplers_ARQOS;
  wire to_memory_to_s00_couplers_ARREADY;
  wire [3:0]to_memory_to_s00_couplers_ARREGION;
  wire [2:0]to_memory_to_s00_couplers_ARSIZE;
  wire to_memory_to_s00_couplers_ARVALID;
  wire [31:0]to_memory_to_s00_couplers_AWADDR;
  wire [1:0]to_memory_to_s00_couplers_AWBURST;
  wire [3:0]to_memory_to_s00_couplers_AWCACHE;
  wire [7:0]to_memory_to_s00_couplers_AWLEN;
  wire [0:0]to_memory_to_s00_couplers_AWLOCK;
  wire [2:0]to_memory_to_s00_couplers_AWPROT;
  wire [3:0]to_memory_to_s00_couplers_AWQOS;
  wire to_memory_to_s00_couplers_AWREADY;
  wire [3:0]to_memory_to_s00_couplers_AWREGION;
  wire [2:0]to_memory_to_s00_couplers_AWSIZE;
  wire to_memory_to_s00_couplers_AWVALID;
  wire to_memory_to_s00_couplers_BREADY;
  wire [1:0]to_memory_to_s00_couplers_BRESP;
  wire to_memory_to_s00_couplers_BVALID;
  wire [511:0]to_memory_to_s00_couplers_RDATA;
  wire to_memory_to_s00_couplers_RLAST;
  wire to_memory_to_s00_couplers_RREADY;
  wire [1:0]to_memory_to_s00_couplers_RRESP;
  wire to_memory_to_s00_couplers_RVALID;
  wire [511:0]to_memory_to_s00_couplers_WDATA;
  wire to_memory_to_s00_couplers_WLAST;
  wire to_memory_to_s00_couplers_WREADY;
  wire [63:0]to_memory_to_s00_couplers_WSTRB;
  wire to_memory_to_s00_couplers_WVALID;
  wire [31:0]to_memory_to_s01_couplers_ARADDR;
  wire [1:0]to_memory_to_s01_couplers_ARBURST;
  wire [3:0]to_memory_to_s01_couplers_ARCACHE;
  wire [0:0]to_memory_to_s01_couplers_ARID;
  wire [7:0]to_memory_to_s01_couplers_ARLEN;
  wire [0:0]to_memory_to_s01_couplers_ARLOCK;
  wire [2:0]to_memory_to_s01_couplers_ARPROT;
  wire [3:0]to_memory_to_s01_couplers_ARQOS;
  wire to_memory_to_s01_couplers_ARREADY;
  wire [3:0]to_memory_to_s01_couplers_ARREGION;
  wire [2:0]to_memory_to_s01_couplers_ARSIZE;
  wire to_memory_to_s01_couplers_ARVALID;
  wire [31:0]to_memory_to_s01_couplers_AWADDR;
  wire [1:0]to_memory_to_s01_couplers_AWBURST;
  wire [3:0]to_memory_to_s01_couplers_AWCACHE;
  wire [0:0]to_memory_to_s01_couplers_AWID;
  wire [7:0]to_memory_to_s01_couplers_AWLEN;
  wire [0:0]to_memory_to_s01_couplers_AWLOCK;
  wire [2:0]to_memory_to_s01_couplers_AWPROT;
  wire [3:0]to_memory_to_s01_couplers_AWQOS;
  wire to_memory_to_s01_couplers_AWREADY;
  wire [3:0]to_memory_to_s01_couplers_AWREGION;
  wire [2:0]to_memory_to_s01_couplers_AWSIZE;
  wire to_memory_to_s01_couplers_AWVALID;
  wire [0:0]to_memory_to_s01_couplers_BID;
  wire to_memory_to_s01_couplers_BREADY;
  wire [1:0]to_memory_to_s01_couplers_BRESP;
  wire to_memory_to_s01_couplers_BVALID;
  wire [511:0]to_memory_to_s01_couplers_RDATA;
  wire [0:0]to_memory_to_s01_couplers_RID;
  wire to_memory_to_s01_couplers_RLAST;
  wire to_memory_to_s01_couplers_RREADY;
  wire [1:0]to_memory_to_s01_couplers_RRESP;
  wire to_memory_to_s01_couplers_RVALID;
  wire [511:0]to_memory_to_s01_couplers_WDATA;
  wire to_memory_to_s01_couplers_WLAST;
  wire to_memory_to_s01_couplers_WREADY;
  wire [63:0]to_memory_to_s01_couplers_WSTRB;
  wire to_memory_to_s01_couplers_WVALID;
  wire [31:0]to_memory_to_s02_couplers_ARADDR;
  wire [1:0]to_memory_to_s02_couplers_ARBURST;
  wire [3:0]to_memory_to_s02_couplers_ARCACHE;
  wire [0:0]to_memory_to_s02_couplers_ARID;
  wire [7:0]to_memory_to_s02_couplers_ARLEN;
  wire to_memory_to_s02_couplers_ARLOCK;
  wire [2:0]to_memory_to_s02_couplers_ARPROT;
  wire [3:0]to_memory_to_s02_couplers_ARQOS;
  wire to_memory_to_s02_couplers_ARREADY;
  wire [2:0]to_memory_to_s02_couplers_ARSIZE;
  wire to_memory_to_s02_couplers_ARVALID;
  wire [31:0]to_memory_to_s02_couplers_AWADDR;
  wire [1:0]to_memory_to_s02_couplers_AWBURST;
  wire [3:0]to_memory_to_s02_couplers_AWCACHE;
  wire [0:0]to_memory_to_s02_couplers_AWID;
  wire [7:0]to_memory_to_s02_couplers_AWLEN;
  wire to_memory_to_s02_couplers_AWLOCK;
  wire [2:0]to_memory_to_s02_couplers_AWPROT;
  wire [3:0]to_memory_to_s02_couplers_AWQOS;
  wire to_memory_to_s02_couplers_AWREADY;
  wire [2:0]to_memory_to_s02_couplers_AWSIZE;
  wire to_memory_to_s02_couplers_AWVALID;
  wire [0:0]to_memory_to_s02_couplers_BID;
  wire to_memory_to_s02_couplers_BREADY;
  wire [1:0]to_memory_to_s02_couplers_BRESP;
  wire to_memory_to_s02_couplers_BVALID;
  wire [511:0]to_memory_to_s02_couplers_RDATA;
  wire [0:0]to_memory_to_s02_couplers_RID;
  wire to_memory_to_s02_couplers_RLAST;
  wire to_memory_to_s02_couplers_RREADY;
  wire [1:0]to_memory_to_s02_couplers_RRESP;
  wire to_memory_to_s02_couplers_RVALID;
  wire [511:0]to_memory_to_s02_couplers_WDATA;
  wire to_memory_to_s02_couplers_WLAST;
  wire to_memory_to_s02_couplers_WREADY;
  wire [63:0]to_memory_to_s02_couplers_WSTRB;
  wire to_memory_to_s02_couplers_WVALID;
  wire [31:0]xbar_to_m00_couplers_ARADDR;
  wire [1:0]xbar_to_m00_couplers_ARBURST;
  wire [3:0]xbar_to_m00_couplers_ARCACHE;
  wire [2:0]xbar_to_m00_couplers_ARID;
  wire [7:0]xbar_to_m00_couplers_ARLEN;
  wire [0:0]xbar_to_m00_couplers_ARLOCK;
  wire [2:0]xbar_to_m00_couplers_ARPROT;
  wire [3:0]xbar_to_m00_couplers_ARQOS;
  wire xbar_to_m00_couplers_ARREADY;
  wire [2:0]xbar_to_m00_couplers_ARSIZE;
  wire [0:0]xbar_to_m00_couplers_ARVALID;
  wire [31:0]xbar_to_m00_couplers_AWADDR;
  wire [1:0]xbar_to_m00_couplers_AWBURST;
  wire [3:0]xbar_to_m00_couplers_AWCACHE;
  wire [2:0]xbar_to_m00_couplers_AWID;
  wire [7:0]xbar_to_m00_couplers_AWLEN;
  wire [0:0]xbar_to_m00_couplers_AWLOCK;
  wire [2:0]xbar_to_m00_couplers_AWPROT;
  wire [3:0]xbar_to_m00_couplers_AWQOS;
  wire xbar_to_m00_couplers_AWREADY;
  wire [2:0]xbar_to_m00_couplers_AWSIZE;
  wire [0:0]xbar_to_m00_couplers_AWVALID;
  wire [2:0]xbar_to_m00_couplers_BID;
  wire [0:0]xbar_to_m00_couplers_BREADY;
  wire [1:0]xbar_to_m00_couplers_BRESP;
  wire xbar_to_m00_couplers_BVALID;
  wire [511:0]xbar_to_m00_couplers_RDATA;
  wire [2:0]xbar_to_m00_couplers_RID;
  wire xbar_to_m00_couplers_RLAST;
  wire [0:0]xbar_to_m00_couplers_RREADY;
  wire [1:0]xbar_to_m00_couplers_RRESP;
  wire xbar_to_m00_couplers_RVALID;
  wire [511:0]xbar_to_m00_couplers_WDATA;
  wire [0:0]xbar_to_m00_couplers_WLAST;
  wire xbar_to_m00_couplers_WREADY;
  wire [63:0]xbar_to_m00_couplers_WSTRB;
  wire [0:0]xbar_to_m00_couplers_WVALID;
  wire [63:32]xbar_to_m01_couplers_ARADDR;
  wire [3:2]xbar_to_m01_couplers_ARBURST;
  wire [7:4]xbar_to_m01_couplers_ARCACHE;
  wire [5:3]xbar_to_m01_couplers_ARID;
  wire [15:8]xbar_to_m01_couplers_ARLEN;
  wire [1:1]xbar_to_m01_couplers_ARLOCK;
  wire [5:3]xbar_to_m01_couplers_ARPROT;
  wire [7:4]xbar_to_m01_couplers_ARQOS;
  wire xbar_to_m01_couplers_ARREADY;
  wire [7:4]xbar_to_m01_couplers_ARREGION;
  wire [5:3]xbar_to_m01_couplers_ARSIZE;
  wire [1:1]xbar_to_m01_couplers_ARVALID;
  wire [63:32]xbar_to_m01_couplers_AWADDR;
  wire [3:2]xbar_to_m01_couplers_AWBURST;
  wire [7:4]xbar_to_m01_couplers_AWCACHE;
  wire [5:3]xbar_to_m01_couplers_AWID;
  wire [15:8]xbar_to_m01_couplers_AWLEN;
  wire [1:1]xbar_to_m01_couplers_AWLOCK;
  wire [5:3]xbar_to_m01_couplers_AWPROT;
  wire [7:4]xbar_to_m01_couplers_AWQOS;
  wire xbar_to_m01_couplers_AWREADY;
  wire [7:4]xbar_to_m01_couplers_AWREGION;
  wire [5:3]xbar_to_m01_couplers_AWSIZE;
  wire [1:1]xbar_to_m01_couplers_AWVALID;
  wire [2:0]xbar_to_m01_couplers_BID;
  wire [1:1]xbar_to_m01_couplers_BREADY;
  wire [1:0]xbar_to_m01_couplers_BRESP;
  wire xbar_to_m01_couplers_BVALID;
  wire [511:0]xbar_to_m01_couplers_RDATA;
  wire [2:0]xbar_to_m01_couplers_RID;
  wire xbar_to_m01_couplers_RLAST;
  wire [1:1]xbar_to_m01_couplers_RREADY;
  wire [1:0]xbar_to_m01_couplers_RRESP;
  wire xbar_to_m01_couplers_RVALID;
  wire [1023:512]xbar_to_m01_couplers_WDATA;
  wire [1:1]xbar_to_m01_couplers_WLAST;
  wire xbar_to_m01_couplers_WREADY;
  wire [127:64]xbar_to_m01_couplers_WSTRB;
  wire [1:1]xbar_to_m01_couplers_WVALID;
  wire [7:0]NLW_xbar_m_axi_arregion_UNCONNECTED;
  wire [7:0]NLW_xbar_m_axi_awregion_UNCONNECTED;
  wire [8:0]NLW_xbar_s_axi_bid_UNCONNECTED;
  wire [8:0]NLW_xbar_s_axi_rid_UNCONNECTED;

  assign M00_ACLK_1 = M00_ACLK;
  assign M00_ARESETN_1 = M00_ARESETN;
  assign M00_AXI_araddr[31:0] = m00_couplers_to_to_memory_ARADDR;
  assign M00_AXI_arburst[1:0] = m00_couplers_to_to_memory_ARBURST;
  assign M00_AXI_arcache[3:0] = m00_couplers_to_to_memory_ARCACHE;
  assign M00_AXI_arid[2:0] = m00_couplers_to_to_memory_ARID;
  assign M00_AXI_arlen[7:0] = m00_couplers_to_to_memory_ARLEN;
  assign M00_AXI_arlock = m00_couplers_to_to_memory_ARLOCK;
  assign M00_AXI_arprot[2:0] = m00_couplers_to_to_memory_ARPROT;
  assign M00_AXI_arqos[3:0] = m00_couplers_to_to_memory_ARQOS;
  assign M00_AXI_arsize[2:0] = m00_couplers_to_to_memory_ARSIZE;
  assign M00_AXI_arvalid = m00_couplers_to_to_memory_ARVALID;
  assign M00_AXI_awaddr[31:0] = m00_couplers_to_to_memory_AWADDR;
  assign M00_AXI_awburst[1:0] = m00_couplers_to_to_memory_AWBURST;
  assign M00_AXI_awcache[3:0] = m00_couplers_to_to_memory_AWCACHE;
  assign M00_AXI_awid[2:0] = m00_couplers_to_to_memory_AWID;
  assign M00_AXI_awlen[7:0] = m00_couplers_to_to_memory_AWLEN;
  assign M00_AXI_awlock = m00_couplers_to_to_memory_AWLOCK;
  assign M00_AXI_awprot[2:0] = m00_couplers_to_to_memory_AWPROT;
  assign M00_AXI_awqos[3:0] = m00_couplers_to_to_memory_AWQOS;
  assign M00_AXI_awsize[2:0] = m00_couplers_to_to_memory_AWSIZE;
  assign M00_AXI_awvalid = m00_couplers_to_to_memory_AWVALID;
  assign M00_AXI_bready = m00_couplers_to_to_memory_BREADY;
  assign M00_AXI_rready = m00_couplers_to_to_memory_RREADY;
  assign M00_AXI_wdata[511:0] = m00_couplers_to_to_memory_WDATA;
  assign M00_AXI_wlast = m00_couplers_to_to_memory_WLAST;
  assign M00_AXI_wstrb[63:0] = m00_couplers_to_to_memory_WSTRB;
  assign M00_AXI_wvalid = m00_couplers_to_to_memory_WVALID;
  assign M01_ACLK_1 = M01_ACLK;
  assign M01_ARESETN_1 = M01_ARESETN;
  assign M01_AXI_araddr[31:0] = m01_couplers_to_to_memory_ARADDR;
  assign M01_AXI_arburst[1:0] = m01_couplers_to_to_memory_ARBURST;
  assign M01_AXI_arlen[7:0] = m01_couplers_to_to_memory_ARLEN;
  assign M01_AXI_arregion[3:0] = m01_couplers_to_to_memory_ARREGION;
  assign M01_AXI_arsize[2:0] = m01_couplers_to_to_memory_ARSIZE;
  assign M01_AXI_arvalid = m01_couplers_to_to_memory_ARVALID;
  assign M01_AXI_awaddr[31:0] = m01_couplers_to_to_memory_AWADDR;
  assign M01_AXI_awburst[1:0] = m01_couplers_to_to_memory_AWBURST;
  assign M01_AXI_awlen[7:0] = m01_couplers_to_to_memory_AWLEN;
  assign M01_AXI_awregion[3:0] = m01_couplers_to_to_memory_AWREGION;
  assign M01_AXI_awsize[2:0] = m01_couplers_to_to_memory_AWSIZE;
  assign M01_AXI_awvalid = m01_couplers_to_to_memory_AWVALID;
  assign M01_AXI_bready = m01_couplers_to_to_memory_BREADY;
  assign M01_AXI_rready = m01_couplers_to_to_memory_RREADY;
  assign M01_AXI_wdata[127:0] = m01_couplers_to_to_memory_WDATA;
  assign M01_AXI_wlast = m01_couplers_to_to_memory_WLAST;
  assign M01_AXI_wstrb[15:0] = m01_couplers_to_to_memory_WSTRB;
  assign M01_AXI_wvalid = m01_couplers_to_to_memory_WVALID;
  assign S00_ACLK_1 = S00_ACLK;
  assign S00_ARESETN_1 = S00_ARESETN;
  assign S00_AXI_arready = to_memory_to_s00_couplers_ARREADY;
  assign S00_AXI_awready = to_memory_to_s00_couplers_AWREADY;
  assign S00_AXI_bresp[1:0] = to_memory_to_s00_couplers_BRESP;
  assign S00_AXI_bvalid = to_memory_to_s00_couplers_BVALID;
  assign S00_AXI_rdata[511:0] = to_memory_to_s00_couplers_RDATA;
  assign S00_AXI_rlast = to_memory_to_s00_couplers_RLAST;
  assign S00_AXI_rresp[1:0] = to_memory_to_s00_couplers_RRESP;
  assign S00_AXI_rvalid = to_memory_to_s00_couplers_RVALID;
  assign S00_AXI_wready = to_memory_to_s00_couplers_WREADY;
  assign S01_ACLK_1 = S01_ACLK;
  assign S01_ARESETN_1 = S01_ARESETN;
  assign S01_AXI_arready = to_memory_to_s01_couplers_ARREADY;
  assign S01_AXI_awready = to_memory_to_s01_couplers_AWREADY;
  assign S01_AXI_bid[0] = to_memory_to_s01_couplers_BID;
  assign S01_AXI_bresp[1:0] = to_memory_to_s01_couplers_BRESP;
  assign S01_AXI_bvalid = to_memory_to_s01_couplers_BVALID;
  assign S01_AXI_rdata[511:0] = to_memory_to_s01_couplers_RDATA;
  assign S01_AXI_rid[0] = to_memory_to_s01_couplers_RID;
  assign S01_AXI_rlast = to_memory_to_s01_couplers_RLAST;
  assign S01_AXI_rresp[1:0] = to_memory_to_s01_couplers_RRESP;
  assign S01_AXI_rvalid = to_memory_to_s01_couplers_RVALID;
  assign S01_AXI_wready = to_memory_to_s01_couplers_WREADY;
  assign S02_ACLK_1 = S02_ACLK;
  assign S02_ARESETN_1 = S02_ARESETN;
  assign S02_AXI_arready = to_memory_to_s02_couplers_ARREADY;
  assign S02_AXI_awready = to_memory_to_s02_couplers_AWREADY;
  assign S02_AXI_bid[0] = to_memory_to_s02_couplers_BID;
  assign S02_AXI_bresp[1:0] = to_memory_to_s02_couplers_BRESP;
  assign S02_AXI_bvalid = to_memory_to_s02_couplers_BVALID;
  assign S02_AXI_rdata[511:0] = to_memory_to_s02_couplers_RDATA;
  assign S02_AXI_rid[0] = to_memory_to_s02_couplers_RID;
  assign S02_AXI_rlast = to_memory_to_s02_couplers_RLAST;
  assign S02_AXI_rresp[1:0] = to_memory_to_s02_couplers_RRESP;
  assign S02_AXI_rvalid = to_memory_to_s02_couplers_RVALID;
  assign S02_AXI_wready = to_memory_to_s02_couplers_WREADY;
  assign m00_couplers_to_to_memory_ARREADY = M00_AXI_arready;
  assign m00_couplers_to_to_memory_AWREADY = M00_AXI_awready;
  assign m00_couplers_to_to_memory_BID = M00_AXI_bid[2:0];
  assign m00_couplers_to_to_memory_BRESP = M00_AXI_bresp[1:0];
  assign m00_couplers_to_to_memory_BVALID = M00_AXI_bvalid;
  assign m00_couplers_to_to_memory_RDATA = M00_AXI_rdata[511:0];
  assign m00_couplers_to_to_memory_RID = M00_AXI_rid[2:0];
  assign m00_couplers_to_to_memory_RLAST = M00_AXI_rlast;
  assign m00_couplers_to_to_memory_RRESP = M00_AXI_rresp[1:0];
  assign m00_couplers_to_to_memory_RVALID = M00_AXI_rvalid;
  assign m00_couplers_to_to_memory_WREADY = M00_AXI_wready;
  assign m01_couplers_to_to_memory_ARREADY = M01_AXI_arready;
  assign m01_couplers_to_to_memory_AWREADY = M01_AXI_awready;
  assign m01_couplers_to_to_memory_BRESP = M01_AXI_bresp[1:0];
  assign m01_couplers_to_to_memory_BVALID = M01_AXI_bvalid;
  assign m01_couplers_to_to_memory_RDATA = M01_AXI_rdata[127:0];
  assign m01_couplers_to_to_memory_RLAST = M01_AXI_rlast;
  assign m01_couplers_to_to_memory_RRESP = M01_AXI_rresp[1:0];
  assign m01_couplers_to_to_memory_RVALID = M01_AXI_rvalid;
  assign m01_couplers_to_to_memory_WREADY = M01_AXI_wready;
  assign to_memory_ACLK_net = ACLK;
  assign to_memory_ARESETN_net = ARESETN;
  assign to_memory_to_s00_couplers_ARADDR = S00_AXI_araddr[31:0];
  assign to_memory_to_s00_couplers_ARBURST = S00_AXI_arburst[1:0];
  assign to_memory_to_s00_couplers_ARCACHE = S00_AXI_arcache[3:0];
  assign to_memory_to_s00_couplers_ARLEN = S00_AXI_arlen[7:0];
  assign to_memory_to_s00_couplers_ARLOCK = S00_AXI_arlock[0];
  assign to_memory_to_s00_couplers_ARPROT = S00_AXI_arprot[2:0];
  assign to_memory_to_s00_couplers_ARQOS = S00_AXI_arqos[3:0];
  assign to_memory_to_s00_couplers_ARREGION = S00_AXI_arregion[3:0];
  assign to_memory_to_s00_couplers_ARSIZE = S00_AXI_arsize[2:0];
  assign to_memory_to_s00_couplers_ARVALID = S00_AXI_arvalid;
  assign to_memory_to_s00_couplers_AWADDR = S00_AXI_awaddr[31:0];
  assign to_memory_to_s00_couplers_AWBURST = S00_AXI_awburst[1:0];
  assign to_memory_to_s00_couplers_AWCACHE = S00_AXI_awcache[3:0];
  assign to_memory_to_s00_couplers_AWLEN = S00_AXI_awlen[7:0];
  assign to_memory_to_s00_couplers_AWLOCK = S00_AXI_awlock[0];
  assign to_memory_to_s00_couplers_AWPROT = S00_AXI_awprot[2:0];
  assign to_memory_to_s00_couplers_AWQOS = S00_AXI_awqos[3:0];
  assign to_memory_to_s00_couplers_AWREGION = S00_AXI_awregion[3:0];
  assign to_memory_to_s00_couplers_AWSIZE = S00_AXI_awsize[2:0];
  assign to_memory_to_s00_couplers_AWVALID = S00_AXI_awvalid;
  assign to_memory_to_s00_couplers_BREADY = S00_AXI_bready;
  assign to_memory_to_s00_couplers_RREADY = S00_AXI_rready;
  assign to_memory_to_s00_couplers_WDATA = S00_AXI_wdata[511:0];
  assign to_memory_to_s00_couplers_WLAST = S00_AXI_wlast;
  assign to_memory_to_s00_couplers_WSTRB = S00_AXI_wstrb[63:0];
  assign to_memory_to_s00_couplers_WVALID = S00_AXI_wvalid;
  assign to_memory_to_s01_couplers_ARADDR = S01_AXI_araddr[31:0];
  assign to_memory_to_s01_couplers_ARBURST = S01_AXI_arburst[1:0];
  assign to_memory_to_s01_couplers_ARCACHE = S01_AXI_arcache[3:0];
  assign to_memory_to_s01_couplers_ARID = S01_AXI_arid[0];
  assign to_memory_to_s01_couplers_ARLEN = S01_AXI_arlen[7:0];
  assign to_memory_to_s01_couplers_ARLOCK = S01_AXI_arlock[0];
  assign to_memory_to_s01_couplers_ARPROT = S01_AXI_arprot[2:0];
  assign to_memory_to_s01_couplers_ARQOS = S01_AXI_arqos[3:0];
  assign to_memory_to_s01_couplers_ARREGION = S01_AXI_arregion[3:0];
  assign to_memory_to_s01_couplers_ARSIZE = S01_AXI_arsize[2:0];
  assign to_memory_to_s01_couplers_ARVALID = S01_AXI_arvalid;
  assign to_memory_to_s01_couplers_AWADDR = S01_AXI_awaddr[31:0];
  assign to_memory_to_s01_couplers_AWBURST = S01_AXI_awburst[1:0];
  assign to_memory_to_s01_couplers_AWCACHE = S01_AXI_awcache[3:0];
  assign to_memory_to_s01_couplers_AWID = S01_AXI_awid[0];
  assign to_memory_to_s01_couplers_AWLEN = S01_AXI_awlen[7:0];
  assign to_memory_to_s01_couplers_AWLOCK = S01_AXI_awlock[0];
  assign to_memory_to_s01_couplers_AWPROT = S01_AXI_awprot[2:0];
  assign to_memory_to_s01_couplers_AWQOS = S01_AXI_awqos[3:0];
  assign to_memory_to_s01_couplers_AWREGION = S01_AXI_awregion[3:0];
  assign to_memory_to_s01_couplers_AWSIZE = S01_AXI_awsize[2:0];
  assign to_memory_to_s01_couplers_AWVALID = S01_AXI_awvalid;
  assign to_memory_to_s01_couplers_BREADY = S01_AXI_bready;
  assign to_memory_to_s01_couplers_RREADY = S01_AXI_rready;
  assign to_memory_to_s01_couplers_WDATA = S01_AXI_wdata[511:0];
  assign to_memory_to_s01_couplers_WLAST = S01_AXI_wlast;
  assign to_memory_to_s01_couplers_WSTRB = S01_AXI_wstrb[63:0];
  assign to_memory_to_s01_couplers_WVALID = S01_AXI_wvalid;
  assign to_memory_to_s02_couplers_ARADDR = S02_AXI_araddr[31:0];
  assign to_memory_to_s02_couplers_ARBURST = S02_AXI_arburst[1:0];
  assign to_memory_to_s02_couplers_ARCACHE = S02_AXI_arcache[3:0];
  assign to_memory_to_s02_couplers_ARID = S02_AXI_arid[0];
  assign to_memory_to_s02_couplers_ARLEN = S02_AXI_arlen[7:0];
  assign to_memory_to_s02_couplers_ARLOCK = S02_AXI_arlock;
  assign to_memory_to_s02_couplers_ARPROT = S02_AXI_arprot[2:0];
  assign to_memory_to_s02_couplers_ARQOS = S02_AXI_arqos[3:0];
  assign to_memory_to_s02_couplers_ARSIZE = S02_AXI_arsize[2:0];
  assign to_memory_to_s02_couplers_ARVALID = S02_AXI_arvalid;
  assign to_memory_to_s02_couplers_AWADDR = S02_AXI_awaddr[31:0];
  assign to_memory_to_s02_couplers_AWBURST = S02_AXI_awburst[1:0];
  assign to_memory_to_s02_couplers_AWCACHE = S02_AXI_awcache[3:0];
  assign to_memory_to_s02_couplers_AWID = S02_AXI_awid[0];
  assign to_memory_to_s02_couplers_AWLEN = S02_AXI_awlen[7:0];
  assign to_memory_to_s02_couplers_AWLOCK = S02_AXI_awlock;
  assign to_memory_to_s02_couplers_AWPROT = S02_AXI_awprot[2:0];
  assign to_memory_to_s02_couplers_AWQOS = S02_AXI_awqos[3:0];
  assign to_memory_to_s02_couplers_AWSIZE = S02_AXI_awsize[2:0];
  assign to_memory_to_s02_couplers_AWVALID = S02_AXI_awvalid;
  assign to_memory_to_s02_couplers_BREADY = S02_AXI_bready;
  assign to_memory_to_s02_couplers_RREADY = S02_AXI_rready;
  assign to_memory_to_s02_couplers_WDATA = S02_AXI_wdata[511:0];
  assign to_memory_to_s02_couplers_WLAST = S02_AXI_wlast;
  assign to_memory_to_s02_couplers_WSTRB = S02_AXI_wstrb[63:0];
  assign to_memory_to_s02_couplers_WVALID = S02_AXI_wvalid;
  m00_couplers_imp_1VGQYVT m00_couplers
       (.M_ACLK(M00_ACLK_1),
        .M_ARESETN(M00_ARESETN_1),
        .M_AXI_araddr(m00_couplers_to_to_memory_ARADDR),
        .M_AXI_arburst(m00_couplers_to_to_memory_ARBURST),
        .M_AXI_arcache(m00_couplers_to_to_memory_ARCACHE),
        .M_AXI_arid(m00_couplers_to_to_memory_ARID),
        .M_AXI_arlen(m00_couplers_to_to_memory_ARLEN),
        .M_AXI_arlock(m00_couplers_to_to_memory_ARLOCK),
        .M_AXI_arprot(m00_couplers_to_to_memory_ARPROT),
        .M_AXI_arqos(m00_couplers_to_to_memory_ARQOS),
        .M_AXI_arready(m00_couplers_to_to_memory_ARREADY),
        .M_AXI_arsize(m00_couplers_to_to_memory_ARSIZE),
        .M_AXI_arvalid(m00_couplers_to_to_memory_ARVALID),
        .M_AXI_awaddr(m00_couplers_to_to_memory_AWADDR),
        .M_AXI_awburst(m00_couplers_to_to_memory_AWBURST),
        .M_AXI_awcache(m00_couplers_to_to_memory_AWCACHE),
        .M_AXI_awid(m00_couplers_to_to_memory_AWID),
        .M_AXI_awlen(m00_couplers_to_to_memory_AWLEN),
        .M_AXI_awlock(m00_couplers_to_to_memory_AWLOCK),
        .M_AXI_awprot(m00_couplers_to_to_memory_AWPROT),
        .M_AXI_awqos(m00_couplers_to_to_memory_AWQOS),
        .M_AXI_awready(m00_couplers_to_to_memory_AWREADY),
        .M_AXI_awsize(m00_couplers_to_to_memory_AWSIZE),
        .M_AXI_awvalid(m00_couplers_to_to_memory_AWVALID),
        .M_AXI_bid(m00_couplers_to_to_memory_BID),
        .M_AXI_bready(m00_couplers_to_to_memory_BREADY),
        .M_AXI_bresp(m00_couplers_to_to_memory_BRESP),
        .M_AXI_bvalid(m00_couplers_to_to_memory_BVALID),
        .M_AXI_rdata(m00_couplers_to_to_memory_RDATA),
        .M_AXI_rid(m00_couplers_to_to_memory_RID),
        .M_AXI_rlast(m00_couplers_to_to_memory_RLAST),
        .M_AXI_rready(m00_couplers_to_to_memory_RREADY),
        .M_AXI_rresp(m00_couplers_to_to_memory_RRESP),
        .M_AXI_rvalid(m00_couplers_to_to_memory_RVALID),
        .M_AXI_wdata(m00_couplers_to_to_memory_WDATA),
        .M_AXI_wlast(m00_couplers_to_to_memory_WLAST),
        .M_AXI_wready(m00_couplers_to_to_memory_WREADY),
        .M_AXI_wstrb(m00_couplers_to_to_memory_WSTRB),
        .M_AXI_wvalid(m00_couplers_to_to_memory_WVALID),
        .S_ACLK(to_memory_ACLK_net),
        .S_ARESETN(to_memory_ARESETN_net),
        .S_AXI_araddr(xbar_to_m00_couplers_ARADDR),
        .S_AXI_arburst(xbar_to_m00_couplers_ARBURST),
        .S_AXI_arcache(xbar_to_m00_couplers_ARCACHE),
        .S_AXI_arid(xbar_to_m00_couplers_ARID),
        .S_AXI_arlen(xbar_to_m00_couplers_ARLEN),
        .S_AXI_arlock(xbar_to_m00_couplers_ARLOCK),
        .S_AXI_arprot(xbar_to_m00_couplers_ARPROT),
        .S_AXI_arqos(xbar_to_m00_couplers_ARQOS),
        .S_AXI_arready(xbar_to_m00_couplers_ARREADY),
        .S_AXI_arsize(xbar_to_m00_couplers_ARSIZE),
        .S_AXI_arvalid(xbar_to_m00_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m00_couplers_AWADDR),
        .S_AXI_awburst(xbar_to_m00_couplers_AWBURST),
        .S_AXI_awcache(xbar_to_m00_couplers_AWCACHE),
        .S_AXI_awid(xbar_to_m00_couplers_AWID),
        .S_AXI_awlen(xbar_to_m00_couplers_AWLEN),
        .S_AXI_awlock(xbar_to_m00_couplers_AWLOCK),
        .S_AXI_awprot(xbar_to_m00_couplers_AWPROT),
        .S_AXI_awqos(xbar_to_m00_couplers_AWQOS),
        .S_AXI_awready(xbar_to_m00_couplers_AWREADY),
        .S_AXI_awsize(xbar_to_m00_couplers_AWSIZE),
        .S_AXI_awvalid(xbar_to_m00_couplers_AWVALID),
        .S_AXI_bid(xbar_to_m00_couplers_BID),
        .S_AXI_bready(xbar_to_m00_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m00_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m00_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m00_couplers_RDATA),
        .S_AXI_rid(xbar_to_m00_couplers_RID),
        .S_AXI_rlast(xbar_to_m00_couplers_RLAST),
        .S_AXI_rready(xbar_to_m00_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m00_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m00_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m00_couplers_WDATA),
        .S_AXI_wlast(xbar_to_m00_couplers_WLAST),
        .S_AXI_wready(xbar_to_m00_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m00_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m00_couplers_WVALID));
  m01_couplers_imp_PW0UIW m01_couplers
       (.M_ACLK(M01_ACLK_1),
        .M_ARESETN(M01_ARESETN_1),
        .M_AXI_araddr(m01_couplers_to_to_memory_ARADDR),
        .M_AXI_arburst(m01_couplers_to_to_memory_ARBURST),
        .M_AXI_arlen(m01_couplers_to_to_memory_ARLEN),
        .M_AXI_arready(m01_couplers_to_to_memory_ARREADY),
        .M_AXI_arregion(m01_couplers_to_to_memory_ARREGION),
        .M_AXI_arsize(m01_couplers_to_to_memory_ARSIZE),
        .M_AXI_arvalid(m01_couplers_to_to_memory_ARVALID),
        .M_AXI_awaddr(m01_couplers_to_to_memory_AWADDR),
        .M_AXI_awburst(m01_couplers_to_to_memory_AWBURST),
        .M_AXI_awlen(m01_couplers_to_to_memory_AWLEN),
        .M_AXI_awready(m01_couplers_to_to_memory_AWREADY),
        .M_AXI_awregion(m01_couplers_to_to_memory_AWREGION),
        .M_AXI_awsize(m01_couplers_to_to_memory_AWSIZE),
        .M_AXI_awvalid(m01_couplers_to_to_memory_AWVALID),
        .M_AXI_bready(m01_couplers_to_to_memory_BREADY),
        .M_AXI_bresp(m01_couplers_to_to_memory_BRESP),
        .M_AXI_bvalid(m01_couplers_to_to_memory_BVALID),
        .M_AXI_rdata(m01_couplers_to_to_memory_RDATA),
        .M_AXI_rlast(m01_couplers_to_to_memory_RLAST),
        .M_AXI_rready(m01_couplers_to_to_memory_RREADY),
        .M_AXI_rresp(m01_couplers_to_to_memory_RRESP),
        .M_AXI_rvalid(m01_couplers_to_to_memory_RVALID),
        .M_AXI_wdata(m01_couplers_to_to_memory_WDATA),
        .M_AXI_wlast(m01_couplers_to_to_memory_WLAST),
        .M_AXI_wready(m01_couplers_to_to_memory_WREADY),
        .M_AXI_wstrb(m01_couplers_to_to_memory_WSTRB),
        .M_AXI_wvalid(m01_couplers_to_to_memory_WVALID),
        .S_ACLK(to_memory_ACLK_net),
        .S_ARESETN(to_memory_ARESETN_net),
        .S_AXI_araddr(xbar_to_m01_couplers_ARADDR),
        .S_AXI_arburst(xbar_to_m01_couplers_ARBURST),
        .S_AXI_arcache(xbar_to_m01_couplers_ARCACHE),
        .S_AXI_arid(xbar_to_m01_couplers_ARID),
        .S_AXI_arlen(xbar_to_m01_couplers_ARLEN),
        .S_AXI_arlock(xbar_to_m01_couplers_ARLOCK),
        .S_AXI_arprot(xbar_to_m01_couplers_ARPROT),
        .S_AXI_arqos(xbar_to_m01_couplers_ARQOS),
        .S_AXI_arready(xbar_to_m01_couplers_ARREADY),
        .S_AXI_arregion(xbar_to_m01_couplers_ARREGION),
        .S_AXI_arsize(xbar_to_m01_couplers_ARSIZE),
        .S_AXI_arvalid(xbar_to_m01_couplers_ARVALID),
        .S_AXI_awaddr(xbar_to_m01_couplers_AWADDR),
        .S_AXI_awburst(xbar_to_m01_couplers_AWBURST),
        .S_AXI_awcache(xbar_to_m01_couplers_AWCACHE),
        .S_AXI_awid(xbar_to_m01_couplers_AWID),
        .S_AXI_awlen(xbar_to_m01_couplers_AWLEN),
        .S_AXI_awlock(xbar_to_m01_couplers_AWLOCK),
        .S_AXI_awprot(xbar_to_m01_couplers_AWPROT),
        .S_AXI_awqos(xbar_to_m01_couplers_AWQOS),
        .S_AXI_awready(xbar_to_m01_couplers_AWREADY),
        .S_AXI_awregion(xbar_to_m01_couplers_AWREGION),
        .S_AXI_awsize(xbar_to_m01_couplers_AWSIZE),
        .S_AXI_awvalid(xbar_to_m01_couplers_AWVALID),
        .S_AXI_bid(xbar_to_m01_couplers_BID),
        .S_AXI_bready(xbar_to_m01_couplers_BREADY),
        .S_AXI_bresp(xbar_to_m01_couplers_BRESP),
        .S_AXI_bvalid(xbar_to_m01_couplers_BVALID),
        .S_AXI_rdata(xbar_to_m01_couplers_RDATA),
        .S_AXI_rid(xbar_to_m01_couplers_RID),
        .S_AXI_rlast(xbar_to_m01_couplers_RLAST),
        .S_AXI_rready(xbar_to_m01_couplers_RREADY),
        .S_AXI_rresp(xbar_to_m01_couplers_RRESP),
        .S_AXI_rvalid(xbar_to_m01_couplers_RVALID),
        .S_AXI_wdata(xbar_to_m01_couplers_WDATA),
        .S_AXI_wlast(xbar_to_m01_couplers_WLAST),
        .S_AXI_wready(xbar_to_m01_couplers_WREADY),
        .S_AXI_wstrb(xbar_to_m01_couplers_WSTRB),
        .S_AXI_wvalid(xbar_to_m01_couplers_WVALID));
  s00_couplers_imp_1IKCXOO s00_couplers
       (.M_ACLK(to_memory_ACLK_net),
        .M_ARESETN(to_memory_ARESETN_net),
        .M_AXI_araddr(s00_couplers_to_xbar_ARADDR),
        .M_AXI_arburst(s00_couplers_to_xbar_ARBURST),
        .M_AXI_arcache(s00_couplers_to_xbar_ARCACHE),
        .M_AXI_arlen(s00_couplers_to_xbar_ARLEN),
        .M_AXI_arlock(s00_couplers_to_xbar_ARLOCK),
        .M_AXI_arprot(s00_couplers_to_xbar_ARPROT),
        .M_AXI_arqos(s00_couplers_to_xbar_ARQOS),
        .M_AXI_arready(s00_couplers_to_xbar_ARREADY),
        .M_AXI_arsize(s00_couplers_to_xbar_ARSIZE),
        .M_AXI_arvalid(s00_couplers_to_xbar_ARVALID),
        .M_AXI_awaddr(s00_couplers_to_xbar_AWADDR),
        .M_AXI_awburst(s00_couplers_to_xbar_AWBURST),
        .M_AXI_awcache(s00_couplers_to_xbar_AWCACHE),
        .M_AXI_awlen(s00_couplers_to_xbar_AWLEN),
        .M_AXI_awlock(s00_couplers_to_xbar_AWLOCK),
        .M_AXI_awprot(s00_couplers_to_xbar_AWPROT),
        .M_AXI_awqos(s00_couplers_to_xbar_AWQOS),
        .M_AXI_awready(s00_couplers_to_xbar_AWREADY),
        .M_AXI_awsize(s00_couplers_to_xbar_AWSIZE),
        .M_AXI_awvalid(s00_couplers_to_xbar_AWVALID),
        .M_AXI_bready(s00_couplers_to_xbar_BREADY),
        .M_AXI_bresp(s00_couplers_to_xbar_BRESP),
        .M_AXI_bvalid(s00_couplers_to_xbar_BVALID),
        .M_AXI_rdata(s00_couplers_to_xbar_RDATA),
        .M_AXI_rlast(s00_couplers_to_xbar_RLAST),
        .M_AXI_rready(s00_couplers_to_xbar_RREADY),
        .M_AXI_rresp(s00_couplers_to_xbar_RRESP),
        .M_AXI_rvalid(s00_couplers_to_xbar_RVALID),
        .M_AXI_wdata(s00_couplers_to_xbar_WDATA),
        .M_AXI_wlast(s00_couplers_to_xbar_WLAST),
        .M_AXI_wready(s00_couplers_to_xbar_WREADY),
        .M_AXI_wstrb(s00_couplers_to_xbar_WSTRB),
        .M_AXI_wvalid(s00_couplers_to_xbar_WVALID),
        .S_ACLK(S00_ACLK_1),
        .S_ARESETN(S00_ARESETN_1),
        .S_AXI_araddr(to_memory_to_s00_couplers_ARADDR),
        .S_AXI_arburst(to_memory_to_s00_couplers_ARBURST),
        .S_AXI_arcache(to_memory_to_s00_couplers_ARCACHE),
        .S_AXI_arlen(to_memory_to_s00_couplers_ARLEN),
        .S_AXI_arlock(to_memory_to_s00_couplers_ARLOCK),
        .S_AXI_arprot(to_memory_to_s00_couplers_ARPROT),
        .S_AXI_arqos(to_memory_to_s00_couplers_ARQOS),
        .S_AXI_arready(to_memory_to_s00_couplers_ARREADY),
        .S_AXI_arregion(to_memory_to_s00_couplers_ARREGION),
        .S_AXI_arsize(to_memory_to_s00_couplers_ARSIZE),
        .S_AXI_arvalid(to_memory_to_s00_couplers_ARVALID),
        .S_AXI_awaddr(to_memory_to_s00_couplers_AWADDR),
        .S_AXI_awburst(to_memory_to_s00_couplers_AWBURST),
        .S_AXI_awcache(to_memory_to_s00_couplers_AWCACHE),
        .S_AXI_awlen(to_memory_to_s00_couplers_AWLEN),
        .S_AXI_awlock(to_memory_to_s00_couplers_AWLOCK),
        .S_AXI_awprot(to_memory_to_s00_couplers_AWPROT),
        .S_AXI_awqos(to_memory_to_s00_couplers_AWQOS),
        .S_AXI_awready(to_memory_to_s00_couplers_AWREADY),
        .S_AXI_awregion(to_memory_to_s00_couplers_AWREGION),
        .S_AXI_awsize(to_memory_to_s00_couplers_AWSIZE),
        .S_AXI_awvalid(to_memory_to_s00_couplers_AWVALID),
        .S_AXI_bready(to_memory_to_s00_couplers_BREADY),
        .S_AXI_bresp(to_memory_to_s00_couplers_BRESP),
        .S_AXI_bvalid(to_memory_to_s00_couplers_BVALID),
        .S_AXI_rdata(to_memory_to_s00_couplers_RDATA),
        .S_AXI_rlast(to_memory_to_s00_couplers_RLAST),
        .S_AXI_rready(to_memory_to_s00_couplers_RREADY),
        .S_AXI_rresp(to_memory_to_s00_couplers_RRESP),
        .S_AXI_rvalid(to_memory_to_s00_couplers_RVALID),
        .S_AXI_wdata(to_memory_to_s00_couplers_WDATA),
        .S_AXI_wlast(to_memory_to_s00_couplers_WLAST),
        .S_AXI_wready(to_memory_to_s00_couplers_WREADY),
        .S_AXI_wstrb(to_memory_to_s00_couplers_WSTRB),
        .S_AXI_wvalid(to_memory_to_s00_couplers_WVALID));
  s01_couplers_imp_TMBZ15 s01_couplers
       (.M_ACLK(to_memory_ACLK_net),
        .M_ARESETN(to_memory_ARESETN_net),
        .M_AXI_araddr(s01_couplers_to_xbar_ARADDR),
        .M_AXI_arburst(s01_couplers_to_xbar_ARBURST),
        .M_AXI_arcache(s01_couplers_to_xbar_ARCACHE),
        .M_AXI_arid(s01_couplers_to_xbar_ARID),
        .M_AXI_arlen(s01_couplers_to_xbar_ARLEN),
        .M_AXI_arlock(s01_couplers_to_xbar_ARLOCK),
        .M_AXI_arprot(s01_couplers_to_xbar_ARPROT),
        .M_AXI_arqos(s01_couplers_to_xbar_ARQOS),
        .M_AXI_arready(s01_couplers_to_xbar_ARREADY),
        .M_AXI_arsize(s01_couplers_to_xbar_ARSIZE),
        .M_AXI_arvalid(s01_couplers_to_xbar_ARVALID),
        .M_AXI_awaddr(s01_couplers_to_xbar_AWADDR),
        .M_AXI_awburst(s01_couplers_to_xbar_AWBURST),
        .M_AXI_awcache(s01_couplers_to_xbar_AWCACHE),
        .M_AXI_awid(s01_couplers_to_xbar_AWID),
        .M_AXI_awlen(s01_couplers_to_xbar_AWLEN),
        .M_AXI_awlock(s01_couplers_to_xbar_AWLOCK),
        .M_AXI_awprot(s01_couplers_to_xbar_AWPROT),
        .M_AXI_awqos(s01_couplers_to_xbar_AWQOS),
        .M_AXI_awready(s01_couplers_to_xbar_AWREADY),
        .M_AXI_awsize(s01_couplers_to_xbar_AWSIZE),
        .M_AXI_awvalid(s01_couplers_to_xbar_AWVALID),
        .M_AXI_bid(s01_couplers_to_xbar_BID),
        .M_AXI_bready(s01_couplers_to_xbar_BREADY),
        .M_AXI_bresp(s01_couplers_to_xbar_BRESP),
        .M_AXI_bvalid(s01_couplers_to_xbar_BVALID),
        .M_AXI_rdata(s01_couplers_to_xbar_RDATA),
        .M_AXI_rid(s01_couplers_to_xbar_RID),
        .M_AXI_rlast(s01_couplers_to_xbar_RLAST),
        .M_AXI_rready(s01_couplers_to_xbar_RREADY),
        .M_AXI_rresp(s01_couplers_to_xbar_RRESP),
        .M_AXI_rvalid(s01_couplers_to_xbar_RVALID),
        .M_AXI_wdata(s01_couplers_to_xbar_WDATA),
        .M_AXI_wlast(s01_couplers_to_xbar_WLAST),
        .M_AXI_wready(s01_couplers_to_xbar_WREADY),
        .M_AXI_wstrb(s01_couplers_to_xbar_WSTRB),
        .M_AXI_wvalid(s01_couplers_to_xbar_WVALID),
        .S_ACLK(S01_ACLK_1),
        .S_ARESETN(S01_ARESETN_1),
        .S_AXI_araddr(to_memory_to_s01_couplers_ARADDR),
        .S_AXI_arburst(to_memory_to_s01_couplers_ARBURST),
        .S_AXI_arcache(to_memory_to_s01_couplers_ARCACHE),
        .S_AXI_arid(to_memory_to_s01_couplers_ARID),
        .S_AXI_arlen(to_memory_to_s01_couplers_ARLEN),
        .S_AXI_arlock(to_memory_to_s01_couplers_ARLOCK),
        .S_AXI_arprot(to_memory_to_s01_couplers_ARPROT),
        .S_AXI_arqos(to_memory_to_s01_couplers_ARQOS),
        .S_AXI_arready(to_memory_to_s01_couplers_ARREADY),
        .S_AXI_arregion(to_memory_to_s01_couplers_ARREGION),
        .S_AXI_arsize(to_memory_to_s01_couplers_ARSIZE),
        .S_AXI_arvalid(to_memory_to_s01_couplers_ARVALID),
        .S_AXI_awaddr(to_memory_to_s01_couplers_AWADDR),
        .S_AXI_awburst(to_memory_to_s01_couplers_AWBURST),
        .S_AXI_awcache(to_memory_to_s01_couplers_AWCACHE),
        .S_AXI_awid(to_memory_to_s01_couplers_AWID),
        .S_AXI_awlen(to_memory_to_s01_couplers_AWLEN),
        .S_AXI_awlock(to_memory_to_s01_couplers_AWLOCK),
        .S_AXI_awprot(to_memory_to_s01_couplers_AWPROT),
        .S_AXI_awqos(to_memory_to_s01_couplers_AWQOS),
        .S_AXI_awready(to_memory_to_s01_couplers_AWREADY),
        .S_AXI_awregion(to_memory_to_s01_couplers_AWREGION),
        .S_AXI_awsize(to_memory_to_s01_couplers_AWSIZE),
        .S_AXI_awvalid(to_memory_to_s01_couplers_AWVALID),
        .S_AXI_bid(to_memory_to_s01_couplers_BID),
        .S_AXI_bready(to_memory_to_s01_couplers_BREADY),
        .S_AXI_bresp(to_memory_to_s01_couplers_BRESP),
        .S_AXI_bvalid(to_memory_to_s01_couplers_BVALID),
        .S_AXI_rdata(to_memory_to_s01_couplers_RDATA),
        .S_AXI_rid(to_memory_to_s01_couplers_RID),
        .S_AXI_rlast(to_memory_to_s01_couplers_RLAST),
        .S_AXI_rready(to_memory_to_s01_couplers_RREADY),
        .S_AXI_rresp(to_memory_to_s01_couplers_RRESP),
        .S_AXI_rvalid(to_memory_to_s01_couplers_RVALID),
        .S_AXI_wdata(to_memory_to_s01_couplers_WDATA),
        .S_AXI_wlast(to_memory_to_s01_couplers_WLAST),
        .S_AXI_wready(to_memory_to_s01_couplers_WREADY),
        .S_AXI_wstrb(to_memory_to_s01_couplers_WSTRB),
        .S_AXI_wvalid(to_memory_to_s01_couplers_WVALID));
  s02_couplers_imp_ILKESR s02_couplers
       (.M_ACLK(to_memory_ACLK_net),
        .M_ARESETN(to_memory_ARESETN_net),
        .M_AXI_araddr(s02_couplers_to_xbar_ARADDR),
        .M_AXI_arburst(s02_couplers_to_xbar_ARBURST),
        .M_AXI_arcache(s02_couplers_to_xbar_ARCACHE),
        .M_AXI_arid(s02_couplers_to_xbar_ARID),
        .M_AXI_arlen(s02_couplers_to_xbar_ARLEN),
        .M_AXI_arlock(s02_couplers_to_xbar_ARLOCK),
        .M_AXI_arprot(s02_couplers_to_xbar_ARPROT),
        .M_AXI_arqos(s02_couplers_to_xbar_ARQOS),
        .M_AXI_arready(s02_couplers_to_xbar_ARREADY),
        .M_AXI_arsize(s02_couplers_to_xbar_ARSIZE),
        .M_AXI_arvalid(s02_couplers_to_xbar_ARVALID),
        .M_AXI_awaddr(s02_couplers_to_xbar_AWADDR),
        .M_AXI_awburst(s02_couplers_to_xbar_AWBURST),
        .M_AXI_awcache(s02_couplers_to_xbar_AWCACHE),
        .M_AXI_awid(s02_couplers_to_xbar_AWID),
        .M_AXI_awlen(s02_couplers_to_xbar_AWLEN),
        .M_AXI_awlock(s02_couplers_to_xbar_AWLOCK),
        .M_AXI_awprot(s02_couplers_to_xbar_AWPROT),
        .M_AXI_awqos(s02_couplers_to_xbar_AWQOS),
        .M_AXI_awready(s02_couplers_to_xbar_AWREADY),
        .M_AXI_awsize(s02_couplers_to_xbar_AWSIZE),
        .M_AXI_awvalid(s02_couplers_to_xbar_AWVALID),
        .M_AXI_bid(s02_couplers_to_xbar_BID),
        .M_AXI_bready(s02_couplers_to_xbar_BREADY),
        .M_AXI_bresp(s02_couplers_to_xbar_BRESP),
        .M_AXI_bvalid(s02_couplers_to_xbar_BVALID),
        .M_AXI_rdata(s02_couplers_to_xbar_RDATA),
        .M_AXI_rid(s02_couplers_to_xbar_RID),
        .M_AXI_rlast(s02_couplers_to_xbar_RLAST),
        .M_AXI_rready(s02_couplers_to_xbar_RREADY),
        .M_AXI_rresp(s02_couplers_to_xbar_RRESP),
        .M_AXI_rvalid(s02_couplers_to_xbar_RVALID),
        .M_AXI_wdata(s02_couplers_to_xbar_WDATA),
        .M_AXI_wlast(s02_couplers_to_xbar_WLAST),
        .M_AXI_wready(s02_couplers_to_xbar_WREADY),
        .M_AXI_wstrb(s02_couplers_to_xbar_WSTRB),
        .M_AXI_wvalid(s02_couplers_to_xbar_WVALID),
        .S_ACLK(S02_ACLK_1),
        .S_ARESETN(S02_ARESETN_1),
        .S_AXI_araddr(to_memory_to_s02_couplers_ARADDR),
        .S_AXI_arburst(to_memory_to_s02_couplers_ARBURST),
        .S_AXI_arcache(to_memory_to_s02_couplers_ARCACHE),
        .S_AXI_arid(to_memory_to_s02_couplers_ARID),
        .S_AXI_arlen(to_memory_to_s02_couplers_ARLEN),
        .S_AXI_arlock(to_memory_to_s02_couplers_ARLOCK),
        .S_AXI_arprot(to_memory_to_s02_couplers_ARPROT),
        .S_AXI_arqos(to_memory_to_s02_couplers_ARQOS),
        .S_AXI_arready(to_memory_to_s02_couplers_ARREADY),
        .S_AXI_arsize(to_memory_to_s02_couplers_ARSIZE),
        .S_AXI_arvalid(to_memory_to_s02_couplers_ARVALID),
        .S_AXI_awaddr(to_memory_to_s02_couplers_AWADDR),
        .S_AXI_awburst(to_memory_to_s02_couplers_AWBURST),
        .S_AXI_awcache(to_memory_to_s02_couplers_AWCACHE),
        .S_AXI_awid(to_memory_to_s02_couplers_AWID),
        .S_AXI_awlen(to_memory_to_s02_couplers_AWLEN),
        .S_AXI_awlock(to_memory_to_s02_couplers_AWLOCK),
        .S_AXI_awprot(to_memory_to_s02_couplers_AWPROT),
        .S_AXI_awqos(to_memory_to_s02_couplers_AWQOS),
        .S_AXI_awready(to_memory_to_s02_couplers_AWREADY),
        .S_AXI_awsize(to_memory_to_s02_couplers_AWSIZE),
        .S_AXI_awvalid(to_memory_to_s02_couplers_AWVALID),
        .S_AXI_bid(to_memory_to_s02_couplers_BID),
        .S_AXI_bready(to_memory_to_s02_couplers_BREADY),
        .S_AXI_bresp(to_memory_to_s02_couplers_BRESP),
        .S_AXI_bvalid(to_memory_to_s02_couplers_BVALID),
        .S_AXI_rdata(to_memory_to_s02_couplers_RDATA),
        .S_AXI_rid(to_memory_to_s02_couplers_RID),
        .S_AXI_rlast(to_memory_to_s02_couplers_RLAST),
        .S_AXI_rready(to_memory_to_s02_couplers_RREADY),
        .S_AXI_rresp(to_memory_to_s02_couplers_RRESP),
        .S_AXI_rvalid(to_memory_to_s02_couplers_RVALID),
        .S_AXI_wdata(to_memory_to_s02_couplers_WDATA),
        .S_AXI_wlast(to_memory_to_s02_couplers_WLAST),
        .S_AXI_wready(to_memory_to_s02_couplers_WREADY),
        .S_AXI_wstrb(to_memory_to_s02_couplers_WSTRB),
        .S_AXI_wvalid(to_memory_to_s02_couplers_WVALID));
  System_xbar_3 xbar
       (.aclk(to_memory_ACLK_net),
        .aresetn(to_memory_ARESETN_net),
        .m_axi_araddr({xbar_to_m01_couplers_ARADDR,xbar_to_m00_couplers_ARADDR}),
        .m_axi_arburst({xbar_to_m01_couplers_ARBURST,xbar_to_m00_couplers_ARBURST}),
        .m_axi_arcache({xbar_to_m01_couplers_ARCACHE,xbar_to_m00_couplers_ARCACHE}),
        .m_axi_arid({xbar_to_m01_couplers_ARID,xbar_to_m00_couplers_ARID}),
        .m_axi_arlen({xbar_to_m01_couplers_ARLEN,xbar_to_m00_couplers_ARLEN}),
        .m_axi_arlock({xbar_to_m01_couplers_ARLOCK,xbar_to_m00_couplers_ARLOCK}),
        .m_axi_arprot({xbar_to_m01_couplers_ARPROT,xbar_to_m00_couplers_ARPROT}),
        .m_axi_arqos({xbar_to_m01_couplers_ARQOS,xbar_to_m00_couplers_ARQOS}),
        .m_axi_arready({xbar_to_m01_couplers_ARREADY,xbar_to_m00_couplers_ARREADY}),
        .m_axi_arregion({xbar_to_m01_couplers_ARREGION,NLW_xbar_m_axi_arregion_UNCONNECTED[3:0]}),
        .m_axi_arsize({xbar_to_m01_couplers_ARSIZE,xbar_to_m00_couplers_ARSIZE}),
        .m_axi_arvalid({xbar_to_m01_couplers_ARVALID,xbar_to_m00_couplers_ARVALID}),
        .m_axi_awaddr({xbar_to_m01_couplers_AWADDR,xbar_to_m00_couplers_AWADDR}),
        .m_axi_awburst({xbar_to_m01_couplers_AWBURST,xbar_to_m00_couplers_AWBURST}),
        .m_axi_awcache({xbar_to_m01_couplers_AWCACHE,xbar_to_m00_couplers_AWCACHE}),
        .m_axi_awid({xbar_to_m01_couplers_AWID,xbar_to_m00_couplers_AWID}),
        .m_axi_awlen({xbar_to_m01_couplers_AWLEN,xbar_to_m00_couplers_AWLEN}),
        .m_axi_awlock({xbar_to_m01_couplers_AWLOCK,xbar_to_m00_couplers_AWLOCK}),
        .m_axi_awprot({xbar_to_m01_couplers_AWPROT,xbar_to_m00_couplers_AWPROT}),
        .m_axi_awqos({xbar_to_m01_couplers_AWQOS,xbar_to_m00_couplers_AWQOS}),
        .m_axi_awready({xbar_to_m01_couplers_AWREADY,xbar_to_m00_couplers_AWREADY}),
        .m_axi_awregion({xbar_to_m01_couplers_AWREGION,NLW_xbar_m_axi_awregion_UNCONNECTED[3:0]}),
        .m_axi_awsize({xbar_to_m01_couplers_AWSIZE,xbar_to_m00_couplers_AWSIZE}),
        .m_axi_awvalid({xbar_to_m01_couplers_AWVALID,xbar_to_m00_couplers_AWVALID}),
        .m_axi_bid({xbar_to_m01_couplers_BID,xbar_to_m00_couplers_BID}),
        .m_axi_bready({xbar_to_m01_couplers_BREADY,xbar_to_m00_couplers_BREADY}),
        .m_axi_bresp({xbar_to_m01_couplers_BRESP,xbar_to_m00_couplers_BRESP}),
        .m_axi_bvalid({xbar_to_m01_couplers_BVALID,xbar_to_m00_couplers_BVALID}),
        .m_axi_rdata({xbar_to_m01_couplers_RDATA,xbar_to_m00_couplers_RDATA}),
        .m_axi_rid({xbar_to_m01_couplers_RID,xbar_to_m00_couplers_RID}),
        .m_axi_rlast({xbar_to_m01_couplers_RLAST,xbar_to_m00_couplers_RLAST}),
        .m_axi_rready({xbar_to_m01_couplers_RREADY,xbar_to_m00_couplers_RREADY}),
        .m_axi_rresp({xbar_to_m01_couplers_RRESP,xbar_to_m00_couplers_RRESP}),
        .m_axi_rvalid({xbar_to_m01_couplers_RVALID,xbar_to_m00_couplers_RVALID}),
        .m_axi_wdata({xbar_to_m01_couplers_WDATA,xbar_to_m00_couplers_WDATA}),
        .m_axi_wlast({xbar_to_m01_couplers_WLAST,xbar_to_m00_couplers_WLAST}),
        .m_axi_wready({xbar_to_m01_couplers_WREADY,xbar_to_m00_couplers_WREADY}),
        .m_axi_wstrb({xbar_to_m01_couplers_WSTRB,xbar_to_m00_couplers_WSTRB}),
        .m_axi_wvalid({xbar_to_m01_couplers_WVALID,xbar_to_m00_couplers_WVALID}),
        .s_axi_araddr({s02_couplers_to_xbar_ARADDR,s01_couplers_to_xbar_ARADDR,s00_couplers_to_xbar_ARADDR}),
        .s_axi_arburst({s02_couplers_to_xbar_ARBURST,s01_couplers_to_xbar_ARBURST,s00_couplers_to_xbar_ARBURST}),
        .s_axi_arcache({s02_couplers_to_xbar_ARCACHE,s01_couplers_to_xbar_ARCACHE,s00_couplers_to_xbar_ARCACHE}),
        .s_axi_arid({1'b0,1'b0,s02_couplers_to_xbar_ARID,1'b0,1'b0,s01_couplers_to_xbar_ARID,1'b0,1'b0,1'b0}),
        .s_axi_arlen({s02_couplers_to_xbar_ARLEN,s01_couplers_to_xbar_ARLEN,s00_couplers_to_xbar_ARLEN}),
        .s_axi_arlock({s02_couplers_to_xbar_ARLOCK,s01_couplers_to_xbar_ARLOCK,s00_couplers_to_xbar_ARLOCK}),
        .s_axi_arprot({s02_couplers_to_xbar_ARPROT,s01_couplers_to_xbar_ARPROT,s00_couplers_to_xbar_ARPROT}),
        .s_axi_arqos({s02_couplers_to_xbar_ARQOS,s01_couplers_to_xbar_ARQOS,s00_couplers_to_xbar_ARQOS}),
        .s_axi_arready({s02_couplers_to_xbar_ARREADY,s01_couplers_to_xbar_ARREADY,s00_couplers_to_xbar_ARREADY}),
        .s_axi_arsize({s02_couplers_to_xbar_ARSIZE,s01_couplers_to_xbar_ARSIZE,s00_couplers_to_xbar_ARSIZE}),
        .s_axi_arvalid({s02_couplers_to_xbar_ARVALID,s01_couplers_to_xbar_ARVALID,s00_couplers_to_xbar_ARVALID}),
        .s_axi_awaddr({s02_couplers_to_xbar_AWADDR,s01_couplers_to_xbar_AWADDR,s00_couplers_to_xbar_AWADDR}),
        .s_axi_awburst({s02_couplers_to_xbar_AWBURST,s01_couplers_to_xbar_AWBURST,s00_couplers_to_xbar_AWBURST}),
        .s_axi_awcache({s02_couplers_to_xbar_AWCACHE,s01_couplers_to_xbar_AWCACHE,s00_couplers_to_xbar_AWCACHE}),
        .s_axi_awid({1'b0,1'b0,s02_couplers_to_xbar_AWID,1'b0,1'b0,s01_couplers_to_xbar_AWID,1'b0,1'b0,1'b0}),
        .s_axi_awlen({s02_couplers_to_xbar_AWLEN,s01_couplers_to_xbar_AWLEN,s00_couplers_to_xbar_AWLEN}),
        .s_axi_awlock({s02_couplers_to_xbar_AWLOCK,s01_couplers_to_xbar_AWLOCK,s00_couplers_to_xbar_AWLOCK}),
        .s_axi_awprot({s02_couplers_to_xbar_AWPROT,s01_couplers_to_xbar_AWPROT,s00_couplers_to_xbar_AWPROT}),
        .s_axi_awqos({s02_couplers_to_xbar_AWQOS,s01_couplers_to_xbar_AWQOS,s00_couplers_to_xbar_AWQOS}),
        .s_axi_awready({s02_couplers_to_xbar_AWREADY,s01_couplers_to_xbar_AWREADY,s00_couplers_to_xbar_AWREADY}),
        .s_axi_awsize({s02_couplers_to_xbar_AWSIZE,s01_couplers_to_xbar_AWSIZE,s00_couplers_to_xbar_AWSIZE}),
        .s_axi_awvalid({s02_couplers_to_xbar_AWVALID,s01_couplers_to_xbar_AWVALID,s00_couplers_to_xbar_AWVALID}),
        .s_axi_bid({s02_couplers_to_xbar_BID,s01_couplers_to_xbar_BID,NLW_xbar_s_axi_bid_UNCONNECTED[2:0]}),
        .s_axi_bready({s02_couplers_to_xbar_BREADY,s01_couplers_to_xbar_BREADY,s00_couplers_to_xbar_BREADY}),
        .s_axi_bresp({s02_couplers_to_xbar_BRESP,s01_couplers_to_xbar_BRESP,s00_couplers_to_xbar_BRESP}),
        .s_axi_bvalid({s02_couplers_to_xbar_BVALID,s01_couplers_to_xbar_BVALID,s00_couplers_to_xbar_BVALID}),
        .s_axi_rdata({s02_couplers_to_xbar_RDATA,s01_couplers_to_xbar_RDATA,s00_couplers_to_xbar_RDATA}),
        .s_axi_rid({s02_couplers_to_xbar_RID,s01_couplers_to_xbar_RID,NLW_xbar_s_axi_rid_UNCONNECTED[2:0]}),
        .s_axi_rlast({s02_couplers_to_xbar_RLAST,s01_couplers_to_xbar_RLAST,s00_couplers_to_xbar_RLAST}),
        .s_axi_rready({s02_couplers_to_xbar_RREADY,s01_couplers_to_xbar_RREADY,s00_couplers_to_xbar_RREADY}),
        .s_axi_rresp({s02_couplers_to_xbar_RRESP,s01_couplers_to_xbar_RRESP,s00_couplers_to_xbar_RRESP}),
        .s_axi_rvalid({s02_couplers_to_xbar_RVALID,s01_couplers_to_xbar_RVALID,s00_couplers_to_xbar_RVALID}),
        .s_axi_wdata({s02_couplers_to_xbar_WDATA,s01_couplers_to_xbar_WDATA,s00_couplers_to_xbar_WDATA}),
        .s_axi_wlast({s02_couplers_to_xbar_WLAST,s01_couplers_to_xbar_WLAST,s00_couplers_to_xbar_WLAST}),
        .s_axi_wready({s02_couplers_to_xbar_WREADY,s01_couplers_to_xbar_WREADY,s00_couplers_to_xbar_WREADY}),
        .s_axi_wstrb({s02_couplers_to_xbar_WSTRB,s01_couplers_to_xbar_WSTRB,s00_couplers_to_xbar_WSTRB}),
        .s_axi_wvalid({s02_couplers_to_xbar_WVALID,s01_couplers_to_xbar_WVALID,s00_couplers_to_xbar_WVALID}));
endmodule

module m00_couplers_imp_13MYVT9
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arregion,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awregion,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [7:0]M_AXI_arlen;
  output [0:0]M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [3:0]M_AXI_arregion;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [7:0]M_AXI_awlen;
  output [0:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [3:0]M_AXI_awregion;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [511:0]M_AXI_rdata;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [511:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [63:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input [0:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [0:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [127:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [127:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [15:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire S_ACLK_1;
  wire S_ARESETN_1;
  wire [31:0]auto_cc_to_m00_couplers_ARADDR;
  wire [1:0]auto_cc_to_m00_couplers_ARBURST;
  wire [3:0]auto_cc_to_m00_couplers_ARCACHE;
  wire [7:0]auto_cc_to_m00_couplers_ARLEN;
  wire [0:0]auto_cc_to_m00_couplers_ARLOCK;
  wire [2:0]auto_cc_to_m00_couplers_ARPROT;
  wire [3:0]auto_cc_to_m00_couplers_ARQOS;
  wire auto_cc_to_m00_couplers_ARREADY;
  wire [3:0]auto_cc_to_m00_couplers_ARREGION;
  wire [2:0]auto_cc_to_m00_couplers_ARSIZE;
  wire auto_cc_to_m00_couplers_ARVALID;
  wire [31:0]auto_cc_to_m00_couplers_AWADDR;
  wire [1:0]auto_cc_to_m00_couplers_AWBURST;
  wire [3:0]auto_cc_to_m00_couplers_AWCACHE;
  wire [7:0]auto_cc_to_m00_couplers_AWLEN;
  wire [0:0]auto_cc_to_m00_couplers_AWLOCK;
  wire [2:0]auto_cc_to_m00_couplers_AWPROT;
  wire [3:0]auto_cc_to_m00_couplers_AWQOS;
  wire auto_cc_to_m00_couplers_AWREADY;
  wire [3:0]auto_cc_to_m00_couplers_AWREGION;
  wire [2:0]auto_cc_to_m00_couplers_AWSIZE;
  wire auto_cc_to_m00_couplers_AWVALID;
  wire auto_cc_to_m00_couplers_BREADY;
  wire [1:0]auto_cc_to_m00_couplers_BRESP;
  wire auto_cc_to_m00_couplers_BVALID;
  wire [511:0]auto_cc_to_m00_couplers_RDATA;
  wire auto_cc_to_m00_couplers_RLAST;
  wire auto_cc_to_m00_couplers_RREADY;
  wire [1:0]auto_cc_to_m00_couplers_RRESP;
  wire auto_cc_to_m00_couplers_RVALID;
  wire [511:0]auto_cc_to_m00_couplers_WDATA;
  wire auto_cc_to_m00_couplers_WLAST;
  wire auto_cc_to_m00_couplers_WREADY;
  wire [63:0]auto_cc_to_m00_couplers_WSTRB;
  wire auto_cc_to_m00_couplers_WVALID;
  wire [31:0]auto_us_to_auto_cc_ARADDR;
  wire [1:0]auto_us_to_auto_cc_ARBURST;
  wire [3:0]auto_us_to_auto_cc_ARCACHE;
  wire [7:0]auto_us_to_auto_cc_ARLEN;
  wire [0:0]auto_us_to_auto_cc_ARLOCK;
  wire [2:0]auto_us_to_auto_cc_ARPROT;
  wire [3:0]auto_us_to_auto_cc_ARQOS;
  wire auto_us_to_auto_cc_ARREADY;
  wire [3:0]auto_us_to_auto_cc_ARREGION;
  wire [2:0]auto_us_to_auto_cc_ARSIZE;
  wire auto_us_to_auto_cc_ARVALID;
  wire [31:0]auto_us_to_auto_cc_AWADDR;
  wire [1:0]auto_us_to_auto_cc_AWBURST;
  wire [3:0]auto_us_to_auto_cc_AWCACHE;
  wire [7:0]auto_us_to_auto_cc_AWLEN;
  wire [0:0]auto_us_to_auto_cc_AWLOCK;
  wire [2:0]auto_us_to_auto_cc_AWPROT;
  wire [3:0]auto_us_to_auto_cc_AWQOS;
  wire auto_us_to_auto_cc_AWREADY;
  wire [3:0]auto_us_to_auto_cc_AWREGION;
  wire [2:0]auto_us_to_auto_cc_AWSIZE;
  wire auto_us_to_auto_cc_AWVALID;
  wire auto_us_to_auto_cc_BREADY;
  wire [1:0]auto_us_to_auto_cc_BRESP;
  wire auto_us_to_auto_cc_BVALID;
  wire [511:0]auto_us_to_auto_cc_RDATA;
  wire auto_us_to_auto_cc_RLAST;
  wire auto_us_to_auto_cc_RREADY;
  wire [1:0]auto_us_to_auto_cc_RRESP;
  wire auto_us_to_auto_cc_RVALID;
  wire [511:0]auto_us_to_auto_cc_WDATA;
  wire auto_us_to_auto_cc_WLAST;
  wire auto_us_to_auto_cc_WREADY;
  wire [63:0]auto_us_to_auto_cc_WSTRB;
  wire auto_us_to_auto_cc_WVALID;
  wire [31:0]m00_couplers_to_auto_us_ARADDR;
  wire [1:0]m00_couplers_to_auto_us_ARBURST;
  wire [3:0]m00_couplers_to_auto_us_ARCACHE;
  wire [7:0]m00_couplers_to_auto_us_ARLEN;
  wire [0:0]m00_couplers_to_auto_us_ARLOCK;
  wire [2:0]m00_couplers_to_auto_us_ARPROT;
  wire [3:0]m00_couplers_to_auto_us_ARQOS;
  wire m00_couplers_to_auto_us_ARREADY;
  wire [3:0]m00_couplers_to_auto_us_ARREGION;
  wire [2:0]m00_couplers_to_auto_us_ARSIZE;
  wire m00_couplers_to_auto_us_ARVALID;
  wire [31:0]m00_couplers_to_auto_us_AWADDR;
  wire [1:0]m00_couplers_to_auto_us_AWBURST;
  wire [3:0]m00_couplers_to_auto_us_AWCACHE;
  wire [7:0]m00_couplers_to_auto_us_AWLEN;
  wire [0:0]m00_couplers_to_auto_us_AWLOCK;
  wire [2:0]m00_couplers_to_auto_us_AWPROT;
  wire [3:0]m00_couplers_to_auto_us_AWQOS;
  wire m00_couplers_to_auto_us_AWREADY;
  wire [3:0]m00_couplers_to_auto_us_AWREGION;
  wire [2:0]m00_couplers_to_auto_us_AWSIZE;
  wire m00_couplers_to_auto_us_AWVALID;
  wire m00_couplers_to_auto_us_BREADY;
  wire [1:0]m00_couplers_to_auto_us_BRESP;
  wire m00_couplers_to_auto_us_BVALID;
  wire [127:0]m00_couplers_to_auto_us_RDATA;
  wire m00_couplers_to_auto_us_RLAST;
  wire m00_couplers_to_auto_us_RREADY;
  wire [1:0]m00_couplers_to_auto_us_RRESP;
  wire m00_couplers_to_auto_us_RVALID;
  wire [127:0]m00_couplers_to_auto_us_WDATA;
  wire m00_couplers_to_auto_us_WLAST;
  wire m00_couplers_to_auto_us_WREADY;
  wire [15:0]m00_couplers_to_auto_us_WSTRB;
  wire m00_couplers_to_auto_us_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = auto_cc_to_m00_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = auto_cc_to_m00_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = auto_cc_to_m00_couplers_ARCACHE;
  assign M_AXI_arlen[7:0] = auto_cc_to_m00_couplers_ARLEN;
  assign M_AXI_arlock[0] = auto_cc_to_m00_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = auto_cc_to_m00_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = auto_cc_to_m00_couplers_ARQOS;
  assign M_AXI_arregion[3:0] = auto_cc_to_m00_couplers_ARREGION;
  assign M_AXI_arsize[2:0] = auto_cc_to_m00_couplers_ARSIZE;
  assign M_AXI_arvalid = auto_cc_to_m00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = auto_cc_to_m00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = auto_cc_to_m00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = auto_cc_to_m00_couplers_AWCACHE;
  assign M_AXI_awlen[7:0] = auto_cc_to_m00_couplers_AWLEN;
  assign M_AXI_awlock[0] = auto_cc_to_m00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = auto_cc_to_m00_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = auto_cc_to_m00_couplers_AWQOS;
  assign M_AXI_awregion[3:0] = auto_cc_to_m00_couplers_AWREGION;
  assign M_AXI_awsize[2:0] = auto_cc_to_m00_couplers_AWSIZE;
  assign M_AXI_awvalid = auto_cc_to_m00_couplers_AWVALID;
  assign M_AXI_bready = auto_cc_to_m00_couplers_BREADY;
  assign M_AXI_rready = auto_cc_to_m00_couplers_RREADY;
  assign M_AXI_wdata[511:0] = auto_cc_to_m00_couplers_WDATA;
  assign M_AXI_wlast = auto_cc_to_m00_couplers_WLAST;
  assign M_AXI_wstrb[63:0] = auto_cc_to_m00_couplers_WSTRB;
  assign M_AXI_wvalid = auto_cc_to_m00_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN;
  assign S_AXI_arready = m00_couplers_to_auto_us_ARREADY;
  assign S_AXI_awready = m00_couplers_to_auto_us_AWREADY;
  assign S_AXI_bresp[1:0] = m00_couplers_to_auto_us_BRESP;
  assign S_AXI_bvalid = m00_couplers_to_auto_us_BVALID;
  assign S_AXI_rdata[127:0] = m00_couplers_to_auto_us_RDATA;
  assign S_AXI_rlast = m00_couplers_to_auto_us_RLAST;
  assign S_AXI_rresp[1:0] = m00_couplers_to_auto_us_RRESP;
  assign S_AXI_rvalid = m00_couplers_to_auto_us_RVALID;
  assign S_AXI_wready = m00_couplers_to_auto_us_WREADY;
  assign auto_cc_to_m00_couplers_ARREADY = M_AXI_arready;
  assign auto_cc_to_m00_couplers_AWREADY = M_AXI_awready;
  assign auto_cc_to_m00_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_cc_to_m00_couplers_BVALID = M_AXI_bvalid;
  assign auto_cc_to_m00_couplers_RDATA = M_AXI_rdata[511:0];
  assign auto_cc_to_m00_couplers_RLAST = M_AXI_rlast;
  assign auto_cc_to_m00_couplers_RRESP = M_AXI_rresp[1:0];
  assign auto_cc_to_m00_couplers_RVALID = M_AXI_rvalid;
  assign auto_cc_to_m00_couplers_WREADY = M_AXI_wready;
  assign m00_couplers_to_auto_us_ARADDR = S_AXI_araddr[31:0];
  assign m00_couplers_to_auto_us_ARBURST = S_AXI_arburst[1:0];
  assign m00_couplers_to_auto_us_ARCACHE = S_AXI_arcache[3:0];
  assign m00_couplers_to_auto_us_ARLEN = S_AXI_arlen[7:0];
  assign m00_couplers_to_auto_us_ARLOCK = S_AXI_arlock[0];
  assign m00_couplers_to_auto_us_ARPROT = S_AXI_arprot[2:0];
  assign m00_couplers_to_auto_us_ARQOS = S_AXI_arqos[3:0];
  assign m00_couplers_to_auto_us_ARREGION = S_AXI_arregion[3:0];
  assign m00_couplers_to_auto_us_ARSIZE = S_AXI_arsize[2:0];
  assign m00_couplers_to_auto_us_ARVALID = S_AXI_arvalid;
  assign m00_couplers_to_auto_us_AWADDR = S_AXI_awaddr[31:0];
  assign m00_couplers_to_auto_us_AWBURST = S_AXI_awburst[1:0];
  assign m00_couplers_to_auto_us_AWCACHE = S_AXI_awcache[3:0];
  assign m00_couplers_to_auto_us_AWLEN = S_AXI_awlen[7:0];
  assign m00_couplers_to_auto_us_AWLOCK = S_AXI_awlock[0];
  assign m00_couplers_to_auto_us_AWPROT = S_AXI_awprot[2:0];
  assign m00_couplers_to_auto_us_AWQOS = S_AXI_awqos[3:0];
  assign m00_couplers_to_auto_us_AWREGION = S_AXI_awregion[3:0];
  assign m00_couplers_to_auto_us_AWSIZE = S_AXI_awsize[2:0];
  assign m00_couplers_to_auto_us_AWVALID = S_AXI_awvalid;
  assign m00_couplers_to_auto_us_BREADY = S_AXI_bready;
  assign m00_couplers_to_auto_us_RREADY = S_AXI_rready;
  assign m00_couplers_to_auto_us_WDATA = S_AXI_wdata[127:0];
  assign m00_couplers_to_auto_us_WLAST = S_AXI_wlast;
  assign m00_couplers_to_auto_us_WSTRB = S_AXI_wstrb[15:0];
  assign m00_couplers_to_auto_us_WVALID = S_AXI_wvalid;
  System_auto_cc_0 auto_cc
       (.m_axi_aclk(M_ACLK_1),
        .m_axi_araddr(auto_cc_to_m00_couplers_ARADDR),
        .m_axi_arburst(auto_cc_to_m00_couplers_ARBURST),
        .m_axi_arcache(auto_cc_to_m00_couplers_ARCACHE),
        .m_axi_aresetn(M_ARESETN_1),
        .m_axi_arlen(auto_cc_to_m00_couplers_ARLEN),
        .m_axi_arlock(auto_cc_to_m00_couplers_ARLOCK),
        .m_axi_arprot(auto_cc_to_m00_couplers_ARPROT),
        .m_axi_arqos(auto_cc_to_m00_couplers_ARQOS),
        .m_axi_arready(auto_cc_to_m00_couplers_ARREADY),
        .m_axi_arregion(auto_cc_to_m00_couplers_ARREGION),
        .m_axi_arsize(auto_cc_to_m00_couplers_ARSIZE),
        .m_axi_arvalid(auto_cc_to_m00_couplers_ARVALID),
        .m_axi_awaddr(auto_cc_to_m00_couplers_AWADDR),
        .m_axi_awburst(auto_cc_to_m00_couplers_AWBURST),
        .m_axi_awcache(auto_cc_to_m00_couplers_AWCACHE),
        .m_axi_awlen(auto_cc_to_m00_couplers_AWLEN),
        .m_axi_awlock(auto_cc_to_m00_couplers_AWLOCK),
        .m_axi_awprot(auto_cc_to_m00_couplers_AWPROT),
        .m_axi_awqos(auto_cc_to_m00_couplers_AWQOS),
        .m_axi_awready(auto_cc_to_m00_couplers_AWREADY),
        .m_axi_awregion(auto_cc_to_m00_couplers_AWREGION),
        .m_axi_awsize(auto_cc_to_m00_couplers_AWSIZE),
        .m_axi_awvalid(auto_cc_to_m00_couplers_AWVALID),
        .m_axi_bready(auto_cc_to_m00_couplers_BREADY),
        .m_axi_bresp(auto_cc_to_m00_couplers_BRESP),
        .m_axi_bvalid(auto_cc_to_m00_couplers_BVALID),
        .m_axi_rdata(auto_cc_to_m00_couplers_RDATA),
        .m_axi_rlast(auto_cc_to_m00_couplers_RLAST),
        .m_axi_rready(auto_cc_to_m00_couplers_RREADY),
        .m_axi_rresp(auto_cc_to_m00_couplers_RRESP),
        .m_axi_rvalid(auto_cc_to_m00_couplers_RVALID),
        .m_axi_wdata(auto_cc_to_m00_couplers_WDATA),
        .m_axi_wlast(auto_cc_to_m00_couplers_WLAST),
        .m_axi_wready(auto_cc_to_m00_couplers_WREADY),
        .m_axi_wstrb(auto_cc_to_m00_couplers_WSTRB),
        .m_axi_wvalid(auto_cc_to_m00_couplers_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(auto_us_to_auto_cc_ARADDR),
        .s_axi_arburst(auto_us_to_auto_cc_ARBURST),
        .s_axi_arcache(auto_us_to_auto_cc_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arlen(auto_us_to_auto_cc_ARLEN),
        .s_axi_arlock(auto_us_to_auto_cc_ARLOCK),
        .s_axi_arprot(auto_us_to_auto_cc_ARPROT),
        .s_axi_arqos(auto_us_to_auto_cc_ARQOS),
        .s_axi_arready(auto_us_to_auto_cc_ARREADY),
        .s_axi_arregion(auto_us_to_auto_cc_ARREGION),
        .s_axi_arsize(auto_us_to_auto_cc_ARSIZE),
        .s_axi_arvalid(auto_us_to_auto_cc_ARVALID),
        .s_axi_awaddr(auto_us_to_auto_cc_AWADDR),
        .s_axi_awburst(auto_us_to_auto_cc_AWBURST),
        .s_axi_awcache(auto_us_to_auto_cc_AWCACHE),
        .s_axi_awlen(auto_us_to_auto_cc_AWLEN),
        .s_axi_awlock(auto_us_to_auto_cc_AWLOCK),
        .s_axi_awprot(auto_us_to_auto_cc_AWPROT),
        .s_axi_awqos(auto_us_to_auto_cc_AWQOS),
        .s_axi_awready(auto_us_to_auto_cc_AWREADY),
        .s_axi_awregion(auto_us_to_auto_cc_AWREGION),
        .s_axi_awsize(auto_us_to_auto_cc_AWSIZE),
        .s_axi_awvalid(auto_us_to_auto_cc_AWVALID),
        .s_axi_bready(auto_us_to_auto_cc_BREADY),
        .s_axi_bresp(auto_us_to_auto_cc_BRESP),
        .s_axi_bvalid(auto_us_to_auto_cc_BVALID),
        .s_axi_rdata(auto_us_to_auto_cc_RDATA),
        .s_axi_rlast(auto_us_to_auto_cc_RLAST),
        .s_axi_rready(auto_us_to_auto_cc_RREADY),
        .s_axi_rresp(auto_us_to_auto_cc_RRESP),
        .s_axi_rvalid(auto_us_to_auto_cc_RVALID),
        .s_axi_wdata(auto_us_to_auto_cc_WDATA),
        .s_axi_wlast(auto_us_to_auto_cc_WLAST),
        .s_axi_wready(auto_us_to_auto_cc_WREADY),
        .s_axi_wstrb(auto_us_to_auto_cc_WSTRB),
        .s_axi_wvalid(auto_us_to_auto_cc_WVALID));
  System_auto_us_0 auto_us
       (.m_axi_araddr(auto_us_to_auto_cc_ARADDR),
        .m_axi_arburst(auto_us_to_auto_cc_ARBURST),
        .m_axi_arcache(auto_us_to_auto_cc_ARCACHE),
        .m_axi_arlen(auto_us_to_auto_cc_ARLEN),
        .m_axi_arlock(auto_us_to_auto_cc_ARLOCK),
        .m_axi_arprot(auto_us_to_auto_cc_ARPROT),
        .m_axi_arqos(auto_us_to_auto_cc_ARQOS),
        .m_axi_arready(auto_us_to_auto_cc_ARREADY),
        .m_axi_arregion(auto_us_to_auto_cc_ARREGION),
        .m_axi_arsize(auto_us_to_auto_cc_ARSIZE),
        .m_axi_arvalid(auto_us_to_auto_cc_ARVALID),
        .m_axi_awaddr(auto_us_to_auto_cc_AWADDR),
        .m_axi_awburst(auto_us_to_auto_cc_AWBURST),
        .m_axi_awcache(auto_us_to_auto_cc_AWCACHE),
        .m_axi_awlen(auto_us_to_auto_cc_AWLEN),
        .m_axi_awlock(auto_us_to_auto_cc_AWLOCK),
        .m_axi_awprot(auto_us_to_auto_cc_AWPROT),
        .m_axi_awqos(auto_us_to_auto_cc_AWQOS),
        .m_axi_awready(auto_us_to_auto_cc_AWREADY),
        .m_axi_awregion(auto_us_to_auto_cc_AWREGION),
        .m_axi_awsize(auto_us_to_auto_cc_AWSIZE),
        .m_axi_awvalid(auto_us_to_auto_cc_AWVALID),
        .m_axi_bready(auto_us_to_auto_cc_BREADY),
        .m_axi_bresp(auto_us_to_auto_cc_BRESP),
        .m_axi_bvalid(auto_us_to_auto_cc_BVALID),
        .m_axi_rdata(auto_us_to_auto_cc_RDATA),
        .m_axi_rlast(auto_us_to_auto_cc_RLAST),
        .m_axi_rready(auto_us_to_auto_cc_RREADY),
        .m_axi_rresp(auto_us_to_auto_cc_RRESP),
        .m_axi_rvalid(auto_us_to_auto_cc_RVALID),
        .m_axi_wdata(auto_us_to_auto_cc_WDATA),
        .m_axi_wlast(auto_us_to_auto_cc_WLAST),
        .m_axi_wready(auto_us_to_auto_cc_WREADY),
        .m_axi_wstrb(auto_us_to_auto_cc_WSTRB),
        .m_axi_wvalid(auto_us_to_auto_cc_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(m00_couplers_to_auto_us_ARADDR),
        .s_axi_arburst(m00_couplers_to_auto_us_ARBURST),
        .s_axi_arcache(m00_couplers_to_auto_us_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arlen(m00_couplers_to_auto_us_ARLEN),
        .s_axi_arlock(m00_couplers_to_auto_us_ARLOCK),
        .s_axi_arprot(m00_couplers_to_auto_us_ARPROT),
        .s_axi_arqos(m00_couplers_to_auto_us_ARQOS),
        .s_axi_arready(m00_couplers_to_auto_us_ARREADY),
        .s_axi_arregion(m00_couplers_to_auto_us_ARREGION),
        .s_axi_arsize(m00_couplers_to_auto_us_ARSIZE),
        .s_axi_arvalid(m00_couplers_to_auto_us_ARVALID),
        .s_axi_awaddr(m00_couplers_to_auto_us_AWADDR),
        .s_axi_awburst(m00_couplers_to_auto_us_AWBURST),
        .s_axi_awcache(m00_couplers_to_auto_us_AWCACHE),
        .s_axi_awlen(m00_couplers_to_auto_us_AWLEN),
        .s_axi_awlock(m00_couplers_to_auto_us_AWLOCK),
        .s_axi_awprot(m00_couplers_to_auto_us_AWPROT),
        .s_axi_awqos(m00_couplers_to_auto_us_AWQOS),
        .s_axi_awready(m00_couplers_to_auto_us_AWREADY),
        .s_axi_awregion(m00_couplers_to_auto_us_AWREGION),
        .s_axi_awsize(m00_couplers_to_auto_us_AWSIZE),
        .s_axi_awvalid(m00_couplers_to_auto_us_AWVALID),
        .s_axi_bready(m00_couplers_to_auto_us_BREADY),
        .s_axi_bresp(m00_couplers_to_auto_us_BRESP),
        .s_axi_bvalid(m00_couplers_to_auto_us_BVALID),
        .s_axi_rdata(m00_couplers_to_auto_us_RDATA),
        .s_axi_rlast(m00_couplers_to_auto_us_RLAST),
        .s_axi_rready(m00_couplers_to_auto_us_RREADY),
        .s_axi_rresp(m00_couplers_to_auto_us_RRESP),
        .s_axi_rvalid(m00_couplers_to_auto_us_RVALID),
        .s_axi_wdata(m00_couplers_to_auto_us_WDATA),
        .s_axi_wlast(m00_couplers_to_auto_us_WLAST),
        .s_axi_wready(m00_couplers_to_auto_us_WREADY),
        .s_axi_wstrb(m00_couplers_to_auto_us_WSTRB),
        .s_axi_wvalid(m00_couplers_to_auto_us_WVALID));
endmodule

module m00_couplers_imp_1VGQYVT
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arid,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awid,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rid,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arid,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awid,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rid,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [2:0]M_AXI_arid;
  output [7:0]M_AXI_arlen;
  output M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [2:0]M_AXI_awid;
  output [7:0]M_AXI_awlen;
  output M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  input [2:0]M_AXI_bid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [511:0]M_AXI_rdata;
  input [2:0]M_AXI_rid;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [511:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [63:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [2:0]S_AXI_arid;
  input [7:0]S_AXI_arlen;
  input S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [2:0]S_AXI_awid;
  input [7:0]S_AXI_awlen;
  input S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  output [2:0]S_AXI_bid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [511:0]S_AXI_rdata;
  output [2:0]S_AXI_rid;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [511:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]m00_couplers_to_m00_couplers_ARADDR;
  wire [1:0]m00_couplers_to_m00_couplers_ARBURST;
  wire [3:0]m00_couplers_to_m00_couplers_ARCACHE;
  wire [2:0]m00_couplers_to_m00_couplers_ARID;
  wire [7:0]m00_couplers_to_m00_couplers_ARLEN;
  wire m00_couplers_to_m00_couplers_ARLOCK;
  wire [2:0]m00_couplers_to_m00_couplers_ARPROT;
  wire [3:0]m00_couplers_to_m00_couplers_ARQOS;
  wire m00_couplers_to_m00_couplers_ARREADY;
  wire [2:0]m00_couplers_to_m00_couplers_ARSIZE;
  wire m00_couplers_to_m00_couplers_ARVALID;
  wire [31:0]m00_couplers_to_m00_couplers_AWADDR;
  wire [1:0]m00_couplers_to_m00_couplers_AWBURST;
  wire [3:0]m00_couplers_to_m00_couplers_AWCACHE;
  wire [2:0]m00_couplers_to_m00_couplers_AWID;
  wire [7:0]m00_couplers_to_m00_couplers_AWLEN;
  wire m00_couplers_to_m00_couplers_AWLOCK;
  wire [2:0]m00_couplers_to_m00_couplers_AWPROT;
  wire [3:0]m00_couplers_to_m00_couplers_AWQOS;
  wire m00_couplers_to_m00_couplers_AWREADY;
  wire [2:0]m00_couplers_to_m00_couplers_AWSIZE;
  wire m00_couplers_to_m00_couplers_AWVALID;
  wire [2:0]m00_couplers_to_m00_couplers_BID;
  wire m00_couplers_to_m00_couplers_BREADY;
  wire [1:0]m00_couplers_to_m00_couplers_BRESP;
  wire m00_couplers_to_m00_couplers_BVALID;
  wire [511:0]m00_couplers_to_m00_couplers_RDATA;
  wire [2:0]m00_couplers_to_m00_couplers_RID;
  wire m00_couplers_to_m00_couplers_RLAST;
  wire m00_couplers_to_m00_couplers_RREADY;
  wire [1:0]m00_couplers_to_m00_couplers_RRESP;
  wire m00_couplers_to_m00_couplers_RVALID;
  wire [511:0]m00_couplers_to_m00_couplers_WDATA;
  wire m00_couplers_to_m00_couplers_WLAST;
  wire m00_couplers_to_m00_couplers_WREADY;
  wire [63:0]m00_couplers_to_m00_couplers_WSTRB;
  wire m00_couplers_to_m00_couplers_WVALID;

  assign M_AXI_araddr[31:0] = m00_couplers_to_m00_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = m00_couplers_to_m00_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = m00_couplers_to_m00_couplers_ARCACHE;
  assign M_AXI_arid[2:0] = m00_couplers_to_m00_couplers_ARID;
  assign M_AXI_arlen[7:0] = m00_couplers_to_m00_couplers_ARLEN;
  assign M_AXI_arlock = m00_couplers_to_m00_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = m00_couplers_to_m00_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = m00_couplers_to_m00_couplers_ARQOS;
  assign M_AXI_arsize[2:0] = m00_couplers_to_m00_couplers_ARSIZE;
  assign M_AXI_arvalid = m00_couplers_to_m00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m00_couplers_to_m00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = m00_couplers_to_m00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = m00_couplers_to_m00_couplers_AWCACHE;
  assign M_AXI_awid[2:0] = m00_couplers_to_m00_couplers_AWID;
  assign M_AXI_awlen[7:0] = m00_couplers_to_m00_couplers_AWLEN;
  assign M_AXI_awlock = m00_couplers_to_m00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = m00_couplers_to_m00_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = m00_couplers_to_m00_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = m00_couplers_to_m00_couplers_AWSIZE;
  assign M_AXI_awvalid = m00_couplers_to_m00_couplers_AWVALID;
  assign M_AXI_bready = m00_couplers_to_m00_couplers_BREADY;
  assign M_AXI_rready = m00_couplers_to_m00_couplers_RREADY;
  assign M_AXI_wdata[511:0] = m00_couplers_to_m00_couplers_WDATA;
  assign M_AXI_wlast = m00_couplers_to_m00_couplers_WLAST;
  assign M_AXI_wstrb[63:0] = m00_couplers_to_m00_couplers_WSTRB;
  assign M_AXI_wvalid = m00_couplers_to_m00_couplers_WVALID;
  assign S_AXI_arready = m00_couplers_to_m00_couplers_ARREADY;
  assign S_AXI_awready = m00_couplers_to_m00_couplers_AWREADY;
  assign S_AXI_bid[2:0] = m00_couplers_to_m00_couplers_BID;
  assign S_AXI_bresp[1:0] = m00_couplers_to_m00_couplers_BRESP;
  assign S_AXI_bvalid = m00_couplers_to_m00_couplers_BVALID;
  assign S_AXI_rdata[511:0] = m00_couplers_to_m00_couplers_RDATA;
  assign S_AXI_rid[2:0] = m00_couplers_to_m00_couplers_RID;
  assign S_AXI_rlast = m00_couplers_to_m00_couplers_RLAST;
  assign S_AXI_rresp[1:0] = m00_couplers_to_m00_couplers_RRESP;
  assign S_AXI_rvalid = m00_couplers_to_m00_couplers_RVALID;
  assign S_AXI_wready = m00_couplers_to_m00_couplers_WREADY;
  assign m00_couplers_to_m00_couplers_ARADDR = S_AXI_araddr[31:0];
  assign m00_couplers_to_m00_couplers_ARBURST = S_AXI_arburst[1:0];
  assign m00_couplers_to_m00_couplers_ARCACHE = S_AXI_arcache[3:0];
  assign m00_couplers_to_m00_couplers_ARID = S_AXI_arid[2:0];
  assign m00_couplers_to_m00_couplers_ARLEN = S_AXI_arlen[7:0];
  assign m00_couplers_to_m00_couplers_ARLOCK = S_AXI_arlock;
  assign m00_couplers_to_m00_couplers_ARPROT = S_AXI_arprot[2:0];
  assign m00_couplers_to_m00_couplers_ARQOS = S_AXI_arqos[3:0];
  assign m00_couplers_to_m00_couplers_ARREADY = M_AXI_arready;
  assign m00_couplers_to_m00_couplers_ARSIZE = S_AXI_arsize[2:0];
  assign m00_couplers_to_m00_couplers_ARVALID = S_AXI_arvalid;
  assign m00_couplers_to_m00_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign m00_couplers_to_m00_couplers_AWBURST = S_AXI_awburst[1:0];
  assign m00_couplers_to_m00_couplers_AWCACHE = S_AXI_awcache[3:0];
  assign m00_couplers_to_m00_couplers_AWID = S_AXI_awid[2:0];
  assign m00_couplers_to_m00_couplers_AWLEN = S_AXI_awlen[7:0];
  assign m00_couplers_to_m00_couplers_AWLOCK = S_AXI_awlock;
  assign m00_couplers_to_m00_couplers_AWPROT = S_AXI_awprot[2:0];
  assign m00_couplers_to_m00_couplers_AWQOS = S_AXI_awqos[3:0];
  assign m00_couplers_to_m00_couplers_AWREADY = M_AXI_awready;
  assign m00_couplers_to_m00_couplers_AWSIZE = S_AXI_awsize[2:0];
  assign m00_couplers_to_m00_couplers_AWVALID = S_AXI_awvalid;
  assign m00_couplers_to_m00_couplers_BID = M_AXI_bid[2:0];
  assign m00_couplers_to_m00_couplers_BREADY = S_AXI_bready;
  assign m00_couplers_to_m00_couplers_BRESP = M_AXI_bresp[1:0];
  assign m00_couplers_to_m00_couplers_BVALID = M_AXI_bvalid;
  assign m00_couplers_to_m00_couplers_RDATA = M_AXI_rdata[511:0];
  assign m00_couplers_to_m00_couplers_RID = M_AXI_rid[2:0];
  assign m00_couplers_to_m00_couplers_RLAST = M_AXI_rlast;
  assign m00_couplers_to_m00_couplers_RREADY = S_AXI_rready;
  assign m00_couplers_to_m00_couplers_RRESP = M_AXI_rresp[1:0];
  assign m00_couplers_to_m00_couplers_RVALID = M_AXI_rvalid;
  assign m00_couplers_to_m00_couplers_WDATA = S_AXI_wdata[511:0];
  assign m00_couplers_to_m00_couplers_WLAST = S_AXI_wlast;
  assign m00_couplers_to_m00_couplers_WREADY = M_AXI_wready;
  assign m00_couplers_to_m00_couplers_WSTRB = S_AXI_wstrb[63:0];
  assign m00_couplers_to_m00_couplers_WVALID = S_AXI_wvalid;
endmodule

module m01_couplers_imp_92MQL8
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input [0:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [0:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [127:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [127:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [15:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire S_ACLK_1;
  wire S_ARESETN_1;
  wire [31:0]auto_cc_to_auto_ds_ARADDR;
  wire [1:0]auto_cc_to_auto_ds_ARBURST;
  wire [3:0]auto_cc_to_auto_ds_ARCACHE;
  wire [7:0]auto_cc_to_auto_ds_ARLEN;
  wire [0:0]auto_cc_to_auto_ds_ARLOCK;
  wire [2:0]auto_cc_to_auto_ds_ARPROT;
  wire [3:0]auto_cc_to_auto_ds_ARQOS;
  wire auto_cc_to_auto_ds_ARREADY;
  wire [3:0]auto_cc_to_auto_ds_ARREGION;
  wire [2:0]auto_cc_to_auto_ds_ARSIZE;
  wire auto_cc_to_auto_ds_ARVALID;
  wire [31:0]auto_cc_to_auto_ds_AWADDR;
  wire [1:0]auto_cc_to_auto_ds_AWBURST;
  wire [3:0]auto_cc_to_auto_ds_AWCACHE;
  wire [7:0]auto_cc_to_auto_ds_AWLEN;
  wire [0:0]auto_cc_to_auto_ds_AWLOCK;
  wire [2:0]auto_cc_to_auto_ds_AWPROT;
  wire [3:0]auto_cc_to_auto_ds_AWQOS;
  wire auto_cc_to_auto_ds_AWREADY;
  wire [3:0]auto_cc_to_auto_ds_AWREGION;
  wire [2:0]auto_cc_to_auto_ds_AWSIZE;
  wire auto_cc_to_auto_ds_AWVALID;
  wire auto_cc_to_auto_ds_BREADY;
  wire [1:0]auto_cc_to_auto_ds_BRESP;
  wire auto_cc_to_auto_ds_BVALID;
  wire [127:0]auto_cc_to_auto_ds_RDATA;
  wire auto_cc_to_auto_ds_RLAST;
  wire auto_cc_to_auto_ds_RREADY;
  wire [1:0]auto_cc_to_auto_ds_RRESP;
  wire auto_cc_to_auto_ds_RVALID;
  wire [127:0]auto_cc_to_auto_ds_WDATA;
  wire auto_cc_to_auto_ds_WLAST;
  wire auto_cc_to_auto_ds_WREADY;
  wire [15:0]auto_cc_to_auto_ds_WSTRB;
  wire auto_cc_to_auto_ds_WVALID;
  wire [31:0]auto_ds_to_auto_pc_ARADDR;
  wire [1:0]auto_ds_to_auto_pc_ARBURST;
  wire [3:0]auto_ds_to_auto_pc_ARCACHE;
  wire [7:0]auto_ds_to_auto_pc_ARLEN;
  wire [0:0]auto_ds_to_auto_pc_ARLOCK;
  wire [2:0]auto_ds_to_auto_pc_ARPROT;
  wire [3:0]auto_ds_to_auto_pc_ARQOS;
  wire auto_ds_to_auto_pc_ARREADY;
  wire [3:0]auto_ds_to_auto_pc_ARREGION;
  wire [2:0]auto_ds_to_auto_pc_ARSIZE;
  wire auto_ds_to_auto_pc_ARVALID;
  wire [31:0]auto_ds_to_auto_pc_AWADDR;
  wire [1:0]auto_ds_to_auto_pc_AWBURST;
  wire [3:0]auto_ds_to_auto_pc_AWCACHE;
  wire [7:0]auto_ds_to_auto_pc_AWLEN;
  wire [0:0]auto_ds_to_auto_pc_AWLOCK;
  wire [2:0]auto_ds_to_auto_pc_AWPROT;
  wire [3:0]auto_ds_to_auto_pc_AWQOS;
  wire auto_ds_to_auto_pc_AWREADY;
  wire [3:0]auto_ds_to_auto_pc_AWREGION;
  wire [2:0]auto_ds_to_auto_pc_AWSIZE;
  wire auto_ds_to_auto_pc_AWVALID;
  wire auto_ds_to_auto_pc_BREADY;
  wire [1:0]auto_ds_to_auto_pc_BRESP;
  wire auto_ds_to_auto_pc_BVALID;
  wire [31:0]auto_ds_to_auto_pc_RDATA;
  wire auto_ds_to_auto_pc_RLAST;
  wire auto_ds_to_auto_pc_RREADY;
  wire [1:0]auto_ds_to_auto_pc_RRESP;
  wire auto_ds_to_auto_pc_RVALID;
  wire [31:0]auto_ds_to_auto_pc_WDATA;
  wire auto_ds_to_auto_pc_WLAST;
  wire auto_ds_to_auto_pc_WREADY;
  wire [3:0]auto_ds_to_auto_pc_WSTRB;
  wire auto_ds_to_auto_pc_WVALID;
  wire [31:0]auto_pc_to_m01_regslice_ARADDR;
  wire [2:0]auto_pc_to_m01_regslice_ARPROT;
  wire auto_pc_to_m01_regslice_ARREADY;
  wire auto_pc_to_m01_regslice_ARVALID;
  wire [31:0]auto_pc_to_m01_regslice_AWADDR;
  wire [2:0]auto_pc_to_m01_regslice_AWPROT;
  wire auto_pc_to_m01_regslice_AWREADY;
  wire auto_pc_to_m01_regslice_AWVALID;
  wire auto_pc_to_m01_regslice_BREADY;
  wire [1:0]auto_pc_to_m01_regslice_BRESP;
  wire auto_pc_to_m01_regslice_BVALID;
  wire [31:0]auto_pc_to_m01_regslice_RDATA;
  wire auto_pc_to_m01_regslice_RREADY;
  wire [1:0]auto_pc_to_m01_regslice_RRESP;
  wire auto_pc_to_m01_regslice_RVALID;
  wire [31:0]auto_pc_to_m01_regslice_WDATA;
  wire auto_pc_to_m01_regslice_WREADY;
  wire [3:0]auto_pc_to_m01_regslice_WSTRB;
  wire auto_pc_to_m01_regslice_WVALID;
  wire [31:0]m01_couplers_to_auto_cc_ARADDR;
  wire [1:0]m01_couplers_to_auto_cc_ARBURST;
  wire [3:0]m01_couplers_to_auto_cc_ARCACHE;
  wire [7:0]m01_couplers_to_auto_cc_ARLEN;
  wire [0:0]m01_couplers_to_auto_cc_ARLOCK;
  wire [2:0]m01_couplers_to_auto_cc_ARPROT;
  wire [3:0]m01_couplers_to_auto_cc_ARQOS;
  wire m01_couplers_to_auto_cc_ARREADY;
  wire [3:0]m01_couplers_to_auto_cc_ARREGION;
  wire [2:0]m01_couplers_to_auto_cc_ARSIZE;
  wire m01_couplers_to_auto_cc_ARVALID;
  wire [31:0]m01_couplers_to_auto_cc_AWADDR;
  wire [1:0]m01_couplers_to_auto_cc_AWBURST;
  wire [3:0]m01_couplers_to_auto_cc_AWCACHE;
  wire [7:0]m01_couplers_to_auto_cc_AWLEN;
  wire [0:0]m01_couplers_to_auto_cc_AWLOCK;
  wire [2:0]m01_couplers_to_auto_cc_AWPROT;
  wire [3:0]m01_couplers_to_auto_cc_AWQOS;
  wire m01_couplers_to_auto_cc_AWREADY;
  wire [3:0]m01_couplers_to_auto_cc_AWREGION;
  wire [2:0]m01_couplers_to_auto_cc_AWSIZE;
  wire m01_couplers_to_auto_cc_AWVALID;
  wire m01_couplers_to_auto_cc_BREADY;
  wire [1:0]m01_couplers_to_auto_cc_BRESP;
  wire m01_couplers_to_auto_cc_BVALID;
  wire [127:0]m01_couplers_to_auto_cc_RDATA;
  wire m01_couplers_to_auto_cc_RLAST;
  wire m01_couplers_to_auto_cc_RREADY;
  wire [1:0]m01_couplers_to_auto_cc_RRESP;
  wire m01_couplers_to_auto_cc_RVALID;
  wire [127:0]m01_couplers_to_auto_cc_WDATA;
  wire m01_couplers_to_auto_cc_WLAST;
  wire m01_couplers_to_auto_cc_WREADY;
  wire [15:0]m01_couplers_to_auto_cc_WSTRB;
  wire m01_couplers_to_auto_cc_WVALID;
  wire [31:0]m01_regslice_to_m01_couplers_ARADDR;
  wire m01_regslice_to_m01_couplers_ARREADY;
  wire m01_regslice_to_m01_couplers_ARVALID;
  wire [31:0]m01_regslice_to_m01_couplers_AWADDR;
  wire m01_regslice_to_m01_couplers_AWREADY;
  wire m01_regslice_to_m01_couplers_AWVALID;
  wire m01_regslice_to_m01_couplers_BREADY;
  wire [1:0]m01_regslice_to_m01_couplers_BRESP;
  wire m01_regslice_to_m01_couplers_BVALID;
  wire [31:0]m01_regslice_to_m01_couplers_RDATA;
  wire m01_regslice_to_m01_couplers_RREADY;
  wire [1:0]m01_regslice_to_m01_couplers_RRESP;
  wire m01_regslice_to_m01_couplers_RVALID;
  wire [31:0]m01_regslice_to_m01_couplers_WDATA;
  wire m01_regslice_to_m01_couplers_WREADY;
  wire m01_regslice_to_m01_couplers_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = m01_regslice_to_m01_couplers_ARADDR;
  assign M_AXI_arvalid = m01_regslice_to_m01_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = m01_regslice_to_m01_couplers_AWADDR;
  assign M_AXI_awvalid = m01_regslice_to_m01_couplers_AWVALID;
  assign M_AXI_bready = m01_regslice_to_m01_couplers_BREADY;
  assign M_AXI_rready = m01_regslice_to_m01_couplers_RREADY;
  assign M_AXI_wdata[31:0] = m01_regslice_to_m01_couplers_WDATA;
  assign M_AXI_wvalid = m01_regslice_to_m01_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN;
  assign S_AXI_arready = m01_couplers_to_auto_cc_ARREADY;
  assign S_AXI_awready = m01_couplers_to_auto_cc_AWREADY;
  assign S_AXI_bresp[1:0] = m01_couplers_to_auto_cc_BRESP;
  assign S_AXI_bvalid = m01_couplers_to_auto_cc_BVALID;
  assign S_AXI_rdata[127:0] = m01_couplers_to_auto_cc_RDATA;
  assign S_AXI_rlast = m01_couplers_to_auto_cc_RLAST;
  assign S_AXI_rresp[1:0] = m01_couplers_to_auto_cc_RRESP;
  assign S_AXI_rvalid = m01_couplers_to_auto_cc_RVALID;
  assign S_AXI_wready = m01_couplers_to_auto_cc_WREADY;
  assign m01_couplers_to_auto_cc_ARADDR = S_AXI_araddr[31:0];
  assign m01_couplers_to_auto_cc_ARBURST = S_AXI_arburst[1:0];
  assign m01_couplers_to_auto_cc_ARCACHE = S_AXI_arcache[3:0];
  assign m01_couplers_to_auto_cc_ARLEN = S_AXI_arlen[7:0];
  assign m01_couplers_to_auto_cc_ARLOCK = S_AXI_arlock[0];
  assign m01_couplers_to_auto_cc_ARPROT = S_AXI_arprot[2:0];
  assign m01_couplers_to_auto_cc_ARQOS = S_AXI_arqos[3:0];
  assign m01_couplers_to_auto_cc_ARREGION = S_AXI_arregion[3:0];
  assign m01_couplers_to_auto_cc_ARSIZE = S_AXI_arsize[2:0];
  assign m01_couplers_to_auto_cc_ARVALID = S_AXI_arvalid;
  assign m01_couplers_to_auto_cc_AWADDR = S_AXI_awaddr[31:0];
  assign m01_couplers_to_auto_cc_AWBURST = S_AXI_awburst[1:0];
  assign m01_couplers_to_auto_cc_AWCACHE = S_AXI_awcache[3:0];
  assign m01_couplers_to_auto_cc_AWLEN = S_AXI_awlen[7:0];
  assign m01_couplers_to_auto_cc_AWLOCK = S_AXI_awlock[0];
  assign m01_couplers_to_auto_cc_AWPROT = S_AXI_awprot[2:0];
  assign m01_couplers_to_auto_cc_AWQOS = S_AXI_awqos[3:0];
  assign m01_couplers_to_auto_cc_AWREGION = S_AXI_awregion[3:0];
  assign m01_couplers_to_auto_cc_AWSIZE = S_AXI_awsize[2:0];
  assign m01_couplers_to_auto_cc_AWVALID = S_AXI_awvalid;
  assign m01_couplers_to_auto_cc_BREADY = S_AXI_bready;
  assign m01_couplers_to_auto_cc_RREADY = S_AXI_rready;
  assign m01_couplers_to_auto_cc_WDATA = S_AXI_wdata[127:0];
  assign m01_couplers_to_auto_cc_WLAST = S_AXI_wlast;
  assign m01_couplers_to_auto_cc_WSTRB = S_AXI_wstrb[15:0];
  assign m01_couplers_to_auto_cc_WVALID = S_AXI_wvalid;
  assign m01_regslice_to_m01_couplers_ARREADY = M_AXI_arready;
  assign m01_regslice_to_m01_couplers_AWREADY = M_AXI_awready;
  assign m01_regslice_to_m01_couplers_BRESP = M_AXI_bresp[1:0];
  assign m01_regslice_to_m01_couplers_BVALID = M_AXI_bvalid;
  assign m01_regslice_to_m01_couplers_RDATA = M_AXI_rdata[31:0];
  assign m01_regslice_to_m01_couplers_RRESP = M_AXI_rresp[1:0];
  assign m01_regslice_to_m01_couplers_RVALID = M_AXI_rvalid;
  assign m01_regslice_to_m01_couplers_WREADY = M_AXI_wready;
  System_auto_cc_1 auto_cc
       (.m_axi_aclk(M_ACLK_1),
        .m_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .m_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .m_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .m_axi_aresetn(M_ARESETN_1),
        .m_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .m_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .m_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .m_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .m_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .m_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .m_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .m_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .m_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .m_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .m_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .m_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .m_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .m_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .m_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .m_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .m_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .m_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .m_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .m_axi_bready(auto_cc_to_auto_ds_BREADY),
        .m_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .m_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .m_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .m_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .m_axi_rready(auto_cc_to_auto_ds_RREADY),
        .m_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .m_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .m_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .m_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .m_axi_wready(auto_cc_to_auto_ds_WREADY),
        .m_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .m_axi_wvalid(auto_cc_to_auto_ds_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(m01_couplers_to_auto_cc_ARADDR),
        .s_axi_arburst(m01_couplers_to_auto_cc_ARBURST),
        .s_axi_arcache(m01_couplers_to_auto_cc_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arlen(m01_couplers_to_auto_cc_ARLEN),
        .s_axi_arlock(m01_couplers_to_auto_cc_ARLOCK),
        .s_axi_arprot(m01_couplers_to_auto_cc_ARPROT),
        .s_axi_arqos(m01_couplers_to_auto_cc_ARQOS),
        .s_axi_arready(m01_couplers_to_auto_cc_ARREADY),
        .s_axi_arregion(m01_couplers_to_auto_cc_ARREGION),
        .s_axi_arsize(m01_couplers_to_auto_cc_ARSIZE),
        .s_axi_arvalid(m01_couplers_to_auto_cc_ARVALID),
        .s_axi_awaddr(m01_couplers_to_auto_cc_AWADDR),
        .s_axi_awburst(m01_couplers_to_auto_cc_AWBURST),
        .s_axi_awcache(m01_couplers_to_auto_cc_AWCACHE),
        .s_axi_awlen(m01_couplers_to_auto_cc_AWLEN),
        .s_axi_awlock(m01_couplers_to_auto_cc_AWLOCK),
        .s_axi_awprot(m01_couplers_to_auto_cc_AWPROT),
        .s_axi_awqos(m01_couplers_to_auto_cc_AWQOS),
        .s_axi_awready(m01_couplers_to_auto_cc_AWREADY),
        .s_axi_awregion(m01_couplers_to_auto_cc_AWREGION),
        .s_axi_awsize(m01_couplers_to_auto_cc_AWSIZE),
        .s_axi_awvalid(m01_couplers_to_auto_cc_AWVALID),
        .s_axi_bready(m01_couplers_to_auto_cc_BREADY),
        .s_axi_bresp(m01_couplers_to_auto_cc_BRESP),
        .s_axi_bvalid(m01_couplers_to_auto_cc_BVALID),
        .s_axi_rdata(m01_couplers_to_auto_cc_RDATA),
        .s_axi_rlast(m01_couplers_to_auto_cc_RLAST),
        .s_axi_rready(m01_couplers_to_auto_cc_RREADY),
        .s_axi_rresp(m01_couplers_to_auto_cc_RRESP),
        .s_axi_rvalid(m01_couplers_to_auto_cc_RVALID),
        .s_axi_wdata(m01_couplers_to_auto_cc_WDATA),
        .s_axi_wlast(m01_couplers_to_auto_cc_WLAST),
        .s_axi_wready(m01_couplers_to_auto_cc_WREADY),
        .s_axi_wstrb(m01_couplers_to_auto_cc_WSTRB),
        .s_axi_wvalid(m01_couplers_to_auto_cc_WVALID));
  System_auto_ds_0 auto_ds
       (.m_axi_araddr(auto_ds_to_auto_pc_ARADDR),
        .m_axi_arburst(auto_ds_to_auto_pc_ARBURST),
        .m_axi_arcache(auto_ds_to_auto_pc_ARCACHE),
        .m_axi_arlen(auto_ds_to_auto_pc_ARLEN),
        .m_axi_arlock(auto_ds_to_auto_pc_ARLOCK),
        .m_axi_arprot(auto_ds_to_auto_pc_ARPROT),
        .m_axi_arqos(auto_ds_to_auto_pc_ARQOS),
        .m_axi_arready(auto_ds_to_auto_pc_ARREADY),
        .m_axi_arregion(auto_ds_to_auto_pc_ARREGION),
        .m_axi_arsize(auto_ds_to_auto_pc_ARSIZE),
        .m_axi_arvalid(auto_ds_to_auto_pc_ARVALID),
        .m_axi_awaddr(auto_ds_to_auto_pc_AWADDR),
        .m_axi_awburst(auto_ds_to_auto_pc_AWBURST),
        .m_axi_awcache(auto_ds_to_auto_pc_AWCACHE),
        .m_axi_awlen(auto_ds_to_auto_pc_AWLEN),
        .m_axi_awlock(auto_ds_to_auto_pc_AWLOCK),
        .m_axi_awprot(auto_ds_to_auto_pc_AWPROT),
        .m_axi_awqos(auto_ds_to_auto_pc_AWQOS),
        .m_axi_awready(auto_ds_to_auto_pc_AWREADY),
        .m_axi_awregion(auto_ds_to_auto_pc_AWREGION),
        .m_axi_awsize(auto_ds_to_auto_pc_AWSIZE),
        .m_axi_awvalid(auto_ds_to_auto_pc_AWVALID),
        .m_axi_bready(auto_ds_to_auto_pc_BREADY),
        .m_axi_bresp(auto_ds_to_auto_pc_BRESP),
        .m_axi_bvalid(auto_ds_to_auto_pc_BVALID),
        .m_axi_rdata(auto_ds_to_auto_pc_RDATA),
        .m_axi_rlast(auto_ds_to_auto_pc_RLAST),
        .m_axi_rready(auto_ds_to_auto_pc_RREADY),
        .m_axi_rresp(auto_ds_to_auto_pc_RRESP),
        .m_axi_rvalid(auto_ds_to_auto_pc_RVALID),
        .m_axi_wdata(auto_ds_to_auto_pc_WDATA),
        .m_axi_wlast(auto_ds_to_auto_pc_WLAST),
        .m_axi_wready(auto_ds_to_auto_pc_WREADY),
        .m_axi_wstrb(auto_ds_to_auto_pc_WSTRB),
        .m_axi_wvalid(auto_ds_to_auto_pc_WVALID),
        .s_axi_aclk(M_ACLK_1),
        .s_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .s_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .s_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .s_axi_aresetn(M_ARESETN_1),
        .s_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .s_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .s_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .s_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .s_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .s_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .s_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .s_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .s_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .s_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .s_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .s_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .s_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .s_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .s_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .s_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .s_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .s_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .s_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .s_axi_bready(auto_cc_to_auto_ds_BREADY),
        .s_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .s_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .s_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .s_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .s_axi_rready(auto_cc_to_auto_ds_RREADY),
        .s_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .s_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .s_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .s_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .s_axi_wready(auto_cc_to_auto_ds_WREADY),
        .s_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .s_axi_wvalid(auto_cc_to_auto_ds_WVALID));
  System_auto_pc_0 auto_pc
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(auto_pc_to_m01_regslice_ARADDR),
        .m_axi_arprot(auto_pc_to_m01_regslice_ARPROT),
        .m_axi_arready(auto_pc_to_m01_regslice_ARREADY),
        .m_axi_arvalid(auto_pc_to_m01_regslice_ARVALID),
        .m_axi_awaddr(auto_pc_to_m01_regslice_AWADDR),
        .m_axi_awprot(auto_pc_to_m01_regslice_AWPROT),
        .m_axi_awready(auto_pc_to_m01_regslice_AWREADY),
        .m_axi_awvalid(auto_pc_to_m01_regslice_AWVALID),
        .m_axi_bready(auto_pc_to_m01_regslice_BREADY),
        .m_axi_bresp(auto_pc_to_m01_regslice_BRESP),
        .m_axi_bvalid(auto_pc_to_m01_regslice_BVALID),
        .m_axi_rdata(auto_pc_to_m01_regslice_RDATA),
        .m_axi_rready(auto_pc_to_m01_regslice_RREADY),
        .m_axi_rresp(auto_pc_to_m01_regslice_RRESP),
        .m_axi_rvalid(auto_pc_to_m01_regslice_RVALID),
        .m_axi_wdata(auto_pc_to_m01_regslice_WDATA),
        .m_axi_wready(auto_pc_to_m01_regslice_WREADY),
        .m_axi_wstrb(auto_pc_to_m01_regslice_WSTRB),
        .m_axi_wvalid(auto_pc_to_m01_regslice_WVALID),
        .s_axi_araddr(auto_ds_to_auto_pc_ARADDR),
        .s_axi_arburst(auto_ds_to_auto_pc_ARBURST),
        .s_axi_arcache(auto_ds_to_auto_pc_ARCACHE),
        .s_axi_arlen(auto_ds_to_auto_pc_ARLEN),
        .s_axi_arlock(auto_ds_to_auto_pc_ARLOCK),
        .s_axi_arprot(auto_ds_to_auto_pc_ARPROT),
        .s_axi_arqos(auto_ds_to_auto_pc_ARQOS),
        .s_axi_arready(auto_ds_to_auto_pc_ARREADY),
        .s_axi_arregion(auto_ds_to_auto_pc_ARREGION),
        .s_axi_arsize(auto_ds_to_auto_pc_ARSIZE),
        .s_axi_arvalid(auto_ds_to_auto_pc_ARVALID),
        .s_axi_awaddr(auto_ds_to_auto_pc_AWADDR),
        .s_axi_awburst(auto_ds_to_auto_pc_AWBURST),
        .s_axi_awcache(auto_ds_to_auto_pc_AWCACHE),
        .s_axi_awlen(auto_ds_to_auto_pc_AWLEN),
        .s_axi_awlock(auto_ds_to_auto_pc_AWLOCK),
        .s_axi_awprot(auto_ds_to_auto_pc_AWPROT),
        .s_axi_awqos(auto_ds_to_auto_pc_AWQOS),
        .s_axi_awready(auto_ds_to_auto_pc_AWREADY),
        .s_axi_awregion(auto_ds_to_auto_pc_AWREGION),
        .s_axi_awsize(auto_ds_to_auto_pc_AWSIZE),
        .s_axi_awvalid(auto_ds_to_auto_pc_AWVALID),
        .s_axi_bready(auto_ds_to_auto_pc_BREADY),
        .s_axi_bresp(auto_ds_to_auto_pc_BRESP),
        .s_axi_bvalid(auto_ds_to_auto_pc_BVALID),
        .s_axi_rdata(auto_ds_to_auto_pc_RDATA),
        .s_axi_rlast(auto_ds_to_auto_pc_RLAST),
        .s_axi_rready(auto_ds_to_auto_pc_RREADY),
        .s_axi_rresp(auto_ds_to_auto_pc_RRESP),
        .s_axi_rvalid(auto_ds_to_auto_pc_RVALID),
        .s_axi_wdata(auto_ds_to_auto_pc_WDATA),
        .s_axi_wlast(auto_ds_to_auto_pc_WLAST),
        .s_axi_wready(auto_ds_to_auto_pc_WREADY),
        .s_axi_wstrb(auto_ds_to_auto_pc_WSTRB),
        .s_axi_wvalid(auto_ds_to_auto_pc_WVALID));
  System_m01_regslice_0 m01_regslice
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(m01_regslice_to_m01_couplers_ARADDR),
        .m_axi_arready(m01_regslice_to_m01_couplers_ARREADY),
        .m_axi_arvalid(m01_regslice_to_m01_couplers_ARVALID),
        .m_axi_awaddr(m01_regslice_to_m01_couplers_AWADDR),
        .m_axi_awready(m01_regslice_to_m01_couplers_AWREADY),
        .m_axi_awvalid(m01_regslice_to_m01_couplers_AWVALID),
        .m_axi_bready(m01_regslice_to_m01_couplers_BREADY),
        .m_axi_bresp(m01_regslice_to_m01_couplers_BRESP),
        .m_axi_bvalid(m01_regslice_to_m01_couplers_BVALID),
        .m_axi_rdata(m01_regslice_to_m01_couplers_RDATA),
        .m_axi_rready(m01_regslice_to_m01_couplers_RREADY),
        .m_axi_rresp(m01_regslice_to_m01_couplers_RRESP),
        .m_axi_rvalid(m01_regslice_to_m01_couplers_RVALID),
        .m_axi_wdata(m01_regslice_to_m01_couplers_WDATA),
        .m_axi_wready(m01_regslice_to_m01_couplers_WREADY),
        .m_axi_wvalid(m01_regslice_to_m01_couplers_WVALID),
        .s_axi_araddr(auto_pc_to_m01_regslice_ARADDR),
        .s_axi_arprot(auto_pc_to_m01_regslice_ARPROT),
        .s_axi_arready(auto_pc_to_m01_regslice_ARREADY),
        .s_axi_arvalid(auto_pc_to_m01_regslice_ARVALID),
        .s_axi_awaddr(auto_pc_to_m01_regslice_AWADDR),
        .s_axi_awprot(auto_pc_to_m01_regslice_AWPROT),
        .s_axi_awready(auto_pc_to_m01_regslice_AWREADY),
        .s_axi_awvalid(auto_pc_to_m01_regslice_AWVALID),
        .s_axi_bready(auto_pc_to_m01_regslice_BREADY),
        .s_axi_bresp(auto_pc_to_m01_regslice_BRESP),
        .s_axi_bvalid(auto_pc_to_m01_regslice_BVALID),
        .s_axi_rdata(auto_pc_to_m01_regslice_RDATA),
        .s_axi_rready(auto_pc_to_m01_regslice_RREADY),
        .s_axi_rresp(auto_pc_to_m01_regslice_RRESP),
        .s_axi_rvalid(auto_pc_to_m01_regslice_RVALID),
        .s_axi_wdata(auto_pc_to_m01_regslice_WDATA),
        .s_axi_wready(auto_pc_to_m01_regslice_WREADY),
        .s_axi_wstrb(auto_pc_to_m01_regslice_WSTRB),
        .s_axi_wvalid(auto_pc_to_m01_regslice_WVALID));
endmodule

module m01_couplers_imp_PW0UIW
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arlen,
    M_AXI_arready,
    M_AXI_arregion,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awlen,
    M_AXI_awready,
    M_AXI_awregion,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arid,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awid,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rid,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [7:0]M_AXI_arlen;
  input M_AXI_arready;
  output [3:0]M_AXI_arregion;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [7:0]M_AXI_awlen;
  input M_AXI_awready;
  output [3:0]M_AXI_awregion;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [127:0]M_AXI_rdata;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [127:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [15:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [2:0]S_AXI_arid;
  input [7:0]S_AXI_arlen;
  input [0:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [2:0]S_AXI_awid;
  input [7:0]S_AXI_awlen;
  input [0:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  output [2:0]S_AXI_bid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [511:0]S_AXI_rdata;
  output [2:0]S_AXI_rid;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [511:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire S_ACLK_1;
  wire S_ARESETN_1;
  wire [31:0]auto_cc_to_auto_ds_ARADDR;
  wire [1:0]auto_cc_to_auto_ds_ARBURST;
  wire [3:0]auto_cc_to_auto_ds_ARCACHE;
  wire [2:0]auto_cc_to_auto_ds_ARID;
  wire [7:0]auto_cc_to_auto_ds_ARLEN;
  wire [0:0]auto_cc_to_auto_ds_ARLOCK;
  wire [2:0]auto_cc_to_auto_ds_ARPROT;
  wire [3:0]auto_cc_to_auto_ds_ARQOS;
  wire auto_cc_to_auto_ds_ARREADY;
  wire [3:0]auto_cc_to_auto_ds_ARREGION;
  wire [2:0]auto_cc_to_auto_ds_ARSIZE;
  wire auto_cc_to_auto_ds_ARVALID;
  wire [31:0]auto_cc_to_auto_ds_AWADDR;
  wire [1:0]auto_cc_to_auto_ds_AWBURST;
  wire [3:0]auto_cc_to_auto_ds_AWCACHE;
  wire [2:0]auto_cc_to_auto_ds_AWID;
  wire [7:0]auto_cc_to_auto_ds_AWLEN;
  wire [0:0]auto_cc_to_auto_ds_AWLOCK;
  wire [2:0]auto_cc_to_auto_ds_AWPROT;
  wire [3:0]auto_cc_to_auto_ds_AWQOS;
  wire auto_cc_to_auto_ds_AWREADY;
  wire [3:0]auto_cc_to_auto_ds_AWREGION;
  wire [2:0]auto_cc_to_auto_ds_AWSIZE;
  wire auto_cc_to_auto_ds_AWVALID;
  wire [2:0]auto_cc_to_auto_ds_BID;
  wire auto_cc_to_auto_ds_BREADY;
  wire [1:0]auto_cc_to_auto_ds_BRESP;
  wire auto_cc_to_auto_ds_BVALID;
  wire [511:0]auto_cc_to_auto_ds_RDATA;
  wire [2:0]auto_cc_to_auto_ds_RID;
  wire auto_cc_to_auto_ds_RLAST;
  wire auto_cc_to_auto_ds_RREADY;
  wire [1:0]auto_cc_to_auto_ds_RRESP;
  wire auto_cc_to_auto_ds_RVALID;
  wire [511:0]auto_cc_to_auto_ds_WDATA;
  wire auto_cc_to_auto_ds_WLAST;
  wire auto_cc_to_auto_ds_WREADY;
  wire [63:0]auto_cc_to_auto_ds_WSTRB;
  wire auto_cc_to_auto_ds_WVALID;
  wire [31:0]auto_ds_to_m01_couplers_ARADDR;
  wire [1:0]auto_ds_to_m01_couplers_ARBURST;
  wire [7:0]auto_ds_to_m01_couplers_ARLEN;
  wire auto_ds_to_m01_couplers_ARREADY;
  wire [3:0]auto_ds_to_m01_couplers_ARREGION;
  wire [2:0]auto_ds_to_m01_couplers_ARSIZE;
  wire auto_ds_to_m01_couplers_ARVALID;
  wire [31:0]auto_ds_to_m01_couplers_AWADDR;
  wire [1:0]auto_ds_to_m01_couplers_AWBURST;
  wire [7:0]auto_ds_to_m01_couplers_AWLEN;
  wire auto_ds_to_m01_couplers_AWREADY;
  wire [3:0]auto_ds_to_m01_couplers_AWREGION;
  wire [2:0]auto_ds_to_m01_couplers_AWSIZE;
  wire auto_ds_to_m01_couplers_AWVALID;
  wire auto_ds_to_m01_couplers_BREADY;
  wire [1:0]auto_ds_to_m01_couplers_BRESP;
  wire auto_ds_to_m01_couplers_BVALID;
  wire [127:0]auto_ds_to_m01_couplers_RDATA;
  wire auto_ds_to_m01_couplers_RLAST;
  wire auto_ds_to_m01_couplers_RREADY;
  wire [1:0]auto_ds_to_m01_couplers_RRESP;
  wire auto_ds_to_m01_couplers_RVALID;
  wire [127:0]auto_ds_to_m01_couplers_WDATA;
  wire auto_ds_to_m01_couplers_WLAST;
  wire auto_ds_to_m01_couplers_WREADY;
  wire [15:0]auto_ds_to_m01_couplers_WSTRB;
  wire auto_ds_to_m01_couplers_WVALID;
  wire [31:0]m01_couplers_to_auto_cc_ARADDR;
  wire [1:0]m01_couplers_to_auto_cc_ARBURST;
  wire [3:0]m01_couplers_to_auto_cc_ARCACHE;
  wire [2:0]m01_couplers_to_auto_cc_ARID;
  wire [7:0]m01_couplers_to_auto_cc_ARLEN;
  wire [0:0]m01_couplers_to_auto_cc_ARLOCK;
  wire [2:0]m01_couplers_to_auto_cc_ARPROT;
  wire [3:0]m01_couplers_to_auto_cc_ARQOS;
  wire m01_couplers_to_auto_cc_ARREADY;
  wire [3:0]m01_couplers_to_auto_cc_ARREGION;
  wire [2:0]m01_couplers_to_auto_cc_ARSIZE;
  wire m01_couplers_to_auto_cc_ARVALID;
  wire [31:0]m01_couplers_to_auto_cc_AWADDR;
  wire [1:0]m01_couplers_to_auto_cc_AWBURST;
  wire [3:0]m01_couplers_to_auto_cc_AWCACHE;
  wire [2:0]m01_couplers_to_auto_cc_AWID;
  wire [7:0]m01_couplers_to_auto_cc_AWLEN;
  wire [0:0]m01_couplers_to_auto_cc_AWLOCK;
  wire [2:0]m01_couplers_to_auto_cc_AWPROT;
  wire [3:0]m01_couplers_to_auto_cc_AWQOS;
  wire m01_couplers_to_auto_cc_AWREADY;
  wire [3:0]m01_couplers_to_auto_cc_AWREGION;
  wire [2:0]m01_couplers_to_auto_cc_AWSIZE;
  wire m01_couplers_to_auto_cc_AWVALID;
  wire [2:0]m01_couplers_to_auto_cc_BID;
  wire m01_couplers_to_auto_cc_BREADY;
  wire [1:0]m01_couplers_to_auto_cc_BRESP;
  wire m01_couplers_to_auto_cc_BVALID;
  wire [511:0]m01_couplers_to_auto_cc_RDATA;
  wire [2:0]m01_couplers_to_auto_cc_RID;
  wire m01_couplers_to_auto_cc_RLAST;
  wire m01_couplers_to_auto_cc_RREADY;
  wire [1:0]m01_couplers_to_auto_cc_RRESP;
  wire m01_couplers_to_auto_cc_RVALID;
  wire [511:0]m01_couplers_to_auto_cc_WDATA;
  wire m01_couplers_to_auto_cc_WLAST;
  wire m01_couplers_to_auto_cc_WREADY;
  wire [63:0]m01_couplers_to_auto_cc_WSTRB;
  wire m01_couplers_to_auto_cc_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = auto_ds_to_m01_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = auto_ds_to_m01_couplers_ARBURST;
  assign M_AXI_arlen[7:0] = auto_ds_to_m01_couplers_ARLEN;
  assign M_AXI_arregion[3:0] = auto_ds_to_m01_couplers_ARREGION;
  assign M_AXI_arsize[2:0] = auto_ds_to_m01_couplers_ARSIZE;
  assign M_AXI_arvalid = auto_ds_to_m01_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = auto_ds_to_m01_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = auto_ds_to_m01_couplers_AWBURST;
  assign M_AXI_awlen[7:0] = auto_ds_to_m01_couplers_AWLEN;
  assign M_AXI_awregion[3:0] = auto_ds_to_m01_couplers_AWREGION;
  assign M_AXI_awsize[2:0] = auto_ds_to_m01_couplers_AWSIZE;
  assign M_AXI_awvalid = auto_ds_to_m01_couplers_AWVALID;
  assign M_AXI_bready = auto_ds_to_m01_couplers_BREADY;
  assign M_AXI_rready = auto_ds_to_m01_couplers_RREADY;
  assign M_AXI_wdata[127:0] = auto_ds_to_m01_couplers_WDATA;
  assign M_AXI_wlast = auto_ds_to_m01_couplers_WLAST;
  assign M_AXI_wstrb[15:0] = auto_ds_to_m01_couplers_WSTRB;
  assign M_AXI_wvalid = auto_ds_to_m01_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN;
  assign S_AXI_arready = m01_couplers_to_auto_cc_ARREADY;
  assign S_AXI_awready = m01_couplers_to_auto_cc_AWREADY;
  assign S_AXI_bid[2:0] = m01_couplers_to_auto_cc_BID;
  assign S_AXI_bresp[1:0] = m01_couplers_to_auto_cc_BRESP;
  assign S_AXI_bvalid = m01_couplers_to_auto_cc_BVALID;
  assign S_AXI_rdata[511:0] = m01_couplers_to_auto_cc_RDATA;
  assign S_AXI_rid[2:0] = m01_couplers_to_auto_cc_RID;
  assign S_AXI_rlast = m01_couplers_to_auto_cc_RLAST;
  assign S_AXI_rresp[1:0] = m01_couplers_to_auto_cc_RRESP;
  assign S_AXI_rvalid = m01_couplers_to_auto_cc_RVALID;
  assign S_AXI_wready = m01_couplers_to_auto_cc_WREADY;
  assign auto_ds_to_m01_couplers_ARREADY = M_AXI_arready;
  assign auto_ds_to_m01_couplers_AWREADY = M_AXI_awready;
  assign auto_ds_to_m01_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_ds_to_m01_couplers_BVALID = M_AXI_bvalid;
  assign auto_ds_to_m01_couplers_RDATA = M_AXI_rdata[127:0];
  assign auto_ds_to_m01_couplers_RLAST = M_AXI_rlast;
  assign auto_ds_to_m01_couplers_RRESP = M_AXI_rresp[1:0];
  assign auto_ds_to_m01_couplers_RVALID = M_AXI_rvalid;
  assign auto_ds_to_m01_couplers_WREADY = M_AXI_wready;
  assign m01_couplers_to_auto_cc_ARADDR = S_AXI_araddr[31:0];
  assign m01_couplers_to_auto_cc_ARBURST = S_AXI_arburst[1:0];
  assign m01_couplers_to_auto_cc_ARCACHE = S_AXI_arcache[3:0];
  assign m01_couplers_to_auto_cc_ARID = S_AXI_arid[2:0];
  assign m01_couplers_to_auto_cc_ARLEN = S_AXI_arlen[7:0];
  assign m01_couplers_to_auto_cc_ARLOCK = S_AXI_arlock[0];
  assign m01_couplers_to_auto_cc_ARPROT = S_AXI_arprot[2:0];
  assign m01_couplers_to_auto_cc_ARQOS = S_AXI_arqos[3:0];
  assign m01_couplers_to_auto_cc_ARREGION = S_AXI_arregion[3:0];
  assign m01_couplers_to_auto_cc_ARSIZE = S_AXI_arsize[2:0];
  assign m01_couplers_to_auto_cc_ARVALID = S_AXI_arvalid;
  assign m01_couplers_to_auto_cc_AWADDR = S_AXI_awaddr[31:0];
  assign m01_couplers_to_auto_cc_AWBURST = S_AXI_awburst[1:0];
  assign m01_couplers_to_auto_cc_AWCACHE = S_AXI_awcache[3:0];
  assign m01_couplers_to_auto_cc_AWID = S_AXI_awid[2:0];
  assign m01_couplers_to_auto_cc_AWLEN = S_AXI_awlen[7:0];
  assign m01_couplers_to_auto_cc_AWLOCK = S_AXI_awlock[0];
  assign m01_couplers_to_auto_cc_AWPROT = S_AXI_awprot[2:0];
  assign m01_couplers_to_auto_cc_AWQOS = S_AXI_awqos[3:0];
  assign m01_couplers_to_auto_cc_AWREGION = S_AXI_awregion[3:0];
  assign m01_couplers_to_auto_cc_AWSIZE = S_AXI_awsize[2:0];
  assign m01_couplers_to_auto_cc_AWVALID = S_AXI_awvalid;
  assign m01_couplers_to_auto_cc_BREADY = S_AXI_bready;
  assign m01_couplers_to_auto_cc_RREADY = S_AXI_rready;
  assign m01_couplers_to_auto_cc_WDATA = S_AXI_wdata[511:0];
  assign m01_couplers_to_auto_cc_WLAST = S_AXI_wlast;
  assign m01_couplers_to_auto_cc_WSTRB = S_AXI_wstrb[63:0];
  assign m01_couplers_to_auto_cc_WVALID = S_AXI_wvalid;
  System_auto_cc_3 auto_cc
       (.m_axi_aclk(M_ACLK_1),
        .m_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .m_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .m_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .m_axi_aresetn(M_ARESETN_1),
        .m_axi_arid(auto_cc_to_auto_ds_ARID),
        .m_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .m_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .m_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .m_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .m_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .m_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .m_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .m_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .m_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .m_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .m_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .m_axi_awid(auto_cc_to_auto_ds_AWID),
        .m_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .m_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .m_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .m_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .m_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .m_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .m_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .m_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .m_axi_bid(auto_cc_to_auto_ds_BID),
        .m_axi_bready(auto_cc_to_auto_ds_BREADY),
        .m_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .m_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .m_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .m_axi_rid(auto_cc_to_auto_ds_RID),
        .m_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .m_axi_rready(auto_cc_to_auto_ds_RREADY),
        .m_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .m_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .m_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .m_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .m_axi_wready(auto_cc_to_auto_ds_WREADY),
        .m_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .m_axi_wvalid(auto_cc_to_auto_ds_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(m01_couplers_to_auto_cc_ARADDR),
        .s_axi_arburst(m01_couplers_to_auto_cc_ARBURST),
        .s_axi_arcache(m01_couplers_to_auto_cc_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arid(m01_couplers_to_auto_cc_ARID),
        .s_axi_arlen(m01_couplers_to_auto_cc_ARLEN),
        .s_axi_arlock(m01_couplers_to_auto_cc_ARLOCK),
        .s_axi_arprot(m01_couplers_to_auto_cc_ARPROT),
        .s_axi_arqos(m01_couplers_to_auto_cc_ARQOS),
        .s_axi_arready(m01_couplers_to_auto_cc_ARREADY),
        .s_axi_arregion(m01_couplers_to_auto_cc_ARREGION),
        .s_axi_arsize(m01_couplers_to_auto_cc_ARSIZE),
        .s_axi_arvalid(m01_couplers_to_auto_cc_ARVALID),
        .s_axi_awaddr(m01_couplers_to_auto_cc_AWADDR),
        .s_axi_awburst(m01_couplers_to_auto_cc_AWBURST),
        .s_axi_awcache(m01_couplers_to_auto_cc_AWCACHE),
        .s_axi_awid(m01_couplers_to_auto_cc_AWID),
        .s_axi_awlen(m01_couplers_to_auto_cc_AWLEN),
        .s_axi_awlock(m01_couplers_to_auto_cc_AWLOCK),
        .s_axi_awprot(m01_couplers_to_auto_cc_AWPROT),
        .s_axi_awqos(m01_couplers_to_auto_cc_AWQOS),
        .s_axi_awready(m01_couplers_to_auto_cc_AWREADY),
        .s_axi_awregion(m01_couplers_to_auto_cc_AWREGION),
        .s_axi_awsize(m01_couplers_to_auto_cc_AWSIZE),
        .s_axi_awvalid(m01_couplers_to_auto_cc_AWVALID),
        .s_axi_bid(m01_couplers_to_auto_cc_BID),
        .s_axi_bready(m01_couplers_to_auto_cc_BREADY),
        .s_axi_bresp(m01_couplers_to_auto_cc_BRESP),
        .s_axi_bvalid(m01_couplers_to_auto_cc_BVALID),
        .s_axi_rdata(m01_couplers_to_auto_cc_RDATA),
        .s_axi_rid(m01_couplers_to_auto_cc_RID),
        .s_axi_rlast(m01_couplers_to_auto_cc_RLAST),
        .s_axi_rready(m01_couplers_to_auto_cc_RREADY),
        .s_axi_rresp(m01_couplers_to_auto_cc_RRESP),
        .s_axi_rvalid(m01_couplers_to_auto_cc_RVALID),
        .s_axi_wdata(m01_couplers_to_auto_cc_WDATA),
        .s_axi_wlast(m01_couplers_to_auto_cc_WLAST),
        .s_axi_wready(m01_couplers_to_auto_cc_WREADY),
        .s_axi_wstrb(m01_couplers_to_auto_cc_WSTRB),
        .s_axi_wvalid(m01_couplers_to_auto_cc_WVALID));
  System_auto_ds_2 auto_ds
       (.m_axi_araddr(auto_ds_to_m01_couplers_ARADDR),
        .m_axi_arburst(auto_ds_to_m01_couplers_ARBURST),
        .m_axi_arlen(auto_ds_to_m01_couplers_ARLEN),
        .m_axi_arready(auto_ds_to_m01_couplers_ARREADY),
        .m_axi_arregion(auto_ds_to_m01_couplers_ARREGION),
        .m_axi_arsize(auto_ds_to_m01_couplers_ARSIZE),
        .m_axi_arvalid(auto_ds_to_m01_couplers_ARVALID),
        .m_axi_awaddr(auto_ds_to_m01_couplers_AWADDR),
        .m_axi_awburst(auto_ds_to_m01_couplers_AWBURST),
        .m_axi_awlen(auto_ds_to_m01_couplers_AWLEN),
        .m_axi_awready(auto_ds_to_m01_couplers_AWREADY),
        .m_axi_awregion(auto_ds_to_m01_couplers_AWREGION),
        .m_axi_awsize(auto_ds_to_m01_couplers_AWSIZE),
        .m_axi_awvalid(auto_ds_to_m01_couplers_AWVALID),
        .m_axi_bready(auto_ds_to_m01_couplers_BREADY),
        .m_axi_bresp(auto_ds_to_m01_couplers_BRESP),
        .m_axi_bvalid(auto_ds_to_m01_couplers_BVALID),
        .m_axi_rdata(auto_ds_to_m01_couplers_RDATA),
        .m_axi_rlast(auto_ds_to_m01_couplers_RLAST),
        .m_axi_rready(auto_ds_to_m01_couplers_RREADY),
        .m_axi_rresp(auto_ds_to_m01_couplers_RRESP),
        .m_axi_rvalid(auto_ds_to_m01_couplers_RVALID),
        .m_axi_wdata(auto_ds_to_m01_couplers_WDATA),
        .m_axi_wlast(auto_ds_to_m01_couplers_WLAST),
        .m_axi_wready(auto_ds_to_m01_couplers_WREADY),
        .m_axi_wstrb(auto_ds_to_m01_couplers_WSTRB),
        .m_axi_wvalid(auto_ds_to_m01_couplers_WVALID),
        .s_axi_aclk(M_ACLK_1),
        .s_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .s_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .s_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .s_axi_aresetn(M_ARESETN_1),
        .s_axi_arid(auto_cc_to_auto_ds_ARID),
        .s_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .s_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .s_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .s_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .s_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .s_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .s_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .s_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .s_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .s_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .s_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .s_axi_awid(auto_cc_to_auto_ds_AWID),
        .s_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .s_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .s_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .s_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .s_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .s_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .s_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .s_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .s_axi_bid(auto_cc_to_auto_ds_BID),
        .s_axi_bready(auto_cc_to_auto_ds_BREADY),
        .s_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .s_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .s_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .s_axi_rid(auto_cc_to_auto_ds_RID),
        .s_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .s_axi_rready(auto_cc_to_auto_ds_RREADY),
        .s_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .s_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .s_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .s_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .s_axi_wready(auto_cc_to_auto_ds_WREADY),
        .s_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .s_axi_wvalid(auto_cc_to_auto_ds_WVALID));
endmodule

module m02_couplers_imp_2MPU5Q
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arready,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awready,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  input M_AXI_arready;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  input M_AXI_awready;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [31:0]M_AXI_rdata;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [31:0]M_AXI_wdata;
  input M_AXI_wready;
  output [3:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input [0:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [0:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [127:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [127:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [15:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire S_ACLK_1;
  wire S_ARESETN_1;
  wire [31:0]auto_cc_to_auto_ds_ARADDR;
  wire [1:0]auto_cc_to_auto_ds_ARBURST;
  wire [3:0]auto_cc_to_auto_ds_ARCACHE;
  wire [7:0]auto_cc_to_auto_ds_ARLEN;
  wire [0:0]auto_cc_to_auto_ds_ARLOCK;
  wire [2:0]auto_cc_to_auto_ds_ARPROT;
  wire [3:0]auto_cc_to_auto_ds_ARQOS;
  wire auto_cc_to_auto_ds_ARREADY;
  wire [3:0]auto_cc_to_auto_ds_ARREGION;
  wire [2:0]auto_cc_to_auto_ds_ARSIZE;
  wire auto_cc_to_auto_ds_ARVALID;
  wire [31:0]auto_cc_to_auto_ds_AWADDR;
  wire [1:0]auto_cc_to_auto_ds_AWBURST;
  wire [3:0]auto_cc_to_auto_ds_AWCACHE;
  wire [7:0]auto_cc_to_auto_ds_AWLEN;
  wire [0:0]auto_cc_to_auto_ds_AWLOCK;
  wire [2:0]auto_cc_to_auto_ds_AWPROT;
  wire [3:0]auto_cc_to_auto_ds_AWQOS;
  wire auto_cc_to_auto_ds_AWREADY;
  wire [3:0]auto_cc_to_auto_ds_AWREGION;
  wire [2:0]auto_cc_to_auto_ds_AWSIZE;
  wire auto_cc_to_auto_ds_AWVALID;
  wire auto_cc_to_auto_ds_BREADY;
  wire [1:0]auto_cc_to_auto_ds_BRESP;
  wire auto_cc_to_auto_ds_BVALID;
  wire [127:0]auto_cc_to_auto_ds_RDATA;
  wire auto_cc_to_auto_ds_RLAST;
  wire auto_cc_to_auto_ds_RREADY;
  wire [1:0]auto_cc_to_auto_ds_RRESP;
  wire auto_cc_to_auto_ds_RVALID;
  wire [127:0]auto_cc_to_auto_ds_WDATA;
  wire auto_cc_to_auto_ds_WLAST;
  wire auto_cc_to_auto_ds_WREADY;
  wire [15:0]auto_cc_to_auto_ds_WSTRB;
  wire auto_cc_to_auto_ds_WVALID;
  wire [31:0]auto_ds_to_auto_pc_ARADDR;
  wire [1:0]auto_ds_to_auto_pc_ARBURST;
  wire [3:0]auto_ds_to_auto_pc_ARCACHE;
  wire [7:0]auto_ds_to_auto_pc_ARLEN;
  wire [0:0]auto_ds_to_auto_pc_ARLOCK;
  wire [2:0]auto_ds_to_auto_pc_ARPROT;
  wire [3:0]auto_ds_to_auto_pc_ARQOS;
  wire auto_ds_to_auto_pc_ARREADY;
  wire [3:0]auto_ds_to_auto_pc_ARREGION;
  wire [2:0]auto_ds_to_auto_pc_ARSIZE;
  wire auto_ds_to_auto_pc_ARVALID;
  wire [31:0]auto_ds_to_auto_pc_AWADDR;
  wire [1:0]auto_ds_to_auto_pc_AWBURST;
  wire [3:0]auto_ds_to_auto_pc_AWCACHE;
  wire [7:0]auto_ds_to_auto_pc_AWLEN;
  wire [0:0]auto_ds_to_auto_pc_AWLOCK;
  wire [2:0]auto_ds_to_auto_pc_AWPROT;
  wire [3:0]auto_ds_to_auto_pc_AWQOS;
  wire auto_ds_to_auto_pc_AWREADY;
  wire [3:0]auto_ds_to_auto_pc_AWREGION;
  wire [2:0]auto_ds_to_auto_pc_AWSIZE;
  wire auto_ds_to_auto_pc_AWVALID;
  wire auto_ds_to_auto_pc_BREADY;
  wire [1:0]auto_ds_to_auto_pc_BRESP;
  wire auto_ds_to_auto_pc_BVALID;
  wire [31:0]auto_ds_to_auto_pc_RDATA;
  wire auto_ds_to_auto_pc_RLAST;
  wire auto_ds_to_auto_pc_RREADY;
  wire [1:0]auto_ds_to_auto_pc_RRESP;
  wire auto_ds_to_auto_pc_RVALID;
  wire [31:0]auto_ds_to_auto_pc_WDATA;
  wire auto_ds_to_auto_pc_WLAST;
  wire auto_ds_to_auto_pc_WREADY;
  wire [3:0]auto_ds_to_auto_pc_WSTRB;
  wire auto_ds_to_auto_pc_WVALID;
  wire [31:0]auto_pc_to_m02_couplers_ARADDR;
  wire auto_pc_to_m02_couplers_ARREADY;
  wire auto_pc_to_m02_couplers_ARVALID;
  wire [31:0]auto_pc_to_m02_couplers_AWADDR;
  wire auto_pc_to_m02_couplers_AWREADY;
  wire auto_pc_to_m02_couplers_AWVALID;
  wire auto_pc_to_m02_couplers_BREADY;
  wire [1:0]auto_pc_to_m02_couplers_BRESP;
  wire auto_pc_to_m02_couplers_BVALID;
  wire [31:0]auto_pc_to_m02_couplers_RDATA;
  wire auto_pc_to_m02_couplers_RREADY;
  wire [1:0]auto_pc_to_m02_couplers_RRESP;
  wire auto_pc_to_m02_couplers_RVALID;
  wire [31:0]auto_pc_to_m02_couplers_WDATA;
  wire auto_pc_to_m02_couplers_WREADY;
  wire [3:0]auto_pc_to_m02_couplers_WSTRB;
  wire auto_pc_to_m02_couplers_WVALID;
  wire [31:0]m02_couplers_to_auto_cc_ARADDR;
  wire [1:0]m02_couplers_to_auto_cc_ARBURST;
  wire [3:0]m02_couplers_to_auto_cc_ARCACHE;
  wire [7:0]m02_couplers_to_auto_cc_ARLEN;
  wire [0:0]m02_couplers_to_auto_cc_ARLOCK;
  wire [2:0]m02_couplers_to_auto_cc_ARPROT;
  wire [3:0]m02_couplers_to_auto_cc_ARQOS;
  wire m02_couplers_to_auto_cc_ARREADY;
  wire [3:0]m02_couplers_to_auto_cc_ARREGION;
  wire [2:0]m02_couplers_to_auto_cc_ARSIZE;
  wire m02_couplers_to_auto_cc_ARVALID;
  wire [31:0]m02_couplers_to_auto_cc_AWADDR;
  wire [1:0]m02_couplers_to_auto_cc_AWBURST;
  wire [3:0]m02_couplers_to_auto_cc_AWCACHE;
  wire [7:0]m02_couplers_to_auto_cc_AWLEN;
  wire [0:0]m02_couplers_to_auto_cc_AWLOCK;
  wire [2:0]m02_couplers_to_auto_cc_AWPROT;
  wire [3:0]m02_couplers_to_auto_cc_AWQOS;
  wire m02_couplers_to_auto_cc_AWREADY;
  wire [3:0]m02_couplers_to_auto_cc_AWREGION;
  wire [2:0]m02_couplers_to_auto_cc_AWSIZE;
  wire m02_couplers_to_auto_cc_AWVALID;
  wire m02_couplers_to_auto_cc_BREADY;
  wire [1:0]m02_couplers_to_auto_cc_BRESP;
  wire m02_couplers_to_auto_cc_BVALID;
  wire [127:0]m02_couplers_to_auto_cc_RDATA;
  wire m02_couplers_to_auto_cc_RLAST;
  wire m02_couplers_to_auto_cc_RREADY;
  wire [1:0]m02_couplers_to_auto_cc_RRESP;
  wire m02_couplers_to_auto_cc_RVALID;
  wire [127:0]m02_couplers_to_auto_cc_WDATA;
  wire m02_couplers_to_auto_cc_WLAST;
  wire m02_couplers_to_auto_cc_WREADY;
  wire [15:0]m02_couplers_to_auto_cc_WSTRB;
  wire m02_couplers_to_auto_cc_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = auto_pc_to_m02_couplers_ARADDR;
  assign M_AXI_arvalid = auto_pc_to_m02_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = auto_pc_to_m02_couplers_AWADDR;
  assign M_AXI_awvalid = auto_pc_to_m02_couplers_AWVALID;
  assign M_AXI_bready = auto_pc_to_m02_couplers_BREADY;
  assign M_AXI_rready = auto_pc_to_m02_couplers_RREADY;
  assign M_AXI_wdata[31:0] = auto_pc_to_m02_couplers_WDATA;
  assign M_AXI_wstrb[3:0] = auto_pc_to_m02_couplers_WSTRB;
  assign M_AXI_wvalid = auto_pc_to_m02_couplers_WVALID;
  assign S_ACLK_1 = S_ACLK;
  assign S_ARESETN_1 = S_ARESETN;
  assign S_AXI_arready = m02_couplers_to_auto_cc_ARREADY;
  assign S_AXI_awready = m02_couplers_to_auto_cc_AWREADY;
  assign S_AXI_bresp[1:0] = m02_couplers_to_auto_cc_BRESP;
  assign S_AXI_bvalid = m02_couplers_to_auto_cc_BVALID;
  assign S_AXI_rdata[127:0] = m02_couplers_to_auto_cc_RDATA;
  assign S_AXI_rlast = m02_couplers_to_auto_cc_RLAST;
  assign S_AXI_rresp[1:0] = m02_couplers_to_auto_cc_RRESP;
  assign S_AXI_rvalid = m02_couplers_to_auto_cc_RVALID;
  assign S_AXI_wready = m02_couplers_to_auto_cc_WREADY;
  assign auto_pc_to_m02_couplers_ARREADY = M_AXI_arready;
  assign auto_pc_to_m02_couplers_AWREADY = M_AXI_awready;
  assign auto_pc_to_m02_couplers_BRESP = M_AXI_bresp[1:0];
  assign auto_pc_to_m02_couplers_BVALID = M_AXI_bvalid;
  assign auto_pc_to_m02_couplers_RDATA = M_AXI_rdata[31:0];
  assign auto_pc_to_m02_couplers_RRESP = M_AXI_rresp[1:0];
  assign auto_pc_to_m02_couplers_RVALID = M_AXI_rvalid;
  assign auto_pc_to_m02_couplers_WREADY = M_AXI_wready;
  assign m02_couplers_to_auto_cc_ARADDR = S_AXI_araddr[31:0];
  assign m02_couplers_to_auto_cc_ARBURST = S_AXI_arburst[1:0];
  assign m02_couplers_to_auto_cc_ARCACHE = S_AXI_arcache[3:0];
  assign m02_couplers_to_auto_cc_ARLEN = S_AXI_arlen[7:0];
  assign m02_couplers_to_auto_cc_ARLOCK = S_AXI_arlock[0];
  assign m02_couplers_to_auto_cc_ARPROT = S_AXI_arprot[2:0];
  assign m02_couplers_to_auto_cc_ARQOS = S_AXI_arqos[3:0];
  assign m02_couplers_to_auto_cc_ARREGION = S_AXI_arregion[3:0];
  assign m02_couplers_to_auto_cc_ARSIZE = S_AXI_arsize[2:0];
  assign m02_couplers_to_auto_cc_ARVALID = S_AXI_arvalid;
  assign m02_couplers_to_auto_cc_AWADDR = S_AXI_awaddr[31:0];
  assign m02_couplers_to_auto_cc_AWBURST = S_AXI_awburst[1:0];
  assign m02_couplers_to_auto_cc_AWCACHE = S_AXI_awcache[3:0];
  assign m02_couplers_to_auto_cc_AWLEN = S_AXI_awlen[7:0];
  assign m02_couplers_to_auto_cc_AWLOCK = S_AXI_awlock[0];
  assign m02_couplers_to_auto_cc_AWPROT = S_AXI_awprot[2:0];
  assign m02_couplers_to_auto_cc_AWQOS = S_AXI_awqos[3:0];
  assign m02_couplers_to_auto_cc_AWREGION = S_AXI_awregion[3:0];
  assign m02_couplers_to_auto_cc_AWSIZE = S_AXI_awsize[2:0];
  assign m02_couplers_to_auto_cc_AWVALID = S_AXI_awvalid;
  assign m02_couplers_to_auto_cc_BREADY = S_AXI_bready;
  assign m02_couplers_to_auto_cc_RREADY = S_AXI_rready;
  assign m02_couplers_to_auto_cc_WDATA = S_AXI_wdata[127:0];
  assign m02_couplers_to_auto_cc_WLAST = S_AXI_wlast;
  assign m02_couplers_to_auto_cc_WSTRB = S_AXI_wstrb[15:0];
  assign m02_couplers_to_auto_cc_WVALID = S_AXI_wvalid;
  System_auto_cc_2 auto_cc
       (.m_axi_aclk(M_ACLK_1),
        .m_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .m_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .m_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .m_axi_aresetn(M_ARESETN_1),
        .m_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .m_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .m_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .m_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .m_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .m_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .m_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .m_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .m_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .m_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .m_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .m_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .m_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .m_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .m_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .m_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .m_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .m_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .m_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .m_axi_bready(auto_cc_to_auto_ds_BREADY),
        .m_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .m_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .m_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .m_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .m_axi_rready(auto_cc_to_auto_ds_RREADY),
        .m_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .m_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .m_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .m_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .m_axi_wready(auto_cc_to_auto_ds_WREADY),
        .m_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .m_axi_wvalid(auto_cc_to_auto_ds_WVALID),
        .s_axi_aclk(S_ACLK_1),
        .s_axi_araddr(m02_couplers_to_auto_cc_ARADDR),
        .s_axi_arburst(m02_couplers_to_auto_cc_ARBURST),
        .s_axi_arcache(m02_couplers_to_auto_cc_ARCACHE),
        .s_axi_aresetn(S_ARESETN_1),
        .s_axi_arlen(m02_couplers_to_auto_cc_ARLEN),
        .s_axi_arlock(m02_couplers_to_auto_cc_ARLOCK),
        .s_axi_arprot(m02_couplers_to_auto_cc_ARPROT),
        .s_axi_arqos(m02_couplers_to_auto_cc_ARQOS),
        .s_axi_arready(m02_couplers_to_auto_cc_ARREADY),
        .s_axi_arregion(m02_couplers_to_auto_cc_ARREGION),
        .s_axi_arsize(m02_couplers_to_auto_cc_ARSIZE),
        .s_axi_arvalid(m02_couplers_to_auto_cc_ARVALID),
        .s_axi_awaddr(m02_couplers_to_auto_cc_AWADDR),
        .s_axi_awburst(m02_couplers_to_auto_cc_AWBURST),
        .s_axi_awcache(m02_couplers_to_auto_cc_AWCACHE),
        .s_axi_awlen(m02_couplers_to_auto_cc_AWLEN),
        .s_axi_awlock(m02_couplers_to_auto_cc_AWLOCK),
        .s_axi_awprot(m02_couplers_to_auto_cc_AWPROT),
        .s_axi_awqos(m02_couplers_to_auto_cc_AWQOS),
        .s_axi_awready(m02_couplers_to_auto_cc_AWREADY),
        .s_axi_awregion(m02_couplers_to_auto_cc_AWREGION),
        .s_axi_awsize(m02_couplers_to_auto_cc_AWSIZE),
        .s_axi_awvalid(m02_couplers_to_auto_cc_AWVALID),
        .s_axi_bready(m02_couplers_to_auto_cc_BREADY),
        .s_axi_bresp(m02_couplers_to_auto_cc_BRESP),
        .s_axi_bvalid(m02_couplers_to_auto_cc_BVALID),
        .s_axi_rdata(m02_couplers_to_auto_cc_RDATA),
        .s_axi_rlast(m02_couplers_to_auto_cc_RLAST),
        .s_axi_rready(m02_couplers_to_auto_cc_RREADY),
        .s_axi_rresp(m02_couplers_to_auto_cc_RRESP),
        .s_axi_rvalid(m02_couplers_to_auto_cc_RVALID),
        .s_axi_wdata(m02_couplers_to_auto_cc_WDATA),
        .s_axi_wlast(m02_couplers_to_auto_cc_WLAST),
        .s_axi_wready(m02_couplers_to_auto_cc_WREADY),
        .s_axi_wstrb(m02_couplers_to_auto_cc_WSTRB),
        .s_axi_wvalid(m02_couplers_to_auto_cc_WVALID));
  System_auto_ds_1 auto_ds
       (.m_axi_araddr(auto_ds_to_auto_pc_ARADDR),
        .m_axi_arburst(auto_ds_to_auto_pc_ARBURST),
        .m_axi_arcache(auto_ds_to_auto_pc_ARCACHE),
        .m_axi_arlen(auto_ds_to_auto_pc_ARLEN),
        .m_axi_arlock(auto_ds_to_auto_pc_ARLOCK),
        .m_axi_arprot(auto_ds_to_auto_pc_ARPROT),
        .m_axi_arqos(auto_ds_to_auto_pc_ARQOS),
        .m_axi_arready(auto_ds_to_auto_pc_ARREADY),
        .m_axi_arregion(auto_ds_to_auto_pc_ARREGION),
        .m_axi_arsize(auto_ds_to_auto_pc_ARSIZE),
        .m_axi_arvalid(auto_ds_to_auto_pc_ARVALID),
        .m_axi_awaddr(auto_ds_to_auto_pc_AWADDR),
        .m_axi_awburst(auto_ds_to_auto_pc_AWBURST),
        .m_axi_awcache(auto_ds_to_auto_pc_AWCACHE),
        .m_axi_awlen(auto_ds_to_auto_pc_AWLEN),
        .m_axi_awlock(auto_ds_to_auto_pc_AWLOCK),
        .m_axi_awprot(auto_ds_to_auto_pc_AWPROT),
        .m_axi_awqos(auto_ds_to_auto_pc_AWQOS),
        .m_axi_awready(auto_ds_to_auto_pc_AWREADY),
        .m_axi_awregion(auto_ds_to_auto_pc_AWREGION),
        .m_axi_awsize(auto_ds_to_auto_pc_AWSIZE),
        .m_axi_awvalid(auto_ds_to_auto_pc_AWVALID),
        .m_axi_bready(auto_ds_to_auto_pc_BREADY),
        .m_axi_bresp(auto_ds_to_auto_pc_BRESP),
        .m_axi_bvalid(auto_ds_to_auto_pc_BVALID),
        .m_axi_rdata(auto_ds_to_auto_pc_RDATA),
        .m_axi_rlast(auto_ds_to_auto_pc_RLAST),
        .m_axi_rready(auto_ds_to_auto_pc_RREADY),
        .m_axi_rresp(auto_ds_to_auto_pc_RRESP),
        .m_axi_rvalid(auto_ds_to_auto_pc_RVALID),
        .m_axi_wdata(auto_ds_to_auto_pc_WDATA),
        .m_axi_wlast(auto_ds_to_auto_pc_WLAST),
        .m_axi_wready(auto_ds_to_auto_pc_WREADY),
        .m_axi_wstrb(auto_ds_to_auto_pc_WSTRB),
        .m_axi_wvalid(auto_ds_to_auto_pc_WVALID),
        .s_axi_aclk(M_ACLK_1),
        .s_axi_araddr(auto_cc_to_auto_ds_ARADDR),
        .s_axi_arburst(auto_cc_to_auto_ds_ARBURST),
        .s_axi_arcache(auto_cc_to_auto_ds_ARCACHE),
        .s_axi_aresetn(M_ARESETN_1),
        .s_axi_arlen(auto_cc_to_auto_ds_ARLEN),
        .s_axi_arlock(auto_cc_to_auto_ds_ARLOCK),
        .s_axi_arprot(auto_cc_to_auto_ds_ARPROT),
        .s_axi_arqos(auto_cc_to_auto_ds_ARQOS),
        .s_axi_arready(auto_cc_to_auto_ds_ARREADY),
        .s_axi_arregion(auto_cc_to_auto_ds_ARREGION),
        .s_axi_arsize(auto_cc_to_auto_ds_ARSIZE),
        .s_axi_arvalid(auto_cc_to_auto_ds_ARVALID),
        .s_axi_awaddr(auto_cc_to_auto_ds_AWADDR),
        .s_axi_awburst(auto_cc_to_auto_ds_AWBURST),
        .s_axi_awcache(auto_cc_to_auto_ds_AWCACHE),
        .s_axi_awlen(auto_cc_to_auto_ds_AWLEN),
        .s_axi_awlock(auto_cc_to_auto_ds_AWLOCK),
        .s_axi_awprot(auto_cc_to_auto_ds_AWPROT),
        .s_axi_awqos(auto_cc_to_auto_ds_AWQOS),
        .s_axi_awready(auto_cc_to_auto_ds_AWREADY),
        .s_axi_awregion(auto_cc_to_auto_ds_AWREGION),
        .s_axi_awsize(auto_cc_to_auto_ds_AWSIZE),
        .s_axi_awvalid(auto_cc_to_auto_ds_AWVALID),
        .s_axi_bready(auto_cc_to_auto_ds_BREADY),
        .s_axi_bresp(auto_cc_to_auto_ds_BRESP),
        .s_axi_bvalid(auto_cc_to_auto_ds_BVALID),
        .s_axi_rdata(auto_cc_to_auto_ds_RDATA),
        .s_axi_rlast(auto_cc_to_auto_ds_RLAST),
        .s_axi_rready(auto_cc_to_auto_ds_RREADY),
        .s_axi_rresp(auto_cc_to_auto_ds_RRESP),
        .s_axi_rvalid(auto_cc_to_auto_ds_RVALID),
        .s_axi_wdata(auto_cc_to_auto_ds_WDATA),
        .s_axi_wlast(auto_cc_to_auto_ds_WLAST),
        .s_axi_wready(auto_cc_to_auto_ds_WREADY),
        .s_axi_wstrb(auto_cc_to_auto_ds_WSTRB),
        .s_axi_wvalid(auto_cc_to_auto_ds_WVALID));
  System_auto_pc_1 auto_pc
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(auto_pc_to_m02_couplers_ARADDR),
        .m_axi_arready(auto_pc_to_m02_couplers_ARREADY),
        .m_axi_arvalid(auto_pc_to_m02_couplers_ARVALID),
        .m_axi_awaddr(auto_pc_to_m02_couplers_AWADDR),
        .m_axi_awready(auto_pc_to_m02_couplers_AWREADY),
        .m_axi_awvalid(auto_pc_to_m02_couplers_AWVALID),
        .m_axi_bready(auto_pc_to_m02_couplers_BREADY),
        .m_axi_bresp(auto_pc_to_m02_couplers_BRESP),
        .m_axi_bvalid(auto_pc_to_m02_couplers_BVALID),
        .m_axi_rdata(auto_pc_to_m02_couplers_RDATA),
        .m_axi_rready(auto_pc_to_m02_couplers_RREADY),
        .m_axi_rresp(auto_pc_to_m02_couplers_RRESP),
        .m_axi_rvalid(auto_pc_to_m02_couplers_RVALID),
        .m_axi_wdata(auto_pc_to_m02_couplers_WDATA),
        .m_axi_wready(auto_pc_to_m02_couplers_WREADY),
        .m_axi_wstrb(auto_pc_to_m02_couplers_WSTRB),
        .m_axi_wvalid(auto_pc_to_m02_couplers_WVALID),
        .s_axi_araddr(auto_ds_to_auto_pc_ARADDR),
        .s_axi_arburst(auto_ds_to_auto_pc_ARBURST),
        .s_axi_arcache(auto_ds_to_auto_pc_ARCACHE),
        .s_axi_arlen(auto_ds_to_auto_pc_ARLEN),
        .s_axi_arlock(auto_ds_to_auto_pc_ARLOCK),
        .s_axi_arprot(auto_ds_to_auto_pc_ARPROT),
        .s_axi_arqos(auto_ds_to_auto_pc_ARQOS),
        .s_axi_arready(auto_ds_to_auto_pc_ARREADY),
        .s_axi_arregion(auto_ds_to_auto_pc_ARREGION),
        .s_axi_arsize(auto_ds_to_auto_pc_ARSIZE),
        .s_axi_arvalid(auto_ds_to_auto_pc_ARVALID),
        .s_axi_awaddr(auto_ds_to_auto_pc_AWADDR),
        .s_axi_awburst(auto_ds_to_auto_pc_AWBURST),
        .s_axi_awcache(auto_ds_to_auto_pc_AWCACHE),
        .s_axi_awlen(auto_ds_to_auto_pc_AWLEN),
        .s_axi_awlock(auto_ds_to_auto_pc_AWLOCK),
        .s_axi_awprot(auto_ds_to_auto_pc_AWPROT),
        .s_axi_awqos(auto_ds_to_auto_pc_AWQOS),
        .s_axi_awready(auto_ds_to_auto_pc_AWREADY),
        .s_axi_awregion(auto_ds_to_auto_pc_AWREGION),
        .s_axi_awsize(auto_ds_to_auto_pc_AWSIZE),
        .s_axi_awvalid(auto_ds_to_auto_pc_AWVALID),
        .s_axi_bready(auto_ds_to_auto_pc_BREADY),
        .s_axi_bresp(auto_ds_to_auto_pc_BRESP),
        .s_axi_bvalid(auto_ds_to_auto_pc_BVALID),
        .s_axi_rdata(auto_ds_to_auto_pc_RDATA),
        .s_axi_rlast(auto_ds_to_auto_pc_RLAST),
        .s_axi_rready(auto_ds_to_auto_pc_RREADY),
        .s_axi_rresp(auto_ds_to_auto_pc_RRESP),
        .s_axi_rvalid(auto_ds_to_auto_pc_RVALID),
        .s_axi_wdata(auto_ds_to_auto_pc_WDATA),
        .s_axi_wlast(auto_ds_to_auto_pc_WLAST),
        .s_axi_wready(auto_ds_to_auto_pc_WREADY),
        .s_axi_wstrb(auto_ds_to_auto_pc_WSTRB),
        .s_axi_wvalid(auto_ds_to_auto_pc_WVALID));
endmodule

module s00_couplers_imp_1FF9K4C
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arready,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [7:0]M_AXI_arlen;
  output M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [7:0]M_AXI_awlen;
  output M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [127:0]M_AXI_rdata;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [127:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [15:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  output S_AXI_arready;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [127:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [127:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [15:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire [31:0]s00_couplers_to_s00_couplers_ARADDR;
  wire [1:0]s00_couplers_to_s00_couplers_ARBURST;
  wire [3:0]s00_couplers_to_s00_couplers_ARCACHE;
  wire [7:0]s00_couplers_to_s00_couplers_ARLEN;
  wire s00_couplers_to_s00_couplers_ARLOCK;
  wire [2:0]s00_couplers_to_s00_couplers_ARPROT;
  wire s00_couplers_to_s00_couplers_ARREADY;
  wire [2:0]s00_couplers_to_s00_couplers_ARSIZE;
  wire s00_couplers_to_s00_couplers_ARVALID;
  wire [31:0]s00_couplers_to_s00_couplers_AWADDR;
  wire [1:0]s00_couplers_to_s00_couplers_AWBURST;
  wire [3:0]s00_couplers_to_s00_couplers_AWCACHE;
  wire [7:0]s00_couplers_to_s00_couplers_AWLEN;
  wire s00_couplers_to_s00_couplers_AWLOCK;
  wire [2:0]s00_couplers_to_s00_couplers_AWPROT;
  wire s00_couplers_to_s00_couplers_AWREADY;
  wire [2:0]s00_couplers_to_s00_couplers_AWSIZE;
  wire s00_couplers_to_s00_couplers_AWVALID;
  wire s00_couplers_to_s00_couplers_BREADY;
  wire [1:0]s00_couplers_to_s00_couplers_BRESP;
  wire s00_couplers_to_s00_couplers_BVALID;
  wire [127:0]s00_couplers_to_s00_couplers_RDATA;
  wire s00_couplers_to_s00_couplers_RLAST;
  wire s00_couplers_to_s00_couplers_RREADY;
  wire [1:0]s00_couplers_to_s00_couplers_RRESP;
  wire s00_couplers_to_s00_couplers_RVALID;
  wire [127:0]s00_couplers_to_s00_couplers_WDATA;
  wire s00_couplers_to_s00_couplers_WLAST;
  wire s00_couplers_to_s00_couplers_WREADY;
  wire [15:0]s00_couplers_to_s00_couplers_WSTRB;
  wire s00_couplers_to_s00_couplers_WVALID;

  assign M_AXI_araddr[31:0] = s00_couplers_to_s00_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = s00_couplers_to_s00_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = s00_couplers_to_s00_couplers_ARCACHE;
  assign M_AXI_arlen[7:0] = s00_couplers_to_s00_couplers_ARLEN;
  assign M_AXI_arlock = s00_couplers_to_s00_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = s00_couplers_to_s00_couplers_ARPROT;
  assign M_AXI_arsize[2:0] = s00_couplers_to_s00_couplers_ARSIZE;
  assign M_AXI_arvalid = s00_couplers_to_s00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = s00_couplers_to_s00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = s00_couplers_to_s00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = s00_couplers_to_s00_couplers_AWCACHE;
  assign M_AXI_awlen[7:0] = s00_couplers_to_s00_couplers_AWLEN;
  assign M_AXI_awlock = s00_couplers_to_s00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = s00_couplers_to_s00_couplers_AWPROT;
  assign M_AXI_awsize[2:0] = s00_couplers_to_s00_couplers_AWSIZE;
  assign M_AXI_awvalid = s00_couplers_to_s00_couplers_AWVALID;
  assign M_AXI_bready = s00_couplers_to_s00_couplers_BREADY;
  assign M_AXI_rready = s00_couplers_to_s00_couplers_RREADY;
  assign M_AXI_wdata[127:0] = s00_couplers_to_s00_couplers_WDATA;
  assign M_AXI_wlast = s00_couplers_to_s00_couplers_WLAST;
  assign M_AXI_wstrb[15:0] = s00_couplers_to_s00_couplers_WSTRB;
  assign M_AXI_wvalid = s00_couplers_to_s00_couplers_WVALID;
  assign S_AXI_arready = s00_couplers_to_s00_couplers_ARREADY;
  assign S_AXI_awready = s00_couplers_to_s00_couplers_AWREADY;
  assign S_AXI_bresp[1:0] = s00_couplers_to_s00_couplers_BRESP;
  assign S_AXI_bvalid = s00_couplers_to_s00_couplers_BVALID;
  assign S_AXI_rdata[127:0] = s00_couplers_to_s00_couplers_RDATA;
  assign S_AXI_rlast = s00_couplers_to_s00_couplers_RLAST;
  assign S_AXI_rresp[1:0] = s00_couplers_to_s00_couplers_RRESP;
  assign S_AXI_rvalid = s00_couplers_to_s00_couplers_RVALID;
  assign S_AXI_wready = s00_couplers_to_s00_couplers_WREADY;
  assign s00_couplers_to_s00_couplers_ARADDR = S_AXI_araddr[31:0];
  assign s00_couplers_to_s00_couplers_ARBURST = S_AXI_arburst[1:0];
  assign s00_couplers_to_s00_couplers_ARCACHE = S_AXI_arcache[3:0];
  assign s00_couplers_to_s00_couplers_ARLEN = S_AXI_arlen[7:0];
  assign s00_couplers_to_s00_couplers_ARLOCK = S_AXI_arlock;
  assign s00_couplers_to_s00_couplers_ARPROT = S_AXI_arprot[2:0];
  assign s00_couplers_to_s00_couplers_ARREADY = M_AXI_arready;
  assign s00_couplers_to_s00_couplers_ARSIZE = S_AXI_arsize[2:0];
  assign s00_couplers_to_s00_couplers_ARVALID = S_AXI_arvalid;
  assign s00_couplers_to_s00_couplers_AWADDR = S_AXI_awaddr[31:0];
  assign s00_couplers_to_s00_couplers_AWBURST = S_AXI_awburst[1:0];
  assign s00_couplers_to_s00_couplers_AWCACHE = S_AXI_awcache[3:0];
  assign s00_couplers_to_s00_couplers_AWLEN = S_AXI_awlen[7:0];
  assign s00_couplers_to_s00_couplers_AWLOCK = S_AXI_awlock;
  assign s00_couplers_to_s00_couplers_AWPROT = S_AXI_awprot[2:0];
  assign s00_couplers_to_s00_couplers_AWREADY = M_AXI_awready;
  assign s00_couplers_to_s00_couplers_AWSIZE = S_AXI_awsize[2:0];
  assign s00_couplers_to_s00_couplers_AWVALID = S_AXI_awvalid;
  assign s00_couplers_to_s00_couplers_BREADY = S_AXI_bready;
  assign s00_couplers_to_s00_couplers_BRESP = M_AXI_bresp[1:0];
  assign s00_couplers_to_s00_couplers_BVALID = M_AXI_bvalid;
  assign s00_couplers_to_s00_couplers_RDATA = M_AXI_rdata[127:0];
  assign s00_couplers_to_s00_couplers_RLAST = M_AXI_rlast;
  assign s00_couplers_to_s00_couplers_RREADY = S_AXI_rready;
  assign s00_couplers_to_s00_couplers_RRESP = M_AXI_rresp[1:0];
  assign s00_couplers_to_s00_couplers_RVALID = M_AXI_rvalid;
  assign s00_couplers_to_s00_couplers_WDATA = S_AXI_wdata[127:0];
  assign s00_couplers_to_s00_couplers_WLAST = S_AXI_wlast;
  assign s00_couplers_to_s00_couplers_WREADY = M_AXI_wready;
  assign s00_couplers_to_s00_couplers_WSTRB = S_AXI_wstrb[15:0];
  assign s00_couplers_to_s00_couplers_WVALID = S_AXI_wvalid;
endmodule

module s00_couplers_imp_1IKCXOO
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [7:0]M_AXI_arlen;
  output [0:0]M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [7:0]M_AXI_awlen;
  output [0:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [511:0]M_AXI_rdata;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [511:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [63:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [7:0]S_AXI_arlen;
  input [0:0]S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [7:0]S_AXI_awlen;
  input [0:0]S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [511:0]S_AXI_rdata;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [511:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire [31:0]s00_couplers_to_s00_data_fifo_ARADDR;
  wire [1:0]s00_couplers_to_s00_data_fifo_ARBURST;
  wire [3:0]s00_couplers_to_s00_data_fifo_ARCACHE;
  wire [7:0]s00_couplers_to_s00_data_fifo_ARLEN;
  wire [0:0]s00_couplers_to_s00_data_fifo_ARLOCK;
  wire [2:0]s00_couplers_to_s00_data_fifo_ARPROT;
  wire [3:0]s00_couplers_to_s00_data_fifo_ARQOS;
  wire s00_couplers_to_s00_data_fifo_ARREADY;
  wire [3:0]s00_couplers_to_s00_data_fifo_ARREGION;
  wire [2:0]s00_couplers_to_s00_data_fifo_ARSIZE;
  wire s00_couplers_to_s00_data_fifo_ARVALID;
  wire [31:0]s00_couplers_to_s00_data_fifo_AWADDR;
  wire [1:0]s00_couplers_to_s00_data_fifo_AWBURST;
  wire [3:0]s00_couplers_to_s00_data_fifo_AWCACHE;
  wire [7:0]s00_couplers_to_s00_data_fifo_AWLEN;
  wire [0:0]s00_couplers_to_s00_data_fifo_AWLOCK;
  wire [2:0]s00_couplers_to_s00_data_fifo_AWPROT;
  wire [3:0]s00_couplers_to_s00_data_fifo_AWQOS;
  wire s00_couplers_to_s00_data_fifo_AWREADY;
  wire [3:0]s00_couplers_to_s00_data_fifo_AWREGION;
  wire [2:0]s00_couplers_to_s00_data_fifo_AWSIZE;
  wire s00_couplers_to_s00_data_fifo_AWVALID;
  wire s00_couplers_to_s00_data_fifo_BREADY;
  wire [1:0]s00_couplers_to_s00_data_fifo_BRESP;
  wire s00_couplers_to_s00_data_fifo_BVALID;
  wire [511:0]s00_couplers_to_s00_data_fifo_RDATA;
  wire s00_couplers_to_s00_data_fifo_RLAST;
  wire s00_couplers_to_s00_data_fifo_RREADY;
  wire [1:0]s00_couplers_to_s00_data_fifo_RRESP;
  wire s00_couplers_to_s00_data_fifo_RVALID;
  wire [511:0]s00_couplers_to_s00_data_fifo_WDATA;
  wire s00_couplers_to_s00_data_fifo_WLAST;
  wire s00_couplers_to_s00_data_fifo_WREADY;
  wire [63:0]s00_couplers_to_s00_data_fifo_WSTRB;
  wire s00_couplers_to_s00_data_fifo_WVALID;
  wire [31:0]s00_data_fifo_to_s00_couplers_ARADDR;
  wire [1:0]s00_data_fifo_to_s00_couplers_ARBURST;
  wire [3:0]s00_data_fifo_to_s00_couplers_ARCACHE;
  wire [7:0]s00_data_fifo_to_s00_couplers_ARLEN;
  wire [0:0]s00_data_fifo_to_s00_couplers_ARLOCK;
  wire [2:0]s00_data_fifo_to_s00_couplers_ARPROT;
  wire [3:0]s00_data_fifo_to_s00_couplers_ARQOS;
  wire s00_data_fifo_to_s00_couplers_ARREADY;
  wire [2:0]s00_data_fifo_to_s00_couplers_ARSIZE;
  wire s00_data_fifo_to_s00_couplers_ARVALID;
  wire [31:0]s00_data_fifo_to_s00_couplers_AWADDR;
  wire [1:0]s00_data_fifo_to_s00_couplers_AWBURST;
  wire [3:0]s00_data_fifo_to_s00_couplers_AWCACHE;
  wire [7:0]s00_data_fifo_to_s00_couplers_AWLEN;
  wire [0:0]s00_data_fifo_to_s00_couplers_AWLOCK;
  wire [2:0]s00_data_fifo_to_s00_couplers_AWPROT;
  wire [3:0]s00_data_fifo_to_s00_couplers_AWQOS;
  wire s00_data_fifo_to_s00_couplers_AWREADY;
  wire [2:0]s00_data_fifo_to_s00_couplers_AWSIZE;
  wire s00_data_fifo_to_s00_couplers_AWVALID;
  wire s00_data_fifo_to_s00_couplers_BREADY;
  wire [1:0]s00_data_fifo_to_s00_couplers_BRESP;
  wire s00_data_fifo_to_s00_couplers_BVALID;
  wire [511:0]s00_data_fifo_to_s00_couplers_RDATA;
  wire s00_data_fifo_to_s00_couplers_RLAST;
  wire s00_data_fifo_to_s00_couplers_RREADY;
  wire [1:0]s00_data_fifo_to_s00_couplers_RRESP;
  wire s00_data_fifo_to_s00_couplers_RVALID;
  wire [511:0]s00_data_fifo_to_s00_couplers_WDATA;
  wire s00_data_fifo_to_s00_couplers_WLAST;
  wire s00_data_fifo_to_s00_couplers_WREADY;
  wire [63:0]s00_data_fifo_to_s00_couplers_WSTRB;
  wire s00_data_fifo_to_s00_couplers_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = s00_data_fifo_to_s00_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = s00_data_fifo_to_s00_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = s00_data_fifo_to_s00_couplers_ARCACHE;
  assign M_AXI_arlen[7:0] = s00_data_fifo_to_s00_couplers_ARLEN;
  assign M_AXI_arlock[0] = s00_data_fifo_to_s00_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = s00_data_fifo_to_s00_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = s00_data_fifo_to_s00_couplers_ARQOS;
  assign M_AXI_arsize[2:0] = s00_data_fifo_to_s00_couplers_ARSIZE;
  assign M_AXI_arvalid = s00_data_fifo_to_s00_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = s00_data_fifo_to_s00_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = s00_data_fifo_to_s00_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = s00_data_fifo_to_s00_couplers_AWCACHE;
  assign M_AXI_awlen[7:0] = s00_data_fifo_to_s00_couplers_AWLEN;
  assign M_AXI_awlock[0] = s00_data_fifo_to_s00_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = s00_data_fifo_to_s00_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = s00_data_fifo_to_s00_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = s00_data_fifo_to_s00_couplers_AWSIZE;
  assign M_AXI_awvalid = s00_data_fifo_to_s00_couplers_AWVALID;
  assign M_AXI_bready = s00_data_fifo_to_s00_couplers_BREADY;
  assign M_AXI_rready = s00_data_fifo_to_s00_couplers_RREADY;
  assign M_AXI_wdata[511:0] = s00_data_fifo_to_s00_couplers_WDATA;
  assign M_AXI_wlast = s00_data_fifo_to_s00_couplers_WLAST;
  assign M_AXI_wstrb[63:0] = s00_data_fifo_to_s00_couplers_WSTRB;
  assign M_AXI_wvalid = s00_data_fifo_to_s00_couplers_WVALID;
  assign S_AXI_arready = s00_couplers_to_s00_data_fifo_ARREADY;
  assign S_AXI_awready = s00_couplers_to_s00_data_fifo_AWREADY;
  assign S_AXI_bresp[1:0] = s00_couplers_to_s00_data_fifo_BRESP;
  assign S_AXI_bvalid = s00_couplers_to_s00_data_fifo_BVALID;
  assign S_AXI_rdata[511:0] = s00_couplers_to_s00_data_fifo_RDATA;
  assign S_AXI_rlast = s00_couplers_to_s00_data_fifo_RLAST;
  assign S_AXI_rresp[1:0] = s00_couplers_to_s00_data_fifo_RRESP;
  assign S_AXI_rvalid = s00_couplers_to_s00_data_fifo_RVALID;
  assign S_AXI_wready = s00_couplers_to_s00_data_fifo_WREADY;
  assign s00_couplers_to_s00_data_fifo_ARADDR = S_AXI_araddr[31:0];
  assign s00_couplers_to_s00_data_fifo_ARBURST = S_AXI_arburst[1:0];
  assign s00_couplers_to_s00_data_fifo_ARCACHE = S_AXI_arcache[3:0];
  assign s00_couplers_to_s00_data_fifo_ARLEN = S_AXI_arlen[7:0];
  assign s00_couplers_to_s00_data_fifo_ARLOCK = S_AXI_arlock[0];
  assign s00_couplers_to_s00_data_fifo_ARPROT = S_AXI_arprot[2:0];
  assign s00_couplers_to_s00_data_fifo_ARQOS = S_AXI_arqos[3:0];
  assign s00_couplers_to_s00_data_fifo_ARREGION = S_AXI_arregion[3:0];
  assign s00_couplers_to_s00_data_fifo_ARSIZE = S_AXI_arsize[2:0];
  assign s00_couplers_to_s00_data_fifo_ARVALID = S_AXI_arvalid;
  assign s00_couplers_to_s00_data_fifo_AWADDR = S_AXI_awaddr[31:0];
  assign s00_couplers_to_s00_data_fifo_AWBURST = S_AXI_awburst[1:0];
  assign s00_couplers_to_s00_data_fifo_AWCACHE = S_AXI_awcache[3:0];
  assign s00_couplers_to_s00_data_fifo_AWLEN = S_AXI_awlen[7:0];
  assign s00_couplers_to_s00_data_fifo_AWLOCK = S_AXI_awlock[0];
  assign s00_couplers_to_s00_data_fifo_AWPROT = S_AXI_awprot[2:0];
  assign s00_couplers_to_s00_data_fifo_AWQOS = S_AXI_awqos[3:0];
  assign s00_couplers_to_s00_data_fifo_AWREGION = S_AXI_awregion[3:0];
  assign s00_couplers_to_s00_data_fifo_AWSIZE = S_AXI_awsize[2:0];
  assign s00_couplers_to_s00_data_fifo_AWVALID = S_AXI_awvalid;
  assign s00_couplers_to_s00_data_fifo_BREADY = S_AXI_bready;
  assign s00_couplers_to_s00_data_fifo_RREADY = S_AXI_rready;
  assign s00_couplers_to_s00_data_fifo_WDATA = S_AXI_wdata[511:0];
  assign s00_couplers_to_s00_data_fifo_WLAST = S_AXI_wlast;
  assign s00_couplers_to_s00_data_fifo_WSTRB = S_AXI_wstrb[63:0];
  assign s00_couplers_to_s00_data_fifo_WVALID = S_AXI_wvalid;
  assign s00_data_fifo_to_s00_couplers_ARREADY = M_AXI_arready;
  assign s00_data_fifo_to_s00_couplers_AWREADY = M_AXI_awready;
  assign s00_data_fifo_to_s00_couplers_BRESP = M_AXI_bresp[1:0];
  assign s00_data_fifo_to_s00_couplers_BVALID = M_AXI_bvalid;
  assign s00_data_fifo_to_s00_couplers_RDATA = M_AXI_rdata[511:0];
  assign s00_data_fifo_to_s00_couplers_RLAST = M_AXI_rlast;
  assign s00_data_fifo_to_s00_couplers_RRESP = M_AXI_rresp[1:0];
  assign s00_data_fifo_to_s00_couplers_RVALID = M_AXI_rvalid;
  assign s00_data_fifo_to_s00_couplers_WREADY = M_AXI_wready;
  System_s00_data_fifo_0 s00_data_fifo
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(s00_data_fifo_to_s00_couplers_ARADDR),
        .m_axi_arburst(s00_data_fifo_to_s00_couplers_ARBURST),
        .m_axi_arcache(s00_data_fifo_to_s00_couplers_ARCACHE),
        .m_axi_arlen(s00_data_fifo_to_s00_couplers_ARLEN),
        .m_axi_arlock(s00_data_fifo_to_s00_couplers_ARLOCK),
        .m_axi_arprot(s00_data_fifo_to_s00_couplers_ARPROT),
        .m_axi_arqos(s00_data_fifo_to_s00_couplers_ARQOS),
        .m_axi_arready(s00_data_fifo_to_s00_couplers_ARREADY),
        .m_axi_arsize(s00_data_fifo_to_s00_couplers_ARSIZE),
        .m_axi_arvalid(s00_data_fifo_to_s00_couplers_ARVALID),
        .m_axi_awaddr(s00_data_fifo_to_s00_couplers_AWADDR),
        .m_axi_awburst(s00_data_fifo_to_s00_couplers_AWBURST),
        .m_axi_awcache(s00_data_fifo_to_s00_couplers_AWCACHE),
        .m_axi_awlen(s00_data_fifo_to_s00_couplers_AWLEN),
        .m_axi_awlock(s00_data_fifo_to_s00_couplers_AWLOCK),
        .m_axi_awprot(s00_data_fifo_to_s00_couplers_AWPROT),
        .m_axi_awqos(s00_data_fifo_to_s00_couplers_AWQOS),
        .m_axi_awready(s00_data_fifo_to_s00_couplers_AWREADY),
        .m_axi_awsize(s00_data_fifo_to_s00_couplers_AWSIZE),
        .m_axi_awvalid(s00_data_fifo_to_s00_couplers_AWVALID),
        .m_axi_bready(s00_data_fifo_to_s00_couplers_BREADY),
        .m_axi_bresp(s00_data_fifo_to_s00_couplers_BRESP),
        .m_axi_bvalid(s00_data_fifo_to_s00_couplers_BVALID),
        .m_axi_rdata(s00_data_fifo_to_s00_couplers_RDATA),
        .m_axi_rlast(s00_data_fifo_to_s00_couplers_RLAST),
        .m_axi_rready(s00_data_fifo_to_s00_couplers_RREADY),
        .m_axi_rresp(s00_data_fifo_to_s00_couplers_RRESP),
        .m_axi_rvalid(s00_data_fifo_to_s00_couplers_RVALID),
        .m_axi_wdata(s00_data_fifo_to_s00_couplers_WDATA),
        .m_axi_wlast(s00_data_fifo_to_s00_couplers_WLAST),
        .m_axi_wready(s00_data_fifo_to_s00_couplers_WREADY),
        .m_axi_wstrb(s00_data_fifo_to_s00_couplers_WSTRB),
        .m_axi_wvalid(s00_data_fifo_to_s00_couplers_WVALID),
        .s_axi_araddr(s00_couplers_to_s00_data_fifo_ARADDR),
        .s_axi_arburst(s00_couplers_to_s00_data_fifo_ARBURST),
        .s_axi_arcache(s00_couplers_to_s00_data_fifo_ARCACHE),
        .s_axi_arlen(s00_couplers_to_s00_data_fifo_ARLEN),
        .s_axi_arlock(s00_couplers_to_s00_data_fifo_ARLOCK),
        .s_axi_arprot(s00_couplers_to_s00_data_fifo_ARPROT),
        .s_axi_arqos(s00_couplers_to_s00_data_fifo_ARQOS),
        .s_axi_arready(s00_couplers_to_s00_data_fifo_ARREADY),
        .s_axi_arregion(s00_couplers_to_s00_data_fifo_ARREGION),
        .s_axi_arsize(s00_couplers_to_s00_data_fifo_ARSIZE),
        .s_axi_arvalid(s00_couplers_to_s00_data_fifo_ARVALID),
        .s_axi_awaddr(s00_couplers_to_s00_data_fifo_AWADDR),
        .s_axi_awburst(s00_couplers_to_s00_data_fifo_AWBURST),
        .s_axi_awcache(s00_couplers_to_s00_data_fifo_AWCACHE),
        .s_axi_awlen(s00_couplers_to_s00_data_fifo_AWLEN),
        .s_axi_awlock(s00_couplers_to_s00_data_fifo_AWLOCK),
        .s_axi_awprot(s00_couplers_to_s00_data_fifo_AWPROT),
        .s_axi_awqos(s00_couplers_to_s00_data_fifo_AWQOS),
        .s_axi_awready(s00_couplers_to_s00_data_fifo_AWREADY),
        .s_axi_awregion(s00_couplers_to_s00_data_fifo_AWREGION),
        .s_axi_awsize(s00_couplers_to_s00_data_fifo_AWSIZE),
        .s_axi_awvalid(s00_couplers_to_s00_data_fifo_AWVALID),
        .s_axi_bready(s00_couplers_to_s00_data_fifo_BREADY),
        .s_axi_bresp(s00_couplers_to_s00_data_fifo_BRESP),
        .s_axi_bvalid(s00_couplers_to_s00_data_fifo_BVALID),
        .s_axi_rdata(s00_couplers_to_s00_data_fifo_RDATA),
        .s_axi_rlast(s00_couplers_to_s00_data_fifo_RLAST),
        .s_axi_rready(s00_couplers_to_s00_data_fifo_RREADY),
        .s_axi_rresp(s00_couplers_to_s00_data_fifo_RRESP),
        .s_axi_rvalid(s00_couplers_to_s00_data_fifo_RVALID),
        .s_axi_wdata(s00_couplers_to_s00_data_fifo_WDATA),
        .s_axi_wlast(s00_couplers_to_s00_data_fifo_WLAST),
        .s_axi_wready(s00_couplers_to_s00_data_fifo_WREADY),
        .s_axi_wstrb(s00_couplers_to_s00_data_fifo_WSTRB),
        .s_axi_wvalid(s00_couplers_to_s00_data_fifo_WVALID));
endmodule

module s01_couplers_imp_TMBZ15
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arid,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awid,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rid,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arid,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arregion,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awid,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awregion,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rid,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [0:0]M_AXI_arid;
  output [7:0]M_AXI_arlen;
  output [0:0]M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [0:0]M_AXI_awid;
  output [7:0]M_AXI_awlen;
  output [0:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  input [2:0]M_AXI_bid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [511:0]M_AXI_rdata;
  input [2:0]M_AXI_rid;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [511:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [63:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [0:0]S_AXI_arid;
  input [7:0]S_AXI_arlen;
  input S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [3:0]S_AXI_arregion;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [0:0]S_AXI_awid;
  input [7:0]S_AXI_awlen;
  input S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [3:0]S_AXI_awregion;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  output [0:0]S_AXI_bid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [511:0]S_AXI_rdata;
  output [0:0]S_AXI_rid;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [511:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire [31:0]s01_couplers_to_s01_data_fifo_ARADDR;
  wire [1:0]s01_couplers_to_s01_data_fifo_ARBURST;
  wire [3:0]s01_couplers_to_s01_data_fifo_ARCACHE;
  wire [0:0]s01_couplers_to_s01_data_fifo_ARID;
  wire [7:0]s01_couplers_to_s01_data_fifo_ARLEN;
  wire s01_couplers_to_s01_data_fifo_ARLOCK;
  wire [2:0]s01_couplers_to_s01_data_fifo_ARPROT;
  wire [3:0]s01_couplers_to_s01_data_fifo_ARQOS;
  wire s01_couplers_to_s01_data_fifo_ARREADY;
  wire [3:0]s01_couplers_to_s01_data_fifo_ARREGION;
  wire [2:0]s01_couplers_to_s01_data_fifo_ARSIZE;
  wire s01_couplers_to_s01_data_fifo_ARVALID;
  wire [31:0]s01_couplers_to_s01_data_fifo_AWADDR;
  wire [1:0]s01_couplers_to_s01_data_fifo_AWBURST;
  wire [3:0]s01_couplers_to_s01_data_fifo_AWCACHE;
  wire [0:0]s01_couplers_to_s01_data_fifo_AWID;
  wire [7:0]s01_couplers_to_s01_data_fifo_AWLEN;
  wire s01_couplers_to_s01_data_fifo_AWLOCK;
  wire [2:0]s01_couplers_to_s01_data_fifo_AWPROT;
  wire [3:0]s01_couplers_to_s01_data_fifo_AWQOS;
  wire s01_couplers_to_s01_data_fifo_AWREADY;
  wire [3:0]s01_couplers_to_s01_data_fifo_AWREGION;
  wire [2:0]s01_couplers_to_s01_data_fifo_AWSIZE;
  wire s01_couplers_to_s01_data_fifo_AWVALID;
  wire [0:0]s01_couplers_to_s01_data_fifo_BID;
  wire s01_couplers_to_s01_data_fifo_BREADY;
  wire [1:0]s01_couplers_to_s01_data_fifo_BRESP;
  wire s01_couplers_to_s01_data_fifo_BVALID;
  wire [511:0]s01_couplers_to_s01_data_fifo_RDATA;
  wire [0:0]s01_couplers_to_s01_data_fifo_RID;
  wire s01_couplers_to_s01_data_fifo_RLAST;
  wire s01_couplers_to_s01_data_fifo_RREADY;
  wire [1:0]s01_couplers_to_s01_data_fifo_RRESP;
  wire s01_couplers_to_s01_data_fifo_RVALID;
  wire [511:0]s01_couplers_to_s01_data_fifo_WDATA;
  wire s01_couplers_to_s01_data_fifo_WLAST;
  wire s01_couplers_to_s01_data_fifo_WREADY;
  wire [63:0]s01_couplers_to_s01_data_fifo_WSTRB;
  wire s01_couplers_to_s01_data_fifo_WVALID;
  wire [31:0]s01_data_fifo_to_s01_couplers_ARADDR;
  wire [1:0]s01_data_fifo_to_s01_couplers_ARBURST;
  wire [3:0]s01_data_fifo_to_s01_couplers_ARCACHE;
  wire [0:0]s01_data_fifo_to_s01_couplers_ARID;
  wire [7:0]s01_data_fifo_to_s01_couplers_ARLEN;
  wire [0:0]s01_data_fifo_to_s01_couplers_ARLOCK;
  wire [2:0]s01_data_fifo_to_s01_couplers_ARPROT;
  wire [3:0]s01_data_fifo_to_s01_couplers_ARQOS;
  wire s01_data_fifo_to_s01_couplers_ARREADY;
  wire [2:0]s01_data_fifo_to_s01_couplers_ARSIZE;
  wire s01_data_fifo_to_s01_couplers_ARVALID;
  wire [31:0]s01_data_fifo_to_s01_couplers_AWADDR;
  wire [1:0]s01_data_fifo_to_s01_couplers_AWBURST;
  wire [3:0]s01_data_fifo_to_s01_couplers_AWCACHE;
  wire [0:0]s01_data_fifo_to_s01_couplers_AWID;
  wire [7:0]s01_data_fifo_to_s01_couplers_AWLEN;
  wire [0:0]s01_data_fifo_to_s01_couplers_AWLOCK;
  wire [2:0]s01_data_fifo_to_s01_couplers_AWPROT;
  wire [3:0]s01_data_fifo_to_s01_couplers_AWQOS;
  wire s01_data_fifo_to_s01_couplers_AWREADY;
  wire [2:0]s01_data_fifo_to_s01_couplers_AWSIZE;
  wire s01_data_fifo_to_s01_couplers_AWVALID;
  wire [2:0]s01_data_fifo_to_s01_couplers_BID;
  wire s01_data_fifo_to_s01_couplers_BREADY;
  wire [1:0]s01_data_fifo_to_s01_couplers_BRESP;
  wire s01_data_fifo_to_s01_couplers_BVALID;
  wire [511:0]s01_data_fifo_to_s01_couplers_RDATA;
  wire [2:0]s01_data_fifo_to_s01_couplers_RID;
  wire s01_data_fifo_to_s01_couplers_RLAST;
  wire s01_data_fifo_to_s01_couplers_RREADY;
  wire [1:0]s01_data_fifo_to_s01_couplers_RRESP;
  wire s01_data_fifo_to_s01_couplers_RVALID;
  wire [511:0]s01_data_fifo_to_s01_couplers_WDATA;
  wire s01_data_fifo_to_s01_couplers_WLAST;
  wire s01_data_fifo_to_s01_couplers_WREADY;
  wire [63:0]s01_data_fifo_to_s01_couplers_WSTRB;
  wire s01_data_fifo_to_s01_couplers_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = s01_data_fifo_to_s01_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = s01_data_fifo_to_s01_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = s01_data_fifo_to_s01_couplers_ARCACHE;
  assign M_AXI_arid[0] = s01_data_fifo_to_s01_couplers_ARID;
  assign M_AXI_arlen[7:0] = s01_data_fifo_to_s01_couplers_ARLEN;
  assign M_AXI_arlock[0] = s01_data_fifo_to_s01_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = s01_data_fifo_to_s01_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = s01_data_fifo_to_s01_couplers_ARQOS;
  assign M_AXI_arsize[2:0] = s01_data_fifo_to_s01_couplers_ARSIZE;
  assign M_AXI_arvalid = s01_data_fifo_to_s01_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = s01_data_fifo_to_s01_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = s01_data_fifo_to_s01_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = s01_data_fifo_to_s01_couplers_AWCACHE;
  assign M_AXI_awid[0] = s01_data_fifo_to_s01_couplers_AWID;
  assign M_AXI_awlen[7:0] = s01_data_fifo_to_s01_couplers_AWLEN;
  assign M_AXI_awlock[0] = s01_data_fifo_to_s01_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = s01_data_fifo_to_s01_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = s01_data_fifo_to_s01_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = s01_data_fifo_to_s01_couplers_AWSIZE;
  assign M_AXI_awvalid = s01_data_fifo_to_s01_couplers_AWVALID;
  assign M_AXI_bready = s01_data_fifo_to_s01_couplers_BREADY;
  assign M_AXI_rready = s01_data_fifo_to_s01_couplers_RREADY;
  assign M_AXI_wdata[511:0] = s01_data_fifo_to_s01_couplers_WDATA;
  assign M_AXI_wlast = s01_data_fifo_to_s01_couplers_WLAST;
  assign M_AXI_wstrb[63:0] = s01_data_fifo_to_s01_couplers_WSTRB;
  assign M_AXI_wvalid = s01_data_fifo_to_s01_couplers_WVALID;
  assign S_AXI_arready = s01_couplers_to_s01_data_fifo_ARREADY;
  assign S_AXI_awready = s01_couplers_to_s01_data_fifo_AWREADY;
  assign S_AXI_bid[0] = s01_couplers_to_s01_data_fifo_BID;
  assign S_AXI_bresp[1:0] = s01_couplers_to_s01_data_fifo_BRESP;
  assign S_AXI_bvalid = s01_couplers_to_s01_data_fifo_BVALID;
  assign S_AXI_rdata[511:0] = s01_couplers_to_s01_data_fifo_RDATA;
  assign S_AXI_rid[0] = s01_couplers_to_s01_data_fifo_RID;
  assign S_AXI_rlast = s01_couplers_to_s01_data_fifo_RLAST;
  assign S_AXI_rresp[1:0] = s01_couplers_to_s01_data_fifo_RRESP;
  assign S_AXI_rvalid = s01_couplers_to_s01_data_fifo_RVALID;
  assign S_AXI_wready = s01_couplers_to_s01_data_fifo_WREADY;
  assign s01_couplers_to_s01_data_fifo_ARADDR = S_AXI_araddr[31:0];
  assign s01_couplers_to_s01_data_fifo_ARBURST = S_AXI_arburst[1:0];
  assign s01_couplers_to_s01_data_fifo_ARCACHE = S_AXI_arcache[3:0];
  assign s01_couplers_to_s01_data_fifo_ARID = S_AXI_arid[0];
  assign s01_couplers_to_s01_data_fifo_ARLEN = S_AXI_arlen[7:0];
  assign s01_couplers_to_s01_data_fifo_ARLOCK = S_AXI_arlock;
  assign s01_couplers_to_s01_data_fifo_ARPROT = S_AXI_arprot[2:0];
  assign s01_couplers_to_s01_data_fifo_ARQOS = S_AXI_arqos[3:0];
  assign s01_couplers_to_s01_data_fifo_ARREGION = S_AXI_arregion[3:0];
  assign s01_couplers_to_s01_data_fifo_ARSIZE = S_AXI_arsize[2:0];
  assign s01_couplers_to_s01_data_fifo_ARVALID = S_AXI_arvalid;
  assign s01_couplers_to_s01_data_fifo_AWADDR = S_AXI_awaddr[31:0];
  assign s01_couplers_to_s01_data_fifo_AWBURST = S_AXI_awburst[1:0];
  assign s01_couplers_to_s01_data_fifo_AWCACHE = S_AXI_awcache[3:0];
  assign s01_couplers_to_s01_data_fifo_AWID = S_AXI_awid[0];
  assign s01_couplers_to_s01_data_fifo_AWLEN = S_AXI_awlen[7:0];
  assign s01_couplers_to_s01_data_fifo_AWLOCK = S_AXI_awlock;
  assign s01_couplers_to_s01_data_fifo_AWPROT = S_AXI_awprot[2:0];
  assign s01_couplers_to_s01_data_fifo_AWQOS = S_AXI_awqos[3:0];
  assign s01_couplers_to_s01_data_fifo_AWREGION = S_AXI_awregion[3:0];
  assign s01_couplers_to_s01_data_fifo_AWSIZE = S_AXI_awsize[2:0];
  assign s01_couplers_to_s01_data_fifo_AWVALID = S_AXI_awvalid;
  assign s01_couplers_to_s01_data_fifo_BREADY = S_AXI_bready;
  assign s01_couplers_to_s01_data_fifo_RREADY = S_AXI_rready;
  assign s01_couplers_to_s01_data_fifo_WDATA = S_AXI_wdata[511:0];
  assign s01_couplers_to_s01_data_fifo_WLAST = S_AXI_wlast;
  assign s01_couplers_to_s01_data_fifo_WSTRB = S_AXI_wstrb[63:0];
  assign s01_couplers_to_s01_data_fifo_WVALID = S_AXI_wvalid;
  assign s01_data_fifo_to_s01_couplers_ARREADY = M_AXI_arready;
  assign s01_data_fifo_to_s01_couplers_AWREADY = M_AXI_awready;
  assign s01_data_fifo_to_s01_couplers_BID = M_AXI_bid[2:0];
  assign s01_data_fifo_to_s01_couplers_BRESP = M_AXI_bresp[1:0];
  assign s01_data_fifo_to_s01_couplers_BVALID = M_AXI_bvalid;
  assign s01_data_fifo_to_s01_couplers_RDATA = M_AXI_rdata[511:0];
  assign s01_data_fifo_to_s01_couplers_RID = M_AXI_rid[2:0];
  assign s01_data_fifo_to_s01_couplers_RLAST = M_AXI_rlast;
  assign s01_data_fifo_to_s01_couplers_RRESP = M_AXI_rresp[1:0];
  assign s01_data_fifo_to_s01_couplers_RVALID = M_AXI_rvalid;
  assign s01_data_fifo_to_s01_couplers_WREADY = M_AXI_wready;
  System_s01_data_fifo_0 s01_data_fifo
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(s01_data_fifo_to_s01_couplers_ARADDR),
        .m_axi_arburst(s01_data_fifo_to_s01_couplers_ARBURST),
        .m_axi_arcache(s01_data_fifo_to_s01_couplers_ARCACHE),
        .m_axi_arid(s01_data_fifo_to_s01_couplers_ARID),
        .m_axi_arlen(s01_data_fifo_to_s01_couplers_ARLEN),
        .m_axi_arlock(s01_data_fifo_to_s01_couplers_ARLOCK),
        .m_axi_arprot(s01_data_fifo_to_s01_couplers_ARPROT),
        .m_axi_arqos(s01_data_fifo_to_s01_couplers_ARQOS),
        .m_axi_arready(s01_data_fifo_to_s01_couplers_ARREADY),
        .m_axi_arsize(s01_data_fifo_to_s01_couplers_ARSIZE),
        .m_axi_arvalid(s01_data_fifo_to_s01_couplers_ARVALID),
        .m_axi_awaddr(s01_data_fifo_to_s01_couplers_AWADDR),
        .m_axi_awburst(s01_data_fifo_to_s01_couplers_AWBURST),
        .m_axi_awcache(s01_data_fifo_to_s01_couplers_AWCACHE),
        .m_axi_awid(s01_data_fifo_to_s01_couplers_AWID),
        .m_axi_awlen(s01_data_fifo_to_s01_couplers_AWLEN),
        .m_axi_awlock(s01_data_fifo_to_s01_couplers_AWLOCK),
        .m_axi_awprot(s01_data_fifo_to_s01_couplers_AWPROT),
        .m_axi_awqos(s01_data_fifo_to_s01_couplers_AWQOS),
        .m_axi_awready(s01_data_fifo_to_s01_couplers_AWREADY),
        .m_axi_awsize(s01_data_fifo_to_s01_couplers_AWSIZE),
        .m_axi_awvalid(s01_data_fifo_to_s01_couplers_AWVALID),
        .m_axi_bid(s01_data_fifo_to_s01_couplers_BID[0]),
        .m_axi_bready(s01_data_fifo_to_s01_couplers_BREADY),
        .m_axi_bresp(s01_data_fifo_to_s01_couplers_BRESP),
        .m_axi_bvalid(s01_data_fifo_to_s01_couplers_BVALID),
        .m_axi_rdata(s01_data_fifo_to_s01_couplers_RDATA),
        .m_axi_rid(s01_data_fifo_to_s01_couplers_RID[0]),
        .m_axi_rlast(s01_data_fifo_to_s01_couplers_RLAST),
        .m_axi_rready(s01_data_fifo_to_s01_couplers_RREADY),
        .m_axi_rresp(s01_data_fifo_to_s01_couplers_RRESP),
        .m_axi_rvalid(s01_data_fifo_to_s01_couplers_RVALID),
        .m_axi_wdata(s01_data_fifo_to_s01_couplers_WDATA),
        .m_axi_wlast(s01_data_fifo_to_s01_couplers_WLAST),
        .m_axi_wready(s01_data_fifo_to_s01_couplers_WREADY),
        .m_axi_wstrb(s01_data_fifo_to_s01_couplers_WSTRB),
        .m_axi_wvalid(s01_data_fifo_to_s01_couplers_WVALID),
        .s_axi_araddr(s01_couplers_to_s01_data_fifo_ARADDR),
        .s_axi_arburst(s01_couplers_to_s01_data_fifo_ARBURST),
        .s_axi_arcache(s01_couplers_to_s01_data_fifo_ARCACHE),
        .s_axi_arid(s01_couplers_to_s01_data_fifo_ARID),
        .s_axi_arlen(s01_couplers_to_s01_data_fifo_ARLEN),
        .s_axi_arlock(s01_couplers_to_s01_data_fifo_ARLOCK),
        .s_axi_arprot(s01_couplers_to_s01_data_fifo_ARPROT),
        .s_axi_arqos(s01_couplers_to_s01_data_fifo_ARQOS),
        .s_axi_arready(s01_couplers_to_s01_data_fifo_ARREADY),
        .s_axi_arregion(s01_couplers_to_s01_data_fifo_ARREGION),
        .s_axi_arsize(s01_couplers_to_s01_data_fifo_ARSIZE),
        .s_axi_arvalid(s01_couplers_to_s01_data_fifo_ARVALID),
        .s_axi_awaddr(s01_couplers_to_s01_data_fifo_AWADDR),
        .s_axi_awburst(s01_couplers_to_s01_data_fifo_AWBURST),
        .s_axi_awcache(s01_couplers_to_s01_data_fifo_AWCACHE),
        .s_axi_awid(s01_couplers_to_s01_data_fifo_AWID),
        .s_axi_awlen(s01_couplers_to_s01_data_fifo_AWLEN),
        .s_axi_awlock(s01_couplers_to_s01_data_fifo_AWLOCK),
        .s_axi_awprot(s01_couplers_to_s01_data_fifo_AWPROT),
        .s_axi_awqos(s01_couplers_to_s01_data_fifo_AWQOS),
        .s_axi_awready(s01_couplers_to_s01_data_fifo_AWREADY),
        .s_axi_awregion(s01_couplers_to_s01_data_fifo_AWREGION),
        .s_axi_awsize(s01_couplers_to_s01_data_fifo_AWSIZE),
        .s_axi_awvalid(s01_couplers_to_s01_data_fifo_AWVALID),
        .s_axi_bid(s01_couplers_to_s01_data_fifo_BID),
        .s_axi_bready(s01_couplers_to_s01_data_fifo_BREADY),
        .s_axi_bresp(s01_couplers_to_s01_data_fifo_BRESP),
        .s_axi_bvalid(s01_couplers_to_s01_data_fifo_BVALID),
        .s_axi_rdata(s01_couplers_to_s01_data_fifo_RDATA),
        .s_axi_rid(s01_couplers_to_s01_data_fifo_RID),
        .s_axi_rlast(s01_couplers_to_s01_data_fifo_RLAST),
        .s_axi_rready(s01_couplers_to_s01_data_fifo_RREADY),
        .s_axi_rresp(s01_couplers_to_s01_data_fifo_RRESP),
        .s_axi_rvalid(s01_couplers_to_s01_data_fifo_RVALID),
        .s_axi_wdata(s01_couplers_to_s01_data_fifo_WDATA),
        .s_axi_wlast(s01_couplers_to_s01_data_fifo_WLAST),
        .s_axi_wready(s01_couplers_to_s01_data_fifo_WREADY),
        .s_axi_wstrb(s01_couplers_to_s01_data_fifo_WSTRB),
        .s_axi_wvalid(s01_couplers_to_s01_data_fifo_WVALID));
endmodule

module s02_couplers_imp_ILKESR
   (M_ACLK,
    M_ARESETN,
    M_AXI_araddr,
    M_AXI_arburst,
    M_AXI_arcache,
    M_AXI_arid,
    M_AXI_arlen,
    M_AXI_arlock,
    M_AXI_arprot,
    M_AXI_arqos,
    M_AXI_arready,
    M_AXI_arsize,
    M_AXI_arvalid,
    M_AXI_awaddr,
    M_AXI_awburst,
    M_AXI_awcache,
    M_AXI_awid,
    M_AXI_awlen,
    M_AXI_awlock,
    M_AXI_awprot,
    M_AXI_awqos,
    M_AXI_awready,
    M_AXI_awsize,
    M_AXI_awvalid,
    M_AXI_bid,
    M_AXI_bready,
    M_AXI_bresp,
    M_AXI_bvalid,
    M_AXI_rdata,
    M_AXI_rid,
    M_AXI_rlast,
    M_AXI_rready,
    M_AXI_rresp,
    M_AXI_rvalid,
    M_AXI_wdata,
    M_AXI_wlast,
    M_AXI_wready,
    M_AXI_wstrb,
    M_AXI_wvalid,
    S_ACLK,
    S_ARESETN,
    S_AXI_araddr,
    S_AXI_arburst,
    S_AXI_arcache,
    S_AXI_arid,
    S_AXI_arlen,
    S_AXI_arlock,
    S_AXI_arprot,
    S_AXI_arqos,
    S_AXI_arready,
    S_AXI_arsize,
    S_AXI_arvalid,
    S_AXI_awaddr,
    S_AXI_awburst,
    S_AXI_awcache,
    S_AXI_awid,
    S_AXI_awlen,
    S_AXI_awlock,
    S_AXI_awprot,
    S_AXI_awqos,
    S_AXI_awready,
    S_AXI_awsize,
    S_AXI_awvalid,
    S_AXI_bid,
    S_AXI_bready,
    S_AXI_bresp,
    S_AXI_bvalid,
    S_AXI_rdata,
    S_AXI_rid,
    S_AXI_rlast,
    S_AXI_rready,
    S_AXI_rresp,
    S_AXI_rvalid,
    S_AXI_wdata,
    S_AXI_wlast,
    S_AXI_wready,
    S_AXI_wstrb,
    S_AXI_wvalid);
  input M_ACLK;
  input M_ARESETN;
  output [31:0]M_AXI_araddr;
  output [1:0]M_AXI_arburst;
  output [3:0]M_AXI_arcache;
  output [0:0]M_AXI_arid;
  output [7:0]M_AXI_arlen;
  output [0:0]M_AXI_arlock;
  output [2:0]M_AXI_arprot;
  output [3:0]M_AXI_arqos;
  input M_AXI_arready;
  output [2:0]M_AXI_arsize;
  output M_AXI_arvalid;
  output [31:0]M_AXI_awaddr;
  output [1:0]M_AXI_awburst;
  output [3:0]M_AXI_awcache;
  output [0:0]M_AXI_awid;
  output [7:0]M_AXI_awlen;
  output [0:0]M_AXI_awlock;
  output [2:0]M_AXI_awprot;
  output [3:0]M_AXI_awqos;
  input M_AXI_awready;
  output [2:0]M_AXI_awsize;
  output M_AXI_awvalid;
  input [2:0]M_AXI_bid;
  output M_AXI_bready;
  input [1:0]M_AXI_bresp;
  input M_AXI_bvalid;
  input [511:0]M_AXI_rdata;
  input [2:0]M_AXI_rid;
  input M_AXI_rlast;
  output M_AXI_rready;
  input [1:0]M_AXI_rresp;
  input M_AXI_rvalid;
  output [511:0]M_AXI_wdata;
  output M_AXI_wlast;
  input M_AXI_wready;
  output [63:0]M_AXI_wstrb;
  output M_AXI_wvalid;
  input S_ACLK;
  input S_ARESETN;
  input [31:0]S_AXI_araddr;
  input [1:0]S_AXI_arburst;
  input [3:0]S_AXI_arcache;
  input [0:0]S_AXI_arid;
  input [7:0]S_AXI_arlen;
  input S_AXI_arlock;
  input [2:0]S_AXI_arprot;
  input [3:0]S_AXI_arqos;
  output S_AXI_arready;
  input [2:0]S_AXI_arsize;
  input S_AXI_arvalid;
  input [31:0]S_AXI_awaddr;
  input [1:0]S_AXI_awburst;
  input [3:0]S_AXI_awcache;
  input [0:0]S_AXI_awid;
  input [7:0]S_AXI_awlen;
  input S_AXI_awlock;
  input [2:0]S_AXI_awprot;
  input [3:0]S_AXI_awqos;
  output S_AXI_awready;
  input [2:0]S_AXI_awsize;
  input S_AXI_awvalid;
  output [0:0]S_AXI_bid;
  input S_AXI_bready;
  output [1:0]S_AXI_bresp;
  output S_AXI_bvalid;
  output [511:0]S_AXI_rdata;
  output [0:0]S_AXI_rid;
  output S_AXI_rlast;
  input S_AXI_rready;
  output [1:0]S_AXI_rresp;
  output S_AXI_rvalid;
  input [511:0]S_AXI_wdata;
  input S_AXI_wlast;
  output S_AXI_wready;
  input [63:0]S_AXI_wstrb;
  input S_AXI_wvalid;

  wire M_ACLK_1;
  wire M_ARESETN_1;
  wire [31:0]s02_couplers_to_s02_data_fifo_ARADDR;
  wire [1:0]s02_couplers_to_s02_data_fifo_ARBURST;
  wire [3:0]s02_couplers_to_s02_data_fifo_ARCACHE;
  wire [0:0]s02_couplers_to_s02_data_fifo_ARID;
  wire [7:0]s02_couplers_to_s02_data_fifo_ARLEN;
  wire s02_couplers_to_s02_data_fifo_ARLOCK;
  wire [2:0]s02_couplers_to_s02_data_fifo_ARPROT;
  wire [3:0]s02_couplers_to_s02_data_fifo_ARQOS;
  wire s02_couplers_to_s02_data_fifo_ARREADY;
  wire [2:0]s02_couplers_to_s02_data_fifo_ARSIZE;
  wire s02_couplers_to_s02_data_fifo_ARVALID;
  wire [31:0]s02_couplers_to_s02_data_fifo_AWADDR;
  wire [1:0]s02_couplers_to_s02_data_fifo_AWBURST;
  wire [3:0]s02_couplers_to_s02_data_fifo_AWCACHE;
  wire [0:0]s02_couplers_to_s02_data_fifo_AWID;
  wire [7:0]s02_couplers_to_s02_data_fifo_AWLEN;
  wire s02_couplers_to_s02_data_fifo_AWLOCK;
  wire [2:0]s02_couplers_to_s02_data_fifo_AWPROT;
  wire [3:0]s02_couplers_to_s02_data_fifo_AWQOS;
  wire s02_couplers_to_s02_data_fifo_AWREADY;
  wire [2:0]s02_couplers_to_s02_data_fifo_AWSIZE;
  wire s02_couplers_to_s02_data_fifo_AWVALID;
  wire [0:0]s02_couplers_to_s02_data_fifo_BID;
  wire s02_couplers_to_s02_data_fifo_BREADY;
  wire [1:0]s02_couplers_to_s02_data_fifo_BRESP;
  wire s02_couplers_to_s02_data_fifo_BVALID;
  wire [511:0]s02_couplers_to_s02_data_fifo_RDATA;
  wire [0:0]s02_couplers_to_s02_data_fifo_RID;
  wire s02_couplers_to_s02_data_fifo_RLAST;
  wire s02_couplers_to_s02_data_fifo_RREADY;
  wire [1:0]s02_couplers_to_s02_data_fifo_RRESP;
  wire s02_couplers_to_s02_data_fifo_RVALID;
  wire [511:0]s02_couplers_to_s02_data_fifo_WDATA;
  wire s02_couplers_to_s02_data_fifo_WLAST;
  wire s02_couplers_to_s02_data_fifo_WREADY;
  wire [63:0]s02_couplers_to_s02_data_fifo_WSTRB;
  wire s02_couplers_to_s02_data_fifo_WVALID;
  wire [31:0]s02_data_fifo_to_s02_couplers_ARADDR;
  wire [1:0]s02_data_fifo_to_s02_couplers_ARBURST;
  wire [3:0]s02_data_fifo_to_s02_couplers_ARCACHE;
  wire [0:0]s02_data_fifo_to_s02_couplers_ARID;
  wire [7:0]s02_data_fifo_to_s02_couplers_ARLEN;
  wire [0:0]s02_data_fifo_to_s02_couplers_ARLOCK;
  wire [2:0]s02_data_fifo_to_s02_couplers_ARPROT;
  wire [3:0]s02_data_fifo_to_s02_couplers_ARQOS;
  wire s02_data_fifo_to_s02_couplers_ARREADY;
  wire [2:0]s02_data_fifo_to_s02_couplers_ARSIZE;
  wire s02_data_fifo_to_s02_couplers_ARVALID;
  wire [31:0]s02_data_fifo_to_s02_couplers_AWADDR;
  wire [1:0]s02_data_fifo_to_s02_couplers_AWBURST;
  wire [3:0]s02_data_fifo_to_s02_couplers_AWCACHE;
  wire [0:0]s02_data_fifo_to_s02_couplers_AWID;
  wire [7:0]s02_data_fifo_to_s02_couplers_AWLEN;
  wire [0:0]s02_data_fifo_to_s02_couplers_AWLOCK;
  wire [2:0]s02_data_fifo_to_s02_couplers_AWPROT;
  wire [3:0]s02_data_fifo_to_s02_couplers_AWQOS;
  wire s02_data_fifo_to_s02_couplers_AWREADY;
  wire [2:0]s02_data_fifo_to_s02_couplers_AWSIZE;
  wire s02_data_fifo_to_s02_couplers_AWVALID;
  wire [2:0]s02_data_fifo_to_s02_couplers_BID;
  wire s02_data_fifo_to_s02_couplers_BREADY;
  wire [1:0]s02_data_fifo_to_s02_couplers_BRESP;
  wire s02_data_fifo_to_s02_couplers_BVALID;
  wire [511:0]s02_data_fifo_to_s02_couplers_RDATA;
  wire [2:0]s02_data_fifo_to_s02_couplers_RID;
  wire s02_data_fifo_to_s02_couplers_RLAST;
  wire s02_data_fifo_to_s02_couplers_RREADY;
  wire [1:0]s02_data_fifo_to_s02_couplers_RRESP;
  wire s02_data_fifo_to_s02_couplers_RVALID;
  wire [511:0]s02_data_fifo_to_s02_couplers_WDATA;
  wire s02_data_fifo_to_s02_couplers_WLAST;
  wire s02_data_fifo_to_s02_couplers_WREADY;
  wire [63:0]s02_data_fifo_to_s02_couplers_WSTRB;
  wire s02_data_fifo_to_s02_couplers_WVALID;

  assign M_ACLK_1 = M_ACLK;
  assign M_ARESETN_1 = M_ARESETN;
  assign M_AXI_araddr[31:0] = s02_data_fifo_to_s02_couplers_ARADDR;
  assign M_AXI_arburst[1:0] = s02_data_fifo_to_s02_couplers_ARBURST;
  assign M_AXI_arcache[3:0] = s02_data_fifo_to_s02_couplers_ARCACHE;
  assign M_AXI_arid[0] = s02_data_fifo_to_s02_couplers_ARID;
  assign M_AXI_arlen[7:0] = s02_data_fifo_to_s02_couplers_ARLEN;
  assign M_AXI_arlock[0] = s02_data_fifo_to_s02_couplers_ARLOCK;
  assign M_AXI_arprot[2:0] = s02_data_fifo_to_s02_couplers_ARPROT;
  assign M_AXI_arqos[3:0] = s02_data_fifo_to_s02_couplers_ARQOS;
  assign M_AXI_arsize[2:0] = s02_data_fifo_to_s02_couplers_ARSIZE;
  assign M_AXI_arvalid = s02_data_fifo_to_s02_couplers_ARVALID;
  assign M_AXI_awaddr[31:0] = s02_data_fifo_to_s02_couplers_AWADDR;
  assign M_AXI_awburst[1:0] = s02_data_fifo_to_s02_couplers_AWBURST;
  assign M_AXI_awcache[3:0] = s02_data_fifo_to_s02_couplers_AWCACHE;
  assign M_AXI_awid[0] = s02_data_fifo_to_s02_couplers_AWID;
  assign M_AXI_awlen[7:0] = s02_data_fifo_to_s02_couplers_AWLEN;
  assign M_AXI_awlock[0] = s02_data_fifo_to_s02_couplers_AWLOCK;
  assign M_AXI_awprot[2:0] = s02_data_fifo_to_s02_couplers_AWPROT;
  assign M_AXI_awqos[3:0] = s02_data_fifo_to_s02_couplers_AWQOS;
  assign M_AXI_awsize[2:0] = s02_data_fifo_to_s02_couplers_AWSIZE;
  assign M_AXI_awvalid = s02_data_fifo_to_s02_couplers_AWVALID;
  assign M_AXI_bready = s02_data_fifo_to_s02_couplers_BREADY;
  assign M_AXI_rready = s02_data_fifo_to_s02_couplers_RREADY;
  assign M_AXI_wdata[511:0] = s02_data_fifo_to_s02_couplers_WDATA;
  assign M_AXI_wlast = s02_data_fifo_to_s02_couplers_WLAST;
  assign M_AXI_wstrb[63:0] = s02_data_fifo_to_s02_couplers_WSTRB;
  assign M_AXI_wvalid = s02_data_fifo_to_s02_couplers_WVALID;
  assign S_AXI_arready = s02_couplers_to_s02_data_fifo_ARREADY;
  assign S_AXI_awready = s02_couplers_to_s02_data_fifo_AWREADY;
  assign S_AXI_bid[0] = s02_couplers_to_s02_data_fifo_BID;
  assign S_AXI_bresp[1:0] = s02_couplers_to_s02_data_fifo_BRESP;
  assign S_AXI_bvalid = s02_couplers_to_s02_data_fifo_BVALID;
  assign S_AXI_rdata[511:0] = s02_couplers_to_s02_data_fifo_RDATA;
  assign S_AXI_rid[0] = s02_couplers_to_s02_data_fifo_RID;
  assign S_AXI_rlast = s02_couplers_to_s02_data_fifo_RLAST;
  assign S_AXI_rresp[1:0] = s02_couplers_to_s02_data_fifo_RRESP;
  assign S_AXI_rvalid = s02_couplers_to_s02_data_fifo_RVALID;
  assign S_AXI_wready = s02_couplers_to_s02_data_fifo_WREADY;
  assign s02_couplers_to_s02_data_fifo_ARADDR = S_AXI_araddr[31:0];
  assign s02_couplers_to_s02_data_fifo_ARBURST = S_AXI_arburst[1:0];
  assign s02_couplers_to_s02_data_fifo_ARCACHE = S_AXI_arcache[3:0];
  assign s02_couplers_to_s02_data_fifo_ARID = S_AXI_arid[0];
  assign s02_couplers_to_s02_data_fifo_ARLEN = S_AXI_arlen[7:0];
  assign s02_couplers_to_s02_data_fifo_ARLOCK = S_AXI_arlock;
  assign s02_couplers_to_s02_data_fifo_ARPROT = S_AXI_arprot[2:0];
  assign s02_couplers_to_s02_data_fifo_ARQOS = S_AXI_arqos[3:0];
  assign s02_couplers_to_s02_data_fifo_ARSIZE = S_AXI_arsize[2:0];
  assign s02_couplers_to_s02_data_fifo_ARVALID = S_AXI_arvalid;
  assign s02_couplers_to_s02_data_fifo_AWADDR = S_AXI_awaddr[31:0];
  assign s02_couplers_to_s02_data_fifo_AWBURST = S_AXI_awburst[1:0];
  assign s02_couplers_to_s02_data_fifo_AWCACHE = S_AXI_awcache[3:0];
  assign s02_couplers_to_s02_data_fifo_AWID = S_AXI_awid[0];
  assign s02_couplers_to_s02_data_fifo_AWLEN = S_AXI_awlen[7:0];
  assign s02_couplers_to_s02_data_fifo_AWLOCK = S_AXI_awlock;
  assign s02_couplers_to_s02_data_fifo_AWPROT = S_AXI_awprot[2:0];
  assign s02_couplers_to_s02_data_fifo_AWQOS = S_AXI_awqos[3:0];
  assign s02_couplers_to_s02_data_fifo_AWSIZE = S_AXI_awsize[2:0];
  assign s02_couplers_to_s02_data_fifo_AWVALID = S_AXI_awvalid;
  assign s02_couplers_to_s02_data_fifo_BREADY = S_AXI_bready;
  assign s02_couplers_to_s02_data_fifo_RREADY = S_AXI_rready;
  assign s02_couplers_to_s02_data_fifo_WDATA = S_AXI_wdata[511:0];
  assign s02_couplers_to_s02_data_fifo_WLAST = S_AXI_wlast;
  assign s02_couplers_to_s02_data_fifo_WSTRB = S_AXI_wstrb[63:0];
  assign s02_couplers_to_s02_data_fifo_WVALID = S_AXI_wvalid;
  assign s02_data_fifo_to_s02_couplers_ARREADY = M_AXI_arready;
  assign s02_data_fifo_to_s02_couplers_AWREADY = M_AXI_awready;
  assign s02_data_fifo_to_s02_couplers_BID = M_AXI_bid[2:0];
  assign s02_data_fifo_to_s02_couplers_BRESP = M_AXI_bresp[1:0];
  assign s02_data_fifo_to_s02_couplers_BVALID = M_AXI_bvalid;
  assign s02_data_fifo_to_s02_couplers_RDATA = M_AXI_rdata[511:0];
  assign s02_data_fifo_to_s02_couplers_RID = M_AXI_rid[2:0];
  assign s02_data_fifo_to_s02_couplers_RLAST = M_AXI_rlast;
  assign s02_data_fifo_to_s02_couplers_RRESP = M_AXI_rresp[1:0];
  assign s02_data_fifo_to_s02_couplers_RVALID = M_AXI_rvalid;
  assign s02_data_fifo_to_s02_couplers_WREADY = M_AXI_wready;
  System_s02_data_fifo_0 s02_data_fifo
       (.aclk(M_ACLK_1),
        .aresetn(M_ARESETN_1),
        .m_axi_araddr(s02_data_fifo_to_s02_couplers_ARADDR),
        .m_axi_arburst(s02_data_fifo_to_s02_couplers_ARBURST),
        .m_axi_arcache(s02_data_fifo_to_s02_couplers_ARCACHE),
        .m_axi_arid(s02_data_fifo_to_s02_couplers_ARID),
        .m_axi_arlen(s02_data_fifo_to_s02_couplers_ARLEN),
        .m_axi_arlock(s02_data_fifo_to_s02_couplers_ARLOCK),
        .m_axi_arprot(s02_data_fifo_to_s02_couplers_ARPROT),
        .m_axi_arqos(s02_data_fifo_to_s02_couplers_ARQOS),
        .m_axi_arready(s02_data_fifo_to_s02_couplers_ARREADY),
        .m_axi_arsize(s02_data_fifo_to_s02_couplers_ARSIZE),
        .m_axi_arvalid(s02_data_fifo_to_s02_couplers_ARVALID),
        .m_axi_awaddr(s02_data_fifo_to_s02_couplers_AWADDR),
        .m_axi_awburst(s02_data_fifo_to_s02_couplers_AWBURST),
        .m_axi_awcache(s02_data_fifo_to_s02_couplers_AWCACHE),
        .m_axi_awid(s02_data_fifo_to_s02_couplers_AWID),
        .m_axi_awlen(s02_data_fifo_to_s02_couplers_AWLEN),
        .m_axi_awlock(s02_data_fifo_to_s02_couplers_AWLOCK),
        .m_axi_awprot(s02_data_fifo_to_s02_couplers_AWPROT),
        .m_axi_awqos(s02_data_fifo_to_s02_couplers_AWQOS),
        .m_axi_awready(s02_data_fifo_to_s02_couplers_AWREADY),
        .m_axi_awsize(s02_data_fifo_to_s02_couplers_AWSIZE),
        .m_axi_awvalid(s02_data_fifo_to_s02_couplers_AWVALID),
        .m_axi_bid(s02_data_fifo_to_s02_couplers_BID[0]),
        .m_axi_bready(s02_data_fifo_to_s02_couplers_BREADY),
        .m_axi_bresp(s02_data_fifo_to_s02_couplers_BRESP),
        .m_axi_bvalid(s02_data_fifo_to_s02_couplers_BVALID),
        .m_axi_rdata(s02_data_fifo_to_s02_couplers_RDATA),
        .m_axi_rid(s02_data_fifo_to_s02_couplers_RID[0]),
        .m_axi_rlast(s02_data_fifo_to_s02_couplers_RLAST),
        .m_axi_rready(s02_data_fifo_to_s02_couplers_RREADY),
        .m_axi_rresp(s02_data_fifo_to_s02_couplers_RRESP),
        .m_axi_rvalid(s02_data_fifo_to_s02_couplers_RVALID),
        .m_axi_wdata(s02_data_fifo_to_s02_couplers_WDATA),
        .m_axi_wlast(s02_data_fifo_to_s02_couplers_WLAST),
        .m_axi_wready(s02_data_fifo_to_s02_couplers_WREADY),
        .m_axi_wstrb(s02_data_fifo_to_s02_couplers_WSTRB),
        .m_axi_wvalid(s02_data_fifo_to_s02_couplers_WVALID),
        .s_axi_araddr(s02_couplers_to_s02_data_fifo_ARADDR),
        .s_axi_arburst(s02_couplers_to_s02_data_fifo_ARBURST),
        .s_axi_arcache(s02_couplers_to_s02_data_fifo_ARCACHE),
        .s_axi_arid(s02_couplers_to_s02_data_fifo_ARID),
        .s_axi_arlen(s02_couplers_to_s02_data_fifo_ARLEN),
        .s_axi_arlock(s02_couplers_to_s02_data_fifo_ARLOCK),
        .s_axi_arprot(s02_couplers_to_s02_data_fifo_ARPROT),
        .s_axi_arqos(s02_couplers_to_s02_data_fifo_ARQOS),
        .s_axi_arready(s02_couplers_to_s02_data_fifo_ARREADY),
        .s_axi_arregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arsize(s02_couplers_to_s02_data_fifo_ARSIZE),
        .s_axi_arvalid(s02_couplers_to_s02_data_fifo_ARVALID),
        .s_axi_awaddr(s02_couplers_to_s02_data_fifo_AWADDR),
        .s_axi_awburst(s02_couplers_to_s02_data_fifo_AWBURST),
        .s_axi_awcache(s02_couplers_to_s02_data_fifo_AWCACHE),
        .s_axi_awid(s02_couplers_to_s02_data_fifo_AWID),
        .s_axi_awlen(s02_couplers_to_s02_data_fifo_AWLEN),
        .s_axi_awlock(s02_couplers_to_s02_data_fifo_AWLOCK),
        .s_axi_awprot(s02_couplers_to_s02_data_fifo_AWPROT),
        .s_axi_awqos(s02_couplers_to_s02_data_fifo_AWQOS),
        .s_axi_awready(s02_couplers_to_s02_data_fifo_AWREADY),
        .s_axi_awregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awsize(s02_couplers_to_s02_data_fifo_AWSIZE),
        .s_axi_awvalid(s02_couplers_to_s02_data_fifo_AWVALID),
        .s_axi_bid(s02_couplers_to_s02_data_fifo_BID),
        .s_axi_bready(s02_couplers_to_s02_data_fifo_BREADY),
        .s_axi_bresp(s02_couplers_to_s02_data_fifo_BRESP),
        .s_axi_bvalid(s02_couplers_to_s02_data_fifo_BVALID),
        .s_axi_rdata(s02_couplers_to_s02_data_fifo_RDATA),
        .s_axi_rid(s02_couplers_to_s02_data_fifo_RID),
        .s_axi_rlast(s02_couplers_to_s02_data_fifo_RLAST),
        .s_axi_rready(s02_couplers_to_s02_data_fifo_RREADY),
        .s_axi_rresp(s02_couplers_to_s02_data_fifo_RRESP),
        .s_axi_rvalid(s02_couplers_to_s02_data_fifo_RVALID),
        .s_axi_wdata(s02_couplers_to_s02_data_fifo_WDATA),
        .s_axi_wlast(s02_couplers_to_s02_data_fifo_WLAST),
        .s_axi_wready(s02_couplers_to_s02_data_fifo_WREADY),
        .s_axi_wstrb(s02_couplers_to_s02_data_fifo_WSTRB),
        .s_axi_wvalid(s02_couplers_to_s02_data_fifo_WVALID));
endmodule
