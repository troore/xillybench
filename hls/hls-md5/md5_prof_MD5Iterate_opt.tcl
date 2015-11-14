############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project md5_prof_MD5Iterate_opt_prj
set_top MD5Iterate
add_files ./src/md5_opt.c -cflags "-I./src"
add_files -tb ./src/main.c -cflags "-I./src"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

csim_design
csynth_design
#cosim_design -trace_level none
#export_design
exit
