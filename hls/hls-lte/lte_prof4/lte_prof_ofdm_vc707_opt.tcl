open_project lte_prof_ofdm_vc707_opt_prj
set_top xillybus_wrapper
add_files ../src4/xilly_wrapper_ofdm.cpp -cflags "-I../ -I../../xilly_debug -I../src4"
add_files ../src4/OFDM_nrvs_same_array_cyclic_bounded.cpp -cflags "-I../ -I../src4"
add_files ../src4/fft_bounded.cpp -cflags "-I../ -I../src4"
add_files ../../xilly_debug/xilly_debug.cpp -cflags "-I../../xilly_debug"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design
csynth_design
#cosim_design -trace_level none
export_design
exit
