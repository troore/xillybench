############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project coprocess
set_top xillybus_wrapper
add_files coprocess/dct_2d_test/src/dct_2d.c -cflags "-I./."
add_files coprocess/dct_2d_test/src/xilly_debug.c -cflags "-I./."
add_files coprocess/dct_2d_test/src/xilly_wrapper_test.c -cflags "-I./."
open_solution "dct_2d_test"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global
source "./coprocess/dct_2d_test/directives.tcl"
#csim_design
csynth_design
#cosim_design -trace_level none -rtl verilog -tool auto
export_design -format ip_catalog
