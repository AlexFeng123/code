#!/bin/bash
echo "cobnfigure full configurations"
echo "configure 8000 snc, 16000fp; 16 port , 1000  fp per one port"
./create_snc.sh 16 1000
echo "configure 8000 cfm mep mip soamlm soamdm"
./cfm.sh 8 1000
echo "configure 1000 twamp ss"
./twamp.sh 1000

echo "configure full netconfig yang"
./curl.full.sh
