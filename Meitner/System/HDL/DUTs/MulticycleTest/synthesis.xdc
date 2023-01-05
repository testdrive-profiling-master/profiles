# multicycle path #2
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.o_data_reg*"}] 1
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.o_data_reg*"}] 2

# multicycle path #3
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.o_data_reg*"}] 2
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.o_data_reg*"}] 3

# multicycle path #4
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.o_data_reg*"}] 3
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.o_data_reg*"}] 4

# multicycle path #5
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.o_data_reg*"}] 4
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.o_data_reg*"}] 5

# multicycle path #6
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.o_data_reg*"}] 5
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.o_data_reg*"}] 6

# multicycle path #7
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.o_data_reg*"}] 6
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.o_data_reg*"}] 7

# multicycle path #8
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.o_data_reg*"}] 7
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.o_data_reg*"}] 8

# multicycle path #9
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.o_data_reg*"}] 8
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.o_data_reg*"}] 9

# multicycle path #10
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.o_data_reg*"}] 9
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.o_data_reg*"}] 10

# multicycle path #11
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.o_data_reg*"}] 10
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.o_data_reg*"}] 11

# multicycle path #12
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.o_data_reg*"}] 11
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.o_data_reg*"}] 12
