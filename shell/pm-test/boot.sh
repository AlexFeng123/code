#!/bin/bash
# measure device boot time: use wget to visit http server,and get http rsp ok.
function doUsage {
  echo "usage: . ./boot.sh <ip address>"
}

#if no arguement supplied show usage and exit
if [ -z "$1" ]; 
then
  doUsage
else
#first,we need check system is rebooting:the device can't be visited(ping timeout)
while true;do
   if ping -c 1 $1 >/dev/null; then
        continue  
   else 
      echo "device is rebooting"
      break
   fi
done
conn=0;
echo "start waiting for device reboot"
start=`date +%s`
while [ $conn -eq 0 ] ; do
  target=$1
  file="/tmp/1.txt"
  wget $target > $file 2>&1
  cat $file | grep connected
  if [ $? -eq 0 ]; then
      echo "$target can be connected!!"
      conn=1;
  fi
  sleep 1
done
end=`date +%s`
dif=$[ end - start ] 
echo "boot time: $dif s"
fi

