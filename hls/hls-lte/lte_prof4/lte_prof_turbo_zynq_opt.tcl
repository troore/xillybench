open_project lte_prof_turbo_zynq_opt_prj
set_top turbo_decoding
add_files ../src/Turbo_bounded.cpp -cflags "-I../ -I../src"
add_files -tb ../src/xilly_wrapper_turbo.cpp -cflags "-I../ -I../src -I../lib"
add_files -tb ../src/lte_phy.cpp -cflags "-I../src"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"

#open_solution "solution1"
open_solution "solution2"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design -argv {0}
#csynth_design
#cosim_design -trace_level none
export_design
exit
