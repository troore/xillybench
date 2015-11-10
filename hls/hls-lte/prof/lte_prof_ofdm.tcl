############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project lte_prof_ofdm_prj
set_top xillybus_wrapper
add_files xilly_wrapper_ofdm.cpp -cflags "-I../ -I../../xilly_debug -I../OFDM"
add_files ../OFDM/OFDM_nrvs_same_array_cyclic_bounded.cpp -cflags "-I../ -I../OFDM -I../fft"
add_files ../fft/fft_bounded.cpp -cflags "-I../ -I../fft"
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
