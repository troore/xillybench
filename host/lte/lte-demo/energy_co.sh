#!/bin/sh

LOG_NAME=co.log

energy_cpu=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' co.log | awk '{print $2}'`

n_fpga_tasks=1

POWER_DIR[1]=/home/xuechao/Projects/xillybench/hls/hls-lte/lte_prof4/lte_prof_ofdm_vc707_opt_prj/solution1/impl/verilog/report


#echo $energy_cpu J

for((i=1;i<=$n_fpga_tasks;i++))
do
power_fpga[$i]=`sed -n '33p' ${POWER_DIR[$i]}/xillybus_wrapper_power.rpt | awk '{print $7}'`
#echo ${power_fpga[$i]} W
done

time_fpga[1]=`sed -n '/time_ofdm/p' co.log | awk '{print $2}'`

energy_fpga=0

for((i=1;i<=$n_fpga_tasks;i++))
do
#energy_fpga=`echo $mm_power_fpga $mm_time_fpga | awk '{print $1, $2, $1 * $2}'`
#time_fpga[$i]=`echo ${time_fpga[$i]} | awk '{print $1 / 1000.0}'`
#echo ${time_fpga[$i]}
energy_fpga=`echo ${power_fpga[$i]} ${time_fpga[$i]} $energy_fpga | awk '{print $1 * $2 + $3}'`
#echo $energy_fpga J
done

energy_total=`echo $energy_cpu $energy_fpga | awk '{print $1 + $2}'`

time_total=`sed -n '/time_total/p' co.log | awk '{print $2}'`

echo $time_total s
echo $energy_total J
