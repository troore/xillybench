#!/bin/bash

flist=`ls | grep "[a-zA-Z_]*_opt.tcl"`

for i in $flist
do
echo $i
vivado_hls -f $i
done
