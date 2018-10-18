MAX_NUM=1
if [ $# -ge 1 ]
then
    MAX_NUM=$1
fi

echo create dm lm from mep=1 to ${MAX_NUM}

cfmmd=1
ma=1
mep=1
echo
date
echo "starting script..."

while [ ${mep} -le ${MAX_NUM} ]
do
        echo
        date
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{\"in\": {\"mep\": \"/me=1/cfmsys/cfmmd=1/ma=${mep}/mep=1\",\"dmmeatyp\": \"dmm\",\"dmmsgper\": \"1s\",\"dmver\": \"y2011\",\"dmsesty\": \"proactive\",\"dmmcen\": false,\"starttm\": {\"imm\": {}},\"stoptm\": {\"non\": {}},\"repper\": \"12000\",\"macaddr\": \"22:22:32:22:22:33\"}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/1/ma/${mep}/mep/1/soamdm?actn=crtdm
        echo
        curl -k -X POST -H 'Content-Type:application/json;ext=nn' -H "X-Auth-Token: ${token}" -d "{ \"in\": {\"mep\": \"/me=1/cfmsys/cfmmd=1/ma=${mep}/mep=1\",\"lmmeatyp\": \"lmm\",\"lmver\": \"y2011\",\"lmmsgper\": \"1s\",\"lmfrmsz\":64,\"anumcflr\":20,\"aflrthr\":50000,\"anumcint\": 2,\"anumchl\": 3,\"lmsessty\": \"proactive\",\"slmmcen\": false,\"starttm\": {\"imm\": {}},\"stoptm\": {\"non\": {}},\"repper\": \"12000\",\"macaddr\": \"22:22:32:22:22:33\"}}" https://${target}:443/mit/me/1/cfmsys/cfmmd/1/ma/${mep}/mep/1/soamlm?actn=crtlm
        let mep=1+${mep}
done
echo
echo "end script."
date

