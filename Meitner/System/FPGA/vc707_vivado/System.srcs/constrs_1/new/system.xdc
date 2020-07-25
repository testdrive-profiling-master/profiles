###############################################################################
# User Physical Constraints
###############################################################################
## for fast BPI configuration
set_property BITSTREAM.CONFIG.BPI_SYNC_MODE Type1 [current_design]
set_property BITSTREAM.CONFIG.EXTMASTERCCLK_EN div-1 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property BITSTREAM.CONFIG.UNUSEDPIN Pulldown [current_design]
set_property CONFIG_MODE BPI16 [current_design]
set_property CFGBVS GND [current_design]
set_property CONFIG_VOLTAGE 1.8 [current_design]

set_property IOSTANDARD LVDS [get_ports sys_clk_clk_p]
set_property PACKAGE_PIN E19 [get_ports sys_clk_clk_p]
set_property IOSTANDARD LVDS [get_ports sys_clk_clk_n]
set_property PACKAGE_PIN E18 [get_ports sys_clk_clk_n]

set_property IOSTANDARD LVCMOS18 [get_ports sys_rst]
set_property PACKAGE_PIN AV40 [get_ports sys_rst]
set_false_path -from [get_ports sys_rst]

###############################################################################
# MSI generator
###############################################################################
set_false_path -from [get_pins System_i/dut_wrapper_0/inst/processor_inst/clk_gen_slave/INTR_reg/C] -to [get_pins {System_i/MSI_Generator_0/inst/msi_gen/intr_gen[1].intr_pipe/intr_pipe_reg[0]/D}]

###############################################################################
# HDMI controller
###############################################################################
set_property PACKAGE_PIN AU23 [get_ports HDMI_CLK]
set_property PACKAGE_PIN AT22 [get_ports HDMI_VSYNC]
set_property PACKAGE_PIN AU22 [get_ports HDMI_HSYNC]
set_property PACKAGE_PIN AP21 [get_ports HDMI_DE]
set_property PACKAGE_PIN AM22 [get_ports HDMI_DATA[0] ]
set_property PACKAGE_PIN AL22 [get_ports HDMI_DATA[1] ]
set_property PACKAGE_PIN AJ20 [get_ports HDMI_DATA[2] ]
set_property PACKAGE_PIN AJ21 [get_ports HDMI_DATA[3] ]
set_property PACKAGE_PIN AM21 [get_ports HDMI_DATA[4] ]
set_property PACKAGE_PIN AL21 [get_ports HDMI_DATA[5] ]
set_property PACKAGE_PIN AK22 [get_ports HDMI_DATA[6] ]
set_property PACKAGE_PIN AJ22 [get_ports HDMI_DATA[7] ]
set_property PACKAGE_PIN AL20 [get_ports HDMI_DATA[8] ]
set_property PACKAGE_PIN AK20 [get_ports HDMI_DATA[9] ]
set_property PACKAGE_PIN AK23 [get_ports HDMI_DATA[10]]
set_property PACKAGE_PIN AJ23 [get_ports HDMI_DATA[11]]
set_property PACKAGE_PIN AN21 [get_ports HDMI_DATA[12]]
set_property PACKAGE_PIN AP22 [get_ports HDMI_DATA[13]]
set_property PACKAGE_PIN AP23 [get_ports HDMI_DATA[14]]
set_property PACKAGE_PIN AN23 [get_ports HDMI_DATA[15]]
set_property PACKAGE_PIN AM23 [get_ports HDMI_DATA[16]]
set_property PACKAGE_PIN AN24 [get_ports HDMI_DATA[17]]
set_property PACKAGE_PIN AY24 [get_ports HDMI_DATA[18]]
set_property PACKAGE_PIN BB22 [get_ports HDMI_DATA[19]]
set_property PACKAGE_PIN BA22 [get_ports HDMI_DATA[20]]
set_property PACKAGE_PIN BA25 [get_ports HDMI_DATA[21]]
set_property PACKAGE_PIN AY25 [get_ports HDMI_DATA[22]]
set_property PACKAGE_PIN AY22 [get_ports HDMI_DATA[23]]
set_property PACKAGE_PIN AY23 [get_ports HDMI_DATA[24]]
set_property PACKAGE_PIN AV24 [get_ports HDMI_DATA[25]]
set_property PACKAGE_PIN AU24 [get_ports HDMI_DATA[26]]
set_property PACKAGE_PIN AW21 [get_ports HDMI_DATA[27]]
set_property PACKAGE_PIN AV21 [get_ports HDMI_DATA[28]]
set_property PACKAGE_PIN AT24 [get_ports HDMI_DATA[29]]
set_property PACKAGE_PIN AR24 [get_ports HDMI_DATA[30]]
set_property PACKAGE_PIN AU21 [get_ports HDMI_DATA[31]]
set_property PACKAGE_PIN AT21 [get_ports HDMI_DATA[32]]
set_property PACKAGE_PIN AW22 [get_ports HDMI_DATA[33]]
set_property PACKAGE_PIN AW23 [get_ports HDMI_DATA[34]]
set_property PACKAGE_PIN AV23 [get_ports HDMI_DATA[35]]
set_property PACKAGE_PIN AR23 [get_ports HDMI_SPDIF]
set_property PACKAGE_PIN AM24 [get_ports HDMI_INTR]
set_property IOSTANDARD LVCMOS18 [get_ports -filter {NAME =~  "HDMI_*"}]
set_false_path -from [get_ports HDMI_INTR]
set_property slew "fast" [get_ports HDMI_CLK]

## I2C
set_property PACKAGE_PIN AY42 [get_ports I2C_nRST]
set_property PACKAGE_PIN AT35 [get_ports I2C_SCL]
set_property PACKAGE_PIN AU32 [get_ports I2C_SDA]
set_property IOSTANDARD LVCMOS18 [get_ports -filter {NAME =~  "I2C_*"}]
##set_property PULLUP "TRUE" [get_ports -filter {NAME =~  "I2C_S*"}]
##set_property PIO_DIRECTION "BIDIR" [get_ports -filter {NAME =~  "I2C_S*"}]
##set_property drive "8" [get_ports -filter {NAME =~  "I2C_S*"}]
##set_property slew "slow" [get_ports -filter {NAME =~  "I2C_S*"}]
set_false_path -from [get_ports -filter {NAME =~  "I2C_S*"}]
set_false_path -to   [get_ports -filter {NAME =~  "I2C_*"}]

## inter async. clock path
create_clock -name hdmi_aclk -period 5.0 [get_nets System_i/hdmi_controller_0/ACLK]
create_clock -name hdmi_pclk -period 5.0 [get_nets System_i/HDMI_CLK]
set_false_path -from [get_clocks hdmi_aclk] -to [get_clocks hdmi_pclk]
set_false_path -from [get_clocks hdmi_pclk] -to [get_clocks hdmi_aclk]
set_false_path -from [get_clocks hdmi_aclk] -to [get_cells -hierarchical -filter {NAME =~  "*/HDMI_CLK_ODDR"}]

## for processor
create_clock -name proc_clk -period 5.0 [get_pins -hierarchical -filter {NAME =~  "*testdrive_clock_gen/mmcm_inst/CLKOUT0"}]
set_false_path -from [get_clocks hdmi_aclk] -to [get_clocks proc_clk]
set_false_path -from [get_clocks proc_clk] -to [get_clocks hdmi_aclk]
set_false_path -from [get_pins -hierarchical -filter {NAME =~  "*testdrive_clock_gen/mmcm_inst/LOCKED"}]

# multicycle path #2
set_multicycle_path -hold   1 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_2.o_data_reg[*]"}]
set_multicycle_path -setup  2 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_2.o_data_reg[*]"}]
# multicycle path #3
set_multicycle_path -hold   2 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_3.o_data_reg[*]"}]
set_multicycle_path -setup  3 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_3.o_data_reg[*]"}]
# multicycle path #4
set_multicycle_path -hold   3 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_4.o_data_reg[*]"}]
set_multicycle_path -setup  4 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_4.o_data_reg[*]"}]
# multicycle path #5
set_multicycle_path -hold   4 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_5.o_data_reg[*]"}]
set_multicycle_path -setup  5 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_5.o_data_reg[*]"}]
# multicycle path #6
set_multicycle_path -hold   5 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_6.o_data_reg[*]"}]
set_multicycle_path -setup  6 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_6.o_data_reg[*]"}]
# multicycle path #7
set_multicycle_path -hold   6 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_7.o_data_reg[*]"}]
set_multicycle_path -setup  7 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_7.o_data_reg[*]"}]
# multicycle path #8
set_multicycle_path -hold   7 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_8.o_data_reg[*]"}]
set_multicycle_path -setup  8 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_8.o_data_reg[*]"}]
# multicycle path #9
set_multicycle_path -hold   8 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_9.o_data_reg[*]"}]
set_multicycle_path -setup  9 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_9.o_data_reg[*]"}]
# multicycle path #10
set_multicycle_path -hold   9 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_10.o_data_reg[*]"}]
set_multicycle_path -setup 10 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_10.o_data_reg[*]"}]
# multicycle path #11
set_multicycle_path -hold  10 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_11.o_data_reg[*]"}]
set_multicycle_path -setup 11 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_11.o_data_reg[*]"}]
# multicycle path #12
set_multicycle_path -hold  11 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_12.o_data_reg[*]"}]
set_multicycle_path -setup 12 -to [get_cells -hierarchical -filter {NAME =~  "*gen_multicycle.path_12.o_data_reg[*]"}]

###############################################################################
## PCI Express
###############################################################################
set_property IOSTANDARD LVCMOS18 [get_ports pcie_perstn]
set_property PULLUP TRUE [get_ports pcie_perstn]
set_property PACKAGE_PIN AV35 [get_ports pcie_perstn]
set_false_path -from [get_ports pcie_perstn]

set_property PACKAGE_PIN AB8 [get_ports pcie_diff_clock_clk_p]
set_property PACKAGE_PIN AB7 [get_ports pcie_diff_clock_clk_n]
set_property LOC IBUFDS_GTE2_X1Y5 [get_cells {System_i/PCIe_diff_clk_I/U0/USE_IBUFDS_GTE2.GEN_IBUFDS_GTE2[0].IBUFDS_GTE2_I}]

set_property PACKAGE_PIN Y4 [get_ports {pcie_rxp[0]}]
set_property PACKAGE_PIN AA6 [get_ports {pcie_rxp[1]}]
set_property PACKAGE_PIN AB4 [get_ports {pcie_rxp[2]}]
set_property PACKAGE_PIN AC6 [get_ports {pcie_rxp[3]}]

set_property PACKAGE_PIN Y3 [get_ports {pcie_rxn[0]}]
set_property PACKAGE_PIN AA5 [get_ports {pcie_rxn[1]}]
set_property PACKAGE_PIN AB3 [get_ports {pcie_rxn[2]}]
set_property PACKAGE_PIN AC5 [get_ports {pcie_rxn[3]}]

set_property PACKAGE_PIN W2 [get_ports {pcie_txp[0]}]
set_property PACKAGE_PIN AA2 [get_ports {pcie_txp[1]}]
set_property PACKAGE_PIN AC2 [get_ports {pcie_txp[2]}]
set_property PACKAGE_PIN AE2 [get_ports {pcie_txp[3]}]

set_property PACKAGE_PIN W1 [get_ports {pcie_txn[0]}]
set_property PACKAGE_PIN AA1 [get_ports {pcie_txn[1]}]
set_property PACKAGE_PIN AC1 [get_ports {pcie_txn[2]}]
set_property PACKAGE_PIN AE1 [get_ports {pcie_txn[3]}]



#------------------------------------------------------------------------------
# Pinout and Related I/O Constraints
#------------------------------------------------------------------------------
#
# Transceiver instance placement.  This constraint selects the
# transceivers to be used, which also dictates the pinout for the
# transmit and receive differential pairs.  Please refer to the
# Virtex-7 GT Transceiver User Guide (UG) for more information.
#

# PCIe Lane 0
set_property LOC GTXE2_CHANNEL_X1Y11 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.gt_ges.gt_top_i/pipe_wrapper_i/pipe_lane[0].gt_wrapper_i/gtx_channel.gtxe2_channel_i}]
# PCIe Lane 1
set_property LOC GTXE2_CHANNEL_X1Y10 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.gt_ges.gt_top_i/pipe_wrapper_i/pipe_lane[1].gt_wrapper_i/gtx_channel.gtxe2_channel_i}]
# PCIe Lane 2
set_property LOC GTXE2_CHANNEL_X1Y9 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.gt_ges.gt_top_i/pipe_wrapper_i/pipe_lane[2].gt_wrapper_i/gtx_channel.gtxe2_channel_i}]
# PCIe Lane 3
set_property LOC GTXE2_CHANNEL_X1Y8 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.gt_ges.gt_top_i/pipe_wrapper_i/pipe_lane[3].gt_wrapper_i/gtx_channel.gtxe2_channel_i}]

#
# PCI Express Block placement. This constraint selects the PCI Express
# Block to be used.
#

set_property LOC PCIE_X1Y0 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.pcie_top_i/pcie_7x_i/pcie_block_i}]

#
# BlockRAM placement
#

set_property LOC RAMB36_X13Y24 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.pcie_top_i/pcie_7x_i/pcie_bram_top/pcie_brams_rx/brams[0].ram/use_tdp.ramb36/genblk*.bram36_tdp_bl.bram36_tdp_bl}]
set_property LOC RAMB36_X13Y23 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.pcie_top_i/pcie_7x_i/pcie_bram_top/pcie_brams_rx/brams[1].ram/use_tdp.ramb36/genblk*.bram36_tdp_bl.bram36_tdp_bl}]
set_property LOC RAMB36_X13Y21 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.pcie_top_i/pcie_7x_i/pcie_bram_top/pcie_brams_tx/brams[0].ram/use_tdp.ramb36/genblk*.bram36_tdp_bl.bram36_tdp_bl}]
set_property LOC RAMB36_X13Y20 [get_cells {System_i/AXI_PCIe/inst/comp_axi_enhanced_pcie/comp_enhanced_core_top_wrap/axi_pcie_enhanced_core_top_i/pcie_7x_v2_0_2_inst/pcie_top_with_gt_top.pcie_top_i/pcie_7x_i/pcie_bram_top/pcie_brams_tx/brams[1].ram/use_tdp.ramb36/genblk*.bram36_tdp_bl.bram36_tdp_bl}]

#------------------------------------------------------------------------------
# Asynchronous Paths
#------------------------------------------------------------------------------


set_false_path -through [get_pins -hierarchical -filter {NAME=~*/RXELECIDLE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/TXPHINITDONE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/TXPHALIGNDONE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/TXDLYSRESETDONE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/RXDLYSRESETDONE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/RXPHALIGNDONE}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/RXCDRLOCK}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/CFGMSGRECEIVEDPMETO}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/CPLLLOCK}]
set_false_path -through [get_pins -hierarchical -filter {NAME=~*/QPLLLOCK}]



###############################################################################
## DDR3
###############################################################################
set_property CLOCK_DEDICATED_ROUTE FALSE [get_pins -hierarchical *clk_ref_mmcm_gen.mmcm_i*CLKIN1]

# PadFunction: IO_L23N_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[0]}]
set_property SLEW FAST [get_ports {ddr3_dq[0]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[0]}]
set_property PACKAGE_PIN N14 [get_ports {ddr3_dq[0]}]

# PadFunction: IO_L22P_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[1]}]
set_property SLEW FAST [get_ports {ddr3_dq[1]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[1]}]
set_property PACKAGE_PIN N13 [get_ports {ddr3_dq[1]}]

# PadFunction: IO_L20N_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[2]}]
set_property SLEW FAST [get_ports {ddr3_dq[2]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[2]}]
set_property PACKAGE_PIN L14 [get_ports {ddr3_dq[2]}]

# PadFunction: IO_L20P_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[3]}]
set_property SLEW FAST [get_ports {ddr3_dq[3]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[3]}]
set_property PACKAGE_PIN M14 [get_ports {ddr3_dq[3]}]

# PadFunction: IO_L24P_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[4]}]
set_property SLEW FAST [get_ports {ddr3_dq[4]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[4]}]
set_property PACKAGE_PIN M12 [get_ports {ddr3_dq[4]}]

# PadFunction: IO_L23P_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[5]}]
set_property SLEW FAST [get_ports {ddr3_dq[5]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[5]}]
set_property PACKAGE_PIN N15 [get_ports {ddr3_dq[5]}]

# PadFunction: IO_L24N_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[6]}]
set_property SLEW FAST [get_ports {ddr3_dq[6]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[6]}]
set_property PACKAGE_PIN M11 [get_ports {ddr3_dq[6]}]

# PadFunction: IO_L19P_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[7]}]
set_property SLEW FAST [get_ports {ddr3_dq[7]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[7]}]
set_property PACKAGE_PIN L12 [get_ports {ddr3_dq[7]}]

# PadFunction: IO_L17P_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[8]}]
set_property SLEW FAST [get_ports {ddr3_dq[8]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[8]}]
set_property PACKAGE_PIN K14 [get_ports {ddr3_dq[8]}]

# PadFunction: IO_L17N_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[9]}]
set_property SLEW FAST [get_ports {ddr3_dq[9]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[9]}]
set_property PACKAGE_PIN K13 [get_ports {ddr3_dq[9]}]

# PadFunction: IO_L14N_T2_SRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[10]}]
set_property SLEW FAST [get_ports {ddr3_dq[10]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[10]}]
set_property PACKAGE_PIN H13 [get_ports {ddr3_dq[10]}]

# PadFunction: IO_L14P_T2_SRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[11]}]
set_property SLEW FAST [get_ports {ddr3_dq[11]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[11]}]
set_property PACKAGE_PIN J13 [get_ports {ddr3_dq[11]}]

# PadFunction: IO_L18P_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[12]}]
set_property SLEW FAST [get_ports {ddr3_dq[12]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[12]}]
set_property PACKAGE_PIN L16 [get_ports {ddr3_dq[12]}]

# PadFunction: IO_L18N_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[13]}]
set_property SLEW FAST [get_ports {ddr3_dq[13]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[13]}]
set_property PACKAGE_PIN L15 [get_ports {ddr3_dq[13]}]

# PadFunction: IO_L13N_T2_MRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[14]}]
set_property SLEW FAST [get_ports {ddr3_dq[14]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[14]}]
set_property PACKAGE_PIN H14 [get_ports {ddr3_dq[14]}]

# PadFunction: IO_L16N_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[15]}]
set_property SLEW FAST [get_ports {ddr3_dq[15]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[15]}]
set_property PACKAGE_PIN J15 [get_ports {ddr3_dq[15]}]

# PadFunction: IO_L7N_T1_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[16]}]
set_property SLEW FAST [get_ports {ddr3_dq[16]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[16]}]
set_property PACKAGE_PIN E15 [get_ports {ddr3_dq[16]}]

# PadFunction: IO_L8N_T1_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[17]}]
set_property SLEW FAST [get_ports {ddr3_dq[17]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[17]}]
set_property PACKAGE_PIN E13 [get_ports {ddr3_dq[17]}]

# PadFunction: IO_L11P_T1_SRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[18]}]
set_property SLEW FAST [get_ports {ddr3_dq[18]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[18]}]
set_property PACKAGE_PIN F15 [get_ports {ddr3_dq[18]}]

# PadFunction: IO_L8P_T1_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[19]}]
set_property SLEW FAST [get_ports {ddr3_dq[19]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[19]}]
set_property PACKAGE_PIN E14 [get_ports {ddr3_dq[19]}]

# PadFunction: IO_L12N_T1_MRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[20]}]
set_property SLEW FAST [get_ports {ddr3_dq[20]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[20]}]
set_property PACKAGE_PIN G13 [get_ports {ddr3_dq[20]}]

# PadFunction: IO_L10P_T1_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[21]}]
set_property SLEW FAST [get_ports {ddr3_dq[21]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[21]}]
set_property PACKAGE_PIN G12 [get_ports {ddr3_dq[21]}]

# PadFunction: IO_L11N_T1_SRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[22]}]
set_property SLEW FAST [get_ports {ddr3_dq[22]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[22]}]
set_property PACKAGE_PIN F14 [get_ports {ddr3_dq[22]}]

# PadFunction: IO_L12P_T1_MRCC_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[23]}]
set_property SLEW FAST [get_ports {ddr3_dq[23]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[23]}]
set_property PACKAGE_PIN G14 [get_ports {ddr3_dq[23]}]

# PadFunction: IO_L2P_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[24]}]
set_property SLEW FAST [get_ports {ddr3_dq[24]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[24]}]
set_property PACKAGE_PIN B14 [get_ports {ddr3_dq[24]}]

# PadFunction: IO_L4N_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[25]}]
set_property SLEW FAST [get_ports {ddr3_dq[25]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[25]}]
set_property PACKAGE_PIN C13 [get_ports {ddr3_dq[25]}]

# PadFunction: IO_L1N_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[26]}]
set_property SLEW FAST [get_ports {ddr3_dq[26]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[26]}]
set_property PACKAGE_PIN B16 [get_ports {ddr3_dq[26]}]

# PadFunction: IO_L5N_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[27]}]
set_property SLEW FAST [get_ports {ddr3_dq[27]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[27]}]
set_property PACKAGE_PIN D15 [get_ports {ddr3_dq[27]}]

# PadFunction: IO_L4P_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[28]}]
set_property SLEW FAST [get_ports {ddr3_dq[28]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[28]}]
set_property PACKAGE_PIN D13 [get_ports {ddr3_dq[28]}]

# PadFunction: IO_L6P_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[29]}]
set_property SLEW FAST [get_ports {ddr3_dq[29]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[29]}]
set_property PACKAGE_PIN E12 [get_ports {ddr3_dq[29]}]

# PadFunction: IO_L1P_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[30]}]
set_property SLEW FAST [get_ports {ddr3_dq[30]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[30]}]
set_property PACKAGE_PIN C16 [get_ports {ddr3_dq[30]}]

# PadFunction: IO_L5P_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[31]}]
set_property SLEW FAST [get_ports {ddr3_dq[31]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[31]}]
set_property PACKAGE_PIN D16 [get_ports {ddr3_dq[31]}]

# PadFunction: IO_L1P_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[32]}]
set_property SLEW FAST [get_ports {ddr3_dq[32]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[32]}]
set_property PACKAGE_PIN A24 [get_ports {ddr3_dq[32]}]

# PadFunction: IO_L4N_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[33]}]
set_property SLEW FAST [get_ports {ddr3_dq[33]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[33]}]
set_property PACKAGE_PIN B23 [get_ports {ddr3_dq[33]}]

# PadFunction: IO_L5N_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[34]}]
set_property SLEW FAST [get_ports {ddr3_dq[34]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[34]}]
set_property PACKAGE_PIN B27 [get_ports {ddr3_dq[34]}]

# PadFunction: IO_L5P_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[35]}]
set_property SLEW FAST [get_ports {ddr3_dq[35]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[35]}]
set_property PACKAGE_PIN B26 [get_ports {ddr3_dq[35]}]

# PadFunction: IO_L2N_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[36]}]
set_property SLEW FAST [get_ports {ddr3_dq[36]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[36]}]
set_property PACKAGE_PIN A22 [get_ports {ddr3_dq[36]}]

# PadFunction: IO_L2P_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[37]}]
set_property SLEW FAST [get_ports {ddr3_dq[37]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[37]}]
set_property PACKAGE_PIN B22 [get_ports {ddr3_dq[37]}]

# PadFunction: IO_L1N_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[38]}]
set_property SLEW FAST [get_ports {ddr3_dq[38]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[38]}]
set_property PACKAGE_PIN A25 [get_ports {ddr3_dq[38]}]

# PadFunction: IO_L6P_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[39]}]
set_property SLEW FAST [get_ports {ddr3_dq[39]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[39]}]
set_property PACKAGE_PIN C24 [get_ports {ddr3_dq[39]}]

# PadFunction: IO_L7N_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[40]}]
set_property SLEW FAST [get_ports {ddr3_dq[40]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[40]}]
set_property PACKAGE_PIN E24 [get_ports {ddr3_dq[40]}]

# PadFunction: IO_L10N_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[41]}]
set_property SLEW FAST [get_ports {ddr3_dq[41]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[41]}]
set_property PACKAGE_PIN D23 [get_ports {ddr3_dq[41]}]

# PadFunction: IO_L11N_T1_SRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[42]}]
set_property SLEW FAST [get_ports {ddr3_dq[42]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[42]}]
set_property PACKAGE_PIN D26 [get_ports {ddr3_dq[42]}]

# PadFunction: IO_L12P_T1_MRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[43]}]
set_property SLEW FAST [get_ports {ddr3_dq[43]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[43]}]
set_property PACKAGE_PIN C25 [get_ports {ddr3_dq[43]}]

# PadFunction: IO_L7P_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[44]}]
set_property SLEW FAST [get_ports {ddr3_dq[44]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[44]}]
set_property PACKAGE_PIN E23 [get_ports {ddr3_dq[44]}]

# PadFunction: IO_L10P_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[45]}]
set_property SLEW FAST [get_ports {ddr3_dq[45]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[45]}]
set_property PACKAGE_PIN D22 [get_ports {ddr3_dq[45]}]

# PadFunction: IO_L8P_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[46]}]
set_property SLEW FAST [get_ports {ddr3_dq[46]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[46]}]
set_property PACKAGE_PIN F22 [get_ports {ddr3_dq[46]}]

# PadFunction: IO_L8N_T1_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[47]}]
set_property SLEW FAST [get_ports {ddr3_dq[47]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[47]}]
set_property PACKAGE_PIN E22 [get_ports {ddr3_dq[47]}]

# PadFunction: IO_L17N_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[48]}]
set_property SLEW FAST [get_ports {ddr3_dq[48]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[48]}]
set_property PACKAGE_PIN A30 [get_ports {ddr3_dq[48]}]

# PadFunction: IO_L13P_T2_MRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[49]}]
set_property SLEW FAST [get_ports {ddr3_dq[49]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[49]}]
set_property PACKAGE_PIN D27 [get_ports {ddr3_dq[49]}]

# PadFunction: IO_L17P_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[50]}]
set_property SLEW FAST [get_ports {ddr3_dq[50]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[50]}]
set_property PACKAGE_PIN A29 [get_ports {ddr3_dq[50]}]

# PadFunction: IO_L14P_T2_SRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[51]}]
set_property SLEW FAST [get_ports {ddr3_dq[51]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[51]}]
set_property PACKAGE_PIN C28 [get_ports {ddr3_dq[51]}]

# PadFunction: IO_L13N_T2_MRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[52]}]
set_property SLEW FAST [get_ports {ddr3_dq[52]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[52]}]
set_property PACKAGE_PIN D28 [get_ports {ddr3_dq[52]}]

# PadFunction: IO_L18N_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[53]}]
set_property SLEW FAST [get_ports {ddr3_dq[53]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[53]}]
set_property PACKAGE_PIN B31 [get_ports {ddr3_dq[53]}]

# PadFunction: IO_L16P_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[54]}]
set_property SLEW FAST [get_ports {ddr3_dq[54]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[54]}]
set_property PACKAGE_PIN A31 [get_ports {ddr3_dq[54]}]

# PadFunction: IO_L16N_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[55]}]
set_property SLEW FAST [get_ports {ddr3_dq[55]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[55]}]
set_property PACKAGE_PIN A32 [get_ports {ddr3_dq[55]}]

# PadFunction: IO_L19P_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[56]}]
set_property SLEW FAST [get_ports {ddr3_dq[56]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[56]}]
set_property PACKAGE_PIN E30 [get_ports {ddr3_dq[56]}]

# PadFunction: IO_L22P_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[57]}]
set_property SLEW FAST [get_ports {ddr3_dq[57]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[57]}]
set_property PACKAGE_PIN F29 [get_ports {ddr3_dq[57]}]

# PadFunction: IO_L24P_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[58]}]
set_property SLEW FAST [get_ports {ddr3_dq[58]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[58]}]
set_property PACKAGE_PIN F30 [get_ports {ddr3_dq[58]}]

# PadFunction: IO_L23N_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[59]}]
set_property SLEW FAST [get_ports {ddr3_dq[59]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[59]}]
set_property PACKAGE_PIN F27 [get_ports {ddr3_dq[59]}]

# PadFunction: IO_L20N_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[60]}]
set_property SLEW FAST [get_ports {ddr3_dq[60]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[60]}]
set_property PACKAGE_PIN C30 [get_ports {ddr3_dq[60]}]

# PadFunction: IO_L22N_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[61]}]
set_property SLEW FAST [get_ports {ddr3_dq[61]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[61]}]
set_property PACKAGE_PIN E29 [get_ports {ddr3_dq[61]}]

# PadFunction: IO_L23P_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[62]}]
set_property SLEW FAST [get_ports {ddr3_dq[62]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[62]}]
set_property PACKAGE_PIN F26 [get_ports {ddr3_dq[62]}]

# PadFunction: IO_L20P_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[63]}]
set_property SLEW FAST [get_ports {ddr3_dq[63]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[63]}]
set_property PACKAGE_PIN D30 [get_ports {ddr3_dq[63]}]

# PadFunction: IO_L5N_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[13]}]
set_property SLEW FAST [get_ports {ddr3_addr[13]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[13]}]
set_property PACKAGE_PIN A21 [get_ports {ddr3_addr[13]}]

# PadFunction: IO_L2N_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[12]}]
set_property SLEW FAST [get_ports {ddr3_addr[12]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[12]}]
set_property PACKAGE_PIN A15 [get_ports {ddr3_addr[12]}]

# PadFunction: IO_L4P_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[11]}]
set_property SLEW FAST [get_ports {ddr3_addr[11]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[11]}]
set_property PACKAGE_PIN B17 [get_ports {ddr3_addr[11]}]

# PadFunction: IO_L5P_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[10]}]
set_property SLEW FAST [get_ports {ddr3_addr[10]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[10]}]
set_property PACKAGE_PIN B21 [get_ports {ddr3_addr[10]}]

# PadFunction: IO_L1P_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[9]}]
set_property SLEW FAST [get_ports {ddr3_addr[9]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[9]}]
set_property PACKAGE_PIN C19 [get_ports {ddr3_addr[9]}]

# PadFunction: IO_L10N_T1_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[8]}]
set_property SLEW FAST [get_ports {ddr3_addr[8]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[8]}]
set_property PACKAGE_PIN D17 [get_ports {ddr3_addr[8]}]

# PadFunction: IO_L6P_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[7]}]
set_property SLEW FAST [get_ports {ddr3_addr[7]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[7]}]
set_property PACKAGE_PIN C18 [get_ports {ddr3_addr[7]}]

# PadFunction: IO_L7P_T1_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[6]}]
set_property SLEW FAST [get_ports {ddr3_addr[6]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[6]}]
set_property PACKAGE_PIN D20 [get_ports {ddr3_addr[6]}]

# PadFunction: IO_L2P_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[5]}]
set_property SLEW FAST [get_ports {ddr3_addr[5]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[5]}]
set_property PACKAGE_PIN A16 [get_ports {ddr3_addr[5]}]

# PadFunction: IO_L4N_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[4]}]
set_property SLEW FAST [get_ports {ddr3_addr[4]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[4]}]
set_property PACKAGE_PIN A17 [get_ports {ddr3_addr[4]}]

# PadFunction: IO_L3N_T0_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[3]}]
set_property SLEW FAST [get_ports {ddr3_addr[3]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[3]}]
set_property PACKAGE_PIN A19 [get_ports {ddr3_addr[3]}]

# PadFunction: IO_L7N_T1_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[2]}]
set_property SLEW FAST [get_ports {ddr3_addr[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[2]}]
set_property PACKAGE_PIN C20 [get_ports {ddr3_addr[2]}]

# PadFunction: IO_L1N_T0_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[1]}]
set_property SLEW FAST [get_ports {ddr3_addr[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[1]}]
set_property PACKAGE_PIN B19 [get_ports {ddr3_addr[1]}]

# PadFunction: IO_L3P_T0_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[0]}]
set_property SLEW FAST [get_ports {ddr3_addr[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[0]}]
set_property PACKAGE_PIN A20 [get_ports {ddr3_addr[0]}]

# PadFunction: IO_L10P_T1_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[2]}]
set_property SLEW FAST [get_ports {ddr3_ba[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[2]}]
set_property PACKAGE_PIN D18 [get_ports {ddr3_ba[2]}]

# PadFunction: IO_L9N_T1_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[1]}]
set_property SLEW FAST [get_ports {ddr3_ba[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[1]}]
set_property PACKAGE_PIN C21 [get_ports {ddr3_ba[1]}]

# PadFunction: IO_L9P_T1_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[0]}]
set_property SLEW FAST [get_ports {ddr3_ba[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[0]}]
set_property PACKAGE_PIN D21 [get_ports {ddr3_ba[0]}]

# PadFunction: IO_L15N_T2_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ras_n}]
set_property SLEW FAST [get_ports {ddr3_ras_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ras_n}]
set_property PACKAGE_PIN E20 [get_ports {ddr3_ras_n}]

# PadFunction: IO_L16P_T2_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_cas_n}]
set_property SLEW FAST [get_ports {ddr3_cas_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_cas_n}]
set_property PACKAGE_PIN K17 [get_ports {ddr3_cas_n}]

# PadFunction: IO_L15P_T2_DQS_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_we_n}]
set_property SLEW FAST [get_ports {ddr3_we_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_we_n}]
set_property PACKAGE_PIN F20 [get_ports {ddr3_we_n}]

# PadFunction: IO_L14N_T2_SRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_reset_n}]
set_property SLEW FAST [get_ports {ddr3_reset_n}]
set_property IOSTANDARD LVCMOS15 [get_ports {ddr3_reset_n}]
set_property PACKAGE_PIN C29 [get_ports {ddr3_reset_n}]

# PadFunction: IO_L14P_T2_SRCC_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_cke[0]}]
set_property SLEW FAST [get_ports {ddr3_cke[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_cke[0]}]
set_property PACKAGE_PIN K19 [get_ports {ddr3_cke[0]}]

# PadFunction: IO_L17N_T2_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_odt[0]}]
set_property SLEW FAST [get_ports {ddr3_odt[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_odt[0]}]
set_property PACKAGE_PIN H20 [get_ports {ddr3_odt[0]}]

# PadFunction: IO_L16N_T2_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_cs_n[0]}]
set_property SLEW FAST [get_ports {ddr3_cs_n[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_cs_n[0]}]
set_property PACKAGE_PIN J17 [get_ports {ddr3_cs_n[0]}]

# PadFunction: IO_L22N_T3_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[0]}]
set_property SLEW FAST [get_ports {ddr3_dm[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[0]}]
set_property PACKAGE_PIN M13 [get_ports {ddr3_dm[0]}]

# PadFunction: IO_L16P_T2_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[1]}]
set_property SLEW FAST [get_ports {ddr3_dm[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[1]}]
set_property PACKAGE_PIN K15 [get_ports {ddr3_dm[1]}]

# PadFunction: IO_L10N_T1_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[2]}]
set_property SLEW FAST [get_ports {ddr3_dm[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[2]}]
set_property PACKAGE_PIN F12 [get_ports {ddr3_dm[2]}]

# PadFunction: IO_L2N_T0_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[3]}]
set_property SLEW FAST [get_ports {ddr3_dm[3]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[3]}]
set_property PACKAGE_PIN A14 [get_ports {ddr3_dm[3]}]

# PadFunction: IO_L4P_T0_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[4]}]
set_property SLEW FAST [get_ports {ddr3_dm[4]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[4]}]
set_property PACKAGE_PIN C23 [get_ports {ddr3_dm[4]}]

# PadFunction: IO_L11P_T1_SRCC_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[5]}]
set_property SLEW FAST [get_ports {ddr3_dm[5]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[5]}]
set_property PACKAGE_PIN D25 [get_ports {ddr3_dm[5]}]

# PadFunction: IO_L18P_T2_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[6]}]
set_property SLEW FAST [get_ports {ddr3_dm[6]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[6]}]
set_property PACKAGE_PIN C31 [get_ports {ddr3_dm[6]}]

# PadFunction: IO_L24N_T3_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[7]}]
set_property SLEW FAST [get_ports {ddr3_dm[7]}]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[7]}]
set_property PACKAGE_PIN F31 [get_ports {ddr3_dm[7]}]

# PadFunction: IO_L21P_T3_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[0]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[0]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[0]}]
set_property PACKAGE_PIN N16 [get_ports {ddr3_dqs_p[0]}]

# PadFunction: IO_L21N_T3_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[0]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[0]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[0]}]
set_property PACKAGE_PIN M16 [get_ports {ddr3_dqs_n[0]}]

# PadFunction: IO_L15P_T2_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[1]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[1]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[1]}]
set_property PACKAGE_PIN K12 [get_ports {ddr3_dqs_p[1]}]

# PadFunction: IO_L15N_T2_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[1]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[1]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[1]}]
set_property PACKAGE_PIN J12 [get_ports {ddr3_dqs_n[1]}]

# PadFunction: IO_L9P_T1_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[2]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[2]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[2]}]
set_property PACKAGE_PIN H16 [get_ports {ddr3_dqs_p[2]}]

# PadFunction: IO_L9N_T1_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[2]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[2]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[2]}]
set_property PACKAGE_PIN G16 [get_ports {ddr3_dqs_n[2]}]

# PadFunction: IO_L3P_T0_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[3]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[3]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[3]}]
set_property PACKAGE_PIN C15 [get_ports {ddr3_dqs_p[3]}]

# PadFunction: IO_L3N_T0_DQS_39 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[3]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[3]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[3]}]
set_property PACKAGE_PIN C14 [get_ports {ddr3_dqs_n[3]}]

# PadFunction: IO_L3P_T0_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[4]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[4]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[4]}]
set_property PACKAGE_PIN A26 [get_ports {ddr3_dqs_p[4]}]

# PadFunction: IO_L3N_T0_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[4]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[4]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[4]}]
set_property PACKAGE_PIN A27 [get_ports {ddr3_dqs_n[4]}]

# PadFunction: IO_L9P_T1_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[5]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[5]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[5]}]
set_property PACKAGE_PIN F25 [get_ports {ddr3_dqs_p[5]}]

# PadFunction: IO_L9N_T1_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[5]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[5]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[5]}]
set_property PACKAGE_PIN E25 [get_ports {ddr3_dqs_n[5]}]

# PadFunction: IO_L15P_T2_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[6]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[6]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[6]}]
set_property PACKAGE_PIN B28 [get_ports {ddr3_dqs_p[6]}]

# PadFunction: IO_L15N_T2_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[6]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[6]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[6]}]
set_property PACKAGE_PIN B29 [get_ports {ddr3_dqs_n[6]}]

# PadFunction: IO_L21P_T3_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[7]}]
set_property SLEW FAST [get_ports {ddr3_dqs_p[7]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[7]}]
set_property PACKAGE_PIN E27 [get_ports {ddr3_dqs_p[7]}]

# PadFunction: IO_L21N_T3_DQS_37 
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_n[7]}]
set_property SLEW FAST [get_ports {ddr3_dqs_n[7]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[7]}]
set_property PACKAGE_PIN E28 [get_ports {ddr3_dqs_n[7]}]

# PadFunction: IO_L13P_T2_MRCC_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ck_p[0]}]
set_property SLEW FAST [get_ports {ddr3_ck_p[0]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddr3_ck_p[0]}]
set_property PACKAGE_PIN H19 [get_ports {ddr3_ck_p[0]}]

# PadFunction: IO_L13N_T2_MRCC_38 
set_property VCCAUX_IO HIGH [get_ports {ddr3_ck_n[0]}]
set_property SLEW FAST [get_ports {ddr3_ck_n[0]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddr3_ck_n[0]}]
set_property PACKAGE_PIN G18 [get_ports {ddr3_ck_n[0]}]



set_property LOC PHASER_OUT_PHY_X1Y19 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y18 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y17 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y16 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y23 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y22 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y21 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y27 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y26 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y25 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out}]
set_property LOC PHASER_OUT_PHY_X1Y24 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out}]

set_property LOC PHASER_IN_PHY_X1Y19 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y18 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y17 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y16 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_in_gen.phaser_in}]
## set_property LOC PHASER_IN_PHY_X1Y23 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in}]
## set_property LOC PHASER_IN_PHY_X1Y22 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in}]
## set_property LOC PHASER_IN_PHY_X1Y21 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y27 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y26 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y25 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in}]
set_property LOC PHASER_IN_PHY_X1Y24 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_in_gen.phaser_in}]



set_property LOC OUT_FIFO_X1Y19 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/out_fifo}]
set_property LOC OUT_FIFO_X1Y18 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/out_fifo}]
set_property LOC OUT_FIFO_X1Y17 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/out_fifo}]
set_property LOC OUT_FIFO_X1Y16 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/out_fifo}]
set_property LOC OUT_FIFO_X1Y23 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/out_fifo}]
set_property LOC OUT_FIFO_X1Y22 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/out_fifo}]
set_property LOC OUT_FIFO_X1Y21 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/out_fifo}]
set_property LOC OUT_FIFO_X1Y27 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/out_fifo}]
set_property LOC OUT_FIFO_X1Y26 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/out_fifo}]
set_property LOC OUT_FIFO_X1Y25 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/out_fifo}]
set_property LOC OUT_FIFO_X1Y24 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/out_fifo}]

set_property LOC IN_FIFO_X1Y19 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y18 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y17 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y16 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y27 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y26 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y25 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/in_fifo_gen.in_fifo}]
set_property LOC IN_FIFO_X1Y24 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/in_fifo_gen.in_fifo}]

set_property LOC PHY_CONTROL_X1Y4 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/phy_control_i}]
set_property LOC PHY_CONTROL_X1Y5 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/phy_control_i}]
set_property LOC PHY_CONTROL_X1Y6 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/phy_control_i}]

set_property LOC PHASER_REF_X1Y4 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/phaser_ref_i}]
set_property LOC PHASER_REF_X1Y5 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/phaser_ref_i}]
set_property LOC PHASER_REF_X1Y6 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/phaser_ref_i}]

set_property LOC OLOGIC_X1Y243 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y231 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y219 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y207 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_2.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y343 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y331 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y319 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/ddr_byte_group_io/*slave_ts}]
set_property LOC OLOGIC_X1Y307 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/ddr_byte_group_io/*slave_ts}]

set_property LOC PLLE2_ADV_X1Y5 [get_cells -hier -filter {NAME =~ */u_ddr3_infrastructure/plle2_i}]
set_property LOC MMCME2_ADV_X1Y5 [get_cells -hier -filter {NAME =~ */u_ddr3_infrastructure/gen_mmcm.mmcm_i}]


set_multicycle_path -from [get_cells -hier -filter {NAME =~ */mc0/mc_read_idle_r_reg}] \
                    -to   [get_cells -hier -filter {NAME =~ */input_[?].iserdes_dq_.iserdesdq}] \
                    -setup 6

set_multicycle_path -from [get_cells -hier -filter {NAME =~ */mc0/mc_read_idle_r_reg}] \
                    -to   [get_cells -hier -filter {NAME =~ */input_[?].iserdes_dq_.iserdesdq}] \
                    -hold 5

set_false_path -through [get_pins -filter {NAME =~ */DQSFOUND} -of [get_cells -hier -filter {REF_NAME == PHASER_IN_PHY}]]

set_multicycle_path -through [get_pins -filter {NAME =~ */OSERDESRST} -of [get_cells -hier -filter {REF_NAME == PHASER_OUT_PHY}]] -setup 2 -start
set_multicycle_path -through [get_pins -filter {NAME =~ */OSERDESRST} -of [get_cells -hier -filter {REF_NAME == PHASER_OUT_PHY}]] -hold 1 -start

set_max_delay -datapath_only -from [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/* && IS_SEQUENTIAL}] -to [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/device_temp_sync_r1*}] 20
set_max_delay -from [get_cells -hier *rstdiv0_sync_r1_reg*] -to [get_pins -filter {NAME =~ */RESET} -of [get_cells -hier -filter {REF_NAME == PHY_CONTROL}]] -datapath_only 5
set_false_path -through [get_pins -hier -filter {NAME =~ */u_iodelay_ctrl/sys_rst}]

set_max_delay -datapath_only -from [get_cells -hier -filter {NAME =~ *ddr3_infrastructure/rstdiv0_sync_r1_reg*}] -to [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/xadc_supplied_temperature.rst_r1*}] 20
