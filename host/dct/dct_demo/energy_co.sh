#!/bin/sh

# host part

echo "CPU only:"

HOST_LOG_NAME=host.log

./dct_host.out > $HOST_LOG_NAME

host_time_total=`sed -n '/time_total/p' $HOST_LOG_NAME | awk '{print $2}'`

host_energy_total=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' $HOST_LOG_NAME | awk '{print $2}'`

echo $host_time_total s

echo $host_energy_total J

# co part

echo "Coprocessing:"

CO_LOG_NAME=co.log

./dct_co.out > $CO_LOG_NAME

co_energy_cpu=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' $CO_LOG_NAME | awk '{print $2}'`

n_fpga_tasks=1

POWER_DIR[1]=/home/xuechao/Projects/xillybench/hls/hls-dct/dct_2d_test/dct_2d_test_prj/solution1/impl/verilog/report


for((i=1;i<=$n_fpga_tasks;i++))
do
co_power_fpga[$i]=`sed -n '33p' ${POWER_DIR[$i]}/xillybus_wrapper_power.rpt | awk '{print $7}'`
#echo ${power_fpga[$i]} W
done

co_time_fpga[1]=`sed -n /time_dct_2d/p co.log | awk '{print $2}'`

co_energy_fpga=0

for((i=1;i<=$n_fpga_tasks;i++))
do
#energy_fpga=`echo $mm_power_fpga $mm_time_fpga | awk '{print $1, $2, $1 * $2}'`
#time_fpga[$i]=`echo ${time_fpga[$i]} | awk '{print $1 / 1000.0}'`
#echo ${time_fpga[$i]}
co_energy_fpga=`echo ${co_power_fpga[$i]} ${co_time_fpga[$i]} $co_energy_fpga | awk '{print $1 * $2 + $3}'`
#echo $energy_fpga J
done

co_energy_total=`echo $co_energy_cpu $co_energy_fpga | awk '{print $1 + $2}'`

co_time_total=`sed -n '/time_total/p' $CO_LOG_NAME | awk '{print $2}'`

echo $co_time_total s
echo $co_energy_total J

# check result

echo "Numbr of errors:"

n=64
errno=0
for((i=1;i<=$n;i++))
do
out_host[$i]=`sed -n "${i}p" ../out_host.dat`
out_co[$i]=`sed -n "${i}p" ../out_co.dat`
flag=`echo ${out_host[$i]} ${out_co[$i]} | awk '{print $1 == $2}'`
errno=`echo $errno $flag | awk '{print $1 + $2}'`
done

echo $errno
