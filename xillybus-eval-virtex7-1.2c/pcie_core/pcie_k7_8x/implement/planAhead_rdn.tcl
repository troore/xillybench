
set device xc7vx485t-ffg1761-2
set projName pcie_k7_8x
set design pcie_k7_8x
set projDir  [file dirname [info script]]

set projDir [file dirname [info script]]
create_project $projName $projDir/results/$projName -part $device -force
set_property design_mode RTL [current_fileset -srcset]


set top_module xilinx_pcie_2_1_ep_7x
set_property top xilinx_pcie_2_1_ep_7x [get_property srcset [current_run]]

add_files -norecurse {../../source/pcie_k7_8x_gt_rx_valid_filter_7x.v}
add_files -norecurse {../../source/pcie_k7_8x_gt_top.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_bram_top_7x.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_brams_7x.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_bram_7x.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_7x.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_pipe_pipeline.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_pipe_lane.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_pipe_misc.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_tx_thrtl_ctl.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_rx.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_rx_null_gen.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_rx_pipeline.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_tx_pipeline.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_tx.v}
add_files -norecurse {../../source/pcie_k7_8x_axi_basic_top.v}
add_files -norecurse {../../source/pcie_k7_8x_pcie_top.v}
add_files -norecurse {../../source/pcie_k7_8x.v}
add_files -norecurse {../../source/pcie_k7_8x_gt_wrapper.v}
add_files -norecurse {../../source/pcie_k7_8x_gtp_pipe_reset.v}
add_files -norecurse {../../source/pcie_k7_8x_gtp_pipe_rate.v}
add_files -norecurse {../../source/pcie_k7_8x_qpll_wrapper.v}
add_files -norecurse {../../source/pcie_k7_8x_qpll_drp.v}
add_files -norecurse {../../source/pcie_k7_8x_qpll_reset.v}
add_files -norecurse {../../source/pcie_k7_8x_rxeq_scan.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_eq.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_clock.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_drp.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_rate.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_reset.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_user.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_sync.v}
add_files -norecurse {../../source/pcie_k7_8x_pipe_wrapper.v}
	add_files -norecurse {../../example_design/xilinx_pcie_2_1_ep_7x.v}
add_files -norecurse {../../example_design/pcie_app_7x.v}
add_files -norecurse {../../example_design/PIO.v}
add_files -norecurse {../../example_design/PIO_EP.v}
add_files -norecurse {../../example_design/PIO_EP_MEM_ACCESS.v}
add_files -norecurse {../../example_design/EP_MEM.v}
add_files -norecurse {../../example_design/PIO_TO_CTRL.v}
add_files -norecurse {../../example_design/PIO_RX_ENGINE.v}
add_files -norecurse {../../example_design/PIO_TX_ENGINE.v}
read_xdc ../../example_design/xilinx_pcie_2_1_ep_7x_08_lane_gen1_xc7vx485t-ffg1761-2_VC707.xdc
synth_design -flatten_hierarchy none
opt_design
place_design
route_design
set_param sta.dlyMediator true
write_sdf -rename_top_module xilinx_pcie_2_1_ep_7x -file routed.sdf
write_verilog -nolib -mode funcsim -sdf_anno true -rename_top_module xilinx_pcie_2_1_ep_7x -file routed.v
report_timing -nworst 30 -path_type full -file routed.twr
report_drc
