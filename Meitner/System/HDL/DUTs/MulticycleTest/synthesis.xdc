# multicycle path #2
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.o_*"}] 1
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.o_*"}] 2

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.i_*"}] 1
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_2.i_*"}] 2

# multicycle path #3
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.o_*"}] 2
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.o_*"}] 3

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.i_*"}] 2
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_3.i_*"}] 3

# multicycle path #4
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.o_*"}] 3
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.o_*"}] 4

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.i_*"}] 3
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_4.i_*"}] 4

# multicycle path #5
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.o_*"}] 4
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.o_*"}] 5

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.i_*"}] 4
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_5.i_*"}] 5

# multicycle path #6
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.o_*"}] 5
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.o_*"}] 6

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.i_*"}] 5
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_6.i_*"}] 6

# multicycle path #7
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.o_*"}] 6
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.o_*"}] 7

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.i_*"}] 6
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_7.i_*"}] 7

# multicycle path #8
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.o_*"}] 7
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.o_*"}] 8

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.i_*"}] 7
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_8.i_*"}] 8

# multicycle path #9
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.o_*"}] 8
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.o_*"}] 9

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.i_*"}] 8
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_9.i_*"}] 9

# multicycle path #10
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.o_*"}] 9
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.o_*"}] 10

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.i_*"}] 9
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_10.i_*"}] 10

# multicycle path #11
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.o_*"}] 10
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.o_*"}] 11

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.i_*"}] 10
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_11.i_*"}] 11

# multicycle path #12
set_multicycle_path -hold  -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.o_*"}] 11
set_multicycle_path -setup -to [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.o_*"}] 12

set_multicycle_path -hold  -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.i_*"}] 11
set_multicycle_path -setup -from [get_cells -hierarchical -filter {NAME =~ "*gen_multicycle.path_12.i_*"}] 12
