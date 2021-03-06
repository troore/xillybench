#!/bin/sh

# host part

echo "CPU only:"

HOST_LOG_NAME=host.log

./mm_host.out > $HOST_LOG_NAME

host_time_total=`sed -n '/time_total/p' $HOST_LOG_NAME | awk '{print $2}'`

host_energy_total=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' $HOST_LOG_NAME | awk '{print $2}'`

echo $host_time_total s

echo $host_energy_total J

# co part

CO_LOG_NAME=co.log

./mm_wr.out > $CO_LOG_NAME

co_energy_cpu=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' $CO_LOG_NAME | awk '{print $2}'`

n_fpga_tasks=1

POWER_DIR[1]=/home/xuechao/Projects/xillybench/hls/hls-mm/fpga/fpga_opt_prj/solution1/impl/verilog/report


for((i=1;i<=$n_fpga_tasks;i++))
do
co_power_fpga[$i]=`sed -n '33p' ${POWER_DIR[$i]}/xillybus_wrapper_power.rpt | awk '{print $7}'`
done

co_time_fpga[1]=`sed -n /time_mm/p $CO_LOG_NAME | awk '{print $2}'`

co_energy_fpga=0

for((i=1;i<=$n_fpga_tasks;i++))
do
co_energy_fpga=`echo ${co_power_fpga[$i]} ${co_time_fpga[$i]} $co_energy_fpga | awk '{print $1 * $2 + $3}'`
done

co_energy_total=`echo $co_energy_cpu $co_energy_fpga | awk '{print $1 + $2}'`
co_time_total=`sed -n '/time_total/p' $CO_LOG_NAME | awk '{print $2}'`

echo $co_time_total s
echo $co_energy_total J

# check result

echo "Number of errors:"

errno=`sed -n '/errno/p' $CO_LOG_NAME | awk '{print $2}'`

echo $errno
