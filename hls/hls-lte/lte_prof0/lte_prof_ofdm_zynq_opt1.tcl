open_project lte_prof_ofdm_zynq_opt1_prj
set_top xillybus_wrapper
add_files ../src/xilly_wrapper_ofdm.cpp -cflags "-I../ -I../../xilly_debug -I../src"
add_files ../src/OFDM_nrvs_same_array_cyclic_bounded_opt1.cpp -cflags "-I../ -I../src"
add_files ../src/fft_bounded_opt1.cpp -cflags "-I../ -I../src"
add_files ../../xilly_debug/xilly_debug.cpp -cflags "-I../../xilly_debug"

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design
csynth_design
#cosim_design -trace_level none
#export_design
exit
