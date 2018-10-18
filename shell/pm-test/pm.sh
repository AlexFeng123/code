#!/bin/bash
ip=;
port_num=;
script=;
image=;
factory_db=;
reboot1=;
reboot2=;
function usage(){
  echo "usage: ./pm.sh <device_ip> <script> <image> <factory db> <first_reboot> <reboot_after_configure>"
  echo "script = null mean do not exec configuration script"
  echo "image = null mean do not upgrade"
}
function capturePmData(){
   #for ssh session setup
   rm -rf ~/.ssh/known_hosts 
   ./scp.sh $ip start_pm.sh  $script_dir

   echo "====== start to configure and capture pm data ====="
   ssh -p 614  -t root@$ip /opt/adva/aos/bin/aosVersion > aosVersion
   source ./getToken.sh $ip

   echo "===== start_pm ====="
   ssh -p 614  -t root@$ip  /root/start_pm.sh
   sleep 5 
   if [ $1 -eq 1 ]; then
       time ./${script}
   fi

   echo "===== stop_pm ====="
   nohup ssh -p 614  -t root@$ip  /root/stop_pm.sh &

   #waiting apl configure done
   echo "===== waiting capture pm data stop... ====="
   while true; do
       ssh -p 614  -t root@$ip "ps -ef|grep pm_test.sh|grep -v grep" > pm_test_app
       if grep pm_test.sh pm_test_app > /dev/null;then
           continue;
       else
           echo "===== capture pm data finish! ====="
           break;
       fi
   done
}
function getPmData(){
   #get pm data from device
   echo "====== get pm data from device:$ip ====="
   data_dir="pm_test_data_$(date +%Y%m%d%H%M%S)"
   mkdir -p $data_dir
   data_path=$(readlink  -e $data_dir)
   scp -o Port=614 root@$ip:/root/*.txt $data_path
   echo "====== get pm data to $data_path done ====="
   #filter pm raw data to category 
   echo "====== filter pm data ====="
   filter="filter.sh"
   chmod 777 $filter && ./$filter $data_path
}

function Reboot(){
   #for ssh session setup
   rm -rf ~/.ssh/known_hosts 
   ./scp.sh $ip start_pm.sh  $script_dir
   echo "===== reboot ====="
   nohup  ssh -p 614  -t root@$ip  /root/reboot.sh &
   echo "===== waiting device startup ====="
   ./boot.sh $ip   
}

if [ $# -le 5 ]; then
    usage
else

    start=`date +%s`
    ip=$1;
    script=$2;
    image=$3
    factory_db=$4;
    reboot1=$5;
    reboot2=$6;
(
    if [ $image != "null" ]; then
       #send shell script to destination device to capture pm data
       rm -rf ~/.ssh/known_hosts 
       script_file_list="start_pm.sh stop_pm.sh pm_test.sh pmap.sh upgrade.sh db.sh reboot.sh app_list"
       script_dir="/root"
       arr=($script_file_list);  
       echo "===== send script files: $script_file_list to $ip:$script_dir"
       #for ssh session setup
       ./scp.sh $ip start_pm.sh  $script_dir
       
       for file in ${arr[*]}
       do
           chmod 777 $file;
           scp -o Port=614 $file root@$ip:$script_dir;
       done 
       
       #send image
       image_dir="/tmp"
       echo "send image file $image to $ip:$image_dir"
       scp -o Port=614 $image root@$ip:$image_dir
       
       #upgrade
       echo "=====upgrate device with image: $image ====="
       #./remote.sh $ip /root/upgrade.sh
       ssh -p 614  -t root@$ip  /root/upgrade.sh

       echo "=====upgrate device with image finished! ====="
   fi
   if [ $factory_db -eq 1 ]; then
       #factory db and reboot
       echo "=====factory db and reboot====="
       nohup ssh -p 614  -t root@$ip  /root/db.sh &

       #wait device rebooting 
       echo "=====waiting device startup====="
       ./boot.sh $ip 
   fi
   if [ $reboot1 -eq 1 ];then
       Reboot;
   fi
   if [ $script != "null" ];then
       start=`date +%s`
       capturePmData 1;
       getPmData;
       end=`date +%s`
       diff=$[ end - start ]
       hours=$(expr $diff / 3600)
       mins=$(expr $(expr $diff % 3600) / 60)
       secs=$(expr $(expr $diff % 3600) % 60)
       echo "===== pm test end at date: $(date) ====="
       echo "===== using time: ${diff}s = ${hours}h${mins}m${secs}s ====="
   fi
   if [ $reboot2 -eq 1 ];then
        start=`date +%s`
        Reboot;
        capturePmData 0;
        getPmData;
        end=`date +%s`
        diff=$[ end - start ]
        hours=$(expr $diff / 3600)
        mins=$(expr $(expr $diff % 3600) / 60)
        secs=$(expr $(expr $diff % 3600) % 60)
        echo "===== after reboot pm test end at date: $(date) ====="
        echo "===== using time: ${diff}s = ${hours}h${mins}m${secs}s ====="
    fi
    ) | tee -a ${script}_$(date +%Y%m%d%H%M%S).log
fi
