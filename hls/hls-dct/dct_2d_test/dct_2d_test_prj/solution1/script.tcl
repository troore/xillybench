############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project dct_2d_test_prj
set_top xillybus_wrapper
add_files ../../xilly_debug/xilly_debug.c -cflags "-I../../xilly_debug"
add_files ./src/dct_2d.c -cflags "-I../../xilly_debug"
add_files ./src/xilly_wrapper_test.c -cflags "-I../../xilly_debug"
open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global
source "./dct_2d_test_prj/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design -trace_level none -rtl verilog -tool auto
export_design -format ip_catalog
