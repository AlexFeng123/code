#!/bin/bash
mount -o remount,rw /opt/adva
rm -rf /opt/adva/aos
tar xjf /tmp/*arm8-64bit.tar.bz2 -C /

