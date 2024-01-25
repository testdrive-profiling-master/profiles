# flash type constraints.
set_property BITSTREAM.CONFIG.EXTMASTERCCLK_EN div-1 [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 8 [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES [current_design]
set_property BITSTREAM.CONFIG.UNUSEDPIN Pullnone [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property CONFIG_MODE SPIx8 [current_design]
set_property CONFIG_VOLTAGE 1.8 [current_design]
set_property CFGBVS GND [current_design]

set_false_path -from [get_ports pcie_perstn]
set_false_path -from [get_ports reset]
set_false_path -to [get_ports ddr4_sdram_c1_062_reset_n]
set_false_path -to [get_ports ddr4_sdram_c2_062_reset_n]

###############################################################################
# HDMI controller
###############################################################################
set_property PACKAGE_PIN L36 [get_ports HDMI_CLK]
set_property PACKAGE_PIN AJ12 [get_ports HDMI_VSYNC]
set_property PACKAGE_PIN AK13 [get_ports HDMI_HSYNC]
set_property PACKAGE_PIN AJ13 [get_ports HDMI_DE]
set_property PACKAGE_PIN AK14 [get_ports HDMI_DATA[0] ]
set_property PACKAGE_PIN AL12 [get_ports HDMI_DATA[1] ]
set_property PACKAGE_PIN AM12 [get_ports HDMI_DATA[2] ]
set_property PACKAGE_PIN AK12 [get_ports HDMI_DATA[3] ]
set_property PACKAGE_PIN AM13 [get_ports HDMI_DATA[4] ]
set_property PACKAGE_PIN AP15 [get_ports HDMI_DATA[5] ]
set_property PACKAGE_PIN AV10 [get_ports HDMI_DATA[6] ]
set_property PACKAGE_PIN AN15 [get_ports HDMI_DATA[7] ]
set_property PACKAGE_PIN AL15 [get_ports HDMI_DATA[8] ]
set_property PACKAGE_PIN AR13 [get_ports HDMI_DATA[9] ]
set_property PACKAGE_PIN AU12 [get_ports HDMI_DATA[10]]
set_property PACKAGE_PIN AP13 [get_ports HDMI_DATA[11]]
set_property PACKAGE_PIN AT12 [get_ports HDMI_DATA[12]]
set_property PACKAGE_PIN AP16 [get_ports HDMI_DATA[13]]
set_property PACKAGE_PIN AV11 [get_ports HDMI_DATA[14]]
set_property PACKAGE_PIN AY13 [get_ports HDMI_DATA[15]]
set_property PACKAGE_PIN AU11 [get_ports HDMI_DATA[16]]
set_property PACKAGE_PIN AN16 [get_ports HDMI_DATA[17]]
set_property PACKAGE_PIN AW13 [get_ports HDMI_DATA[18]]
set_property PACKAGE_PIN AY12 [get_ports HDMI_DATA[19]]
set_property PACKAGE_PIN AY10 [get_ports HDMI_DATA[20]]
set_property PACKAGE_PIN AW12 [get_ports HDMI_DATA[21]]
set_property PACKAGE_PIN AW11 [get_ports HDMI_DATA[22]]
set_property PACKAGE_PIN AR14 [get_ports HDMI_DATA[23]]
set_property PACKAGE_PIN AP12 [get_ports HDMI_INTR]
set_property PACKAGE_PIN AK15 [get_ports HDMI_nRST]
set_property PACKAGE_PIN AT14 [get_ports HDMI_HPD]
set_property PACKAGE_PIN AL14 [get_ports HDMI_I2C_SCL]
set_property PACKAGE_PIN AM14 [get_ports HDMI_I2C_SDA]

set_false_path -to [get_ports HDMI_nRST]
set_false_path -from [get_ports HDMI_INTR]
set_property slew "fast" [get_ports HDMI_CLK]
set_property IOSTANDARD LVCMOS18 [get_ports -filter {NAME =~  "HDMI_*"}]
set_false_path -from [get_ports -filter {NAME =~  "HDMI_I2C_*"}]
set_false_path -to   [get_ports -filter {NAME =~  "HDMI_I2C_*"}]

###############################################################################
# APB bus (out-of-context synthesis bug?)
###############################################################################
create_clock -name apb_aclk -period 4.0 [get_pins system_design_i/axi_apb_bridge/s_axi_aclk] -quiet
