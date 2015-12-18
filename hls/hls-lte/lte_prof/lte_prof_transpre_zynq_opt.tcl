open_project lte_prof_transpre_zynq_opt_prj
set_top TransformDecoding
add_files ../src/TransformPrecoder_bounded.cpp -cflags "-I../ -I../src"
add_files ../src/dft_bounded.cpp -cflags "-I../ -I../src"
#add_files -tb ../src/xilly_wrapper_turbo.cpp -cflags "-I../ -I../src -I../lib"
#add_files -tb ../src/lte_phy.cpp -cflags "-I../src"
#add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
#add_files -tb ../lib/gauss.cpp -cflags "-I../lib"

open_solution "solution1"
set_part {xc7z020clg484-1}
#set_part {$argv[1]}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design -argv {0}
csynth_design
#cosim_design -trace_level none
export_design
exit
