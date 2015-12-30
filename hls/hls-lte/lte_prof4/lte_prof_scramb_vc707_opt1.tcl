open_project lte_prof_scramb_vc707_opt1_prj
set_top	Descrambling
add_files ../src4/Scrambler_bounded_opt1.cpp -cflags "-I../ -I../src4"
add_files -tb ../src4/xilly_wrapper_scramb.cpp -cflags "-I../ -I../src4 -I../lib"
add_files -tb ../src4/lte_phy.cpp -cflags "-I../ -I../src4"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../ -I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../ -I../lib"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

csim_design -argv {4}
csynth_design
#cosim_design -trace_level none
export_design
exit
