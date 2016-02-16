#!/bin/sh

energy_cpu=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' log | awk '{print $2}'`

echo $energy_cpu J
