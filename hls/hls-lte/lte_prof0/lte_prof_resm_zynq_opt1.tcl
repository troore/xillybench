open_project lte_prof_resm_zynq_opt1_prj
set_top SubCarrierDemapping
add_files ../src/ResMapper_bounded_opt1.cpp -cflags "-I../ -I../src"
add_files ../src/dmrs_bounded_opt1.cpp -cflags "-I../ -I../src"

open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design
csynth_design
#cosim_design -trace_level none
export_design
exit
