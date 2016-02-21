#!/bin/bash

LOG_NAME=host.log

time_total=`sed -n '/time_total/p' $LOG_NAME | awk '{print $2}'`

energy_total=`sed -n '/PACKAGE_ENERGY:PACKAGE0/p' $LOG_NAME | awk '{print $2}'`

echo $time_total s

echo $energy_total J
