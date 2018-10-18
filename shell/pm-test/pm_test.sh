#!/bin/bash

file="pm_data_$(date +%Y%m%d%H%M%S).txt"
echo $file
while true;do
/root/pmap.sh 0 >> $file
sleep 600
done


