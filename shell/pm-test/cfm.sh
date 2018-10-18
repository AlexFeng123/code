#!/bin/bash
CFMMD=1
MAX_CFMMD_NUM=8
MAX_MA_NUM_PER_PORT=1000
MA=1
PORT=1
function usage(){
  echo "usage: ./cfm.sh <max_cfmmd_num> <max_ma_num_per_cfmmd>"
}
if [ $# -le 1 ]; then
    usage
    exit 0;
else
    MAX_CFMMD_NUM=$1
    MAX_MA_NUM_PER_PORT=$2
fi
echo create cfmmd from 1 to ${MAX_CFMMD_NUM}
echo create ma,mep,mip from 1 to ${MAX_MA_NUM_PER_PORT} per cfmmd
echo create dm lm under mep

echo
date
echo "starting script..."

while [ ${CFMMD} -le ${MAX_CFMMD_NUM} ]
do  
    curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"mdl\": $(expr ${CFMMD} - 1), 
    \"mhfc\": \"dflt\", \"idperm\": \"none\", \"nmtyp\": \"none\", \"id\": \"${CFMMD}\"}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}
    MA=1 
    while [ ${MA} -le ${MAX_MA_NUM_PER_PORT} ]
    do
        echo
        date
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"id\": \"${MA}\",
        \"ccmint\": \"1s\", \"meps\": [1,2], \"nmtyp\": \"uint16\", \"manm\": {\"uint16\": ${MA}}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}/ma/${MA}
        echo

        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"pvid\": ${MA},
        \"compid\": \"/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth\", \"mhfc\": \"defer\", \"idperm\": \"none\",\"fp\": \"/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth/ctp/fp-${MA}/fp\"}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}/ma/${MA}/macomp
        echo
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{  \"id\":1,\"if\":\"/mit/me/1/eqh/shelf,1/eqh/slot,1/eq/card/ptp/port,${PORT}/ctp/eth\",\"dir\": \"up\", \"pvid\": ${MA}, \"ethtyp\": 34984, \"ccmltmp\": 7, \"sm\": {\"admin\": \"oos\"}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}/ma/${MA}/mep/1
        echo
        date        
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"in\": {\"mep\": \"/me=1/cfmsys/cfmmd=1/ma=${mep}/mep=1\",\"dmmeatyp\": \"dmm\",\"dmmsgper\": \"1s\",\"dmver\": \"y2011\",\"dmsesty\": \"proactive\",\"dmmcen\": false,\"starttm\": {\"imm\": {}},\"stoptm\": {\"non\": {}},\"repper\": \"12000\",\"macaddr\": \"22:22:32:22:22:33\"}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}/ma/${MA}/mep/1/soamdm?actn=crtdm
        echo
        date
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"in\": {\"mep\": \"/me=1/cfmsys/cfmmd=1/ma=${mep}/mep=1\",\"lmmeatyp\": \"lmm\",\"lmver\": \"y2011\",\"lmmsgper\": \"1s\",\"lmfrmsz\":64,\"anumcflr\":20,\"aflrthr\":50000,\"anumcint\": 2,\"anumchl\": 3,\"lmsessty\": \"proactive\",\"slmmcen\": false,\"starttm\": {\"imm\": {}},\"stoptm\": {\"non\": {}},\"repper\": \"12000\",\"macaddr\": \"22:22:32:22:22:33\"}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/${CFMMD}/ma/${MA}/mep/1/soamlm?actn=crtlm
       let MA=1+${MA}
    done

    let CFMMD=1+${CFMMD}
    let PORT=1+${PORT}
done

echo
echo "end script."
date
