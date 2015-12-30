open_project lte_prof_transpre_vc707_opt_prj
set_top TransformDecoding
add_files ../src4/TransformPrecoder_bounded.cpp -cflags "-I../ -I../src4"
add_files ../src4/dft_bounded.cpp -cflags "-I../ -I../src4"
#add_files -tb ../src4/xilly_wrapper_turbo.cpp -cflags "-I../ -I../src -I../lib"
#add_files -tb ../src4/lte_phy.cpp -cflags "-I../src"
#add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
#add_files -tb ../lib/gauss.cpp -cflags "-I../lib"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
#set_part {$argv[1]}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design -argv {4}
csynth_design
#cosim_design -trace_level none
export_design
exit
