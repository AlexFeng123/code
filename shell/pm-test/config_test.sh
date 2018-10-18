#!/bin/bash
echo "cobnfigure test configurations"
echo "configure 1 snc, 2 port , 1 fp per one port"
./create_snc.sh 1 1
#echo "configure 1 cfm soamlm soamdm"
./cfmi.sh 1 1 
#echo "configure 1 twamp ss"
./twamp.sh 1
