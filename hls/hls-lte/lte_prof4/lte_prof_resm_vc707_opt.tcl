open_project lte_prof_resm_vc707_opt_prj
set_top SubCarrierDemapping
add_files ../src4/ResMapper_bounded.cpp -cflags "-I../ -I../src4"
add_files ../src4/dmrs_bounded.cpp -cflags "-I../ -I../src4"

open_solution "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global

#csim_design
csynth_design
#cosim_design -trace_level none
export_design
exit
