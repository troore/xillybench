open_project fpga_zynq_base_prj
set_top xillybus_wrapper
add_files ../../xilly_debug/xilly_debug.c -cflags "-I../../xilly_debug"
add_files ./src/main.c -cflags "-I../../xilly_debug"
add_files ./src/mm_base.c -cflags "-I../../xilly_debug"

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

config_interface -all ap_fifo -expose_global
#source "./coprocess/example/directives.tcl"
#csim_design
csynth_design
#cosim_design -trace_level none
export_design
exit
