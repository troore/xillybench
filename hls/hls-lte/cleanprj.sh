#!/bin/bash
PRJ_LIST="dft fft Equalizing Modulation RateMatching ResMapping Scrambling Turbo dmrs fft OFDM TransformPrecoding"

#PRJ_LIST="Scrambling"

for i in $PRJ_LIST
do
#	cd ../$i
#	echo rm -rf ../$i/*_prj ../$i/*.log
	rm -rf $i/*_prj $i/*.log
done
