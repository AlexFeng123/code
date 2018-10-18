#!/bin/bash

user="root";
port="614";
ip=$1;
cmd=$2;
#ssh -p $port $user@$ip $cmd
/usr/bin/expect expect.sh $ip $port $user 0 $cmd
if [ $? -eq 0 ]; then
    echo "cmd run ok!"
else
    echo "cmd run failed!"
fi





