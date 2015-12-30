open_project lte_prof_scramb_zynq_opt1_prj
set_top	Descrambling
add_files ../src/Scrambler_bounded_opt1.cpp -cflags "-I../ -I../src"
add_files -tb ../src/xilly_wrapper_scramb.cpp -cflags "-I../ -I../src -I../lib"
add_files -tb ../src/lte_phy.cpp -cflags "-I../ -I../src"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../ -I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../ -I../lib"

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

csim_design -argv {0}
csynth_design
#cosim_design -trace_level none
#export_design
exit
