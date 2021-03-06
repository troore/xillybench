# 
# Synthesis run script generated by Vivado
# 

  set_param gui.test TreeTableDev
set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7vx485tffg1761-2
set_property target_language Verilog [current_project]
set_param project.compositeFile.enableAutoGeneration 0
set_property default_lib xil_defaultlib [current_project]

read_ip /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048.xci
set_property used_in_implementation false [get_files /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048.dcp]
set_property used_in_implementation false [get_files -all /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048.dcp]
set_msg_config -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property is_locked true [get_files /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048.xci]

read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/fft/vivado/xillydemo.cache/wt [current_project]
set_property parent.project_dir /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/fft/vivado [current_project]
synth_design -top fifo_8x2048 -part xc7vx485tffg1761-2 -mode out_of_context

rename_ref -prefix_all fifo_8x2048
write_checkpoint -noxdef fifo_8x2048.dcp
report_utilization -file fifo_8x2048_utilization_synth.rpt -pb fifo_8x2048_utilization_synth.pb
if { [catch {
  file copy -force /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/fft/vivado/xillydemo.runs/fifo_8x2048_synth_1/fifo_8x2048.dcp /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048.dcp
  write_verilog -force -mode synth_stub /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048_stub.v
  write_vhdl -force -mode synth_stub /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048_stub.vhdl
  write_verilog -force -mode funcsim /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048_funcsim.v
  write_vhdl -force -mode funcsim /home/xuechao/Projects/xillybus-eval-virtex7-1.2c/vivado-essentials/fifo_8x2048/fifo_8x2048_funcsim.vhdl
} _RESULT ] } { 
  error "ERROR: Unable to successfully create or copy supporting IP files."
}
