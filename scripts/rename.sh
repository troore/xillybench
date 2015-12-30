#!/bin/bash

hlsdirs='hls-aes hls-dct/dct_prof hls-jpeg hls-md5'

rootdir=$PWD

#echo $rootdir

for i in $hlsdirs
do

cd $i

flist=`ls | grep zynq`

for j in $flist
do
k=`echo $j | sed 's/zynq/vc707/'`
echo $k

cp $j $k

sed -i 's/zynq/vc707/' $k
#sed -i 's/src/src4/' $k
sed -i 's/xc7z020clg484-1/xc7vx485tffg1761-2/' $k

done

cd $rootdir
done
