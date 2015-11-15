############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project jpeg_prof_IZigzagMatrix_opt_prj
set_top IZigzagMatrix
add_files ./src/decode_opt.c -cflags "-I./src"
add_files -tb ./src/main.c -cflags "-I./src"
add_files -tb ./src/chenidct.c -cflags "-I./src"
add_files -tb ./src/jfif_read.c -cflags "-I./src"
add_files -tb ./src/jpeg2bmp.c -cflags "-I./src"
add_files -tb ./src/marker.c -cflags "-I./src"
add_files -tb ./src/huffman_base.c -cflags "-I./src"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

csim_design
csynth_design
#cosim_design -trace_level none
#export_design
exit
