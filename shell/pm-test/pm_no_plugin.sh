#!/bin/bash
   
function usage(){
  echo "usage: ./pm.sh <device_ip> <port_num> <snc_num_per_port> <image>"
  echo "max fp num of one port is 4000"
  echo "max port num of the device is 62"
  echo "max fp num of the device is 16000"
}
if [ $# -le 3 ]; then
    usage
else
    start=`date +%s`;
    port_num=$2;
    snc_per_port=$3;
    image=$4;
    #send shell script to destination device to capture pm data
    rm -rf ~/.ssh/known_hosts 
    script_file_list="stop_pm.sh pm_test.sh pmap.sh upgrade.sh db.sh rm_plugin_reboot.sh app_list";
    script_dir="/root";
    arr=($script_file_list);  
    echo "===== send script files: $script_file_list to $1:$script_dir"
    #for ssh session setup
    ./scp.sh $1 start_pm.sh  $script_dir
    
    for file in ${arr[*]}
    do
        chmod 777 $file;
        scp -o Port=614 $file root@$1:$script_dir;
    done 
    
    #send image
    image_dir="/tmp"
    echo "send image file $image to $1:$image_dir"
    scp -o Port=614 $image root@$1:$image_dir
    
    #upgrade
    echo "=====upgrate device with image: $4====="
    #./remote.sh $1 /root/upgrade.sh  
    ssh -p 614  -t root@$1  /root/upgrade.sh

    # factory db and reboot
    echo "=====factory db and reboot====="
    #./remote.sh $1 /root/db.sh  
    nohup ssh -p 614  -t root@$1  /root/db.sh  &
    
    #wait device rebooting 
    echo "=====waiting device startup====="
    ./boot.sh $1 
    
    #for ssh session setup
    rm -rf ~/.ssh/known_hosts 
    ./scp.sh $1 start_pm.sh  $script_dir
    
    echo "====== start to configure and capture pm data ====="
    ssh -p 614  -t root@$1 /opt/adva/aos/bin/aosVersion > aosVersion
    source ./getToken.sh $1
    
    echo "===== start_pm ====="
    ssh -p 614  -t root@$1  /root/start_pm.sh
    
    time ./ss2.sh $port_num $snc_per_port
    echo "===== stop_pm ====="
    nohup ssh -p 614  -t root@$1  /root/stop_pm.sh &

    #waiting apl configure done
    while true; do
        echo -ne "===== waiting apl configure.\r"
        sleep 1
        echo -ne "===== waiting apl configure..\r"
        sleep 1
        echo -ne "===== waiting apl configure...\r"
        ssh -p 614  -t root@$1 "ps -ef|grep pm_test.sh|grep -v grep" > pm_test_app
        if grep pm_test.sh pm_test_app > /dev/null;then
            continue;
        else
           echo "===== pm test finish ====="
           break;
        fi
    done
    #rm domain plugin and reboot
    echo "===== rm domain plugin and reboot ====="
    nohup ssh -p 614  -t root@$1  /root/rm_plugin_reboot.sh &
    echo "===== waiting device startup ====="
    ./boot.sh $1 
    #for ssh session setup
    rm -rf ~/.ssh/known_hosts 
    ./scp.sh $1 start_pm.sh  $script_dir
    echo "===== start_pm ====="
    ssh -p 614  -t root@$1  /root/start_pm.sh
    echo "===== stop_pm ====="
    nohup ssh -p 614  -t root@$1  /root/stop_pm.sh &
    
    while true; do
        echo -ne "===== waiting Pm test stop.\r"
        sleep 1
        echo -ne "===== waiting Pm test stop..\r"
        sleep 1
        echo -ne "===== waiting Pm test stop...\r"
        ssh -p 614  -t root@$1 "ps -ef|grep pm_test.sh|grep -v grep" > pm_test_app
        if grep pm_test.sh pm_test_app > /dev/null;then
            continue;
        else
           echo "===== pm test finish ====="
           break;
        fi
    done
    #get pm data from device
    echo "====== get pm data from device:$1 ====="
    aosBaseVersion=$(cat aosVersion | grep build:|head -n1|awk -F' ' '{print $4}')
    xg480Version=$(cat aosVersion | grep build:|tail -n1|awk -F' ' '{print $4}')
    data_dir="pm_test_${aosBaseVersion}_${xg480Version}"
    mkdir -p $data_dir
    data_path=$(readlink  -e $data_dir)
    #srcIp=$(ip addr |grep eth0|awk '{print $2}'|grep -v eth0)
    #srcIp=${srcIp%/*};
    scp -o Port=614 root@$1:/root/*.txt $data_path
    echo "====== get pm data to $data_path done ====="
    #filter pm raw data to category 
    echo "====== filter pm data ====="
    filter="filter.sh"
    chmod 777 $filter && ./$filter $data_path  
    end=`date +%s`
    diff=$[ end - start ]
    hours=$(expr $diff / 3600)
    mins=$(expr $(expr $diff % 3600) / 60)
    secs=$(expr $(expr $diff % 3600) % 60)
    echo "===== pm test end at date: $(date) ====="
    echo "===== using time: ${diff}s = ${hours}h${mins}m${secs}s ====="
fi
