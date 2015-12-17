############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project jpeg_prof_DecodeHuffMCU_zynq_base_prj
set_top DecodeHuffMCU
add_files ./src/huffman_base.c -cflags "-I./src"
add_files -tb ./src/main.c -cflags "-I./src"
add_files -tb ./src/chenidct_base.c -cflags "-I./src"
add_files -tb ./src/jfif_read.c -cflags "-I./src"
add_files -tb ./src/jpeg2bmp.c -cflags "-I./src"
add_files -tb ./src/marker.c -cflags "-I./src"
add_files -tb ./src/decode_base.c -cflags "-I./src"

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

csim_design
csynth_design
cosim_design -trace_level none
export_design
exit
