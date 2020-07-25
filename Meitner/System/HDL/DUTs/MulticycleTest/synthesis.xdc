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
