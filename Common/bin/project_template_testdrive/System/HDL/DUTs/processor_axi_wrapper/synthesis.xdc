create_clock -name proc_clk -period 5.0 [get_pins -hierarchical -filter {NAME =~  "*testdrive_clock_gen/mmcm_inst/CLKOUT0"}]
set_false_path -from [get_clocks main_clk] -to [get_clocks proc_clk]
set_false_path -from [get_clocks proc_clk] -to [get_clocks main_clk]
set_false_path -from [get_clocks main_clk] -to [get_cells -hierarchical -filter {NAME =~ "*testdrive_clock_gen/mmcm_inst*"}]
