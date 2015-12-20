#!/bin/bash

hlsdirs='hls-aes hls-dct/dct_prof hls-jpeg hls-md5 hls-lte/lte_prof'
#hlsdirs='hls-lte/lte_prof'

for dir in $hlsdirs
do
curdir=$PWD

cd $dir

flist=`ls | grep "[a-zA-Z0-9_]*_prof_[a-zA-Z0-9]*_zynq_[a-zA-Z0-9]*.tcl"`
for f in $flist
do
echo $f
#vivado_hls -f $f
#prjdir=`echo $f | sed -e 's/.tcl/_prj/'`
#cd $prjdir/solution1/impl/verilog
#sed -i '62 areport_power -file \$outputDir/\${outputBaseName}_power.rpt' run_vivado.tcl
#./impl.sh
#cd ../../../../
done

cd $curdir

done
