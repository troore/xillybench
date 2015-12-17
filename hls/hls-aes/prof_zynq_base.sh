#!/bin/bash

flist=`ls | grep "[a-zA-Z_]*_zynq_base.tcl"`

for i in $flist
do
echo $i
vivado_hls -f $i
dir=`echo $i | sed -e 's/.tcl/_prj/'`
echo $dir

cd $dir/solution1/impl/verilog

#ls
sed -i '62 areport_power -file \$outputDir/\${outputBaseName}_power.rpt' run_vivado.tcl

./impl.sh

cd ../../../../

#ls

done
