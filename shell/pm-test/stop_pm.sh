#!/bin/bash
function stopPm(){
kill -9 $(ps -ef|grep pm_test.sh |grep -v "grep" |awk -F " " '{print $2}')
}
app_pid=$(ps -ef|grep aosFwApl |grep -v "grep" |awk -F " " '{print $2}')
appPss=$(pmap -X $app_pid |tail -n 1|awk -F' ' '{print $3}')
old_size=$(expr $appPss / 1024)
new_size=$old_size
while true; do
    sleep 600
    appPss=$(pmap -X $app_pid |tail -n 1|awk -F' ' '{print $3}')
    new_size=$(expr $appPss / 1024)
    if [ $new_size == $old_size ];then
        echo "apl pss is stabilize at $new_size"
        break;
    fi
    echo "apl pss is increasing to $new_size"
    old_size=$new_size
done
stopPm
./pmap.sh 1 > pm_end.txt
