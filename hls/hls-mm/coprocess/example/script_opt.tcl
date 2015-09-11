############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project coprocess
set_top xillybus_wrapper
add_files coprocess/example/src/xilly_debug.c
add_files coprocess/example/src/main.c -cflags "-DOPT"
add_files coprocess/example/src/mm_opt.c -cflags "-DOPT"
open_solution "example"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global
source "./coprocess/example/directives.tcl"
#csim_design
csynth_design
#cosim_design -trace_level none
export_design
exit
