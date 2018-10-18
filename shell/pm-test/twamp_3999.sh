#!/bin/bash
MAX_SS_NUM=1
function usage(){
  echo "usage: ./twamp.sh <max_twampss_num> "
}
if [ $# -le 0 ]; then
    usage
    exit 0;
else
    MAX_SS_NUM=$1
fi
echo create twamp ss from 1 to ${MAX_SS_NUM}

ss=1
echo
date
echo "starting script..."

curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"name\": \"twamp\"}" https://${target}:443/mit/me/1/twamp/iprouter/1
echo 

curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{  \"v4addrs\": [\"172.23.111.226/24\"],\"name\": \"intf1\",\"type\": \"if\" }" https://${target}:443/mit/me/1/twamp/iprouter/1/ipif/1
echo

curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"id\":\"1\",\"ipif\":\"/me=1/twamp/iprouter=1/ipif=1\", \"usrlbl\":\"cc1\", \"port\": \"/me=1/eqh=shelf,1/eqh=slot,1/eq=card/ptp=port,2/ctp=eth/mac\",\"mode\": \"unauth\",\"ctrlen\": false}" https://${target}:443/mit/me/1/twamp/twpcc/1
ss_list=
while [ ${ss} -le ${MAX_SS_NUM} ]
do
    echo
    date
    ssport=$(expr 1000 + $ss);
    curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"id\":\"${ss}\", \"ipif\":\"/me=1/twamp/iprouter=1/ipif=1\",\"starttm\":{\"imm\":{}}, \"usrlbl\":\"ss1\", \"numpkts\": 10000,\"ssdscp\": 2,\"ssudpprt\":${ssport} , \"srudpprt\": 12345,\"pktsize\":100,\"rsptimeo\": 5, \"pktintvl\": 100,\"srip\": \"172.23.111.229\",\"pattern\": \"all0\",\"vlanlst\": [ { \"ethtyp\": \"ctag\",\"vlanpos\": \"outer\", \"vlantag\": \"3999-1\"}]}"  https://${target}:443/mit/me/1/twamp/twpss/${ss} 
    #curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"id\":\"${ss}\", \"ipif\":\"/me=1/twamp/iprouter=1/ipif=1\",\"starttm\":{\"imm\":{}}, \"usrlbl\":\"ss1\", \"numpkts\": 10000,\"ssdscp\": 2,\"ssudpprt\":${ssport} , \"srudpprt\": 22345,\"pktsize\":100,\"rsptimeo\": 5, \"pktintvl\": 100,\"srip\": \"172.23.222.229\",\"pattern\": \"all0\",\"vlanlst\": [ { \"ethtyp\": \"ctag\",\"vlanpos\": \"outer\", \"vlantag\": \"4094-1\"}]}"  https://${target}:443/mit/me/1/twamp/twpss/${ss} 
    echo
    if [ ${ss} -eq 1 ];then
    ss_list=\"/me=1/twamp/twpss=${ss}\"
    else 
    ss_list=${ss_list}","\"/me=1/twamp/twpss=${ss}\"
    fi
    let ss=1+${ss}
done
    echo $ss_list
    curl -k -X PATCH -H 'Content-Type:application/json-patch+json;ext=nn' -H "X-Auth-Token: ${token}" -d "[{\"op\": \"add\",\"path\": \"/sslst\", \"value\": [${ss_list}]}]" https://${target}:443/mit/me/1/twamp/twpcc/1

echo

    curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"in":{}}' https://${target}:443/mit/me/1/twamp/twpcc/1?actn=startss
echo "end script."
date
