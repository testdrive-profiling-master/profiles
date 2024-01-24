###############################################################################
# APB bus (out-of-context synthesis bug?)
###############################################################################
create_clock -name apb_aclk -period 4.0 [get_pins system_design_i/axi_apb_bridge/s_axi_aclk] -quiet

###############################################################################
# PCIe XDMA
###############################################################################
create_clock -name pcie_aclk -period 4.0 [get_pins system_design_i/pcie_xdma/axi_aclk] -quiet

###############################################################################
# DUT target
###############################################################################
create_clock -name proc_clk -period 4.0 [get_pins -hierarchical -filter {NAME =~ "*dut_clock_gen/CLK_OUT"}]
set_false_path -from [get_clocks proc_clk] -to [get_clocks pcie_aclk]
set_false_path -from [get_clocks pcie_aclk] -to [get_clocks proc_clk]
