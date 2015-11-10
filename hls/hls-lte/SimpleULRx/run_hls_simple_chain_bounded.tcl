
open_project -reset equalizer_chain_bounded_prj
set_top equalizer_chain
add_files simple_rx.cpp -cflags "-I../ -I../lib -I../ResMapping -I../OFDM -I../Equalizing"
add_files ../dmrs/dmrs_bounded.cpp -cflags "-I../ -I../lib"
add_files ../ResMapping/ResMapper_bounded.cpp -cflags "-I../ -I../lib"
add_files ../Equalizing/Equalizer_bounded.cpp -cflags "-I../ -I../lib"
add_files ../fft/fft_bounded.cpp
add_files ../OFDM/OFDM_nrvs_same_array_cyclic_bounded.cpp -cflags "-I../ -I../lib"
add_files -tb simple_rx_main.cpp -cflags "-I../ -I../lib -DCHAIN"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lib/check.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx485tffg1761-2}
create_clock -period 10 -name default

csim_design -argv {4}
csynth_design
#cosim_design  -trace_level none -argv {4}
#export_design 
exit
