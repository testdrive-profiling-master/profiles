# flash type constraints.
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 8 [current_design]
set_property BITSTREAM.CONFIG.EXTMASTERCCLK_EN div-1 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property BITSTREAM.CONFIG.SPI_FALL_EDGE YES [current_design]

set_false_path -from [get_ports pcie_perstn]
set_false_path -from [get_ports reset]
set_false_path -to [get_ports ddr4_sdram_c1_062_reset_n]
set_false_path -to [get_ports ddr4_sdram_c2_062_reset_n]
