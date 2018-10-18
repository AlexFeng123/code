#!/bin/bash
MAX_NUM=1
PORT=1
PORTB=1
vid=2
SNCID=1
MAXVID=1
function usage(){
  echo "usage: ./ctrean_snc.sh <max_port_num> <max_fp_num_per_port>"
}
if [ $# -le 1 ]; then
    usage
    exit 0;
else
    MAX_NUM=$1
    MAXVID=$2
fi

echo create ${MAXVID} fp from port,1 to port,${MAX_NUM}

##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{}' https://${target}:443/mit/me/1/ethernet
##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"name":"1", "cir":1, "eir":1}' https://${target}:443/mit/me/1/ethernet/bwprof/1
##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"name":"1", "bufsz":1}' https://${target}:443/mit/me/1/ethernet/bsprof/1
##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"name":"1", "cir":1, "cbs":10}' https://${target}:443/mit/me/1/ethernet/polprof/1
curl -k -X PATCH -H 'Content-Type:application/json-patch+json; ext=nn' -H "X-Auth-Token: ${token}" -d '[{"op":"replace", "path":"/cir", "value": 22000}, {"op":"replace", "path":"/cbs", "value": 20}]' https://${target}:443/mit/me/1/ethernet/polprof/1
curl -k -X PATCH -H 'Content-Type:application/json-patch+json; ext=nn' -H "X-Auth-Token: ${token}" -d '[{"op":"replace", "path":"/cir", "value": 22200000}, {"op":"replace", "path":"/eir", "value": 22200000}]' https://${target}:443/mit/me/1/ethernet/bwprof/1
curl -k -X PATCH -H 'Content-Type:application/json-patch+json; ext=nn' -H "X-Auth-Token: ${token}" -d '[{"op":"replace", "path":"/bufsz", "value": 20}]' https://${target}:443/mit/me/1/ethernet/bsprof/1

curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"name":"2", "cir": 497664, "eir": 200}' https://${target}:443/mit/me/1/ethernet/bwprof/2

echo
date
echo "starting script..."

while [ ${PORT} -le ${MAX_NUM} ]
do
    echo port=${PORT}
    let PORTB=1+${PORT}
    echo portb=${PORTB}
    
    ## 32 MOs per vid
    while [ $vid -le ${MAXVID} ]
    do
        date
        ## create ctp
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"layer":["flow"],"termmode":"nss","tpdir":"bi","sm":{"admin":"oos","oosst":["uas"]}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}
        echo
        date
        ## create fp
        if [ $vid -eq 1 ];then
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"ingmbrs":"3999"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp
        else
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"ingmbrs\":\"${vid}\"}" https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp
        fi
        echo
        date
        ## create hcos
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"gfb":10,"hcosen":false,"mfb":2000}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/hcos
        echo
        date
        ## create mcos
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"mcosen":false}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/mcos
        echo
        date
        ## create ratelm
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bcasten":false,"bcastlm":1000,"mbcasten":false,"mbcastlm":1000,"mcasten":false,"mcastlm":1000}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/ratelm
        ##echo
        ##date
        ## create wfq
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"wfqid":0,"wfqprio":0,"geir":0}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/wfq
        echo
        date
        ## create priority mapping
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"egprof":"/me=1/ethernet/epriprof=1","inprof":"/me=1/ethernet/ipriprof=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/priomap
        echo
        date
        ## create pcp
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"decprof":"/me=1/ethernet/pcpdprof=1","encprof":"/me=1/ethernet/pcpeprof=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/pcpde
        ##echo
        ##date
        ## create l2cp
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"prof":"/me=1/ethernet/fpl2cppf=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/l2cp
        ##echo
        ##date
        ## create queue 1
        if [ $vid -eq 1 ];then
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=1","id":1,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/que/tm,1
        else
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=2","id":1,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/que/tm,1
        fi
        echo
        date
        ## create queue 2
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=1","id":2,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/que/tm,2
        ##echo
        ##date
        ## create policer 1
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"id":1,"polprof":"/me=1/ethernet/polprof=1", "sm":{"admin":"oos","oosst":["uas"]}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/qospol/1
        echo
        date
        ## create policer 2
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"id":2,"polprof":"/me=1/ethernet/polprof=1", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${vid}/fp/qospol/2
        ##echo
        ##date

        ## create ctp
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"layer":["flow"],"termmode":"nss","tpdir":"bi","sm":{"admin":"oos","oosst":["uas"]}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}
        echo
        date
        ## create fp
        if [ $vid -eq 1 ];then
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"ingmbrs":"3999"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp
        else
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"ingmbrs\":\"${vid}\"}" https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp
        fi
        echo
        date
        ## create hcos
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"gfb":10,"hcosen":false,"mfb":2000}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/hcos
        echo
        date
        ## create mcos
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"mcosen":false}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/mcos
        echo
        date
        ## create ratelm
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bcasten":false,"bcastlm":1000,"mbcasten":false,"mbcastlm":1000,"mcasten":false,"mcastlm":1000}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/ratelm
        ##echo
        ##date
        ## create wfq
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"wfqid":0,"wfqprio":0,"geir":0}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/wfq
        echo
        date
        ## create priority mapping
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"egprof":"/me=1/ethernet/epriprof=1","inprof":"/me=1/ethernet/ipriprof=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/priomap
        echo
        date
        ## create pcp
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"decprof":"/me=1/ethernet/pcpdprof=1","encprof":"/me=1/ethernet/pcpeprof=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/pcpde
        ##echo
        ##date
        ## create l2cp
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"prof":"/me=1/ethernet/fpl2cppf=1"}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/l2cp
        ##echo
        ##date
        ## create queue 1

        if [ $vid -eq 1 ];then
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=1","id":1,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/que/tm,1
        else
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=2","id":1,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/que/tm,1
        fi
        echo
        date
        ## create queue 2
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"bsprof":"/me=1/ethernet/bsprof=1","bwprof":"/me=1/ethernet/bwprof=1","id":2,"type":"tm", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/que/tm,2
        ##echo
        ##date
        ## create policer 1
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"id":1,"polprof":"/me=1/ethernet/polprof=1", "sm":{"admin":"oos","oosst":["uas"]}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/qospol/1
        echo
        date
        ## create policer 2
        ##curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d '{"id":2,"polprof":"/me=1/ethernet/polprof=1", "sm":{"admin":"is"}}' https://${target}:443/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORTB}/ctp/eth/ctp/fp-${vid}/fp/qospol/2
        ##echo
        ##date

        let SNCID=(${PORT}-1)/2*${MAXVID}+${vid}
        echo snc=${SNCID}
        ## create snc
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"tplist\":[\"/me=1/eqh=shelf,1/eqh=slot,1/eq=card/ptp=port,${PORT}/ctp=eth/ctp=fp-${vid}\", \"/me=1/eqh=shelf,1/eqh=slot,1/eq=card/ptp=port,${PORTB}/ctp=eth/ctp=fp-${vid}\"],\"layer\":\"flow\",\"snctype\":\"p2pflow\",\"sm\":{\"admin\":\"is\"}}" https://${target}:443/mit/me/1/sn/1/snc/${SNCID}

        echo
        date
        let vid=1+${vid}
    done
    let PORT=2+${PORT}
    let vid=1
done

echo "end script."
date
