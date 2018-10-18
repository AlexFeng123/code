#!/bin/bash
date 
function usage(){
  echo "usage: ./pmap.sh <is_configure_end>"
}
if [ $# -le 1 ]; then
  usage
fi
IsCfgEnd=$1;
#capture pm data for each app
apps=$(cat app_list);
for app in $apps ;do
app_pid=$(ps -ef|grep $app|grep -v "grep" |awk -F " " '{print $2}')

if [ -$app_pid != - ] ; then

appPss=$( pmap -X $app_pid |tail -n 1|awk -F' ' '{print $3}')

echo "$app memory consumption PSS: $(expr $appPss / 1024) Mb"

ps u -p $app_pid 
top -b -p $app_pid -n 1

fi
done


du -h /var/opt/adva/aos/db/ 
df -h
echo 0 > /var/opt/adva/aos/log/tracelog/aosFwHal.log
(
 sleep 1;
 echo "go /debug"; 
 echo "set timeout 60000";
 echo "commit";
 echo "go /debug/aosFwApl/fw-apl-fw-mitdump";  
 echo "stats";  
 sleep 10;
 echo "go /debug/aosCorePmSrv";  
 echo "calculateBins";  
 sleep 2;
 echo "calculateDistBins";  
 sleep 2;
 echo "go /debug/aosFwHal/adva.hbmcard.xg480";  
 echo "tree 0 stats";  
 sleep 2;
 echo exit  
 )|telnet 127.0.0.1 9889


if [ ${IsCfgEnd} -eq 1 ]; then
(
 sleep 1;
 echo "go /debug/aosCoreSirm/sirm";  
 echo "showAppPerformanceDiagnostics";  
 sleep 3;
 echo exit  
 )|telnet 127.0.0.1 9889
fi

