############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project coprocess
set_top dct_2d
#set_top transpose
add_files coprocess/example/src/dct.c -cflags "-I./"
add_files -tb coprocess/example/src/dct_test.c -cflags "-I./"
add_files -tb coprocess/example/src/in.dat
open_solution "example"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global
source "./coprocess/example/directives.tcl"
csim_design
csynth_design
#cosim_design -trace_level none
#export_design
exit
