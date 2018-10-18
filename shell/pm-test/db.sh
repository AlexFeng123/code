#!/bin/bash

#factory db and reboot

rm -rf /var/opt/adva/aos/db/aplfw/current/*
tar -xvf /var/opt/adva/aos/db/aplfw/defaults/factory/FactoryDatabase.tar.gz -C /var/opt/adva/aos/db/aplfw/current/ 
rm -rf /var/db/*
rm -rf /dev/shm/*
rm -rf /var/opt/adva/aos/core-apps/security
rm -rf /var/opt/adva/aos/db/pm/* 
rm /var/opt/adva/aos/log/tracelog/*.log

echo "reboot"
/usr/local/sbin/reboot cold
