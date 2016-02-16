#!/bin/sh

energy_cpu=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' log | awk '{print $2}'`

n_fpga_tasks=1

POWER_DIR[1]=/home/xuechao/Projects/xillybench/hls/hls-mm/fpga/fpga_opt_prj/solution1/impl/verilog/report


#echo $energy_cpu J

for((i=1;i<=$n_fpga_tasks;i++))
do
power_fpga[$i]=`sed -n '33p' ${POWER_DIR[$i]}/xillybus_wrapper_power.rpt | awk '{print $7}'`
echo ${power_fpga[$i]} W
done

time_fpga[1]='0.08'

energy_fpga=0

for((i=1;i<=$n_fpga_tasks;i++))
do
#energy_fpga=`echo $mm_power_fpga $mm_time_fpga | awk '{print $1, $2, $1 * $2}'`
energy_fpga=`echo ${power_fpga[$i]} ${time_fpga[$i]} $energy_fpga | awk '{print $1 * $2 + $3}'`

echo $energy_fpga J
done

energy_total=`echo $energy_cpu $energy_fpga | awk '{print $1 + $2}'`

echo $energy_total J
