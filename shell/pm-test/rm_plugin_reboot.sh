#!/bin/bash

#remove domain plugin
echo "remove domain plugins"
mount -o remount,rw /opt/adva/
rm -rf /opt/adva/aos/lib/plugin/aplfw/libAosDom*
rm -rf /opt/adva/aos/lib/plugin/aplfw/libXg480*
rm -rf /opt/adva/aos/lib/plugin/aplfw/libAosXg480Prod*
rm -rf /opt/adva/aos/lib/plugin/aplfw/libAosProductXg480AplFwPlugin*

#disable pmc process
sed -i "442,442s/true/false/g" /opt/adva/aos/etc/core-apps/sirm/sirmConfigurationFile.json

echo "rm plugin reboot"
/usr/local/sbin/reboot 

