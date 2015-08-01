#!/bin/sh

# Clean up the results directory
rm -rf results
mkdir results

#Synthesize the Wrapper Files
echo 'Synthesizing example design with XST';
xst -ifn xilinx_pcie_2_1_ep_7x.xst -ofn xilinx_pcie_2_1_ep_7x.log

cp xilinx_pcie_2_1_ep_7x.ngc ./results/

cp xilinx_pcie_2_1_ep_7x.log xst.srp

rm -rf *.mgo xlnx_auto_0_xdb xlnx_auto_0.ise netlist.lst smart

cd results

echo 'Running ngdbuild'
ngdbuild -verbose -uc ../../example_design/xilinx_pcie_2_1_ep_7x_08_lane_gen1_xc7vx485t-ffg1761-2_VC707.ucf xilinx_pcie_2_1_ep_7x.ngc -sd .


echo 'Running map'
map -w \
  -register_duplication on \
  -ol high \
  -o mapped.ncd \
  xilinx_pcie_2_1_ep_7x.ngd \
  mapped.pcf

echo 'Running par'
par \
  -ol high \
  -w mapped.ncd \
  routed.ncd \
  mapped.pcf

echo 'Running trce'
trce -u -e 100 \
  routed.ncd \
  mapped.pcf

echo 'Running design through netgen'
netgen -sim -ofmt verilog -ne -w -tm xilinx_pcie_2_1_ep_7x -sdf_path . routed.ncd

#echo 'Running design through bitgen'
bitgen -w routed.ncd

echo 'Generating PROM file for programming'
promgen -w -p mcs -x xcf128x -data_width 16 -o ./ML705.mcs -u 0 ./routed.bit
